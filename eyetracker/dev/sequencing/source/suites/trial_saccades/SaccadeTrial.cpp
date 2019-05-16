/**
 * @file SaccadeTrial.cpp
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
#include "SaccadeTrial.h"
#include "TrialSaccades.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

/**
 * @brief Interface to a calculator for fixating and deviated eye positions.
 * 
 * Calculations differ when computing locations for the center cross and
 * peripheral target.  In all cases, cache the location of the center cross by 
 * calling NewPoint(xx, yy).  
 *
 * For the center cross, call the virtual Get[Left/Right]Eye[Fix/Dev]
 * functions to calculate strabismic locations for the left and right eyes.  For 
 * example, if the cross location is nominally (0 deg, 0 deg) for cyclopean 
 * behavior, call
 * <code>
 *     eyePositionsInst.NewPoint(0,0);
 * </code>
 * Then, for a red cross (right eye), call
 * <code>
 *     eyePositionsInst.GetRightEyeFix(xDeg, yDeg);
 * </code>
 * The class uses the stored (approximate) deviation to calculate cross 
 * positions.
 *
 * To get the location of the deviated eye for use with a peripheral target
 * vector, functionality differs between approximation mode and eye-tracking 
 * mode.  In approximation mode, the same virtual Get[Left/Right]Eye[Fix/Dev]
 * functions are used.  In eye-tracking mode, however, the tracked position of
 * the eye is needed.  Thus the non-virtual function should be used: it will
 * return the tracked location for eye-tracking mode but dispatch to the
 * virtual functions for approximation mode.
 */
class CEyePositions
{
public:
	explicit CEyePositions(double devDeg);
	virtual ~CEyePositions();

	void NewPoint(double xDeg, double yDeg);

	virtual void GetLeftEyeFix(double& xDeg, double& yDeg)  const = 0;
	virtual void GetRightEyeFix(double& xDeg, double& yDeg) const = 0;
	virtual void GetLeftEyeDev(double& xDeg, double& yDeg)  const = 0;
	virtual void GetRightEyeDev(double& xDeg, double& yDeg) const = 0;

	/**
	 * @TODO Handle real-time eye tracking better.
	 * Store the CEyePositions object in the global CTrialSaccades object.
	 * Non-virtual methods in CEyePositions will return real-time eye position
	 * if RTET is enabled, or will dispatch to virtual approximation methods
	 * if RTET is not enabled.
	 */

protected:
	double m_devDeg;
	double m_xDeg;
	double m_yDeg;
};

/**
 * @brief Typedef for auto pointer to CEyePositions.
 */
typedef std::auto_ptr<CEyePositions> pEyePositions_t;

/****************************************************************************/

/**
 * @brief Calculator for "cyclopean" eye positions, where each actual eye 
 * position falls to the left or right of the cached point.
 *
 * For cyclopean eye fixation, the _Fix and _Dev functions return the same
 * values for respective eyes:
 * <code>
 *     double xx, yy;
 *
 *     GetLeftEyeFix(xx, yy);  // Equivalent to
 *     GetLeftEyeDev(xx, yy);  // each other.
 *
 *     GetRightEyeFix(xx, yy); // Equivalent to
 *     GetRightEyeDev(xx, yy); // each other.
 * </code>
 */
class CCyclopeanEyePositions : public CEyePositions
{
public:
	explicit CCyclopeanEyePositions(double devDeg);
	virtual ~CCyclopeanEyePositions();

	virtual void GetLeftEyeFix(double& xDeg, double& yDeg) const;
	virtual void GetRightEyeFix(double& xDeg, double& yDeg) const;
	virtual void GetLeftEyeDev(double& xDeg, double& yDeg) const;
	virtual void GetRightEyeDev(double& xDeg, double& yDeg) const;
};

/****************************************************************************/

/**
 * @brief Calculator for random eye positions, where one eye is fixated and
 * the other is deviated.
 */
class CRandomEyePositions : public CEyePositions
{
public:
	explicit CRandomEyePositions(double devDeg);
	virtual ~CRandomEyePositions();

	virtual void GetLeftEyeFix(double& xDeg, double& yDeg) const;
	virtual void GetRightEyeFix(double& xDeg, double& yDeg) const;
	virtual void GetLeftEyeDev(double& xDeg, double& yDeg) const;
	virtual void GetRightEyeDev(double& xDeg, double& yDeg) const;
};

/****************************************************************************/

/**
 * @brief Generator for Saccade Trials with random coverage of eyes.
 */
class CSpatiotopicTrialGenerator : public CSaccadeTrialGenerator
{
public:
	CSpatiotopicTrialGenerator(const CTrialSaccades& trialSaccades, 
		etu::pGrid_t& fixationGrid, etu::pGrid_t& peripheralGrid);
	virtual ~CSpatiotopicTrialGenerator();

	virtual void GenerateTrials(TrialPtrContainer_t& trials);

private:
	void GenerateLocationsByCode(CSaccadeTrial::STIM_CODE stimCode, 
		TrialPtrContainer_t& trials, long numGrids);
	void ZipContainers(CTrialSaccades::COLOR_RADIO_VALUE method, 
		int blockSize, TrialPtrContainer_t& outZipped, 
		TrialPtrContainer_t& inFirst, TrialPtrContainer_t& inSecond, 
		TrialPtrContainer_t& inThird);
	void GetRandomCtrLocation(double& outX, double& outY);
	
	const CTrialSaccades& m_trialSaccades;
	etu::pGrid_t m_fixationGrid;
	etu::pGrid_t m_peripheralGrid;

	/* Not supported. */
	CSpatiotopicTrialGenerator(const CSpatiotopicTrialGenerator&);
	CSpatiotopicTrialGenerator& operator=(const CSpatiotopicTrialGenerator&);
};

/****************************************************************************/

/**
 * @brief Object to generate Saccade Trials with reciprocal coverage for each 
 * eye.
 */
class CRetinotopicTrialGenerator : public CSaccadeTrialGenerator
{
public:
	CRetinotopicTrialGenerator(const CTrialSaccades& trialSaccades,
		etu::pGrid_t& fixationGrid, etu::pGrid_t& peripheralGrid, 
		pEyePositions_t& eyePositions);
	virtual ~CRetinotopicTrialGenerator();

	virtual void GenerateTrials(TrialPtrContainer_t& trials);

private:
	const CTrialSaccades& m_trialSaccades;
	etu::pGrid_t m_fixationGrid;
	etu::pGrid_t m_peripheralGrid;
	pEyePositions_t m_eyePositions;

	/* Not supported */
	CRetinotopicTrialGenerator(const CRetinotopicTrialGenerator&);
	CRetinotopicTrialGenerator& operator=(const CRetinotopicTrialGenerator&);
};

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

/**
 * @brief Create new trial for given code and coordinates.
 *
 * Assigns default stim eye based on color.
 *
 * @param stimCode Code for the stimulus.  See STIM_CODE.
 * @param ctrXDeg The X coordinate, in degrees, of center cross.
 * @param ctrYDeg The Y coordinate, in degrees, of center cross.
 * @param perXDeg The X coordinate, in degrees, of saccade target.
 * @param perYDeg The Y coordinate, in degrees, of saccade target.
 * @param perADeg The radial angle, in degrees, of saccade target.
 * @param perRDeg The radius (eccentricity), in degrees, of saccade target.
 */
CSaccadeTrial::CSaccadeTrial(STIM_CODE stimCode, double ctrXDeg, 
							 double ctrYDeg, double perXDeg, double perYDeg, 
							 double perADeg, double perRDeg)
: m_stimCode(stimCode), m_ctrXDeg(ctrXDeg), m_ctrYDeg(ctrYDeg), 
  m_perXDeg(perXDeg), m_perYDeg(perYDeg), m_perADeg(perADeg), 
  m_perRDeg(perRDeg)
{
	int randomRight = etu::RandCoinFlip();

	switch (m_stimCode) {
		case CODE_C_RED_P_RED:
		case CODE_C_BLUE_P_RED:
		case CODE_C_PURPLE_P_RED:
			m_stimEye = EYE_RIGHT;
			break;

		case CODE_C_RED_P_BLUE:
		case CODE_C_BLUE_P_BLUE:
		case CODE_C_PURPLE_P_BLUE:
			m_stimEye = EYE_LEFT;
			break;

		case CODE_C_RED_P_PURPLE:
		case CODE_C_BLUE_P_PURPLE:
		case CODE_C_PURPLE_P_PURPLE:
		default:
			m_stimEye = (randomRight ? EYE_RIGHT : EYE_LEFT);
			break;
	}
}

/**
 * @brief Create new trial for given code, stim eye, and coordinates.
 *
 * @param stimCode Code for the stimulus.  See STIM_CODE.
 * @param stimEye Code for which eye was stimulated.  See STIM_EYE.
 * @param ctrXDeg The X coordinate, in degrees, of center cross.
 * @param ctrYDeg The Y coordinate, in degrees, of center cross.
 * @param perXDeg The X coordinate, in degrees, of saccade target.
 * @param perYDeg The Y coordinate, in degrees, of saccade target.
 * @param perADeg The radial angle, in degrees, of saccade target.
 * @param perRDeg The radius (eccentricity), in degrees, of saccade target.
 */
CSaccadeTrial::CSaccadeTrial(STIM_CODE stimCode, STIM_EYE stimEye,
							 double ctrXDeg, double ctrYDeg, double perXDeg, 
							 double perYDeg, double perADeg, double perRDeg)
: m_stimCode(stimCode), m_stimEye(stimEye), m_ctrXDeg(ctrXDeg), 
  m_ctrYDeg(ctrYDeg), m_perXDeg(perXDeg), m_perYDeg(perYDeg), 
  m_perADeg(perADeg), m_perRDeg(perRDeg)
{}

/**
 * @brief Clean up.
 */
CSaccadeTrial::~CSaccadeTrial() { /* Empty. */}

/**
 * @brief True if code has red center target; false for blue or purple.
 */
bool CSaccadeTrial::isCenterRed(CSaccadeTrial::STIM_CODE stimCode)
{
	return (
		(stimCode == CODE_C_RED_P_RED)    || 
		(stimCode == CODE_C_RED_P_BLUE)   ||
		(stimCode == CODE_C_RED_P_PURPLE));
}

/**
 * @brief True if code has blue center target; false for red or purple
 */
bool CSaccadeTrial::isCenterBlue(CSaccadeTrial::STIM_CODE stimCode)
{
	return (
		(stimCode == CODE_C_BLUE_P_RED)    || 
		(stimCode == CODE_C_BLUE_P_BLUE)   ||
		(stimCode == CODE_C_BLUE_P_PURPLE));
}

/**
 * @brief True if code has purple center target; false for red or blue
 */
bool CSaccadeTrial::isCenterPurple(CSaccadeTrial::STIM_CODE stimCode)
{
	return (
		(stimCode == CODE_C_PURPLE_P_RED)    || 
		(stimCode == CODE_C_PURPLE_P_BLUE)   ||
		(stimCode == CODE_C_PURPLE_P_PURPLE));
}

/**
 * @brief True if code has red peripheral target; false for blue or purple.
 */
bool CSaccadeTrial::isPeriphRed(CSaccadeTrial::STIM_CODE stimCode)
{
	return (
		(stimCode == CODE_C_RED_P_RED)    || 
		(stimCode == CODE_C_BLUE_P_RED)   ||
		(stimCode == CODE_C_PURPLE_P_RED));
}

/**
 * @brief True if code has blue peripheral target; false for red or purple.
 */
bool CSaccadeTrial::isPeriphBlue(CSaccadeTrial::STIM_CODE stimCode)
{
	return (
		(stimCode == CODE_C_RED_P_BLUE)    || 
		(stimCode == CODE_C_BLUE_P_BLUE)   ||
		(stimCode == CODE_C_PURPLE_P_BLUE));
}

/**
 * @brief True if code has purple peripheral target; false for red or blue.
 */
bool CSaccadeTrial::isPeriphPurple(CSaccadeTrial::STIM_CODE stimCode)
{
	return (
		(stimCode == CODE_C_RED_P_PURPLE)    || 
		(stimCode == CODE_C_BLUE_P_PURPLE)   ||
		(stimCode == CODE_C_PURPLE_P_PURPLE));
}

/****************************************************************************/

/**
 * @brief Store the deviation angle
 * @param devDeg Deviation angle between eyes, in degrees.
 */
CEyePositions::CEyePositions(double devDeg) 
: m_devDeg(devDeg), m_xDeg(0.0), m_yDeg(0.0) 
{ /* Empty */ }

/**
 * @brief Clean up.
 */
CEyePositions::~CEyePositions() { /* Empty */ }

/**
 * @brief Cache the new point.
 * @param xDeg X coordinate, in degrees.
 * @param yDeg Y coordinate, in degrees.
 */
void CEyePositions::NewPoint(double xDeg, double yDeg)
{
	m_xDeg = xDeg;
	m_yDeg = yDeg;
}

/****************************************************************************/

/**
 * @brief Store the deviation angle
 * @param devDeg Deviation angle between eyes, in degrees.
 */
CCyclopeanEyePositions::CCyclopeanEyePositions(double devDeg) 
: CEyePositions(devDeg) 
{ /* Empty */ }

/**
 * @brief Clean up.
 */
CCyclopeanEyePositions::~CCyclopeanEyePositions() { /* Empty */ }

/**
 * @brief Assuming left eye is fixating cached point, get left eye position.
 * @param[out] xDeg X coordinate, in degrees.
 * @param[out] yDeg Y coordinate, in degrees.
 */
void CCyclopeanEyePositions::GetLeftEyeFix(double& xDeg, double& yDeg) const
{
	xDeg = m_xDeg - (0.5 * m_devDeg);
	yDeg = m_yDeg;
}

/**
 * @brief Assuming right eye is fixating cached point, get right eye position.
 * @param[out] xDeg X coordinate, in degrees.
 * @param[out] yDeg Y coordinate, in degrees.
 */
void CCyclopeanEyePositions::GetRightEyeFix(double& xDeg, double& yDeg) const
{
	xDeg = m_xDeg + (0.5 * m_devDeg);
	yDeg = m_yDeg;
}

/**
 * @brief Assuming right eye is fixating cached point, get left eye position.
 * @param[out] xDeg X coordinate, in degrees.
 * @param[out] yDeg Y coordinate, in degrees.
 */
void CCyclopeanEyePositions::GetLeftEyeDev(double& xDeg, double& yDeg) const
{
	GetLeftEyeFix(xDeg, yDeg);
}

/**
 * @brief Assuming left eye is fixating cached point, get right eye position.
 * @param[out] xDeg X coordinate, in degrees.
 * @param[out] yDeg Y coordinate, in degrees.
 */
void CCyclopeanEyePositions::GetRightEyeDev(double& xDeg, double& yDeg) const
{
	GetRightEyeFix(xDeg, yDeg);
}

/****************************************************************************/

/**
 * @brief Store the deviation angle
 * @param devDeg Deviation angle between eyes, in degrees.
 */
CRandomEyePositions::CRandomEyePositions(double devDeg) 
: CEyePositions(devDeg)
{ /* Empty */ }

/**
 * @brief Clean up.
 */
CRandomEyePositions::~CRandomEyePositions() { /* Empty */ }

/**
 * @brief Assuming left eye is fixating cached point, get left eye position.
 * @param[out] xDeg X coordinate, in degrees.
 * @param[out] yDeg Y coordinate, in degrees.
 */
void CRandomEyePositions::GetLeftEyeFix(double& xDeg, double& yDeg) const
{
	xDeg = m_xDeg;
	yDeg = m_yDeg;
}

/**
 * @brief Assuming right eye is fixating cached point, get right eye position.
 * @param[out] xDeg X coordinate, in degrees.
 * @param[out] yDeg Y coordinate, in degrees.
 */
void CRandomEyePositions::GetRightEyeFix(double& xDeg, double& yDeg) const
{
	xDeg = m_xDeg;
	yDeg = m_yDeg;
}

/**
 * @brief Assuming right eye is fixating cached point, get left eye position.
 * @param[out] xDeg X coordinate, in degrees.
 * @param[out] yDeg Y coordinate, in degrees.
 */
void CRandomEyePositions::GetLeftEyeDev(double& xDeg, double& yDeg) const
{
	xDeg = m_xDeg - m_devDeg;
	yDeg = m_yDeg;
}

/**
 * @brief Assuming left eye is fixating cached point, get right eye position.
 * @param[out] xDeg X coordinate, in degrees.
 * @param[out] yDeg Y coordinate, in degrees.
 */
void CRandomEyePositions::GetRightEyeDev(double& xDeg, double& yDeg) const
{
	xDeg = m_xDeg + m_devDeg;
	yDeg = m_yDeg;
}



/****************************************************************************/

/**
 * @brief Clean up.
 */
CSaccadeTrialGenerator::~CSaccadeTrialGenerator() { /* Empty */ }

/**
 * @brief Make generator for given grids based on state of Trial Saccades 
 * model.
 * @param trialSaccades Trial Saccades model for state information.
 * @param fixationGrid Grid from which to take fixation points; takes ownership.
 * @param peripheralGrid Grid from which to take peripheral saccade target 
 * points; takes ownership.
 */
CSaccadeTrialGenerator* CSaccadeTrialGenerator::GetGenerator(
	const CTrialSaccades& trialSaccades, etu::pGrid_t& fixationGrid, 
	etu::pGrid_t& peripheralGrid)
{
	if (trialSaccades.getEyeCoverage() == CTrialSaccades::COVERAGE_RETINOTOPIC)
	{
		double deviationAngle = trialSaccades.getDeviationAngle();
		pEyePositions_t eyePositions;

		if (trialSaccades.getFixationEye() == CTrialSaccades::FIXATION_RANDOM)
		{
			eyePositions.reset(new CRandomEyePositions(deviationAngle));
		}
		else {
			eyePositions.reset(new CCyclopeanEyePositions(deviationAngle));
		}

		return (new CRetinotopicTrialGenerator(trialSaccades, fixationGrid,
			peripheralGrid, eyePositions));
	}
	else {
		return (new CSpatiotopicTrialGenerator(trialSaccades, fixationGrid, 
			peripheralGrid));
	}
}

/****************************************************************************/

/**
 * @brief Create generator for random eye coverage.
 * @param trialSaccades Reference to the trialSaccades model.
 * @param fixationGrid Grid for center fixation points; takes ownership.
 * @param stepsY Grid for peripheral saccade targets; takes ownership.
 */
CSpatiotopicTrialGenerator::CSpatiotopicTrialGenerator(
	const CTrialSaccades& trialSaccades, etu::pGrid_t& fixationGrid, 
	etu::pGrid_t& peripheralGrid)
: m_trialSaccades(trialSaccades), m_fixationGrid(fixationGrid), 
  m_peripheralGrid(peripheralGrid)
{}

/**
 * @brief Clean up.
 */
CSpatiotopicTrialGenerator::~CSpatiotopicTrialGenerator() 
{}

/**
 * @brief Generate trials for random eye coverage according to stored grids.
 * @param trials Container to receive the objects.
 */
void CSpatiotopicTrialGenerator::GenerateTrials(TrialPtrContainer_t& trials)
{
	/* If user requested alternation of central color, peripheral color, or 
	 * both, we need to segregate trials. May use fewer than all of these, but
	 * tough to code w/o them. 
	 */
	TrialPtrContainer_t trials_redC_redP;
	TrialPtrContainer_t trials_redC_blueP;
	TrialPtrContainer_t trials_redC_purpleP;

	TrialPtrContainer_t trials_blueC_redP;
	TrialPtrContainer_t trials_blueC_blueP;
	TrialPtrContainer_t trials_blueC_purpleP;

	TrialPtrContainer_t trials_purpleC_redP;
	TrialPtrContainer_t trials_purpleC_blueP;
	TrialPtrContainer_t trials_purpleC_purpleP;

	TrialPtrContainer_t trials_redC_all;
	TrialPtrContainer_t trials_blueC_all;
	TrialPtrContainer_t trials_purpleC_all;

	/* Step 1. Generate lists of trials */
	int ratioRedC = 0, ratioBlueC = 0, ratioPurpleC = 0;
	int ratioRedP = 0, ratioBlueP = 0, ratioPurpleP = 0;
	
	switch (m_trialSaccades.getCenterColorRadio()) {
		case CTrialSaccades::COLOR_RANDOM:
			etu::reduceRatioV(3, m_trialSaccades.getPctRedCtr(), 
				m_trialSaccades.getPctBlueCtr(), 
				m_trialSaccades.getPctPurpleCtr(),
				&ratioRedC, &ratioBlueC, &ratioPurpleC);
			break;
		
		case CTrialSaccades::COLOR_ALTERNATE:
			ratioRedC = 1;
			ratioBlueC = 1;
			ratioPurpleC = 1;
			break;

		case CTrialSaccades::COLOR_PRIMARY:
			ratioRedC = 1;
			break;

		case CTrialSaccades::COLOR_SECONDARY:
			ratioBlueC = 1;
			break;

		case CTrialSaccades::COLOR_TERTIARY:
			ratioPurpleC = 1;
			break;
	}

	switch (m_trialSaccades.getPeriphColorRadio()) {
		case CTrialSaccades::COLOR_RANDOM:
			etu::reduceRatioV(3, m_trialSaccades.getPctRedGrids(),
				m_trialSaccades.getPctBlueGrids(), 
				m_trialSaccades.getPctPurpleGrids(),
				&ratioRedP, &ratioBlueP, &ratioPurpleP);
			break;
		case CTrialSaccades::COLOR_ALTERNATE:
			ratioRedP = 1;
			ratioBlueP = 1;
			ratioPurpleP = 1;
			break;

		case CTrialSaccades::COLOR_PRIMARY:
			ratioRedP = 1;
			break;

		case CTrialSaccades::COLOR_SECONDARY:
			ratioBlueP = 1;
			break;

		case CTrialSaccades::COLOR_TERTIARY:
			ratioPurpleP = 1;
			break;
	}
	
	for ( ; ratioRedC ; ratioRedC--) {
		GenerateLocationsByCode(CSaccadeTrial::CODE_C_RED_P_RED, 
			trials_redC_redP, ratioRedP);
	
		GenerateLocationsByCode(CSaccadeTrial::CODE_C_RED_P_BLUE, 
			trials_redC_blueP, ratioBlueP);
	
		GenerateLocationsByCode(CSaccadeTrial::CODE_C_RED_P_PURPLE, 
			trials_redC_purpleP, ratioPurpleP);
	}

	for ( ; ratioBlueC ; ratioBlueC--) {
		GenerateLocationsByCode(CSaccadeTrial::CODE_C_BLUE_P_RED, 
			trials_blueC_redP, ratioRedP);

		GenerateLocationsByCode(CSaccadeTrial::CODE_C_BLUE_P_BLUE, 
			trials_blueC_blueP, ratioBlueP);

		GenerateLocationsByCode(CSaccadeTrial::CODE_C_BLUE_P_PURPLE, 
			trials_blueC_purpleP, ratioPurpleP);
	}

	for ( ; ratioPurpleC ; ratioPurpleC--) {
		GenerateLocationsByCode(CSaccadeTrial::CODE_C_PURPLE_P_RED, 
			trials_purpleC_redP, ratioRedP);

		GenerateLocationsByCode(CSaccadeTrial::CODE_C_PURPLE_P_BLUE, 
			trials_purpleC_blueP, ratioBlueP);

		GenerateLocationsByCode(CSaccadeTrial::CODE_C_PURPLE_P_PURPLE, 
			trials_purpleC_purpleP, ratioPurpleP);
	}

	/* Step 2. Sort list according to settings 
	 *
	 * We now have nine containers of trials, with each container randomized.
	 * We now stitch them together by alternating or randomizing.  We use the
	 * three _all containers to aggregate.
	 */
	ZipContainers(m_trialSaccades.getPeriphColorRadio(), 1, trials_redC_all,
		trials_redC_redP, trials_redC_blueP, trials_redC_purpleP);

	ZipContainers(m_trialSaccades.getPeriphColorRadio(), 1, trials_blueC_all,
		trials_blueC_redP, trials_blueC_blueP, trials_blueC_purpleP);

	ZipContainers(m_trialSaccades.getPeriphColorRadio(), 1, trials_purpleC_all,
		trials_purpleC_redP, trials_purpleC_blueP, trials_purpleC_purpleP);

	ZipContainers(m_trialSaccades.getCenterColorRadio(), 
		m_trialSaccades.getAlternateBlockSize(), trials,
		trials_redC_all, trials_blueC_all, trials_purpleC_all);
}

/**
 * @brief Generate the types of trials.
 * @param stimCode The stimulus code for the center and peripheral colors.
 * @param trials The container in which to store the generated trials.
 * @param numGrids Number of full grids to generate; if 0, no-op.
 */
void CSpatiotopicTrialGenerator::GenerateLocationsByCode(
	CSaccadeTrial::STIM_CODE stimCode, TrialPtrContainer_t& trials, 
	long numGrids)
{
	double ctrXDeg, ctrYDeg;
	etu::GridIter_t ctrEnd = m_fixationGrid->end();
	etu::GridIter_t periphEnd = m_peripheralGrid->end();
	etu::GridIter_t ctrIter;
	etu::GridIter_t periphIter;

	/**
	 * @todo HACK ALERT
	 * This is a hack to enable an even bigger hack. Here, we iterate over
	 * each location for the center cross, generating full peripheral
	 * coverage for each of those cross locations. This isn't how it's
	 * supposed to work, but doing this here enables the nuclear hack in
	 * CTrialSaccades::DisplayStimulus. Read more of my sins there.
	 * (JVB 10/25/2013)
	 */
	for (ctrIter = m_fixationGrid->begin(); ctrIter < ctrEnd; ++ctrIter) {

		ctrXDeg = ctrIter->xDeg;
		ctrYDeg = ctrIter->yDeg;

		for (int count = 0 ; count < numGrids; count++) {
			
			for (periphIter = m_peripheralGrid->begin(); periphIter < periphEnd; ++periphIter) 
			{
				trials.push_back(new CSaccadeTrial(stimCode, ctrXDeg, ctrYDeg,
					periphIter->xDeg, periphIter->yDeg, periphIter->aDeg, 
					periphIter->rDeg));
			}

			std::random_shuffle<TrialPtrContainer_t::iterator>(
				trials.end() - m_peripheralGrid->size(), 
				trials.end());
		}	

	}
}

/**
 * @brief Merge multiple trial containers together in user-specified fashion.
 * 
 * @param method One of the symbolic integers defined as COLOR_ in CTrialSaccades.
 *        -- COLOR_ALTERNATE: Function takes blockSize elements from each 
 *               container in turn.
 *        -- COLOR_RANDOM: Function copies all containers to output, then 
 *               shuffles the output container.
 *        -- Any other: Function copies all containers to output in order.
 * @param blockSize Size of blocks to alternate, if method is COLOR_ALTERNATE; 
 * ignored otherwise.
 * @param outZipped The results.
 * @param inFirst First source container to zip.
 * @param inSecond Second source container to zip.
 * @param inThird Third source container to zip.
 */
void CSpatiotopicTrialGenerator::ZipContainers(
	CTrialSaccades::COLOR_RADIO_VALUE method, int blockSize, 
	TrialPtrContainer_t& outZipped, TrialPtrContainer_t& inFirst,
	TrialPtrContainer_t& inSecond, TrialPtrContainer_t& inThird) 
{
	/* Make handling of special cases explicit. */
	if (!inFirst.size() && !inSecond.size() && !inThird.size()) {
		ETU_DEBUG_BLOCK {
			CString msg;
			msg.Format(
				"ZIP\n"
				"Nothing to do\0");
			AfxMessageBox((LPCTSTR)msg);
		}
		return;
	}
	else if (!inFirst.size() && !inSecond.size()) {
		std::copy(inThird.begin(), inThird.end(), std::back_inserter(outZipped));
	}
	else if (!inFirst.size() && !inThird.size()) {
		std::copy(inSecond.begin(), inSecond.end(), std::back_inserter(outZipped));
	}
	else if (!inSecond.size() && !inThird.size()) {
		std::copy(inFirst.begin(), inFirst.end(), std::back_inserter(outZipped));
	}
	else if (method == CTrialSaccades::COLOR_ALTERNATE) {
		TrialPtrContainer_t::iterator firstIter = inFirst.begin();
		TrialPtrContainer_t::iterator firstEnd = inFirst.end();
		TrialPtrContainer_t::iterator secondIter = inSecond.begin();
		TrialPtrContainer_t::iterator secondEnd = inSecond.end();
		TrialPtrContainer_t::iterator thirdIter = inThird.begin();
		TrialPtrContainer_t::iterator thirdEnd = inThird.end();
		std::back_insert_iterator<TrialPtrContainer_t> outIter = std::back_inserter(outZipped);

		while ((firstIter < firstEnd) || (secondIter < secondEnd) || 
			(thirdIter < thirdEnd)) 
		{
			for (int count = 0; (firstIter < firstEnd) && (count < blockSize); 
				++count, ++firstIter, ++outIter) 
			{
				(*outIter) = (*firstIter);
			}

			for (int count = 0; (secondIter < secondEnd) && (count < blockSize); 
				++count, ++secondIter, ++outIter) 
			{
				(*outIter) = (*secondIter);
			}

			for (int count = 0; (thirdIter < thirdEnd) && (count < blockSize); 
				++count, ++thirdIter, ++outIter) 
			{
				(*outIter) = (*thirdIter);
			}
		}
	}
	else {
		std::back_insert_iterator<TrialPtrContainer_t> outIter = std::back_inserter(outZipped);
		std::copy(inFirst.begin(), inFirst.end(), outIter);
		std::copy(inSecond.begin(), inSecond.end(), outIter);
		std::copy(inThird.begin(), inThird.end(), outIter);
	}

	if (method == CTrialSaccades::COLOR_RANDOM) {
		std::random_shuffle<TrialPtrContainer_t::iterator>(outZipped.begin(), 
			outZipped.end());
	}
}

/**
 * @brief Get a random location for the center fixation target.
 * @param[out] outX The X location, in degrees.
 * @param[out] outY The Y location, in degrees.
 */
void CSpatiotopicTrialGenerator::GetRandomCtrLocation(double& outX, double& outY)
{
	int index = etu::RandChoice(0, m_fixationGrid->size() - 1);
	etu::SPoint& ctrPt = (*m_fixationGrid)[index];

	outX = ctrPt.xDeg;
	outY = ctrPt.yDeg;
}

/****************************************************************************/

/**
 * @brief Construct object to generate points for reciprocal eye coverage.
 * @param trialSaccades Reference to the trialSaccades model.
 * @param fixationGrid Grid for center fixation points; takes ownership.
 * @param peripheralGrid Grid for peripheral saccade targets; takes ownership.
 * @param eyePositions Eye position calculated; takes ownership.
 */
CRetinotopicTrialGenerator::CRetinotopicTrialGenerator(
	const CTrialSaccades& trialSaccades, etu::pGrid_t& fixationGrid, 
	etu::pGrid_t& peripheralGrid, pEyePositions_t& eyePositions)
: m_trialSaccades(trialSaccades), m_fixationGrid(fixationGrid), 
  m_peripheralGrid(peripheralGrid), m_eyePositions(eyePositions)
{}

/**
 * @brief Cleanup
 */
CRetinotopicTrialGenerator::~CRetinotopicTrialGenerator() 
{}

/**
 * @brief Create new trial given eye position, fixation point, and peripheral 
 * point.
 * @param stimEye The eye upon which to base the retinotopic behavior.
 * @param stimCode The type of trial to make.
 * @param eyePosition CEyePositions object to calculate eye positions; 
 *        point must be cached..
 * @param perVector SPoint struct describing the peripheral target vector.
 * @return The trial object; caller owns new memory.
 */
static CSaccadeTrial* newTrial(CSaccadeTrial::STIM_EYE stimEye,
							   CSaccadeTrial::STIM_CODE stimCode, 
							   const CEyePositions* eyePosition, 
							   const etu::SPoint& perVector)
{
	bool randomRight = etu::RandCoinFlip();
	double xCrossDeg, yCrossDeg;
	double xTargDeg = perVector.xDeg;
	double yTargDeg = perVector.yDeg;

	if (CSaccadeTrial::isCenterRed(stimCode)) {
		eyePosition->GetRightEyeFix(xCrossDeg, yCrossDeg);
	}
	else if (CSaccadeTrial::isCenterBlue(stimCode)) {
		eyePosition->GetLeftEyeFix(xCrossDeg, yCrossDeg);
	}
	/* Purple centers split half-half randomly. */
	else if (randomRight) {
		eyePosition->GetRightEyeFix(xCrossDeg, yCrossDeg);
	}
	else {
		eyePosition->GetLeftEyeFix(xCrossDeg, yCrossDeg);
	}

	/**
	 * @TODO Handle real-time eye tracking better.
	 * Store the CEyePositions object in the global CTrialSaccades object.
	 * Non-virtual methods in CEyePositions will return real-time eye position
	 * if RTET is enabled, or will dispatch to virtual approximation methods
	 * if RTET is not enabled.
	 */

	if (g_pTrialSaccades->getUseApprox()) {
		double xSaccEyeDeg, ySaccEyeDeg;
		
		switch (stimEye) {
			case CSaccadeTrial::EYE_RIGHT: 
				if (CSaccadeTrial::isCenterRed(stimCode)) {
					xSaccEyeDeg = xCrossDeg;
					ySaccEyeDeg = yCrossDeg;
				}
				else if (CSaccadeTrial::isCenterBlue(stimCode)) {
					eyePosition->GetRightEyeDev(xSaccEyeDeg, ySaccEyeDeg);
				}
				else if (randomRight) {
					xSaccEyeDeg = xCrossDeg;
					ySaccEyeDeg = yCrossDeg;
				}
				else {
					eyePosition->GetRightEyeDev(xSaccEyeDeg, ySaccEyeDeg);
				}
				break;

			case CSaccadeTrial::EYE_LEFT:
				if (CSaccadeTrial::isCenterRed(stimCode)) {
					eyePosition->GetLeftEyeDev(xSaccEyeDeg, ySaccEyeDeg);
				}
				else if (CSaccadeTrial::isCenterBlue(stimCode)) {
					xSaccEyeDeg = xCrossDeg;
					ySaccEyeDeg = yCrossDeg;
				}
				else if (!randomRight) {
					eyePosition->GetLeftEyeDev(xSaccEyeDeg, ySaccEyeDeg);
				}
				else {
					xSaccEyeDeg = xCrossDeg;
					ySaccEyeDeg = yCrossDeg;
				}
				break;
		}

		xTargDeg += xSaccEyeDeg;
		yTargDeg += ySaccEyeDeg;
	}

	return (new CSaccadeTrial(stimCode, stimEye, xCrossDeg, yCrossDeg, 
		xTargDeg, yTargDeg, perVector.aDeg, perVector.rDeg));
}

/**
 * @brief Generate a radial grid.
 * 
 * @param stimCode Code to use for the trial.
 * @param trials Container to receive the objects.
 * @param numGrids The number of grids to generate; default of 1.
 */
void CRetinotopicTrialGenerator::GenerateTrials(TrialPtrContainer_t& trials)
{
	const etu::GridIter_t fixEnd = m_fixationGrid->end();
	const etu::GridIter_t perEnd = m_peripheralGrid->end();

	etu::GridIter_t fixIter = m_fixationGrid->begin();
	etu::GridIter_t perIter = m_peripheralGrid->begin();

	for ( ; fixIter != fixEnd; fixIter++, perIter = m_peripheralGrid->begin()) {

		m_eyePositions->NewPoint(fixIter->xDeg, fixIter->yDeg);
		
		for ( ; perIter != perEnd; perIter++) {
			/* Right-eye centered trials. */
			trials.push_back(newTrial(CSaccadeTrial::EYE_RIGHT,
				CSaccadeTrial::CODE_C_RED_P_RED, m_eyePositions.get(), *perIter));
			trials.push_back(newTrial(CSaccadeTrial::EYE_RIGHT,
				CSaccadeTrial::CODE_C_RED_P_BLUE, m_eyePositions.get(), *perIter));
			trials.push_back(newTrial(CSaccadeTrial::EYE_RIGHT, 
				CSaccadeTrial::CODE_C_BLUE_P_RED, m_eyePositions.get(), *perIter));
			trials.push_back(newTrial(CSaccadeTrial::EYE_RIGHT, 
				CSaccadeTrial::CODE_C_BLUE_P_BLUE, m_eyePositions.get(), *perIter));

			/** @TODO Why add purple to retinotopic -- was this a bug from
			 * day 0? -- JVB 23 Jun 2014
		    trials.push_back(newTrial(CSaccadeTrial::EYE_RIGHT,
				CSaccadeTrial::CODE_C_RED_P_PURPLE, m_eyePositions.get(), *perIter));
			trials.push_back(newTrial(CSaccadeTrial::EYE_RIGHT, 
				CSaccadeTrial::CODE_C_BLUE_P_PURPLE, m_eyePositions.get(), *perIter));
			trials.push_back(newTrial(CSaccadeTrial::EYE_RIGHT,
				CSaccadeTrial::CODE_C_PURPLE_P_RED, m_eyePositions.get(), *perIter));
			trials.push_back(newTrial(CSaccadeTrial::EYE_RIGHT,
				CSaccadeTrial::CODE_C_PURPLE_P_BLUE, m_eyePositions.get(), *perIter));
			trials.push_back(newTrial(CSaccadeTrial::EYE_RIGHT,
				CSaccadeTrial::CODE_C_PURPLE_P_PURPLE, m_eyePositions.get(), *perIter));
			*/

			/* Left-eye centered trials. */
			trials.push_back(newTrial(CSaccadeTrial::EYE_LEFT,
				CSaccadeTrial::CODE_C_RED_P_RED, m_eyePositions.get(), *perIter));
			trials.push_back(newTrial(CSaccadeTrial::EYE_LEFT,
				CSaccadeTrial::CODE_C_RED_P_BLUE, m_eyePositions.get(), *perIter));
			trials.push_back(newTrial(CSaccadeTrial::EYE_LEFT,
				CSaccadeTrial::CODE_C_BLUE_P_RED, m_eyePositions.get(), *perIter));
			trials.push_back(newTrial(CSaccadeTrial::EYE_LEFT,
				CSaccadeTrial::CODE_C_BLUE_P_BLUE, m_eyePositions.get(), *perIter));
			
			/** @TODO Why add purple to retinotopic -- was this a bug from
			 * day 0? -- JVB 23 Jun 2014
		    trials.push_back(newTrial(CSaccadeTrial::EYE_LEFT,
				CSaccadeTrial::CODE_C_RED_P_PURPLE, m_eyePositions.get(), *perIter));
			trials.push_back(newTrial(CSaccadeTrial::EYE_LEFT,
				CSaccadeTrial::CODE_C_BLUE_P_PURPLE, m_eyePositions.get(), *perIter));
			trials.push_back(newTrial(CSaccadeTrial::EYE_LEFT,
				CSaccadeTrial::CODE_C_PURPLE_P_RED, m_eyePositions.get(), *perIter));
			trials.push_back(newTrial(CSaccadeTrial::EYE_LEFT,
				CSaccadeTrial::CODE_C_PURPLE_P_BLUE, m_eyePositions.get(), *perIter));
			trials.push_back(newTrial(CSaccadeTrial::EYE_LEFT,
				CSaccadeTrial::CODE_C_PURPLE_P_PURPLE, m_eyePositions.get(), *perIter));
			*/
		}
	}

	std::random_shuffle(trials.begin(), trials.end());
}

/*****************************************************************************
 * Functions
 *****************************************************************************/

/**
 * @brief Put a trial to an out stream in a readable format.
 *
 * The trial's central and peripheral colors are written, followed by the 
 * dimensions.  The codes are finally printed as a logic check.
 *
 * @param out Output stream.
 * @param trial The trial.
 */
std::ostream& operator<< (std::ostream& out, const CSaccadeTrial& trial)
{
	out << (trial.isCenterRed() ? "Red" : (trial.isCenterBlue() ? "Blue" : "Purple")) << "\t"
		<< "Ctr Deg: (" << trial.getCtrXDeg() << ", " << trial.getCtrYDeg() << ") \t"
		<< (trial.isPeriphRed() ? "Red" : (trial.isPeriphBlue() ? "Blue" : "Purple")) << "\t"
		<< "Per Deg: (" << trial.getPerXDeg() << ", " << trial.getPerYDeg() << ") \t"
		<< trial.getStimCode() << std::endl;
	return out;
}

/**
 * @brief Convert a radial angle to its hex code.
 * @param angleDeg Radial angle with offset applied.
 * @param offsetDeg The offset.
 * @return The hex code; guaranteed to fill only the lowest byte.
 */
int angleToCode(double angleDeg, double offsetDeg)
{
	/* Remove the offset to normalize to the radial grid GUI. 
	 * Then take (angle % 360) just to be sure. */
	int angle = (int)(angleDeg - offsetDeg);
	int offset = (angle % 360) / 5;
	return ((0x21 + offset) & 0xFF);
}

/**
 * @brief Convert a radius to its hex code.
 * @param angleDeg Radius with offset applied.
 * @param offsetDeg The offset.
 * @return The hex code; guaranteed to fill only the lowest byte.
 */
int radiusToCode(double radiusDeg, double offsetDeg)
{
	int offset = (int)(radiusDeg - offsetDeg) / 5;
	return ((0x21 + offset) & 0xFF);
}

/**
 * @brief Get string description of trial type by stim code.
 * 
 * @param stimCode Stimulus code
 * @return string, for example ctrR perB, or ctrP perP.
 */
const char* stimCodeToString(CSaccadeTrial::STIM_CODE stimCode)
{
	static char* strings[] = {
		"Red  Red", 
		"Red  Blue", 
		"Blue Red", 
		"Blue Blue", 
		"Red  Prpl", 
		"Blue Prpl", 
		"Prpl Red", 
		"Prpl Blue", 
		"Prpl Prpl", 
		"unknown"};

	const char* pString;

	switch (stimCode) {
		case CSaccadeTrial::CODE_C_RED_P_RED:       pString = strings[0]; break;
		case CSaccadeTrial::CODE_C_RED_P_BLUE:      pString = strings[1]; break;
		case CSaccadeTrial::CODE_C_BLUE_P_RED:      pString = strings[2]; break;
		case CSaccadeTrial::CODE_C_BLUE_P_BLUE:     pString = strings[3]; break;
		case CSaccadeTrial::CODE_C_RED_P_PURPLE:    pString = strings[4]; break;
		case CSaccadeTrial::CODE_C_BLUE_P_PURPLE:   pString = strings[5]; break;
		case CSaccadeTrial::CODE_C_PURPLE_P_RED:    pString = strings[6]; break;
		case CSaccadeTrial::CODE_C_PURPLE_P_BLUE:   pString = strings[7]; break;
		case CSaccadeTrial::CODE_C_PURPLE_P_PURPLE: pString = strings[8]; break;
		default:                                    pString = strings[9]; break;
	}

	return pString;
}
