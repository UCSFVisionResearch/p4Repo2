/**
 * @file Amblyoscope.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include "Amblyoscope.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

CAmblyoscope::CAmblyoscope() 
{
	defaultFile = CString("C:\\Stimulus Settings\\Amblyoscope\\default.txt");
	
	iXOffset = 0;
	iYOffset = 0;
	iPicture = 0;
	iLRed = 0;
	iLGreen = 0;
	iLBlue = 255;
	iRRed = 255;
	iRGreen = 0;
	iRBlue = 0;
	bBackground = false;
	bFlashTest = false;
	iPctMonocular = 10;
	iFlashDuration = 100;
	iBlankDuration = 200;
	fDeviatedRightX = 5.0;
	fDeviatedRightY = 1.0;
	fDeviatedLeftX = -5.0;
	fDeviatedLeftY = -1.0;
	iFlashCenter = 2;
	iFlashBlocks = 10;
	iArmLength = 10;
	iArmWidth = 3;
	iSpaceSize = 3;
	bFixationDot = true;
	fFixationX = 0.0;
	fFixationY = 0.0;
	fFixationSize = 0.5;
	fCenterX = 0.0;
	fCenterY = -2.0;
	iBGBrightness = 20;
	bAlternate = false;

	LoadFile(true);
}

CAmblyoscope::~CAmblyoscope() {}

void CAmblyoscope::LoadFile(bool bDefault) 
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

	fscanf(fp, "%d\n", &iXOffset);
	fscanf(fp, "%d\n", &iYOffset);
	fscanf(fp, "%d\n", &iPicture);
	fscanf(fp, "%d\n", &iLRed);
	fscanf(fp, "%d\n", &iLGreen);
	fscanf(fp, "%d\n", &iLBlue);
	fscanf(fp, "%d\n", &iRRed);
	fscanf(fp, "%d\n", &iRGreen);
	fscanf(fp, "%d\n", &iRBlue);
	fscanf(fp, "%d\n", &bBackground);
	fscanf(fp, "%d\n", &bFlashTest);
	fscanf(fp, "%d\n", &iPctMonocular);
	fscanf(fp, "%d\n", &iFlashDuration);
	fscanf(fp, "%d\n", &iBlankDuration);
	fscanf(fp, "%f\n", &fDeviatedRightX);
	fscanf(fp, "%f\n", &fDeviatedRightY);
	fscanf(fp, "%f\n", &fDeviatedLeftX);
	fscanf(fp, "%f\n", &fDeviatedLeftY);
	fscanf(fp, "%d\n", &iFlashCenter);
	fscanf(fp, "%d\n", &iFlashBlocks);
	fscanf(fp, "%d\n", &iArmLength);
	fscanf(fp, "%d\n", &iArmWidth);
	fscanf(fp, "%d\n", &iSpaceSize);
	fscanf(fp, "%d\n", &bFixationDot);
	fscanf(fp, "%f\n", &fFixationX);
	fscanf(fp, "%f\n", &fFixationY);
	fscanf(fp, "%f\n", &fFixationSize);
	fscanf(fp, "%f\n", &fCenterX);
	fscanf(fp, "%f\n", &fCenterY);
	fscanf(fp, "%d\n", &iBGBrightness);
	fscanf(fp, "%d\n", &bAlternate);

	fclose(fp);
}

void CAmblyoscope::SaveFile(bool bDefault) 
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

	fprintf(fp, "%d\n", iXOffset);
	fprintf(fp, "%d\n", iYOffset);
	fprintf(fp, "%d\n", iPicture);
	fprintf(fp, "%d\n", iLRed);
	fprintf(fp, "%d\n", iLGreen);
	fprintf(fp, "%d\n", iLBlue);
	fprintf(fp, "%d\n", iRRed);
	fprintf(fp, "%d\n", iRGreen);
	fprintf(fp, "%d\n", iRBlue);
	fprintf(fp, "%d\n", bBackground);
	fprintf(fp, "%d\n", bFlashTest);
	fprintf(fp, "%d\n", iPctMonocular);
	fprintf(fp, "%d\n", iFlashDuration);
	fprintf(fp, "%d\n", iBlankDuration);
	fprintf(fp, "%f\n", fDeviatedRightX);
	fprintf(fp, "%f\n", fDeviatedRightY);
	fprintf(fp, "%f\n", fDeviatedLeftX);
	fprintf(fp, "%f\n", fDeviatedLeftY);
	fprintf(fp, "%d\n", iFlashCenter);
	fprintf(fp, "%d\n", iFlashBlocks);
	fprintf(fp, "%d\n", iArmLength);
	fprintf(fp, "%d\n", iArmWidth);
	fprintf(fp, "%d\n", iSpaceSize);
	fprintf(fp, "%d\n", bFixationDot);
	fprintf(fp, "%f\n", fFixationX);
	fprintf(fp, "%f\n", fFixationY);
	fprintf(fp, "%f\n", fFixationSize);
	fprintf(fp, "%f\n", fCenterX);
	fprintf(fp, "%f\n", fCenterY);
	fprintf(fp, "%d\n", iBGBrightness);
	fprintf(fp, "%d\n", bAlternate);

	fclose(fp);
}

void CAmblyoscope::sendSettings(CSerialComm& theComm) 
{
	char buffer[80];
	const int code = 0xC8; // 200
	
	/* time_t is 64 bits.  The %ld conversion expected long, which is only
	 * guaranteed to be 32 bits.  Cast to long so they match.  This will
	 * stop working truncate needed info in about 20 years. */
	long tm = (long)time(NULL);

	sprintf(buffer, "Amblyoscope;Picture;%d;%ld|0x%02x/\0", iPicture, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Amblyoscope;Background;%d;%ld|0x%02x/\0", bBackground, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Amblyoscope;PeriphRed;%d;%ld|0x%02x/\0", iLRed, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Amblyoscope;PeriphGreen;%d;%ld|0x%02x/\0", iLGreen, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Amblyoscope;PeriphBlue;%d;%ld|0x%02x/\0", iLBlue, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Amblyoscope;CenterRed;%d;%ld|0x%02x/\0", iRRed, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Amblyoscope;CenterGreen;%d;%ld|0x%02x/\0", iRGreen, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Amblyoscope;CenterBlue;%d;%ld|0x%02x/\0", iRBlue, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Amblyoscope;FlashTest;%d;%ld|0x%02x/\0", bFlashTest, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Amblyoscope;PctMonocular;%d;%ld|0x%02x/\0", iPctMonocular, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Amblyoscope;FlashDuration;%d;%ld|0x%02x/\0", iFlashDuration, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Amblyoscope;BlankDuration;%d;%ld|0x%02x/\0", iBlankDuration, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Amblyoscope;DeviatedRightX;%f;%ld|0x%02x/\0", fDeviatedRightX, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Amblyoscope;DeviatedRightY;%f;%ld|0x%02x/\0", fDeviatedRightY, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Amblyoscope;DeviatedLeftX;%f;%ld|0x%02x/\0", fDeviatedLeftX, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Amblyoscope;DeviatedLeftY;%f;%ld|0x%02x/\0", fDeviatedLeftY, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Amblyoscope;FlashCenter;%d;%ld|0x%02x/\0", iFlashCenter, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Amblyoscope;FlashBlocks;%d;%ld|0x%02x/\0", iFlashBlocks, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Amblyoscope;ArmLength;%d;%ld|0x%02x/\0", iArmLength, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Amblyoscope;ArmWidth;%d;%ld|0x%02x/\0", iArmWidth, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Amblyoscope;SpaceSize;%d;%ld|0x%02x/\0", iSpaceSize, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Amblyoscope;FixationDot;%d;%ld|0x%02x/\0", bFixationDot, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Amblyoscope;FixationX;%f;%ld|0x%02x/\0", fFixationX, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Amblyoscope;FixationY;%f;%ld|0x%02x/\0", fFixationY, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Amblyoscope;FixationSize;%f;%ld|0x%02x/\0", fFixationSize, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Amblyoscope;CenterX;%f;%ld|0x%02x/\0", fCenterX, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Amblyoscope;CenterY;%f;%ld|0x%02x/\0", fCenterY, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Amblyoscope;BGBrightness;%d;%ld|0x%02x/\0", iBGBrightness, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Amblyoscope;Alternate;%d;%ld|0x%02x/\0", bAlternate, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
}
