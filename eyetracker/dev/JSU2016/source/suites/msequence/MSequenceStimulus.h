/**
 * @file MSequenceStimulus.h
 * @brief 
 */

#ifndef MSEQUENCE_STIMULUS_H
#define MSEQUENCE_STIMULUS_H

/*****************************************************************************
 * Include
 *****************************************************************************/

#include "suites/generic/GenericStimulus.h"
#include "utils/Timer.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CMSequenceStimulus : public CGenericStimulus 
{
	public:
		CMSequenceStimulus();

		// overrides the destructor from CGenericStimulus
		virtual ~CMSequenceStimulus();
		
		// displays the stimulus
		virtual bool DisplayStimulus();
		virtual CString StimulusType();

	private:		
		etu::CTimer FlashTimer;
		int iScrWidthPix, iScrHeightPix;
		
		int iPage;
		bool bOn;
		int iOn;

		CSerialComm theComm;
		
		int iSeqOffset;
		int *piSeq;
		int *piSeq2;
		int *piSeq3;

		VSGLUTBUFFER lutbuffer;
};

#endif // MSEQUENCE_STIMULUS_H
