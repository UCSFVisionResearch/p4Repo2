/**
 * @file Staircase.h
 * @brief 
 */

#ifndef STAIRCASE_H
#define STAIRCASE_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CStaircase 
{
public:
	CStaircase(CStaircaseSettings* pSCSettings);
	~CStaircase();

	// initialize the experiment
	void init();

	// update the exp
	void update(int response);

	// is the experiment complete?
	bool isComplete();

	// what level should be presented
	float getCurrentLevel();

	// return num of reversals
	int getReversals();

	// return curr step size
	float getStepSize();

	void setInitialValue(float value);

	void increaseStep();
	void decreaseStep();

	void outputStaircaseData(FILE* fp);
	void updateStep();

private:
	float initialValue;
	float coarseStep;
	float fineStep;
	float medStep;
	float uberFineStep;
	int numReversals;
	int nUp, nDown;
	float min, max;

	// run time vars
	float currentLevel;
	int reversalsMade;
	int nCorrectResponses;
	int nIncorrectResponses;
	float currStep;
	int previousDirection;

	int iNumSteps;
	int iStepArraySize;
	float* pStepArray;
};

#endif // STAIRCASE_H
