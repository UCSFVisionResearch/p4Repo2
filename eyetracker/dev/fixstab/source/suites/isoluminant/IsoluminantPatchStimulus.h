/**
 * @file IsoluminantPatchStimulus.h
 * @brief 
 */

#ifndef ISOLUMINANT_PATCH_STIMULUS__H
#define ISOLUMINANT_PATCH_STIMULUS__H

/*****************************************************************************
 * Include
 *****************************************************************************/

#include "suites/generic/GenericStimulus.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CIsoluminantPatchStimulus : public CGenericStimulus 
{
public:
	CIsoluminantPatchStimulus();

	// overrides the destructor from CGenericStimulus
	virtual ~CIsoluminantPatchStimulus();
	
	// displays the stimulus
	virtual bool Initialize();
	virtual bool DisplayStimulus();
	void DisplayStimulusCycles();
	virtual CString StimulusType();
	void UpdatePosition();
	void UpdatePositionFlag();

private:
	void GenerateLUT();
	void ReadInput();
	
	bool bUpdatePosition;
	int iScrWidthPix, iScrHeightPix;
	int* pRepeatSequence;
	int** pUniqueSequences;
	int iNumRepeatsComplete;
	bool* bUniquesComplete;
	int iNumUniquesComplete;
	
	int iSequenceNumber;
	int iSequenceLength;
	int iNumUniques;

	int iLoopNumber;
	int iPage;
	VSGTRIVAL* paletteColors;
	VSGLUTBUFFER buff;
	
	int marker;

	int iFixationLoc;
};

#endif // ISUOLUMINANT_PATCH_STIMULUS__H
