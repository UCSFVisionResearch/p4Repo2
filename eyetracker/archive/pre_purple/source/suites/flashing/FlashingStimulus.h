#ifndef _FLASHING_STIMULUS_H
#define _FLASHING_STIMULUS_H

typedef struct myPtScaleFactor {
	int x;
	int y;
} PtScaleFactor;

UINT __cdecl CommControllingFunctionLeft( LPVOID pParam );
UINT __cdecl CommControllingFunctionRight( LPVOID pParam );


class CFlashingStimulus : public CGenericStimulus {
	public:
		CFlashingStimulus();

		// overrides the destructor from CGenericStimulus
		virtual ~CFlashingStimulus();
		
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



#endif