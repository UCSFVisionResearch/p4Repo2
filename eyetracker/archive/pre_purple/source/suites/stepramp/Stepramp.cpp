/**
 * @file Stepramp.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <afx.h>
#include <math.h>
#include "Stepramp.h"
#include "comm/SerialComm.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

CStepRamp::CStepRamp() 
{
	defaultFile = CString("C:\\Stimulus Settings\\Stepramp\\default.txt");
	fStepSize = 3;
	fVelocity = 10;
	iDirection = DIR_RT;
	fExcursion = 15;
	iOnDelay = 1000;
	iOffDelay = 1000;
	fDotSize = 0.25;
	fStepX = 1; 
	fStepY = 0;
	fStartXDeg = -fStepSize;
	fStartYDeg = 0;
	iPlateau = 0;
	bBehavior = false;

	bStepUp = false;
	bStepDown = false;
	bStepLeft = false;
	bStepRight = true;

	fStartXUp = 0;
	fStartYUp = 0;
	fStartXDown = 0;
	fStartYDown = 0;
	fStartXLeft = 0;
	fStartYLeft = 0;
	fStartXRight = 0;
	fStartYRight = 0;
	fFixationX = 0;
	fFixationY = 0;
	fFixationX = 0; fFixationY = 0;
	LoadFile(true);
}

CStepRamp::~CStepRamp() {}

void CStepRamp::chooseDirection() 
{
    int choices[4];
	int iCurrIndex = 0;

	if (bStepUp) {
		choices[iCurrIndex] = DIR_UP;
		iCurrIndex++;
	}
	if (bStepDown) {
		choices[iCurrIndex] = DIR_DN;
		iCurrIndex++;
	}
	if (bStepLeft) {
		choices[iCurrIndex] = DIR_LT;
		iCurrIndex++;
	}
	if (bStepRight) {
		choices[iCurrIndex] = DIR_RT;
		iCurrIndex++;
	}
  
	setDirection(choices[rand() % iCurrIndex]);
}

float CStepRamp::getFixationX() 
{
	switch (iDirection) {
		case DIR_UP: return fStartXUp; break;
		case DIR_DN: return fStartXDown; break;
		case DIR_LT: return fStartXLeft; break;
		default: return fStartXRight; break;//DIR_RT
	}
}

float CStepRamp::getFixationY() 
{
	switch (iDirection) {
		case DIR_UP: return fStartYUp; break;
		case DIR_DN: return fStartYDown; break;
		case DIR_LT: return fStartYLeft; break;
		default: return fStartYRight; break;//DIR_RT
	}
}

void CStepRamp::setDirection(int i) 
{ 
	iDirection = i; 
	switch (iDirection) {
		case DIR_UP: fStepX = 0; fStepY = -1; fStartXDeg = 0; fStartYDeg = -fStepSize; break;
		case DIR_DN: fStepX = 0; fStepY = 1;  fStartXDeg = 0; fStartYDeg = fStepSize; break;
		case DIR_LT: fStepX = -1; fStepY = 0;  fStartYDeg = 0; fStartXDeg = fStepSize; break;
		case DIR_RT: fStepX = 1; fStepY = 0;  fStartYDeg = 0; fStartXDeg = -fStepSize; break;
	}
}

void CStepRamp::LoadFile(bool bDefault) 
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

	fscanf(fp, "%f\n", &fStepSize);
	fscanf(fp, "%f\n", &fVelocity);
	fscanf(fp, "%d\n", &iDirection);
	fscanf(fp, "%f\n", &fExcursion);
	fscanf(fp, "%d\n", &iOnDelay);
	fscanf(fp, "%d\n", &iOffDelay);
	fscanf(fp, "%f\n", &fDotSize);
	fscanf(fp, "%f\n", &fStepX);
	fscanf(fp, "%f\n", &fStepY);
	fscanf(fp, "%f\n", &fStartXDeg);
	fscanf(fp, "%f\n", &fStartYDeg);
	fscanf(fp, "%d\n", &iPlateau);
	fscanf(fp, "%d\n", &bBehavior);

	fscanf(fp, "%d\n", &bStepUp);
	fscanf(fp, "%d\n", &bStepDown);
	fscanf(fp, "%d\n", &bStepLeft);
	fscanf(fp, "%d\n", &bStepRight);

	fscanf(fp, "%f\n", &fStartXUp);
	fscanf(fp, "%f\n", &fStartYUp);
	fscanf(fp, "%f\n", &fStartXDown);
	fscanf(fp, "%f\n", &fStartYDown);
	fscanf(fp, "%f\n", &fStartXRight);
	fscanf(fp, "%f\n", &fStartYRight);
	fscanf(fp, "%f\n", &fStartXLeft);
	fscanf(fp, "%f\n", &fStartYLeft);

	fclose(fp);
}

void CStepRamp::SaveFile(bool bDefault) 
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
	
	fprintf(fp, "%f\n", fStepSize);
	fprintf(fp, "%f\n", fVelocity);
	fprintf(fp, "%d\n", iDirection);
	fprintf(fp, "%f\n", fExcursion);
	fprintf(fp, "%d\n", iOnDelay);
	fprintf(fp, "%d\n", iOffDelay);
	fprintf(fp, "%f\n", fDotSize);
	fprintf(fp, "%f\n", fStepX);
	fprintf(fp, "%f\n", fStepY);
	fprintf(fp, "%f\n", fStartXDeg);
	fprintf(fp, "%f\n", fStartYDeg);
	fprintf(fp, "%d\n", iPlateau);
	fprintf(fp, "%d\n", bBehavior);

	fprintf(fp, "%d\n", bStepUp);
	fprintf(fp, "%d\n", bStepDown);
	fprintf(fp, "%d\n", bStepLeft);
	fprintf(fp, "%d\n", bStepRight);

	fprintf(fp, "%f\n", fStartXUp);
	fprintf(fp, "%f\n", fStartYUp);
	fprintf(fp, "%f\n", fStartXDown);
	fprintf(fp, "%f\n", fStartYDown);
	fprintf(fp, "%f\n", fStartXRight);
	fprintf(fp, "%f\n", fStartYRight);
	fprintf(fp, "%f\n", fStartXLeft);
	fprintf(fp, "%f\n", fStartYLeft);

	fclose(fp);
}

void CStepRamp::sendSettings(CSerialComm& theComm) 
{
	char buffer[80];
	const int code = 0xC8; // 200
	
	/* time_t is 64 bits.  The %ld conversion expected long, which is only
	 * guaranteed to be 32 bits.  Cast to long so they match.  This will
	 * stop working truncate needed info in about 20 years. */
	long tm = (long)time(NULL);
	
	sprintf(buffer, "StepRamp;DotSize;%.4f;%ld|0x%02x/\0", fDotSize, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "StepRamp;StepSize;%.4f;%ld|0x%02x/\0", fStepSize, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "StepRamp;Velocity;%.4f;%ld|0x%02x/\0", fVelocity, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "StepRamp;Excursion;%.4f;%ld|0x%02x/\0", fExcursion, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "StepRamp;OnDelay;%d;%ld|0x%02x/\0", iOnDelay, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "StepRamp;OffDelay;%d;%ld|0x%02x/\0", iOffDelay, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "StepRamp;Plateau;%d;%ld|0x%02x/\0", iPlateau, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "StepRamp;Direction;%d;%ld|0x%02x/\0", iDirection, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "StepRamp;Behavior;%d;%ld|0x%02x/\0", bBehavior, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));

	sprintf(buffer, "StepRamp;StartXUp;%.4f;%ld|0x%02x/\0", fStartXUp, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "StepRamp;StartYUp;%.4f;%ld|0x%02x/\0", fStartYUp, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "StepRamp;StartXDown;%.4f;%ld|0x%02x/\0", fStartXDown, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "StepRamp;StartYDown;%.4f;%ld|0x%02x/\0", fStartYDown, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "StepRamp;StartXRight;%.4f;%ld|0x%02x/\0", fStartXRight, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "StepRamp;StartYRight;%.4f;%ld|0x%02x/\0", fStartYRight, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "StepRamp;StartXLeft;%.4f;%ld|0x%02x/\0", fStartXLeft, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "StepRamp;StartYLeft;%.4f;%ld|0x%02x/\0", fStartYLeft, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
}
