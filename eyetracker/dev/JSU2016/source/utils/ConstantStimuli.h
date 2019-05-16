/**
 * @file ConstantStimuli.h
 * @brief 
 */

#ifndef CONSTANTSTIMULI_H
#define CONSTANTSTIMULI_H

namespace etu {

/*****************************************************************************
 * Includes
 *****************************************************************************/

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CConstantStimuli 
{
public:
	CConstantStimuli(int iPresenations, int iNumLevels, int iRandomType);
	~CConstantStimuli();

	// update the exp
	void update();

	void setNumLevels(int i);
	int getNumLevels();

	int getSeqLength();
	int getCurrentLevel();

	// ist the experiment complete?
	bool isComplete();

private:
	void generateSequence();
		
	// number of levels available available for the stimulus
	int iNumLevels;
	
	// presenation sequence
	int* m_pSequence;
	int iSeqLength;

	// number of presentations per level
	int iPresentations; // presentations per level

	// type of randomness used
	int iRandomType; // 0 = full, 1 = block

	int iSeqIndex;
};

}; // namespace etu

#endif // CONSTANTSTIMULI_H
