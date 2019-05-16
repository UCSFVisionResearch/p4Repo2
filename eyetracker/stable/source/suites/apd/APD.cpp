/**
 * @file APD.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <afx.h>
#include "APD.h"
#include "comm/SerialComm.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

CAPD::CAPD() 
{
	defaultFile = CString("C:\\Stimulus Settings\\APD\\default.txt");
	
	iOnTime = 1000;
	iOffTime = 1000;
	fFixationSize = 0.25;
	fFixationPosX = 0; 
	fFixationPosY = 0;

	iFixationRed = 255;
	iFixationGreen = 0;
	iFixationBlue = 0;

	iOnRed = 255;
	iOnGreen = 255;
	iOnBlue = 255;

	iOffRed = 0;
	iOffGreen = 0;
	iOffBlue = 0;
	
	bShowFixation = true;
	iIntertrialDelay = 1000;
	iFixationDelay = 200;

	iField = 3;

	dFieldWidth = 15;
	dFieldHeight = 15;
	dCenterXLeft = -10;
	dCenterYLeft = 0;
	dCenterXRight = 10;
	dCenterYRight = 0;

	LoadFile(true);
}

CAPD::~CAPD() {}

void CAPD::LoadFile(bool bDefault) 
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
	
	fscanf(fp, "%d\n", &iOnTime);
	fscanf(fp, "%d\n", &iOffTime);
	fscanf(fp, "%f\n", &fFixationSize);
	fscanf(fp, "%f\n", &fFixationPosX);
	fscanf(fp, "%f\n", &fFixationPosY);
	fscanf(fp, "%d\n", &iFixationRed);
	fscanf(fp, "%d\n", &iFixationGreen);
	fscanf(fp, "%d\n", &iFixationBlue);

	fscanf(fp, "%d\n", &iOnRed);
	fscanf(fp, "%d\n", &iOnGreen);
	fscanf(fp, "%d\n", &iOnBlue);

	fscanf(fp, "%d\n", &iOffRed);
	fscanf(fp, "%d\n", &iOffGreen);
	fscanf(fp, "%d\n", &iOffBlue);
	fscanf(fp, "%d\n", &bShowFixation);

	fscanf(fp, "%d\n", &iField);
	fscanf(fp, "%d\n", &iIntertrialDelay);
	fscanf(fp, "%d\n", &iFixationDelay);

	fscanf(fp, "%f\n", &dFieldWidth);
	fscanf(fp, "%f\n", &dFieldHeight);

	fscanf(fp, "%f\n", &dCenterXRight);
	fscanf(fp, "%f\n", &dCenterYRight);

	fscanf(fp, "%f\n", &dCenterXLeft);
	fscanf(fp, "%f\n", &dCenterYLeft);

	fscanf(fp, "%d\n", &bBehavior);

	fclose(fp);
}

void CAPD::SaveFile(bool bDefault)
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
	
	fprintf(fp, "%d\n", iOnTime);
	fprintf(fp, "%d\n", iOffTime);
	fprintf(fp, "%f\n", fFixationSize);
	fprintf(fp, "%f\n", fFixationPosX);
	fprintf(fp, "%f\n", fFixationPosY);
	fprintf(fp, "%d\n", iFixationRed);
	fprintf(fp, "%d\n", iFixationGreen);
	fprintf(fp, "%d\n", iFixationBlue);

	fprintf(fp, "%d\n", iOnRed);
	fprintf(fp, "%d\n", iOnGreen);
	fprintf(fp, "%d\n", iOnBlue);

	fprintf(fp, "%d\n", iOffRed);
	fprintf(fp, "%d\n", iOffGreen);
	fprintf(fp, "%d\n", iOffBlue);

	fprintf(fp, "%d\n", bShowFixation);

	fprintf(fp, "%d\n", iField);
	fprintf(fp, "%d\n", iIntertrialDelay);
	fprintf(fp, "%d\n", iFixationDelay);

	fprintf(fp, "%f\n", dFieldWidth);
	fprintf(fp, "%f\n", dFieldHeight);

	fprintf(fp, "%f\n", dCenterXRight);
	fprintf(fp, "%f\n", dCenterYRight);

	fprintf(fp, "%f\n", dCenterXLeft);
	fprintf(fp, "%f\n", dCenterYLeft);

	fprintf(fp, "%d\n", bBehavior);

	fclose(fp);
}

void CAPD::sendSettings(CSerialComm& theComm) 
{
	char buffer[80];
	const int code = 0xC8; // 200
	
	/* time_t is 64 bits.  The %ld conversion expected long, which is only
	 * guaranteed to be 32 bits.  Cast to long so they match.  This will
	 * stop working truncate needed info in about 20 years. */
	long tm = (long)time(NULL);

	sprintf(buffer, "APD;ShowFixation;%d;%ld|0x%02x/\0", bShowFixation, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "APD;FixationPosX;%.4f;%ld|0x%02x/\0", fFixationPosX, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "APD;FixationPosY;%.4f;%ld|0x%02x/\0", fFixationPosY, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "APD;FixationSize;%.4f;%ld|0x%02x/\0", fFixationSize, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "APD;FixationRed;%d;%ld|0x%02x/\0", iFixationRed, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "APD;FixationGreen;%d;%ld|0x%02x/\0", iFixationGreen, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "APD;FixationBlue;%d;%ld|0x%02x/\0", iFixationBlue, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "APD;Field;%d;%ld|0x%02x/\0", iField, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "APD;OnRed;%d;%ld|0x%02x/\0", iOnRed, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "APD;OnGreen;%d;%ld|0x%02x/\0", iOnGreen, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "APD;OnBlue;%d;%ld|0x%02x/\0", iOnBlue, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "APD;OffRed;%d;%ld|0x%02x/\0", iOffRed, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "APD;OffGreen;%d;%ld|0x%02x/\0", iOffGreen, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "APD;OffBlue;%d;%ld|0x%02x/\0", iOffBlue, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "APD;OnTime;%d;%ld|0x%02x/\0", iOnTime, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "APD;OffTime;%d;%ld|0x%02x/\0", iOffTime, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "APD;IntertrialDelay;%d;%ld|0x%02x/\0", iIntertrialDelay, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "APD;FixationDelay;%d;%ld|0x%02x/\0", iFixationDelay, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "APD;Behavior;%d;%ld|0x%02x/\0", bBehavior, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "APD;FieldWidth;%.4f;%ld|0x%02x/\0", dFieldWidth, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "APD;FieldHeight;%.4f;%ld|0x%02x/\0", dFieldHeight, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "APD;CenterXLeft;%.4f;%ld|0x%02x/\0", dCenterXLeft, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "APD;CenterYLeft;%.4f;%ld|0x%02x/\0", dCenterYLeft, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "APD;CenterXRight;%.4f;%ld|0x%02x/\0", dCenterXRight, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "APD;CenterYRight;%.4f;%ld|0x%02x/\0", dCenterYRight, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
}
