/**
 * @file RandomDotAnaglyph.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <afx.h>
#include <math.h>
#include "RandomDotAnaglyph.h"
#include "comm/SerialComm.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

CRandomDotAnaglyph::CRandomDotAnaglyph()
{
	defaultFile = CString("C:\\Stimulus Settings\\RandomDotAnaglyph\\default.txt");
	
	iRed = 255;
	iBlue = 255;
	iRadio = 0;

	iBGDisparity = 5;
	iNoise = 10;
	
	displaytext = CString("howdy");
	imagepath = CString("c:\\");
	iTextDisparity = 10;
	iMaxDisparity = 100;

	//runtime
	bStarted = false;
	
	LoadFile(true);
}

CRandomDotAnaglyph::~CRandomDotAnaglyph() {}

void CRandomDotAnaglyph::LoadFile(bool bDefault) 
{
	CString filename = defaultFile;
	if (!bDefault) {
		filename = etu::BrowseForFile(true, ".txt", "Text Files (*.txt)|*.txt", filename, false);
	}

	//char buffer[1024];
	//sprintf(buffer, "%s", filename);
	FILE* fp = fopen(filename, "rt");
	if (!fp) {
		AfxMessageBox("Error loading file - file or folder not found");
		return;
	}

	char buffer2[1024];
	fscanf(fp, "%[^\n]\n", &buffer2);
	displaytext = CString(buffer2);

	fscanf(fp, "%[^\n]\n", &buffer2);
	imagepath = CString(buffer2);

	fscanf(fp, "%d\n", &iRed);
	fscanf(fp, "%d\n", &iBlue);
	fscanf(fp, "%d\n", &iRadio);
	
	fscanf(fp, "%d\n", &iBGDisparity);
	fscanf(fp, "%d\n", &iNoise);
	fscanf(fp, "%d\n", &iTextDisparity);
	fscanf(fp, "%d\n", &iMaxDisparity);

	fclose(fp);
}

void CRandomDotAnaglyph::SaveFile(bool bDefault) 
{
	CString filename = defaultFile;
	if (!bDefault) {
		filename = etu::BrowseForFile(false, ".txt", "Text Files (*.txt)|*.txt", filename, false);
	}

	//char buffer[1024];
	//sprintf(buffer, "%s", filename);
	FILE* fp = fopen(filename, "wt");
	if (!fp) {
		AfxMessageBox("Error saving file - file or folder not found");
		return;
	}
	
	fprintf(fp, "%s\n", displaytext);
	fprintf(fp, "%s\n", imagepath);
	fprintf(fp, "%d\n", iRed);
	fprintf(fp, "%d\n", iBlue);
	fprintf(fp, "%d\n", iRadio);
	
	fprintf(fp, "%d\n", iBGDisparity);
	fprintf(fp, "%d\n", iNoise);
	fprintf(fp, "%d\n", iTextDisparity);
	fprintf(fp, "%d\n", iMaxDisparity);
	
	fclose(fp);
}

void CRandomDotAnaglyph::sendSettings(CSerialComm& theComm) 
{
	char buffer[80];
	const int code = 0xC8; // 200
	
	/* time_t is 64 bits.  The %ld conversion expected long, which is only
	 * guaranteed to be 32 bits.  Cast to long so they match.  This will
	 * stop working truncate needed info in about 20 years. */
	long tm = (long)time(NULL);

	sprintf(buffer, "RandomDotAnaglyph;Red;%d;%ld|0x%02x/\0", iRed, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "RandomDotAnaglyph;Blue;%d;%ld|0x%02x/\0", iBlue, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "RandomDotAnaglyph;BGDisparity;%d;%ld|0x%02x/\0", iBGDisparity, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "RandomDotAnaglyph;Noise;%d;%ld|0x%02x/\0", iNoise, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "RandomDotAnaglyph;Display;%d;%ld|0x%02x/\0", iRadio, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "RandomDotAnaglyph;TextDisparity;%d;%ld|0x%02x/\0", iTextDisparity, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "RandomDotAnaglyph;MaxDisparity;%d;%ld|0x%02x/\0", iMaxDisparity, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "RandomDotAnaglyph;displaytext;%.30s;%ld|0x%02x/\0", displaytext, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "RandomDotAnaglyph;imagepath;%.30s;%ld|0x%02x/\0", imagepath, tm, code);			theComm.transmit(buffer, (int)strlen(buffer));
}
