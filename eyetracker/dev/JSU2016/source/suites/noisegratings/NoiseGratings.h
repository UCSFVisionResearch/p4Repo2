/**
 * @file NoiseGratings.h
 * @brief 
 */

#ifndef NOISEGRATINGS_H
#define NOISEGRATINGS_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afx.h>

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CNoiseGratings 
{
protected:
	int iMeanLevel;
	int iAmplitude;
	int iType;
	bool bUseMask;
	float fMaskRadius;
	int iBlending;
	bool bUseMouseControl;
	float xDeg, yDeg;
	int iGratingType;
	float fAutomationSpeed;
	bool bAutoFreq, bAutoContrast, bAutoOrientation, bAutoWidth, bAutoHeight, bAutoPhase;
	float fMinFreq, fMinContrast, fMinOrientation, fMinWidth, fMinHeight, fMinPhase;
	float fMaxFreq, fMaxContrast, fMaxOrientation, fMaxWidth, fMaxHeight, fMaxPhase;
	float fStepFreq, fStepContrast, fStepOrientation, fStepWidth, fStepHeight, fStepPhase;
	float fSetPtFreq, fSetPtContrast, fSetPtOrientation, fSetPtWidth, fSetPtHeight, fSetPtPhase;
	
public:
	CString defaultFile;

	const static int NOISE_TYPE_WHITE = 0;
	const static int NOISE_TYPE_GAUSSIAN = 1;

	const static int BLENDING_TYPE_LINEAR = 0;
	const static int BLENDING_TYPE_QUADRATIC = 1;
	const static int BLENDING_TYPE_EXPONENTIAL = 2;
	
	const static int GRATING_TYPE_SQUARE = 0;
	const static int GRATING_TYPE_SINE = 1;

	CNoiseGratings(); 
	~CNoiseGratings();

	void LoadFile(bool bDefault);
	void SaveFile(bool bDefault);

	void setMeanLevel(int i) { iMeanLevel = i; }
	int getMeanLevel() { return iMeanLevel; }

	void setAmplitude(int i) { iAmplitude = i; }
	int getAmplitude() { return iAmplitude; }

	void setNoiseType(int i) { iType = i; }
	int getNoiseType() { return iType; }

	void setUseMask(bool b) { bUseMask = b; }
	bool useMask() { return bUseMask; }

	void setMaskRadius(float f) { fMaskRadius = f; }
	float getMaskRadius() { return fMaskRadius; }

	void setBlendingType(int i) { iBlending = i; }
	int getBlendingType() { return iBlending; }

	void setUseMouseControl(bool b) { bUseMouseControl = b; }
	bool useMouseControl() { return bUseMouseControl; }

	void setXDeg(float f) { xDeg = f; }
	float getXDeg() { return xDeg; }

	void setYDeg(float f) { yDeg = f; }
	float getYDeg() { return yDeg; }

	void setGratingType(int i) { iGratingType = i; }
	int getGratingType() { return iGratingType; }

	void setAutomationSpeed(float f) { fAutomationSpeed = f; }
	float getAutomationSpeed() { return fAutomationSpeed; }

	void setAutoFreq(bool b) { bAutoFreq = b; }
	bool getAutoFreq() { return bAutoFreq; }

	void setMinFreq(float f) { fMinFreq = f; }
	float getMinFreq() { return fMinFreq; }

	void setMaxFreq(float f) { fMaxFreq = f; }
	float getMaxFreq() { return fMaxFreq; }

	void setStepFreq(float f) { fStepFreq = f; }
	float getStepFreq() { return fStepFreq; }

	void setSetPtFreq(float f) { fSetPtFreq = f; }
	float getSetPtFreq() { return fSetPtFreq; }

	void setAutoContrast(bool b) { bAutoContrast = b; }
	bool getAutoContrast() { return bAutoContrast; }

	void setMinContrast(float f) { fMinContrast = f; }
	float getMinContrast() { return fMinContrast; }

	void setMaxContrast(float f) { fMaxContrast = f; }
	float getMaxContrast() { return fMaxContrast; }

	void setStepContrast(float f) { fStepContrast = f; }
	float getStepContrast() { return fStepContrast; }

	void setSetPtContrast(float f) { fSetPtContrast = f; }
	float getSetPtContrast() { return fSetPtContrast; }

	void setAutoOrientation(bool b) { bAutoOrientation = b; }
	bool getAutoOrientation() { return bAutoOrientation; }

	void setMinOrientation(float f) { fMinOrientation = f; }
	float getMinOrientation() { return fMinOrientation; }

	void setMaxOrientation(float f) { fMaxOrientation = f; }
	float getMaxOrientation() { return fMaxOrientation; }

	void setStepOrientation(float f) { fStepOrientation = f; }
	float getStepOrientation() { return fStepOrientation; }

	void setSetPtOrientation(float f) { fSetPtOrientation = f; }
	float getSetPtOrientation() { return fSetPtOrientation; }

	void setAutoWidth(bool b) { bAutoWidth = b; }
	bool getAutoWidth() { return bAutoWidth; }

	void setMinWidth(float f) { fMinWidth = f; }
	float getMinWidth() { return fMinWidth; }

	void setMaxWidth(float f) { fMaxWidth = f; }
	float getMaxWidth() { return fMaxWidth; }

	void setStepWidth(float f) { fStepWidth = f; }
	float getStepWidth() { return fStepWidth; }

	void setSetPtWidth(float f) { fSetPtWidth = f; }
	float getSetPtWidth() { return fSetPtWidth; }

	void setAutoHeight(bool b) { bAutoHeight = b; }
	bool getAutoHeight() { return bAutoHeight; }

	void setMinHeight(float f) { fMinHeight = f; }
	float getMinHeight() { return fMinHeight; }

	void setMaxHeight(float f) { fMaxHeight = f; }
	float getMaxHeight() { return fMaxHeight; }

	void setStepHeight(float f) { fStepHeight = f; }
	float getStepHeight() { return fStepHeight; }

	void setSetPtHeight(float f) { fSetPtHeight = f; }
	float getSetPtHeight() { return fSetPtHeight; }

	void setAutoPhase(bool b) { bAutoPhase = b; }
	bool getAutoPhase() { return bAutoPhase; }

	void setMinPhase(float f) { fMinPhase = f; }
	float getMinPhase() { return fMinPhase; }

	void setMaxPhase(float f) { fMaxPhase = f; }
	float getMaxPhase() { return fMaxPhase; }

	void setStepPhase(float f) { fStepPhase = f; }
	float getStepPhase() { return fStepPhase; }

	void setSetPtPhase(float f) { fSetPtPhase = f; }
	float getSetPtPhase() { return fSetPtPhase; }
};

#endif
