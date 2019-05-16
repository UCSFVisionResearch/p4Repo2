/**
 * @file RFTrial.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <ostream>
#include <algorithm>
#include <deque>
#include <memory>
#include <math.h>
#include "app/AppCore.h"
#include "utils/Math.h"
#include "utils/Grid.h"
#include "utils/Utils.h"
#include "RFTrial.h"
#include "RFMapping.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

/**
 * @brief Create new trial for given code and coordinates.
 *
 *
 * @param stimcolor for the stimulus.  
 * @param perXDeg The X coordinate, in degrees, of saccade target.
 * @param perYDeg The Y coordinate, in degrees, of saccade target.
 */
CRFTrial::CRFTrial(EYE_CODE stimEye, STIM_COLOR stimColor,  double perXDeg, double perYDeg)
:  m_stimEye(stimEye), m_stimColor(stimColor), m_perXDeg(perXDeg), m_perYDeg(perYDeg)
{}

/**
 * @brief Clean up.
 */
CRFTrial::~CRFTrial() 
{ 

}

/**
 * @brief Clean up.
 */
CRFTrialGenerator::~CRFTrialGenerator() { /* Empty */ }

/**
 * @brief Make generator for given grids based on state of Trial Saccades 
 * model.
 * @param trialSaccades Trial Saccades model for state information.
 * @param fixationGrid Grid from which to take fixation points; takes ownership.
 * @param peripheralGrid Grid from which to take peripheral saccade target 
 * points; takes ownership.
 */

CRFTrialGenerator::CRFTrialGenerator(CRFTrial::RF_MODE eyeMode, CRFTrial::STIM_COLOR colorMode,
									 etu::pGrid_t& peripheralGrid)
:m_eyeMode(eyeMode), m_colorMode(colorMode), m_peripheralGrid(peripheralGrid)
{}

void CRFTrialGenerator::GenerateTrials(RFTrialPtrContainer_t& trials)
{
	const etu::GridIter_t perEnd = m_peripheralGrid->end();
	etu::GridIter_t perIter = m_peripheralGrid->begin();



	switch (m_eyeMode) {
		case CRFTrial::CODE_UNI: //RF Mode 0: Singular Fixation Point, with grid Centered on Point
			for( ; perIter!=perEnd;++perIter){
				trials.push_back(new CRFTrial(CRFTrial::EYE_RIGHT, CRFTrial::CODE_C_RED,perIter->xDeg,perIter->yDeg));
				std::random_shuffle(trials.begin(), trials.end());
			}

		case CRFTrial::CODE_BI:
			//Generate Trials for STIM(L/R)COLOR(RBP)
			for( ; perIter!=perEnd;++perIter){
				if(g_pRFMapping->usePrimaryR()){ trials.push_back(new CRFTrial(CRFTrial::EYE_RIGHT,CRFTrial::CODE_C_RED,perIter->xDeg,perIter->yDeg));}
				if(g_pRFMapping->useSecondaryR()){ trials.push_back(new CRFTrial(CRFTrial::EYE_RIGHT,CRFTrial::CODE_C_BLUE,perIter->xDeg,perIter->yDeg));}
				if(g_pRFMapping->useTertiaryR()){ trials.push_back(new CRFTrial(CRFTrial::EYE_RIGHT,CRFTrial::CODE_C_PURPLE,perIter->xDeg,perIter->yDeg));}
				if(g_pRFMapping->usePrimaryL()){ trials.push_back(new CRFTrial(CRFTrial::EYE_LEFT,CRFTrial::CODE_C_RED,perIter->xDeg,perIter->yDeg));}
				if(g_pRFMapping->useSecondaryL()){ trials.push_back(new CRFTrial(CRFTrial::EYE_LEFT,CRFTrial::CODE_C_BLUE,perIter->xDeg,perIter->yDeg));}
				if(g_pRFMapping->useTertiaryL()){ trials.push_back(new CRFTrial(CRFTrial::EYE_LEFT,CRFTrial::CODE_C_PURPLE,perIter->xDeg,perIter->yDeg));}
				std::random_shuffle(trials.begin(), trials.end());
			}
			break;


	}

}
