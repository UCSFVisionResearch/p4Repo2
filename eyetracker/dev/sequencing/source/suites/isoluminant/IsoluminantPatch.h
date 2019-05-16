/**
 * @file IsoluminantPatch.h
 * @brief 
 */

#ifndef ISOLUMINANTPATCH_DLG_H
#define ISOLUMINANTPATCH_DLG_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afx.h>

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CIsoluminantPatch 
{
protected:
	CString cstrFileName, cstrResponseFile;
	int iNumCycles;
	bool bLoop;
	float fVoltageRange, fVoltageOffset;
	int circleXPosition, circleYPosition;
	int circleRadius;
	bool bSearching;
	bool bHighestPriority;
	bool bUseLeft;
	float fixationSize;
	float fixationX, fixationY;
	float fixationX2, fixationY2;
	int meanLuminance, minLuminance, maxLuminance;
	bool bFNoise;
	int iNumRepeats;
	bool bRepeatsOnly;


public:
	CString defaultFile;
	
	CIsoluminantPatch();
	~CIsoluminantPatch();
	void LoadFile(bool bDefault);
	void SaveFile(bool bDefault);

	CString GetFileName() { return cstrFileName; }
	void SetFileName(CString cstr) { cstrFileName = cstr; }
			
	int GetNumCycles() { return iNumCycles; }
	void SetNumCycles(int i) { iNumCycles = i;}

	bool GetLoop() { return bLoop; }
	void SetLoop(bool b) { bLoop = b; }

	float GetVoltageRange() { return fVoltageRange; }
	void SetVoltageRange(float f) { fVoltageRange =f; }

	float GetVoltageOffset() { return fVoltageOffset; }
	void SetVoltageOffset(float f) { fVoltageOffset =f; }

	int GetCircleRadius() { return circleRadius; }
	void SetCircleRadius(int i) { circleRadius = max(i, 0); }

	int GetCircleXPosition() { return circleXPosition; }
	void SetCircleXPosition(int i) { circleXPosition = i; }

	int GetCircleYPosition() { return circleYPosition; }
	void SetCircleYPosition(int i) { circleYPosition = i; }

	bool isSearching() { return bSearching; }
	void SetSearching(bool b) { bSearching = b; }

	bool useHighestPriority() { return bHighestPriority; }
	void setHighestPriority(bool b) { bHighestPriority = b; }

	bool useLeftHemifield() { return bUseLeft; }
	void setLeftHemifield(bool b) { bUseLeft = b; }

	float GetFixationSize() { return fixationSize; }
	void SetFixationSize(float f) { fixationSize = f; }

	float GetFixationPointX() { return fixationX; }
	void SetFixationPointX(float f) { fixationX = f; }

	float GetFixationPointY() { return fixationY; }
	void SetFixationPointY(float f) { fixationY = f; }

	float GetFixationPointX2() { return fixationX2; }
	void SetFixationPointX2(float f) { fixationX2 = f; }

	float GetFixationPointY2() { return fixationY2; }
	void SetFixationPointY2(float f) { fixationY2 = f; }

	int GetMeanLuminance() { return meanLuminance; }
	void SetMeanLuminance(int i) { meanLuminance = i;}

	bool isFNoise() { return bFNoise; }
	void SetFNoise(bool b) { bFNoise = b; }

	int GetNumRepeats() { return iNumRepeats; }
	void SetNumRepeats(int i) { iNumRepeats = i; }

	bool RepeatsOnly() { return bRepeatsOnly; }
	void SetRepeatsOnly(bool b) { bRepeatsOnly = b; }

	int GetMinLuminance() { return minLuminance; }
	void SetMinLuminance(int i) { minLuminance = i;}

	int GetMaxLuminance() { return maxLuminance; }
	void SetMaxLuminance(int i) { maxLuminance = i;}
};

#endif
