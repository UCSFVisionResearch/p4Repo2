/**
 * @file SaccadeTrial.h
 * @brief 
 */

#ifndef SACCADETRIAL_H
#define SACCADETRIAL_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <ostream>
#include <deque>
#include <utility>
#include "utils/Grid.h"

/*****************************************************************************
 * Forward declaration
 *****************************************************************************/

class CTrialSaccades;

/*****************************************************************************
 * Classes
 *****************************************************************************/

/**
 * @brief Encapsulates info about a Trial-Based Saccades trial.
 *
 * Member functions make queries easy.
 */
class CSaccadeTrial 
{
public:
	/**
	 * @brief Constants for type of stimulus, by center and peripheral color.
	 * Values are a messaging-based convention for JRE's post-processing tools
	 * that run in Spike2.
	 */
	enum STIM_CODE {
		CODE_C_RED_P_RED       = 0, /**< Center red,  peripheral red.    */
		CODE_C_RED_P_BLUE      = 1, /**< Center red,  peripheral blue.   */
		CODE_C_BLUE_P_RED      = 2, /**< Center blue, peripheral red.    */
		CODE_C_BLUE_P_BLUE     = 3, /**< Center blue, peripheral blue.   */
		CODE_C_RED_P_PURPLE    = 4, /**< Center red, peripheral purple.   */
		CODE_C_BLUE_P_PURPLE   = 5, /**< Center blue, peripheral purple.   */
		CODE_C_PURPLE_P_RED    = 6, /**< Center purple, peripheral red.   */
		CODE_C_PURPLE_P_BLUE   = 7, /**< Center purple, peripheral blue.   */
		CODE_C_PURPLE_P_PURPLE = 8,  /**< Center purple, peripheral purple.   */
		NUM_CODES                   /**< Number of codes; ALWAYS LAST */
	};

	/**
	 * @brief Constants for which eye is being stimulated.
	 * Despite dichroic filters, we still may try to stimulate the left eye 
	 * with red or the right eye with blue.  Values are a messaging-based 
	 * convention for JRE's post-processing tools that run in Spike2. 
	 */
	enum STIM_EYE {
		EYE_RIGHT = 0,  /**< Right eye was stimulated. */
		EYE_LEFT  = 1,  /**< Left eye was stimulated. */
	};

	CSaccadeTrial(STIM_CODE stimCode, double ctrXDeg, double ctrYDeg, 
		double perXDeg, double perYDeg, double perADeg, double perRDeg);
	CSaccadeTrial(STIM_CODE stimCode, STIM_EYE stimEye, double ctrXDeg, 
		double ctrYDeg, double perXDeg, double perYDeg, double perADeg, 
		double perRDeg);
	~CSaccadeTrial();

	inline bool isCenterRed() const;
	inline bool isCenterBlue() const;
	inline bool isCenterPurple() const;
	inline bool isPeriphRed() const;
	inline bool isPeriphBlue() const;
	inline bool isPeriphPurple() const;

	static bool isCenterRed(STIM_CODE stimCode);
	static bool isCenterBlue(STIM_CODE stimCode);
	static bool isCenterPurple(STIM_CODE stimCode);
	static bool isPeriphRed(STIM_CODE stimCode);
	static bool isPeriphBlue(STIM_CODE stimCode);
	static bool isPeriphPurple(STIM_CODE stimCode);

	inline STIM_CODE getStimCode() const;
	inline STIM_EYE getStimEye() const;
	inline double getCtrXDeg() const;
	inline double getCtrYDeg() const;
	inline double getPerXDeg() const;
	inline double getPerYDeg() const;
	inline double getPerADeg() const;
	inline double getPerRDeg() const;

private:
	STIM_CODE m_stimCode;
	STIM_EYE m_stimEye;
	double m_ctrXDeg; /**< Center X in degrees. */
	double m_ctrYDeg; /**< Center Y in degrees. */
	double m_perXDeg; /**< Peripheral X in degrees. */
	double m_perYDeg; /**< Peripheral Y in degrees. */
	double m_perADeg; /**< Peripheral compass angle in degrees. */
	double m_perRDeg; /**< Peripheral radius in degrees. */
	
	/* Not supported. */
	CSaccadeTrial(const CSaccadeTrial& other);
	CSaccadeTrial& operator=(const CSaccadeTrial& other);
};

/****************************************************************************/

/** 
 * @brief Typedef for container of pointers to CSaccadeTrial heap objects.
 **/
typedef std::deque<CSaccadeTrial*> TrialPtrContainer_t;

/****************************************************************************/

/**
 * @brief Interface for an object to generate Saccade Trials.
 *
 * Also provides a static factory method for making generators.
 */
class CSaccadeTrialGenerator
{
public:
	virtual ~CSaccadeTrialGenerator();

	/**
	 * @brief Generate trials based on the options set in the Trial Saccades 
	 * model.
	 *
	 * The Trial Saccades model will contain relevant state information 
	 * including the type of coverage for each eye and the handling of the 
	 * fixation cross.  This function will access only public methods.
	 *
	 * @param trialSaccades Reference to the Trial Saccades model that holds 
	 * all state parameters.
	 * @param trials Container in which to store the generated trials.
	 */
	virtual void GenerateTrials(TrialPtrContainer_t& trials) = 0;

	static CSaccadeTrialGenerator* GetGenerator(
		const CTrialSaccades& trialSaccades, etu::pGrid_t& fixationGrid, 
		etu::pGrid_t& peripheralGrid);
};

/*****************************************************************************
 * Prototypes
 *****************************************************************************/

std::ostream& operator<< (std::ostream& out, const CSaccadeTrial& trial);
int angleToCode(double angleDeg, double offsetDeg);
int radiusToCode(double radiusDeg, double offsetDeg);
const char* stimCodeToString(CSaccadeTrial::STIM_CODE stimCode);

/*****************************************************************************
 * Inline Member Functions
 *****************************************************************************/

/**
 * @brief Return true if stim has red fixation target.
 * @return True for all stim codes w/red center target.
 */
bool CSaccadeTrial::isCenterRed() const { return isCenterRed(m_stimCode); }

/**
 * @brief Return true if stim has blue fixation target.
 * @return True for all stim codes w/red center target.
 */
bool CSaccadeTrial::isCenterBlue() const { return isCenterBlue(m_stimCode); }

/**
 * @brief Return true if stim has purple fixation target.
 * @return True for all stim codes w/purple center target.
 */
bool CSaccadeTrial::isCenterPurple() const { return isCenterPurple(m_stimCode); }

/**
 * @brief Return true if peripheral stim is red.
 * @return True for all stim codes w/red peripheral target.
 */
bool CSaccadeTrial::isPeriphRed() const { return isPeriphRed(m_stimCode); }

/**
 * @brief Return true if peripheral stim is blue.
 * @return True for all stim codes w/blue peripheral target.
 */
bool CSaccadeTrial::isPeriphBlue() const { return isPeriphBlue(m_stimCode); }

/**
 * @brief Return true if peripheral stim is purple.
 * @return True for all stim codes w/purple peripheral target.
 */
bool CSaccadeTrial::isPeriphPurple() const { return isPeriphPurple(m_stimCode); }

/** Get stim code for this trial. */
CSaccadeTrial::STIM_CODE CSaccadeTrial::getStimCode() const 
{ return m_stimCode; }

/** Get stim eye for this trial. */
CSaccadeTrial::STIM_EYE CSaccadeTrial::getStimEye() const
{ return m_stimEye; }

/** Get center X coordinate, in degrees, for this trial. */
double CSaccadeTrial::getCtrXDeg() const { return m_ctrXDeg; }

/** Get center Y coordinate, in degrees, for this trial. */
double CSaccadeTrial::getCtrYDeg() const { return m_ctrYDeg; }

/** Get peripheral X coordinate, in degrees, for this trial. */
double CSaccadeTrial::getPerXDeg() const { return m_perXDeg; }

/** Get peripheral Y coordinate, in degrees, for this trial. */
double CSaccadeTrial::getPerYDeg() const { return m_perYDeg; }

/** Get peripheral radial angle, in degrees, for this trial. */
double CSaccadeTrial::getPerADeg() const { return m_perADeg; }

/** Get peripheral radius, in degrees, for this trial. */
double CSaccadeTrial::getPerRDeg() const { return m_perRDeg; }


#endif // SACCADETRIAL_H
