/**
 * @file Settings.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <afx.h>
#include <time.h>
#include <math.h>
#include "VSGV8.h"
#include "VSGV8AD.h"
#include "Settings.h"
#include "comm/SerialComm.h"
#include "utils/Utils.h"
#include "utils/Math.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

CSettings::CSettings()
{
	theSerialComm.init(false, CBR_115200, 1);//global theComm. only init once, here
	defaultFile = CString("C:\\Stimulus Settings\\Settings\\default.txt\0");

	iEyePosition = 370;
	iScreenPosition = 1256;
	iRed = 255;
	iGreen = 0;
	iBlue = 0;
	iRedBG = 0;
	iGreenBG = 0;
	iBlueBG = 0;
	fXOffset = 0;
	fYOffset = 0;
	fDegPerVolt = 10;
	fScreenXOffset = 0;
	fScreenYOffset = 0;
	fPixelSize = 141.0;
	iNSamplesEyePos = 5;
	LoadFile(true);
	sendSettings(theSerialComm);//send settings here because they can't be sent on stim construction (no stim)
}

CSettings::~CSettings()
{
	theSerialComm.stop();//global theComm. only stop once, here
}

void CSettings::LoadFile(bool bDefault)
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
			
	fscanf(fp, "%d\n", &iEyePosition);
	fscanf(fp, "%d\n", &iScreenPosition);
	fscanf(fp, "%d\n", &iRed);
	fscanf(fp, "%d\n", &iGreen);
	fscanf(fp, "%d\n", &iBlue);
	fscanf(fp, "%d\n", &iRedBG);
	fscanf(fp, "%d\n", &iGreenBG);
	fscanf(fp, "%d\n", &iBlueBG);
	fscanf(fp, "%f\n", &fXOffset);
	fscanf(fp, "%f\n", &fYOffset);
	fscanf(fp, "%f\n", &fDegPerVolt);
	fscanf(fp, "%f\n", &fScreenXOffset);
	fscanf(fp, "%f\n", &fScreenYOffset);
	fscanf(fp, "%f\n", &fPixelSize);
	fscanf(fp, "%d\n", &iNSamplesEyePos);

	fclose(fp);
}

void CSettings::SaveFile(bool bDefault)
{
	CString filename = defaultFile;
	if (!bDefault) {
		filename = etu::BrowseForFile(false, ".txt", "Text Files (*.txt)|*.txt", filename, false);
	}

	FILE* fp = fopen(filename, "wt");
	if (!fp) {
		AfxMessageBox("Error saving file - file or folder not found");
		return;
	}
	
	fprintf(fp, "%d\n", iEyePosition);
	fprintf(fp, "%d\n", iScreenPosition);
	fprintf(fp, "%d\n", iRed);
	fprintf(fp, "%d\n", iGreen);
	fprintf(fp, "%d\n", iBlue);
	fprintf(fp, "%d\n", iRedBG);
	fprintf(fp, "%d\n", iGreenBG);
	fprintf(fp, "%d\n", iBlueBG);
	fprintf(fp, "%f\n", fXOffset);
	fprintf(fp, "%f\n", fYOffset);
	fprintf(fp, "%f\n", fDegPerVolt);
	fprintf(fp, "%f\n", fScreenXOffset);
	fprintf(fp, "%f\n", fScreenYOffset);
	fprintf(fp, "%f\n", fPixelSize);
	fprintf(fp, "%d\n", iNSamplesEyePos);

	fclose(fp);
}

void CSettings::tanScreenUnit2Unit(int unit1, double val1, int unit2, double *val2)
{
	if (false) {//if we want to make the correction (we don't!)
		//if there is no conversion to do, don't do one!
		if (unit1==unit2) {
			val2[0] = val1;
			return;
		}
		//this conversion is location-dependent! assume we're at the center
		//double degPerPixel = 0;
		//vsgUnit2Unit(vsgPIXELUNIT, 1, vsgDEGREEUNIT, &degPerPixel);
		double viewdistmm = iScreenPosition - iEyePosition;//vsgGetViewDistMM()
		//get pixel size calibration
		double mmPerPixel = 0;
		mmPerPixel = fPixelSize / 100.0;
		//view distance
		//double viewDistDeg = degPerPixel*((double)vsgGetViewDistMM())/mmPerPixel;
		//deg->pixel
		if (unit1 == vsgDEGREEUNIT && unit2 == vsgPIXELUNIT) {
			*val2 = tan(val1 * PI_DOUBLE / 180.0) * viewdistmm / mmPerPixel;
			return;
		}
		//pixel->deg
		if (unit1 == vsgPIXELUNIT && unit2 == vsgDEGREEUNIT) {
			*val2 = 180.0 * atan(val1 / (viewdistmm / mmPerPixel)) / PI_DOUBLE;
			return;
		}
		//ignore other conversion units
	} 
	else {
		//no correction, project onto sphere
		vsgUnit2Unit(unit1, val1, unit2, val2);
	}
}

void CSettings::sendSettings(CSerialComm& theComm)
{
	char buffer[80];
	const int code = 0xC8; // 200
	
	/* time_t is 64 bits.  The %ld conversion expected long, which is only
	 * guaranteed to be 32 bits.  Cast to long so they match.  This will
	 * stop working truncate needed info in about 20 years. */
	long tm = (long)time(NULL);

	sprintf(buffer, "Settings;EyePos;%d;%ld|0x%02x/\0", iEyePosition, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Settings;ScreenPos;%d;%ld|0x%02x/\0", iScreenPosition, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Settings;ViewDist;%d;%ld|0x%02x/\0", iScreenPosition-iEyePosition, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Settings;StimXOffset;%.4f;%ld|0x%02x/\0", fXOffset, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Settings;StimYOffset;%.4f;%ld|0x%02x/\0", fYOffset, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Settings;ScreenXOffset;%.4f;%ld|0x%02x/\0", fScreenXOffset, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Settings;ScreenYOffset;%.4f;%ld|0x%02x/\0", fScreenYOffset, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Settings;DotColorR;%d;%ld|0x%02x/\0", iRed, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Settings;DotColorG;%d;%ld|0x%02x/\0", iGreen, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Settings;DotColorB;%d;%ld|0x%02x/\0", iBlue, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Settings;BGColorR;%d;%ld|0x%02x/\0", iRedBG, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Settings;BGColorG;%d;%ld|0x%02x/\0", iGreenBG, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Settings;BGColorB;%d;%ld|0x%02x/\0", iBlueBG, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Settings;NSamples;%d;%ld|0x%02x/\0", iNSamplesEyePos, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
}
