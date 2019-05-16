/**
 * @file Gratings.cpp
 * @brief 
 */

#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <afx.h>
#include <fstream>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include "Gratings.h"
#include "comm/SerialComm.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

CGratings::CGratings()
: m_leftStringFromFile(""), m_rightStringFromFile("")
{
	iPhase = TYPE_STATIC;
	iAngle = TYPE_STATIC;
	iType = STYLE_SQUARE;
	fDotSize = 1.0;
	fGratingWidth = 2.0;
	iNumAngles = 10;
	fGaborSD = 5.0;
	fStaticPhase = 0.0;
	fStaticAngle = 0.0;
	iDelay = 50;
	iBlank = 0;
	iNumPhases = 36;
	iGratingRed = 255;
	iGratingGreen = 255;
	iGratingBlue = 255;
	iGratingRed2 = 50;
	iGratingGreen2 = 50;
	iGratingBlue2 = 50;
	iNumRepeats = 2;
	unitTag = CString("MON999R01_01_D001");
	bBehavior = false;
	iInitiation = 200;
	iReward = 200;
	iHSteps = 6;
	iVSteps = 3;
	fStepSize = 5.0;
	bUseSaccades = true;
	iGratingsPerSaccade = 5;
	iShutters = SHUTTER_RANDOM;
	iShutterInterleavedN = 1;
	iShutterDelay = 0;
	fRFX = 0.0;
	fRFY = 0.0;
	fDevX = 0.0;
	fDevY = 0.0;
	fSpeed = 0.0;
	iRFEye = 0;//left
	fInitialSize = 30.0;
	fFinalSize = 2.5;

	numTrials = 0;
	trials = NULL;

	m_leftFixationPoints = NULL;
	m_rightFixationPoints = NULL;
	m_numLeftFixationPoints = 0;
	m_numRightFixationPoints = 0;

	LoadFile(true);
}

CGratings::~CGratings() 
{
	clearPoints(m_leftFixationPoints, m_numLeftFixationPoints);
	clearPoints(m_rightFixationPoints, m_numRightFixationPoints);
}

void CGratings::LoadFile(bool bDefault) 
{
	CString filename = defaultFile;
	if (!bDefault) {
		filename = etu::BrowseForFile(true, ".txt", "Text Files (*.txt)|*.txt", filename, false);
	}

	FILE* fp = fopen(filename, "rt");
	if (!fp) {
		AfxMessageBox("Error loading file - file or folder not found");
		return;
	}

	double fDontCare;

	fscanf(fp, "%d\n", &iPhase);
	fscanf(fp, "%d\n", &iAngle);
	fscanf(fp, "%d\n", &iType);
	fscanf(fp, "%f\n", &fDontCare); //&fXLocL);
	fscanf(fp, "%f\n", &fDontCare); //&fYLocL);
	fscanf(fp, "%f\n", &fDotSize);
	fscanf(fp, "%f\n", &fGratingWidth);
	fscanf(fp, "%d\n", &iNumAngles);
	fscanf(fp, "%f\n", &fGaborSD);
	fscanf(fp, "%f\n", &fStaticPhase);
	fscanf(fp, "%f\n", &fStaticAngle);
	fscanf(fp, "%d\n", &iDelay);
	fscanf(fp, "%d\n", &iBlank);
	fscanf(fp, "%d\n", &iNumPhases);
	fscanf(fp, "%d\n", &iGratingRed);
	fscanf(fp, "%d\n", &iGratingGreen);
	fscanf(fp, "%d\n", &iGratingBlue);
	fscanf(fp, "%d\n", &iGratingRed2);
	fscanf(fp, "%d\n", &iGratingGreen2);
	fscanf(fp, "%d\n", &iGratingBlue2);
	fscanf(fp, "%d\n", &iNumRepeats);

	char buffer2[1024];ZeroMemory(buffer2, 1024);
	fscanf(fp, "%[^\n]\n", buffer2);
	unitTag = CString(buffer2);
	
	fscanf(fp, "%d\n", &bBehavior);//gets trashed with value of iInitiation
	fscanf(fp, "%d\n", &iInitiation);//gets trashed with value of iReward
	fscanf(fp, "%d\n", &iReward);
	fscanf(fp, "%d\n", &iHSteps);
	fscanf(fp, "%d\n", &iVSteps);
	fscanf(fp, "%f\n", &fStepSize);
	fscanf(fp, "%d\n", &bUseSaccades);
	fscanf(fp, "%d\n", &iGratingsPerSaccade);
	fscanf(fp, "%d\n", &iShutters);
	fscanf(fp, "%d\n", &iShutterInterleavedN);
	fscanf(fp, "%d\n", &iShutterDelay);
	fscanf(fp, "%f\n", &fDontCare); //&fXLocR);
	fscanf(fp, "%f\n", &fDontCare); //&fYLocR);
	fscanf(fp, "%f\n", &fRFX);
	fscanf(fp, "%f\n", &fRFY);
	fscanf(fp, "%f\n", &fDevX);
	fscanf(fp, "%f\n", &fDevY);
	fscanf(fp, "%f\n", &fSpeed);
	fscanf(fp, "%d\n", &iRFEye);
	fscanf(fp, "%f\n", &fInitialSize);
	fscanf(fp, "%f\n", &fFinalSize);

	char fixStrBuff[FIXSTR_BUFLEN];
	CString formatStr;
	/* Interpolate (FIXSTR_BUFLEN - 1) into a format string for use w/ fscanf.
	 * The [^\n] specifier says to read until newline; also pull that newline
	 * off the stream. */
	formatStr.Format("%%%d[^\n]\n", FIXSTR_BUFLEN - 1);
	/* Left string first. */
	fscanf(fp, formatStr, fixStrBuff);
	fixStrBuff[FIXSTR_BUFLEN-1] = '\n';
	m_leftStringFromFile = CString(fixStrBuff);
	fscanf(fp, formatStr, fixStrBuff);
	fixStrBuff[FIXSTR_BUFLEN-1] = '\n';
	m_rightStringFromFile = CString(fixStrBuff);
	fclose(fp);
}

void CGratings::SaveFile(bool bDefault) 
{
	CString filename = CString("");

	//debug access violation problems
	if (false){
		if (defaultFile.IsEmpty()) {
			AfxMessageBox("empty defaultFile");
		} 
		else {
			AfxMessageBox("not empty");
			//AfxMessageBox(defaultFile.GetBuffer());
		}
	}

	if (bDefault) {
		filename = CString("C:\\Stimulus Settings\\Gratings\\default.txt");//defaultFile;
	} 
	else {
		filename = etu::BrowseForFile(false, ".txt", "Text Files (*.txt)|*.txt", filename, false);
	}

	FILE* fp = fopen(filename, "wt");
	if (!fp) {
		AfxMessageBox("Error saving file - file or folder not found");
		return;
	}

	fprintf(fp, "%d\n", iPhase);
	fprintf(fp, "%d\n", iAngle);
	fprintf(fp, "%d\n", iType);
	fprintf(fp, "%f\n", 0.0);
	fprintf(fp, "%f\n", 0.0);
	fprintf(fp, "%f\n", fDotSize);
	fprintf(fp, "%f\n", fGratingWidth);
	fprintf(fp, "%d\n", iNumAngles);
	fprintf(fp, "%f\n", fGaborSD);
	fprintf(fp, "%f\n", fStaticPhase);
	fprintf(fp, "%f\n", fStaticAngle);
	fprintf(fp, "%d\n", iDelay);
	fprintf(fp, "%d\n", iBlank);
	fprintf(fp, "%d\n", iNumPhases);
	fprintf(fp, "%d\n", iGratingRed);
	fprintf(fp, "%d\n", iGratingGreen);
	fprintf(fp, "%d\n", iGratingBlue);
	fprintf(fp, "%d\n", iGratingRed2);
	fprintf(fp, "%d\n", iGratingGreen2);
	fprintf(fp, "%d\n", iGratingBlue2);
	fprintf(fp, "%d\n", iNumRepeats);
	if (unitTag.GetLength() <=0) {
		fprintf(fp, "none\n");
	} 
	else {
		fprintf(fp, "%s\n", unitTag);
	}
	fprintf(fp, "%d\n", bBehavior);
	fprintf(fp, "%d\n", iInitiation);
	fprintf(fp, "%d\n", iReward);
	fprintf(fp, "%d\n", iHSteps);
	fprintf(fp, "%d\n", iVSteps);
	fprintf(fp, "%f\n", fStepSize);
	fprintf(fp, "%d\n", bUseSaccades );
	fprintf(fp, "%d\n", iGratingsPerSaccade );
	fprintf(fp, "%d\n", iShutters );
	fprintf(fp, "%d\n", iShutterInterleavedN );
	fprintf(fp, "%d\n", iShutterDelay );
	fprintf(fp, "%f\n", 0.0);
	fprintf(fp, "%f\n", 0.0);
	fprintf(fp, "%f\n", fRFX);
	fprintf(fp, "%f\n", fRFY);
	fprintf(fp, "%f\n", fDevX);
	fprintf(fp, "%f\n", fDevY);
	fprintf(fp, "%f\n", fSpeed);
	fprintf(fp, "%d\n", iRFEye);
	fprintf(fp, "%f\n", fInitialSize);
	fprintf(fp, "%f\n", fFinalSize);

	fprintf(fp, "%s\n", (LPCTSTR)(getLeftFixationString()));
	fprintf(fp, "%s\n", (LPCTSTR)(getRightFixationString()));

	fclose(fp);
}

void CGratings::sendSettings(CSerialComm& theComm) 
{
	char buffer[80];
	const int code = 0xC8; // 200
	
	/* time_t is 64 bits.  The %ld conversion expected long, which is only
	 * guaranteed to be 32 bits.  Cast to long so they match.  This will
	 * stop working truncate needed info in about 20 years. */
	long tm = (long)time(NULL);

	sprintf(buffer, "Gratings;Phase;%d;%ld|0x%02x/\0", iPhase, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Gratings;Angle;%d;%ld|0x%02x/\0", iAngle, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Gratings;Type;%d;%ld|0x%02x/\0", iType, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Gratings;DotSize;%f;%ld|0x%02x/\0", fDotSize, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Gratings;GratingWidth;%f;%ld|0x%02x/\0", fGratingWidth, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Gratings;NumAngles;%d;%ld|0x%02x/\0", iNumAngles, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Gratings;GaborSD;%f;%ld|0x%02x/\0", fGaborSD, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Gratings;StaticPhase;%f;%ld|0x%02x/\0", fStaticPhase, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Gratings;StaticAngle;%f;%ld|0x%02x/\0", fStaticAngle, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Gratings;Delay;%d;%ld|0x%02x/\0", iDelay, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Gratings;Blank;%d;%ld|0x%02x/\0", iBlank, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Gratings;NumPhases;%d;%ld|0x%02x/\0", iNumPhases, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Gratings;GratingRed;%d;%ld|0x%02x/\0", iGratingRed, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Gratings;GratingGreen;%d;%ld|0x%02x/\0", iGratingGreen, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Gratings;GratingBlue;%d;%ld|0x%02x/\0", iGratingBlue, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Gratings;GratingRed2;%d;%ld|0x%02x/\0", iGratingRed2, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Gratings;GratingGreen2;%d;%ld|0x%02x/\0", iGratingGreen2, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Gratings;GratingBlue2;%d;%ld|0x%02x/\0", iGratingBlue2, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Gratings;NumRepeats;%d;%ld|0x%02x/\0", iNumRepeats, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Gratings;unitTag;%15s;%ld|0x%02x/\0", unitTag, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Gratings;bBehavior;%d;%ld|0x%02x/\0", bBehavior, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Gratings;Initiation;%d;%ld|0x%02x/\0", iInitiation, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Gratings;Reward;%d;%ld|0x%02x/\0", iReward, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Gratings;HSteps;%d;%ld|0x%02x/\0", iHSteps, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Gratings;VSteps;%d;%ld|0x%02x/\0", iVSteps, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Gratings;StepSize;%f;%ld|0x%02x/\0", fStepSize, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Gratings;UseSaccades;%d;%ld|0x%02x/\0", bUseSaccades, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Gratings;GratingsPerSaccade;%d;%ld|0x%02x/\0", iGratingsPerSaccade, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Gratings;Shutters;%d;%ld|0x%02x/\0", iShutters, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Gratings;ShutterInterleavedN;%d;%ld|0x%02x/\0", iShutterInterleavedN, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Gratings;ShutterDelay;%d;%ld|0x%02x/\0", iShutterDelay, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	
	sprintf(buffer, "Settings;BGColorR;%d;%ld|0x%02x/\0", g_pSettings->getRedBG(), tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Settings;BGColorG;%d;%ld|0x%02x/\0", g_pSettings->getGreenBG(), tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Settings;BGColorB;%d;%ld|0x%02x/\0", g_pSettings->getBlueBG(), tm, code);		theComm.transmit(buffer, (int)strlen(buffer));

	sprintf(buffer, "Gratings;RFX;%.2f;%ld|0x%02x/\0", fRFX, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Gratings;RFY;%.2f;%ld|0x%02x/\0", fRFY, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Gratings;DevX;%.2f;%ld|0x%02x/\0", fDevX, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Gratings;DevY;%.2f;%ld|0x%02x/\0", fDevY, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Gratings;Speed;%f;%ld|0x%02x/\0", fSpeed, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Gratings;RFEye;%d;%ld|0x%02x/\0", iRFEye, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Gratings;InitialSize;%.2f;%ld|0x%02x/\0", fInitialSize, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Gratings;FinalSize;%.2f;%ld|0x%02x/\0", fFinalSize, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));	
}

void CGratings::GenerateTrials() 
{
	int totalphases = (iPhase == TYPE_STATIC)? 1 : iNumPhases;
	int totalangles = (iAngle == TYPE_STATIC)? 1 : iNumAngles;
	int totalpresentationstates = (iType != STYLE_GABOR)? 4 : 8;
	int totalrepeats = (iNumRepeats<1)? 1: iNumRepeats;
	int totallen = totalrepeats*totalphases*totalangles*totalpresentationstates;
	CGratingsTrial **finallist = new CGratingsTrial* [totallen];
	CGratingsTrial **templist;
	int finalcount = 0;

	for (int ri=0; ri<totalrepeats; ri++) {
		int count = 0;
		templist =  new CGratingsTrial* [totallen/totalrepeats];
		for (int pi=0; pi<totalphases; pi++) {
			for (int ai=0; ai<totalangles; ai++) {
				for (int psi=0; psi<totalpresentationstates; psi++) {//presentation states
					int trialtype = 0;
					if (iType == STYLE_SQUARE) {
						trialtype = CGratingsTrial::TYPE_SQUAREWAVE;
					} 
					else if (iType == STYLE_SINE) {
						trialtype = CGratingsTrial::TYPE_SINEWAVE;
					} 
					else {//STYLE_GABOR
						if ( (psi==0) || (psi==4) || (psi==7) ){
							trialtype = CGratingsTrial::TYPE_GABOR_LEFT;
						} 
						else if ( (psi==1) || (psi==5) || (psi==6) ) {
							trialtype = CGratingsTrial::TYPE_GABOR_RIGHT;
						} 
						else {
							trialtype = CGratingsTrial::TYPE_GABOR_BOTH;
						}
					}
					templist[count] = new CGratingsTrial(
						(iPhase == TYPE_STATIC)? fStaticPhase : 360.0*(double)pi/(double)totalphases,
						(iAngle == TYPE_STATIC)? fStaticAngle : 360.0*(double)ai/(double)totalangles,
						fSpeed,
						(psi != 1),
						(psi != 0),
						(psi%2)==0,
						trialtype
						);
					count++;
				}
			}
		}

		//shuffle this repeat and save it to the list
		CGratingsTrial::shuffleRandom(templist, count);
		for (int i=0; i<count; i++) {
			finallist[finalcount] = templist[i];
			finalcount++;
		}
		delete [] templist;
	}

	//debug
	if (false) {
		std::ofstream fout("c:\\documents and settings\\feusnerm\\Desktop\\etdebuglog.txt", std::ios::app);
		//output counts
		//fout << "n\t" << g_pScotomaMap->getNumLocations() << std::endl;
		//fout << "h\t" << g_pScotomaMap->getNumStepsH() << std::endl;
		//fout << "v\t" << g_pScotomaMap->getNumStepsV() << std::endl;
		//output list of trials
		for (int i=0; i<totallen; i++) {	
			fout << i << "\t"
				<< finallist[i]->phase << "\t"
				<< finallist[i]->orientation << "\t"
				<< finallist[i]->speed << "\t"
				<< ((finallist[i]->leftEyeFixating)? "left" : "right") << "\t"
				<< ((finallist[i]->leftShutterOpen)? "shutterlopen" : "shutterlclosed") << "\t"
				<< ((finallist[i]->rightShutterOpen)? "shutterropen" : "shutterrclosed")
				<< std::endl;
		}
		fout.close();
	}

	//clean up old list
	if (trials != NULL) {
		for (int i=0; i<numTrials; i++) {
			delete trials[i];
			trials[i] = NULL;
		}
		delete [] trials;
		trials = NULL;
	}
	//assign new list
	trials = finallist;
	numTrials = totallen;
}

void CGratings::GenerateRFMappingTrials() 
{
	int totalphases = (iPhase == TYPE_STATIC)? 1 : iNumPhases;
	int totalangles = (iAngle == TYPE_STATIC)? 1 : iNumAngles;
	int totalrepeats = (iNumRepeats<1)? 1: iNumRepeats;
	int numdivisions = 4;
	//fFinalSize * 2^numsteps = fInitialSize
	//	2^numsteps = fInitialSize/fFinalSize
	//	log(2^numsteps) = log(fInitialSize/fFinalSize)
	//	numsteps*log(2) = log(fInitialSize/fFinalSize)
	int numsteps = 1 + (int)ceil(  log(fInitialSize/fFinalSize) / log(2.0)  );

	int totallen = totalrepeats*totalphases*totalangles*numdivisions*numsteps;
	CGratingsTrial **finallist = new CGratingsTrial* [totallen];
	CGratingsTrial **templist;
	int finalcount = 0;

	for (int si=0; si<numsteps; si++) {
		int count = 0;
		templist =  new CGratingsTrial* [totallen/numsteps];

		for (int ri=0; ri<totalrepeats; ri++) {
			for (int pi=0; pi<totalphases; pi++) {
				for (int ai=0; ai<totalangles; ai++) {
					for (int i=0; i<numdivisions; i++) {
						templist[count] = new CGratingsTrial(
							(iPhase == TYPE_STATIC)? fStaticPhase : 360.0*(double)pi/(double)totalphases,
							(iAngle == TYPE_STATIC)? fStaticAngle : 360.0*(double)ai/(double)totalangles,
							fSpeed,
							iRFEye==0,
							iRFEye!=0,
							iRFEye==0,
							CGratingsTrial::TYPE_AUTORFMAP
							);
						templist[count]->locationCode = 1+i;
						count++;
					}
				}
			}
		}

		//shuffle this repeat and save it to the list
		CGratingsTrial::shuffleRandom(templist, count);
		for (int i=0; i<count; i++) {
			finallist[finalcount] = templist[i];
			finalcount++;
		}
		delete [] templist;
	}
		
	trials = finallist;
	numTrials = totallen;
}

CString const CGratings::defaultFile("C:\\Stimulus Settings\\Gratings\\default.txt");

const CString CGratings::getUnitTag() 
{
	return CString("");
}

void CGratings::setUnitTag(CString cstr) {}

const CSmartCoord* CGratings::getRandomLeftFixation()
{ 
	if (m_numLeftFixationPoints <= 0) {
		AfxMessageBox("Empty left fixation point list");
		return NULL;
	}
	int ix = etu::RandChoice(0, m_numLeftFixationPoints - 1);
	return m_leftFixationPoints[ix];
}

const CSmartCoord* CGratings::getRandomRightFixation()
{ 
	if (m_numRightFixationPoints <= 0) {
		AfxMessageBox("Empty right fixation point list");
		return NULL;
	}
	int ix = etu::RandChoice(0, m_numRightFixationPoints - 1);
	return m_rightFixationPoints[ix];
}

/**
 * @brief Scan the string and allocate point objects, storing them in coords.
 *
 * @param fixationStr A string with coordinates in the form "(x,y)". White
 *        is ignored. The open paren, comma, and closing paren are mandatory.
 * @param[out] coords Reference to variable which receives an array of pointers
 *        to coordinate objects allocated on the heap. The array is allocated 
 *        on the heap as well.
 */
void CGratings::populatePoints(const char* fixationStr, CoordPtrArray_t& coords, size_t& numCoords)
{
	/*
	 * If coords is NULL and numCoords is 0, scan the string and count the number 
	 * of parsable coordinates. Then allocate memory into coords; set numCoords; 
	 * and internally call self with new arguments.
	 * 
	 * If coords is not NULL and numCoords is > 0, scan the string and parse
	 * coordinates into coords.
	 *
	 * Long story short, I tried to use an STL container and got strange access 
	 * violation segfaults that I couldn't fix. I decided to handle memory 
	 * allocation myself.
	 */
	char strCopy[FIXSTR_BUFLEN];
	char* current;

	size_t count = 0;

	strncpy(strCopy, fixationStr, FIXSTR_BUFLEN);
	strCopy[FIXSTR_BUFLEN - 1] = '\0';
	current = strCopy;

	while (1) {
		char* openParen = strchr(current, '(');
		char* comma = strchr(current, ',');
		char* closeParen = strchr(current, ')');

		if (openParen && comma && closeParen) {
			double xx, yy;
			char* num1 = NULL;
			char* num2 = NULL;
			*openParen = '\0';
			*comma = '\0';
			*closeParen = '\0';
			xx = strtod(openParen + 1, &num1);
			yy = strtod(comma + 1, &num2);
			if ((num1 == comma) && (num2 == closeParen)) {
				/* Parsed a coordinate; take action depending on arguments. */
				if (!coords && (numCoords == 0)) {
					count++;
				}
				else {
					coords[count++] = new CSmartCoord(xx, yy);
				}

				current = (closeParen + 1);
				continue;
			}
			break;
		}
		break;
	}

	if (!coords && (numCoords == 0)) {
		numCoords = count;
		coords = new CSmartCoord*[numCoords];
		populatePoints(fixationStr, coords, numCoords);
	}
}

void CGratings::clearPoints(CoordPtrArray_t& coords, size_t& numCoords)
{
	if (coords && (numCoords > 0)) {
		for (int ix = 0; ix < numCoords; ix++) {
			delete coords[ix];
		}
	}
	delete coords;
	coords = NULL;
	numCoords = 0;
}
	
/**
 * @brief Buffer must be FIXSTR_BUFLEN in length, including \0.
 */
void CGratings::setLeftFixations(const char* fixationStr)
{
	clearPoints(m_leftFixationPoints, m_numLeftFixationPoints);
	populatePoints(fixationStr, m_leftFixationPoints, m_numLeftFixationPoints);
}

/**
 * @brief Buffer must be FIXSTR_BUFLEN in length, including \0.
 */
void CGratings::setRightFixations(const char* fixationStr)
{
	clearPoints(m_rightFixationPoints, m_numRightFixationPoints);
	populatePoints(fixationStr, m_rightFixationPoints, m_numRightFixationPoints);
}

CString CGratings::constructString(CoordPtrArray_t& coords, size_t numCoords)
{
	CString out;
	if (coords && (numCoords > 0)) {
		for (int ix = 0; ix < numCoords; ix++) {
			out.AppendFormat("(%.1f,%.1f) ", coords[ix]->xDeg(), coords[ix]->yDeg());;
		}
	}
	return out;
}

CString CGratings::getLeftFixationString()
{
	if ((!m_numLeftFixationPoints) && (m_leftStringFromFile.GetLength())) {
		/* We've just launched the program and read the string from file. We
		 * could not parse it at that time because VSG wasn't up. Supply the
		 * string, because the GUI is now requesting it; then erase it,
		 * so this is a one-shot deal. */
		CString out(m_leftStringFromFile);
		m_leftStringFromFile = CString("");
		return out;
	}
	else {
		return constructString(m_leftFixationPoints, m_numLeftFixationPoints);
	}
}

CString CGratings::getRightFixationString()
{
	if ((!m_numRightFixationPoints) && (m_rightStringFromFile.GetLength())) {
		CString out(m_rightStringFromFile);
		m_rightStringFromFile = CString("");
		return out;
	}
	else {
		return constructString(m_rightFixationPoints, m_numRightFixationPoints);
	}
}

#endif // APP_EYE_TRACKING


