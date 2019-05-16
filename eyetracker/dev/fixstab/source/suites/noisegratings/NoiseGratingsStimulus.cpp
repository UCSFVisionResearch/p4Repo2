#ifdef APP_ACUITY

#include "app/AppCore.h"



CNoiseGratingsStimulus::CNoiseGratingsStimulus() {
	Initialize();
	iScrWidthPix = vsgGetScreenWidthPixels();
	iScrHeightPix = vsgGetScreenHeightPixels();
	iPage = 0;
	vsgMoveScreen(0, 0);
}

CNoiseGratingsStimulus::~CNoiseGratingsStimulus() {
}

bool CNoiseGratingsStimulus::DisplayStimulus() {

	srand( (unsigned)time( NULL ) );
	if (!GenerateGrating()) {
	}

	if (!GenerateMask()) {
	}

	if (!Draw()) {
	}

	return true;
}


bool CNoiseGratingsStimulus::GenerateGrating() {

	return true;
}

bool CNoiseGratingsStimulus::GenerateMask() {

	return true;
}

bool CNoiseGratingsStimulus::Draw() {

	return true;
}


CString CNoiseGratingsStimulus::StimulusType() {
	return "Noise Gratings Stimulus";
}

#endif APP_ACUITY