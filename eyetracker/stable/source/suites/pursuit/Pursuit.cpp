/**
 * @file Pursuit.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <afx.h>
#include <math.h>
#include "Pursuit.h"
#include "comm/SerialComm.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

CPursuit::CPursuit()
{
	defaultFile = CString("C:\\Stimulus Settings\\Pursuit\\default.txt");
	fFrequency = 0.25f;
	fAmplitude = 10.0f;
	fSize = 0.25f;
	iOrientation = ORIENT_HORIZ;
	iWaveform = WAVEFORM_SINE;
	bHighFreqEnabled = false;
	fHighAmplitude = 0.5;
	fHighFrequency = 5;
	redrawBG = true;

	bUseText = false;
	sTextString = CString("The_quick_brown_fox_jumps_over_a_lazy_dog.");

	fEccentricity = 30.0;
	iChangeR = 0;
	iChangeG = 0;
	iChangeB = 255;

	bAlternating = false;

	LoadFile(true);
}

CPursuit::~CPursuit() {};

void CPursuit::LoadFile(bool bDefault)
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

	fscanf(fp, "%f\n", &fFrequency);
	fscanf(fp, "%f\n", &fAmplitude);
	fscanf(fp, "%f\n", &fSize);
	fscanf(fp, "%d\n", &iOrientation);
	fscanf(fp, "%d\n", &iWaveform);
	fscanf(fp, "%d\n", &bHighFreqEnabled);
	fscanf(fp, "%f\n", &fHighAmplitude);
	fscanf(fp, "%f\n", &fHighFrequency);

	fscanf(fp, "%d\n", &bUseText);
	char buffer[1024];
	if (fscanf(fp, "%s\n", buffer) > 0) sTextString = CString(buffer);
	fscanf(fp, "%f\n", &fEccentricity);
	fscanf(fp, "%d\n", &iChangeR);
	fscanf(fp, "%d\n", &iChangeG);
	fscanf(fp, "%d\n", &iChangeB);
	fscanf(fp, "%d\n", &bAlternating);
	fclose(fp);

	redrawBG = true;
}

void CPursuit::SaveFile(bool bDefault)
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
	
	fprintf(fp, "%f\n", fFrequency);
	fprintf(fp, "%f\n", fAmplitude);
	fprintf(fp, "%f\n", fSize);
	fprintf(fp, "%d\n", iOrientation);
	fprintf(fp, "%d\n", iWaveform);
	fprintf(fp, "%d\n", bHighFreqEnabled);
	fprintf(fp, "%f\n", fHighAmplitude);
	fprintf(fp, "%f\n", fHighFrequency);

	fprintf(fp, "%d\n", bUseText);
	fprintf(fp, "%s\n", sTextString);

	fprintf(fp, "%f\n", fEccentricity);
	fprintf(fp, "%d\n", iChangeR);
	fprintf(fp, "%d\n", iChangeG);
	fprintf(fp, "%d\n", iChangeB);
	fprintf(fp, "%d\n", bAlternating);

	fclose(fp);
}

void CPursuit::sendSettings(CSerialComm& theComm)
{
	char buffer[80];
	const int code = 0xC8; // 200
	
	/* time_t is 64 bits.  The %ld conversion expected long, which is only
	 * guaranteed to be 32 bits.  Cast to long so they match.  This will
	 * stop working truncate needed info in about 20 years. */
	long tm = (long)time(NULL);

	sprintf(buffer, "Pursuit;Amplitude;%.4f;%ld|0x%02x/\0", fAmplitude, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Pursuit;WaveformType;%d;%ld|0x%02x/\0", iWaveform, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Pursuit;Orientation;%d;%ld|0x%02x/\0", iOrientation, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Pursuit;Frequency;%.4f;%ld|0x%02x/\0", fFrequency, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Pursuit;DotSize;%.4f;%ld|0x%02x/\0", fSize, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Pursuit;Eccentricity;%.4f;%ld|0x%02x/\0", fEccentricity, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Pursuit;ChangeR;%d;%ld|0x%02x/\0", iChangeR, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Pursuit;ChangeG;%d;%ld|0x%02x/\0", iChangeG, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Pursuit;ChangeB;%d;%ld|0x%02x/\0", iChangeB, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "Pursuit;Alternating;%d;%ld|0x%02x/\0", bAlternating, tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
}