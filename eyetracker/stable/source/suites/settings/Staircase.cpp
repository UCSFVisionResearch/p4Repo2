/**
 * @file Staircase.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "app/AppCore.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

CStaircase::CStaircase(CStaircaseSettings* pSCSettings) 
{
	coarseStep = pSCSettings->getCoarseStep();
	medStep = pSCSettings->getMedStep();
	fineStep = pSCSettings->getFineStep();
	numReversals = pSCSettings->getNumReversals();
	nUp = pSCSettings->getNUp();
	nDown = pSCSettings->getNDown();
	min = pSCSettings->getMin();
	max = pSCSettings->getMax();
	uberFineStep = pSCSettings->getUberFineStep();

	iStepArraySize = 10;
	iNumSteps = 0;
	pStepArray = (float*)calloc(iStepArraySize, sizeof(float));
}

CStaircase::~CStaircase() 
{
	free(pStepArray);
	iNumSteps = 0;
	iStepArraySize = 0;
}

void CStaircase::setInitialValue(float value) 
{
	initialValue = value;
	
}

void CStaircase::init() 
{
	currentLevel = initialValue;
	reversalsMade = 0;
	nCorrectResponses = 0;
	nIncorrectResponses = 0;
	currStep = coarseStep;
	previousDirection = 0;
}

void CStaircase::update(int response) 
{
	if (isComplete()) return;

	pStepArray[iNumSteps++] = currentLevel;
	if (iNumSteps == iStepArraySize) {
		iStepArraySize += 10;
		pStepArray = (float*)realloc(pStepArray, sizeof(float) * iStepArraySize);
	}

	bool bReversalMade = false;
	bool bAdvanceWithSmallerStep = false;

	if (response >= 1) {// correct
		nCorrectResponses++;
		nIncorrectResponses = 0;
		
		// check if we shoudl advance the current level
		if (nCorrectResponses == nUp) {
			currentLevel += currStep;

			// do not show trials at zero %
			while (min == 0 && currentLevel <= min) {
				currentLevel -= currStep;
				reversalsMade += 2;
				updateStep();
				if (isComplete()) {
					AfxMessageBox("Your finest step may be too large for this spatial frequency. The threshold is somewhere between 0 and the step size.");
					return;
				}
				currentLevel += currStep;
			}

			if (currentLevel > max) currentLevel = max;
			if (currentLevel < min) currentLevel = min;

			// check if a reversal was made
			if (previousDirection == -1) {
				reversalsMade++;
				bReversalMade = true;
			}

			previousDirection = 1;
			nCorrectResponses = 0;
		}
	} 
	else { // incorrect
		nCorrectResponses = 0;
		nIncorrectResponses++;

		// check if we shoudl advance the current level
		if (nIncorrectResponses == nDown) {
			currentLevel -= currStep;

			if (currentLevel > max) currentLevel = max;
			if (currentLevel < min) currentLevel = min;

			// check if a reversal was made
			if (previousDirection == 1) {
				reversalsMade++;
				bReversalMade = true;
			}

			previousDirection = -1;
			nIncorrectResponses = 0;
		}
	}

	if (bReversalMade) {
		updateStep();
	}
}

void CStaircase::increaseStep() 
{
	if (previousDirection == 1) {
		reversalsMade = max(reversalsMade - 2, 0);
	} 
	else {
		reversalsMade = max(reversalsMade - 1, 0);
		previousDirection = 1;
	}
	nCorrectResponses = 0;
	nIncorrectResponses = 0;

	updateStep();
}

void CStaircase::decreaseStep() 
{
	if (previousDirection == 1) {
		reversalsMade = reversalsMade + 2;
	} 
	else {
		reversalsMade = reversalsMade + 1;
		previousDirection = 1;
	}
	nCorrectResponses = 0;
	nIncorrectResponses = 0;

	updateStep();
}


bool CStaircase::isComplete() 
{
	return reversalsMade >= numReversals;
}

float CStaircase::getStepSize() 
{
	return currStep;
}

float CStaircase::getCurrentLevel() 
{
	return currentLevel;
}

int CStaircase::getReversals() 
{
	return reversalsMade;
}

void CStaircase::outputStaircaseData(FILE* fp) 
{
	for (int i = 0; i < iNumSteps; i++) {
		fprintf(fp, "%f\n", pStepArray[i]);
	}
	// print final level
	fprintf(fp, "%f\n\n", currentLevel);
}

void CStaircase::updateStep() 
{
	if (reversalsMade >=5) {
		currStep = uberFineStep;
	} 
	else if (reversalsMade >= 3) {
		currStep = fineStep;
	} 
	else if (reversalsMade >=1) {
		currStep = medStep;
	} 
	else {
		currStep = coarseStep;
	}
}
