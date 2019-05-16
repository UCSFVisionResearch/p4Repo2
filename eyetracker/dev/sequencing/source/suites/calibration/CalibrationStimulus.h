/**
 * @file CalibrationStimulus.h
 * @brief 
 */

#ifndef CALIBRATION_STIMULUS_H
#define CALIBRATION_STIMULUS_H

/*****************************************************************************
 * Include
 *****************************************************************************/

#include <afxmt.h>
#include "suites/generic/GenericStimulus.h"

/*****************************************************************************
 * Typedefs and Structs
 *****************************************************************************/

typedef struct myPtScaleFactor {
	int x;
	int y;
} PtScaleFactor;

/*****************************************************************************
 * Prototypes
 *****************************************************************************/

UINT __cdecl CommControllingFunctionLeft( LPVOID pParam );
UINT __cdecl CommControllingFunctionRight( LPVOID pParam );

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CCalibrationStimulus : public CGenericStimulus 
{
public:
	CCalibrationStimulus();

	// overrides the destructor from CGenericStimulus
	virtual ~CCalibrationStimulus();
	
	// displays the stimulus
	virtual bool DisplayStimulus();
	virtual CString StimulusType();
	virtual void StopExecution();
	
	void setCalibrationType(int iType);
	void setActiveCalibrationPoint(int iPoint);
	void setCalibrationPoint(int pt, int x, int y);
	void AcceptPoint();
	void DoCycle();
	void DoStuff();
	
	CCriticalSection critSect;
	CSingleLock* singleLock;

private:
	PtScaleFactor TWO_PT_CAL[2];
	PtScaleFactor FIVE_PT_CAL[5];
	PtScaleFactor NINE_PT_CAL[9];
	
	int numAccepts;
	int iScrWidthPix, iScrHeightPix;
	int iDotNum;
	int iCalibrationType;
	int iPage;
	double pointX;
	double pointY;
	CWinThread* pCommThreadLeft;
	CWinThread* pCommThreadRight;
	bool bDrawn;
};

#endif // CALIBRATION_STIMULUS_H
