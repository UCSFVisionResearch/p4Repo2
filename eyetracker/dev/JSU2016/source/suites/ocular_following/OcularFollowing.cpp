/**
 * @file OcularFollowing.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <afx.h>
#include <math.h>
#include "OcularFollowing.h"
#include "comm/SerialComm.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

COcularFollowing::COcularFollowing()
{
	defaultFile = CString("C:\\Stimulus Settings\\OcularFollowing\\default.txt");
	fSize = 1.0f;
	bDirections[0] = false;
	bDirections[1] = false;
	bDirections[2] = true;
	bDirections[3] = true;
	xPosition = 0;
	yPosition = 0;
	iBkgSpeed = 50;
	iTimeOut = 100;
	iBkgDuration = 200;
	iLatency = 50;
	bBehavior=true;
	fMinRadius=0.0;
	fMaxRadius=10.0;
	bRandom=false;
	iNoiseSize=16;
	iNoisePct=50;
	iNoiseContrast=100;

	LoadFile(true);
}

COcularFollowing::~COcularFollowing() {}

int COcularFollowing::getDirection()
{ 
	int i = 0;
	bool bDirExists = false;
	for (int n = 0; n < 4; n++) {
		if (bDirections[n])
			bDirExists = true;
	}
	if (!bDirExists) {
		AfxMessageBox("No Direction Selected");
		return -1;
	}

	do {
	  i = rand() % 4;
	} while (!bDirections[i]);
	return i;
}

void COcularFollowing::LoadFile(bool bDefault)
{
	CString filename = defaultFile;
	if (!bDefault) {
		filename = etu::BrowseForFile(true, ".txt", "Text Files (*.txt)|*.txt", filename, false);
	}

	char buffer[1024];
	sprintf(buffer, "%s", filename);
	FILE* fp = fopen(filename, "rt");
	if (!fp) {
		AfxMessageBox("Error loading file - file or folder not found");
		return;
	}
	
	fscanf(fp, "%f\n", &fSize);
	fscanf(fp, "%d\n", &(bDirections[0]));
	fscanf(fp, "%d\n", &(bDirections[1]));
	fscanf(fp, "%d\n", &(bDirections[2]));
	fscanf(fp, "%d\n", &(bDirections[3]));
	fscanf(fp, "%f\n", &xPosition);
	fscanf(fp, "%f\n", &yPosition);
	fscanf(fp, "%d\n", &iBkgSpeed);
	fscanf(fp, "%d\n", &iTimeOut);
	fscanf(fp, "%d\n", &iBkgDuration);
	fscanf(fp, "%d\n", &iLatency);
	fscanf(fp, "%d\n", &bBehavior);
	fscanf(fp, "%f\n", &fMinRadius);
	fscanf(fp, "%f\n", &fMaxRadius);
	fscanf(fp, "%d\n", &bRandom);
	fscanf(fp, "%d\n", &iNoiseSize);
	fscanf(fp, "%d\n", &iNoisePct);
	fscanf(fp, "%d\n", &iNoiseContrast);

	fclose(fp);
}

void COcularFollowing::SaveFile(bool bDefault)
{
	CString filename = defaultFile;
	if (!bDefault) {
		filename = etu::BrowseForFile(false, ".txt", "Text Files (*.txt)|*.txt", filename, false);
	}

	char buffer[1024];
	sprintf(buffer, "%s", filename);
	FILE* fp = fopen(filename, "wt");
	if (!fp) {
		AfxMessageBox("Error saving file - file or folder not found");
		return;
	}
	
	fprintf(fp, "%f\n", fSize);
	fprintf(fp, "%d\n", bDirections[0]);
	fprintf(fp, "%d\n", bDirections[1]);
	fprintf(fp, "%d\n", bDirections[2]);
	fprintf(fp, "%d\n", bDirections[3]);
	fprintf(fp, "%f\n", xPosition);
	fprintf(fp, "%f\n", yPosition);
	fprintf(fp, "%d\n", iBkgSpeed);
	fprintf(fp, "%d\n", iTimeOut);
	fprintf(fp, "%d\n", iBkgDuration);
	fprintf(fp, "%d\n", iLatency);
	fprintf(fp, "%d\n", bBehavior);
	fprintf(fp, "%f\n", fMinRadius);
	fprintf(fp, "%f\n", fMaxRadius);
	fprintf(fp, "%d\n", bRandom);
	fprintf(fp, "%d\n", iNoiseSize);
	fprintf(fp, "%d\n", iNoisePct);
	fprintf(fp, "%d\n", iNoiseContrast);
	
	fclose(fp);
}

void COcularFollowing::sendSettings(CSerialComm& theComm)
{
	char buffer[80];
	const int code = 0xC8; // 200
	
	/* time_t is 64 bits.  The %ld conversion expected long, which is only
	 * guaranteed to be 32 bits.  Cast to long so they match.  This will
	 * stop working truncate needed info in about 20 years. */
	long tm = (long)time(NULL);

	sprintf(buffer, "OcularFollowing;BkgSpeed;%d;%ld|0x%02x/\0", iBkgSpeed, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "OcularFollowing;Latency;%d;%ld|0x%02x/\0", iLatency, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "OcularFollowing;BkgDuration;%d;%ld|0x%02x/\0", iBkgDuration, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "OcularFollowing;TimeOut;%d;%ld|0x%02x/\0", iTimeOut, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "OcularFollowing;Behavior;%d;%ld|0x%02x/\0", bBehavior, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "OcularFollowing;xPosition;%.4f;%ld|0x%02x/\0", xPosition, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "OcularFollowing;yPosition;%.4f;%ld|0x%02x/\0", yPosition, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "OcularFollowing;Size;%.4f;%ld|0x%02x/\0", fSize, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "OcularFollowing;MinRadius;%.4f;%ld|0x%02x/\0", fMinRadius, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "OcularFollowing;MaxRadius;%.4f;%ld|0x%02x/\0", fMaxRadius, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "OcularFollowing;DirectionUp;%d;%ld|0x%02x/\0", bDirections[0], tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "OcularFollowing;DirectionDown;%d;%ld|0x%02x/\0", bDirections[1], tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "OcularFollowing;DirectionLeft;%d;%ld|0x%02x/\0", bDirections[2], tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "OcularFollowing;DirectionRight;%d;%ld|0x%02x/\0", bDirections[3], tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "OcularFollowing;Random;%d;%ld|0x%02x/\0", bRandom, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "OcularFollowing;NoiseSize;%d;%ld|0x%02x/\0", iNoiseSize, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "OcularFollowing;NoisePct;%d;%ld|0x%02x/\0", iNoisePct, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "OcularFollowing;NoiseCont;%d;%ld|0x%02x/\0", iNoiseContrast, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
}
