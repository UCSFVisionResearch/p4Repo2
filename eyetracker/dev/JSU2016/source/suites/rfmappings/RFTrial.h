/**
 * @file RFTrial.h
 * @brief 
 * Written by Jwong 7/12/16
 * Trial Creation for RF Mapping, goal to enable binocular mode
 */

#ifndef RFTRIAL_H
#define RFTRIAL_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <ostream>
#include <deque>
#include <utility>
#include "utils/Grid.h"
#include "RFMapping.h"



/*****************************************************************************
 * Classes
 *****************************************************************************/

/**
 * @brief Encapsulates info about a Trial-Based Saccades trial.
 *
 */
class CRFTrial 
{
public:
	/**
	 * @brief Constants for color of stimuli.
	 */
	enum STIM_COLOR {
		CODE_C_RED		  = 0, /**< Stim red		*/
		CODE_C_BLUE		  = 1, /**< Stim blue		*/
		CODE_C_PURPLE     = 2, /**< Stim purple		*/
		NUM_CODES                   /**< Number of codes; ALWAYS LAST */
	};
		/**
	 * @brief Constants for which eye is being stimulated.
	 * Despite dichroic filters, we still may try to stimulate the left eye 
	 * with red or the right eye with blue.  Values are a messaging-based 
	 * convention for JRE's post-processing tools that run in Spike2. 
	 */
	enum EYE_CODE {
		EYE_RIGHT = 0,  /**< Right eye was stimulated. */
		EYE_LEFT  = 1,  /**< Left eye was stimulated. */
	};

	/**
	 * @brief Constants for color of stimuli.
	 */
	enum RF_MODE {
		CODE_UNI		  = 0, /**< Single Fixation Point		*/
		CODE_BI			  = 1, /**< Two Fixation Points		*/
	};



	CRFTrial( EYE_CODE StimEye,STIM_COLOR stimColor, double perXDeg, double perYDeg);
	/* Support implicit copy constructor. 
	CRFTrial(const CRFTrial&);    */
	~CRFTrial();


	inline STIM_COLOR getStimColor() const;
	inline EYE_CODE getStimEye() const;
	inline double getPerXDeg() const;
	inline double getPerYDeg() const;

private:
	STIM_COLOR m_stimColor;
	EYE_CODE m_stimEye;
	double m_perXDeg; /**< Peripheral X in degrees. */
	double m_perYDeg; /**< Peripheral Y in degrees. */

	/* Not supported. */
	CRFTrial& operator=(const CRFTrial& other);
};

/****************************************************************************/

/** 
 * @brief Typedef for container of pointers to CRFTrial heap objects.
 **/
typedef std::deque<CRFTrial*> RFTrialPtrContainer_t;

/****************************************************************************/

/**
 * Object to generate RFTrials
 */
class CRFTrialGenerator
{
public:
	CRFTrialGenerator(CRFTrial::RF_MODE eyeMode, CRFTrial::STIM_COLOR colorMode, 
		etu::pGrid_t& peripheralGrid);
	virtual ~CRFTrialGenerator();

	/**
	 * @brief Generate trials based on parameters supplied at creation.
	 *
	 * @param trials The trial container in which to store generated trials.
	 */
	virtual void GenerateTrials(RFTrialPtrContainer_t& trials);

	/**
	 * @brief Create a generator.
	 *
	 * @param trialSaccades Reference to the Trial Saccades model that holds 
	 * all state parameters.
	 * @param fixationGrid The grid object for fixation cross locations.
	 * @param peripheralGrid The grid object for saccade target locations.
	 * @return A heap object implementing this interface.
	 */
	
private:
	int m_swapInt;
	int m_eyeMode;

	int m_colorMode;
	etu::pGrid_t m_peripheralGrid;
};

/*****************************************************************************
 * Prototypes
 *****************************************************************************/

std::ostream& operator<< (std::ostream& out, const CRFTrial& trial);
int angleToCode(double angleDeg, double offsetDeg);
int radiusToCode(double radiusDeg, double offsetDeg);
const char* stimCodeToString(CRFTrial::STIM_COLOR stimColor);

/*****************************************************************************
 * Inline Member Functions
 *****************************************************************************/


/** Get stim color for this trial. */
CRFTrial::STIM_COLOR CRFTrial::getStimColor() const 
{ return m_stimColor; }
/** Get stim eye for this trial. */
CRFTrial::EYE_CODE CRFTrial::getStimEye() const
{return m_stimEye;}
/** Get peripheral X coordinate, in degrees, for this trial. */
double CRFTrial::getPerXDeg() const { return m_perXDeg; }

/** Get peripheral Y coordinate, in degrees, for this trial. */
double CRFTrial::getPerYDeg() const { return m_perYDeg; }


#endif // RFTRIAL_H
