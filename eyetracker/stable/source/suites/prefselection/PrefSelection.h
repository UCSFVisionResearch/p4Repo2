/**
 * @file PrefSelection.h
 * @brief 
 */

#ifndef PREFSELECTION_H
#define PREFSELECTION_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afx.h>
#include "comm/SerialComm.h"

/*****************************************************************************
 * Typedefs and Structs
 *****************************************************************************/

class CStaircase;

typedef struct myLevelStruct {
	float spatialFreq;
	CStaircase* pStaircase;
	float initialContrast;
} level;

/*****************************************************************************
 * Forward Declarations
 *****************************************************************************/

class CStaircaseSettings;

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CPrefSelection 
{
protected:
	int iCorrectTimeOut, iTimeOut;
	CString strSpatFreqList, strInitContrastList;
	float gratingWidth, gratingHeight, gratingPosition, dotPosition, dotSize;
	int iTargetDelay;
	int iPctTrialsRight;
	bool bTrainingMode;

	CStaircaseSettings* pStaircaseSettings;

	level* pLevels;
	int iNumLevels;

public:
	CString defaultFile;
	
	CPrefSelection();
	~CPrefSelection(); 

	int getNumListElements(CString list);
	void extractListElements(CString list, float* pElements, int iNumElements);
	void generateLevelsList();
	void LoadFile(bool bDefault); 
	void SaveFile(bool bDefault);
	void sendSettings(CSerialComm& theComm);
	
	int getCorrectTimeOut() { return iCorrectTimeOut; }
	void setCorrectTimeOut(int i) { iCorrectTimeOut = i; }
	
	int getTimeOut() { return iTimeOut; }
	void setTimeOut(int i) { iTimeOut = i; }

	float getGratingWidth() { return gratingWidth; }
	void setGratingWidth(float f) { gratingWidth = f; }

	float getGratingHeight() { return gratingHeight; }
	void setGratingHeight(float f) { gratingHeight = f; }
	
	float getGratingPosition() { return gratingPosition; }
	void setGratingPosition(float f) { gratingPosition = f; }

	float getDotPosition() { return dotPosition; }
	void setDotPosition(float f) { dotPosition = f; }

	float getDotSize() { return dotSize; }
	void setDotSize(float f) { dotSize = f; }
	
	int getTargetDelay() { return iTargetDelay; }
	void setTargetDelay(int i) { iTargetDelay = i; }

	bool isTrainingMode() { return bTrainingMode; }
	void setTrainingMode(bool b) { bTrainingMode  = b; }

	int getPctTrialsRight() {return iPctTrialsRight; }
	void setPctTrialsRight(int i) { iPctTrialsRight = i;}

	CStaircaseSettings* getStaircaseSettings() { return pStaircaseSettings; }
	
	CString getSpatialFreqList() { return strSpatFreqList; }
	void setSpatialFreqList(CString list);

	CString getInitialContrastList() { return strInitContrastList; }
	void setInitialContrastList(CString list); 

	int getNumLevels() { return iNumLevels; }
	level getLevel(int iLevel) { return pLevels[iLevel]; }
	level* getLevels() { return pLevels; }
};

#endif
