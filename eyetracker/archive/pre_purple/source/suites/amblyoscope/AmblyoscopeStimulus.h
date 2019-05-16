/**
 * @file AmblyoscopeStimulus.h
 * @brief 
 */

#ifndef AMBLYOSCOPE_STIMULUS_H
#define AMBLYOSCOPE_STIMULUS_H

/*****************************************************************************
 * Include
 *****************************************************************************/

#include "suites/generic/GenericStimulus.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CAmblyoscopeStimulus : public CGenericStimulus 
{
public:
	CAmblyoscopeStimulus();

	// overrides the destructor from CGenericStimulus
	virtual ~CAmblyoscopeStimulus();
	
	// displays the stimulus
	virtual bool DisplayStimulus();
	virtual CString StimulusType();
	
	int drawBitmapImage(int color, int x, int y, char *imagefile);
	int drawBitmapImage2(int x, int y, char *imagefile, int x2, int y2, char *imagefile2);
	void refreshPalette();
	void drawAxis(int x, int y, int seglength, int segwidth, int segspace, bool red, bool horizontal);

private:		
	int iScrWidthPix, iScrHeightPix;
	double pointX;
	double pointY;

	int noisepage;
	int iPage;

	int iFlashBlocks;

	BITMAPINFOHEADER *l_ih, *r_ih;
	BITMAPFILEHEADER *l_fh, *r_fh;
	char *l_data, *r_data;
	char l_name[1024], r_name[1024];
};

#endif // AMBLYOSCOPE_STIMULUS_H
