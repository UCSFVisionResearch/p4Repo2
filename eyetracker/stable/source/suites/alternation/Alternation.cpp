/**
 * @file Alternation.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include "Alternation.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

CAlternation::CAlternation() 
{
	defaultFile = CString("C:\\Stimulus Settings\\Alternation\\default.txt");
	
	fDotSize1 = 0.02;
	fDotSize2 = 0.125;
	fXPos1 = 0;
	fYPos1 = 0;
	fXPos2 = 5.0;
	fYPos2 = 0;
	iRed1 = 20;
	iGreen1 = 0;
	iBlue1 = 0;
	iRed2 = 0;
	iGreen2 = 0;
	iBlue2 = 255;
	fDiamond = 0.0;

	LoadFile(true);
}

CAlternation::~CAlternation() {}

void CAlternation::LoadFile(bool bDefault) 
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

	fscanf(fp, "%d\n", &iRed1);
	fscanf(fp, "%d\n", &iGreen1);
	fscanf(fp, "%d\n", &iBlue1);
	fscanf(fp, "%d\n", &iRed2);
	fscanf(fp, "%d\n", &iGreen2);
	fscanf(fp, "%d\n", &iBlue2);
	fscanf(fp, "%lf\n", &fDotSize1);
	fscanf(fp, "%lf\n", &fDotSize2);
	fscanf(fp, "%lf\n", &fXPos1);
	fscanf(fp, "%lf\n", &fYPos1);
	fscanf(fp, "%lf\n", &fXPos2);
	fscanf(fp, "%lf\n", &fYPos2);
	fscanf(fp, "%lf\n", &fDiamond);

	fclose(fp);

}

void CAlternation::SaveFile(bool bDefault) 
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

	fprintf(fp, "%d\n", iRed1);
	fprintf(fp, "%d\n", iGreen1);
	fprintf(fp, "%d\n", iBlue1);
	fprintf(fp, "%d\n", iRed2);
	fprintf(fp, "%d\n", iGreen2);
	fprintf(fp, "%d\n", iBlue2);
	fprintf(fp, "%f\n", fDotSize1);
	fprintf(fp, "%f\n", fDotSize2);
	fprintf(fp, "%f\n", fXPos1);
	fprintf(fp, "%f\n", fYPos1);
	fprintf(fp, "%f\n", fXPos2);
	fprintf(fp, "%f\n", fYPos2);
	fprintf(fp, "%f\n", fDiamond);

	fclose(fp);
}

void CAlternation::sendSettings(CSerialComm& theComm) 
{
	char buffer[80];
	const int code = 0xC8; // 200
	
	/* time_t is 64 bits.  The %ld conversion expected long, which is only
	 * guaranteed to be 32 bits.  Cast to long so they match.  This will
	 * stop working truncate needed info in about 20 years. */
	long tm = (long)time(NULL);

	sprintf(buffer, "Alternation;DotSize1;%.4f;%ld|0x%02x/\0", fDotSize1, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Alternation;XPos1;%.4f;%ld|0x%02x/\0", fXPos1, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Alternation;YPos1;%.4f;%ld|0x%02x/\0", fYPos1, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Alternation;Red1;%d;%ld|0x%02x/\0", iRed1, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Alternation;Green1;%d;%ld|0x%02x/\0", iGreen1, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Alternation;Blue1;%d;%ld|0x%02x/\0", iBlue1, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Alternation;DotSize2;%.4f;%ld|0x%02x/\0", fDotSize2, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Alternation;XPos2;%.4f;%ld|0x%02x/\0", fXPos2, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Alternation;YPos2;%.4f;%ld|0x%02x/\0", fYPos2, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Alternation;Red2;%d;%ld|0x%02x/\0", iRed2, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Alternation;Green2;%d;%ld|0x%02x/\0", iGreen2, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Alternation;Blue2;%d;%ld|0x%02x/\0", iBlue2, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Alternation;Diamond;%.4f;%ld|0x%02x/\0", fDiamond, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
}
