/**
 * @file StaircaseSettings.h
 * @brief 
 */

#ifndef STAIRCASE_SETTINGS_H
#define STAIRCASE_SETTINGS_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CStaircaseSettings 
{
protected:
	float initialValue, coarseStep, fineStep, medStep, uberFineStep;;
	int numReversals, nUp, nDown;
	int iDirection;

	float min, max;
			
public:
	CStaircaseSettings() 
	{
		coarseStep = 25;//-10;
		medStep = 5;//-1;
		fineStep = .5;//-0.33f;
		uberFineStep = .05f;
		numReversals = 7;
		nUp = 3;
		nDown = 2;
		iDirection = 2; // down
		min = 0;
		max = 100;
	
	}

	float getCoarseStep() { return coarseStep; }
	void setCoarseStep(float f) { coarseStep = f; }

	float getMedStep() { return medStep; }
	void setMedStep(float f) { medStep = f;}

	float getFineStep() { return fineStep; }
	void setFineStep(float f) { fineStep = f; }

	float getUberFineStep() { return uberFineStep; }
	void setUberFineStep(float f) { uberFineStep = f;}

	int getNumReversals() { return numReversals; }
	void setNumReversals(int i) {  numReversals = i; }

	int getNUp() { return nUp; }
	void setNUp( int i) { nUp = i; }

	int getNDown() { return nDown; }
	void setNDown(int i) { nDown = i; }

	int getDirection() { return iDirection; }
	void setDirection(int i) { iDirection = i; }

	float getMin() { return min; }
	void setMin(float f) { min = f; }

	float getMax() { return max; }
	void setMax(float f) { max = f; }
};

#endif
