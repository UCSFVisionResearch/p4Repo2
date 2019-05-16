/**
 * @file ConstantStimuli.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <afx.h>
#include "ConstantStimuli.h"

namespace etu {

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

CConstantStimuli::CConstantStimuli(int iPresentations, int iNumLevels, int iRandomType) 
{
	this->iPresentations = iPresentations;
	this->iNumLevels = iNumLevels;
	this->iRandomType = iRandomType;

	m_pSequence = NULL;
	iSeqLength = 0;
	iSeqIndex = 0;

	generateSequence();
}

CConstantStimuli::~CConstantStimuli() 
{
	if (m_pSequence) {
		free(m_pSequence);
		m_pSequence = NULL;
		iSeqLength = 0;
	}
}

void CConstantStimuli::generateSequence() 
{
	if (m_pSequence) free(m_pSequence);

	iSeqLength = iPresentations * iNumLevels;
	m_pSequence  = (int*)calloc(iSeqLength, sizeof(int));

	for (int i = 0; i< iSeqLength; i++) {
		m_pSequence[i] = -1;
	}
	
	if (iRandomType == 0) { // full randomization
		int choices = iSeqLength;
		for (int j = 0; j < iNumLevels; j++) {
			for (int i = 0; i < iPresentations; i++) {
				int loc = rand() % choices;
				int count = 0;
				int index = 0;

				// find the first empty location
				while (m_pSequence[index] >= 0) index++;

				// count loc number of empty locations over
				while (count < loc) {
					index++;
					if (m_pSequence[index] < 0) {
						count++;
					}
				}
				choices--;
				
				// save the value in the sequence
				m_pSequence[index] = j;
			}
		}
	} 
	else {	// block randomization
		for (int i = 0; i < iPresentations; i++) {
			int choices = iNumLevels;
			for (int j = 0; j < iNumLevels; j++) {
				int loc = rand() % choices;
				int count = 0;
				int index = 0;

				// find the first empty location
				while (m_pSequence[i*iNumLevels + index] >= 0) index++;

				// count loc number of empty locations over
				while (count < loc) {
					index++;
					if (m_pSequence[i * iNumLevels + index] < 0) {
						count++;
					}
				}
				choices--;
				
				// save the value in the sequence
				m_pSequence[i * iNumLevels + index] = j;
			}
		}
	}
}

void CConstantStimuli::update() 
{
	if (iSeqIndex >= iSeqLength) {
		AfxMessageBox("Experiment continuing beyond normal execution");
		iSeqIndex = -1;
	}
	iSeqIndex++;
}

void CConstantStimuli::setNumLevels(int i) 
{
	iNumLevels = i;
}

int CConstantStimuli::getNumLevels() 
{
	return iNumLevels;
}

int CConstantStimuli::getSeqLength() 
{
	return iSeqLength;
}

int CConstantStimuli::getCurrentLevel() 
{
	return m_pSequence[iSeqIndex];
}

bool CConstantStimuli::isComplete() 
{
	return iSeqIndex >= iSeqLength;
}

}; // namespace etu
