/**
 * @file OrientationTuning.h
 * @brief 
 */

#ifndef ORIENTATION_TUNING_H
#define ORIENTATION_TUNING_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afx.h>

/*****************************************************************************
 * Classes
 *****************************************************************************/

class COrientationTuning 
{
protected:
	float fBarWidth, fBarHeight;
	float fAngularStep, fMotionSpeed;
	int iIntertrialTime, iTrialDuration;
	int iRepeats;
	float fXPosition, fYPosition;
	CString unitTag;
	int blankPeriod;
	int iReceptiveField;
	float fStartAngle, fEndAngle;
	float fFixX, fFixY, fFixD;
	int iFixColorR, iFixColorG, iFixColorB;
	bool bUseFixation;

public:
	static const int FIELD_RIGHT_EYE = 0;
	static const int FIELD_LEFT_EYE = 1;
	static const int FIELD_INTERLEAVED = 2;
	static const int FIELD_MANUAL = 3;

	COrientationTuning();
	~COrientationTuning();

	void LoadFile(bool bDefault);
	void SaveFile(bool bDefault);

	float getBarWidth() { return fBarWidth; }
	void setBarWidth(float f) { fBarWidth = max(f, 0); }

	float getBarHeight() { return fBarHeight; }
	void setBarHeight(float f) { fBarHeight = max(f, 0); }

	float getAngularStep() { return fAngularStep; }
	void setAngularStep(float f) { fAngularStep = f; }

	float getMotionSpeed() { return fMotionSpeed; }
	void setMotionSpeed(float f) { fMotionSpeed = f; }

	int getIntertrialTime() { return iIntertrialTime; }
	void setIntertrialTime(int i) { iIntertrialTime = i; }
	
	int getTrialDuration() { return iTrialDuration; }
	void setTrialDuration(int i) { iTrialDuration = i; }

	int getRepeats() { return iRepeats; }
	void setRepeats(int i) { iRepeats = i; }

	float getXPosition() { return fXPosition; }
	void setXPosition(float f) { fXPosition = f; }

	float getYPosition() { return fYPosition; }
	void setYPosition(float f) { fYPosition = f; }

	CString getUnitTag() { return unitTag; }
	void setUnitTag(CString cstr) { unitTag = cstr; }

	int getField() { return iReceptiveField; }
	void setField(int i) { iReceptiveField = i; }
		
	int getBlankPeriod() { return blankPeriod; }
	void setBlankPeriod(int i) { blankPeriod = i; }

	float getStartAngle() { return fStartAngle; }
	void setStartAngle(float f) { fStartAngle = f; }

	float getEndAngle() { return fEndAngle; }
	void setEndAngle(float f) { fEndAngle = f; }

	float getFixX() { return fFixX; }
	void setFixX(float f) { fFixX = f; }

	float getFixY() { return fFixY; }
	void setFixY(float f) { fFixY = f; }

	float getFixD() { return fFixD; }
	void setFixD(float f) { fFixD = f; }

	int getFixColorR() { return iFixColorR; }
	void setFixColorR(int i) { iFixColorR = i; }

	int getFixColorG() { return iFixColorG; }
	void setFixColorG(int i) { iFixColorG = i; }

	int getFixColorB() { return iFixColorB; }
	void setFixColorB(int i) { iFixColorB = i; }

	bool getUseFixation() { return bUseFixation; }
	void setUseFixation(bool b) { bUseFixation = b; }
};

#endif
