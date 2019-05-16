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
 * Cache the point being fixated by calling NewPoint(xx, yy).  Then calculate
 * left and right eye positions with the four methods.  Each is a hypothetical
 * calculation.
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
	void ZipContainers(CTrialSaccades::COLOR_RADIO_VALUE method, int blockSize, 
		TrialPtrContainer_t& inLeft, TrialPtrContainer_t& inRight, 
		TrialPtrContainer_t& outZipped);
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
	switch (m_stimCode) {
		case CODE_C_RED_P_RED:
		case CODE_C_BLUE_P_RED:
			m_stimEye = EYE_RIGHT;
			break;

		case CODE_C_RED_P_BLUE:
		case CODE_C_BLUE_P_BLUE:
		default:
			m_stimEye = EYE_LEFT;
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
 * @brief True if code has red center target; false for blue
 */
bool CSaccadeTrial::isCenterRed(CSaccadeTrial::STIM_CODE stimCode)
{
	return (
		(stimCode == CODE_C_RED_P_RED)    || 
		(stimCode == CODE_C_RED_P_BLUE));
}

/**
 * @brief True if code has red peripheral target; false for blue.
 */
bool CSaccadeTrial::isPeriphRed(CSaccadeTrial::STIM_CODE stimCode)
{
	return (
		(stimCode == CODE_C_RED_P_RED)    || 
		(stimCode == CODE_C_BLUE_P_RED));
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
	TrialPtrContainer_t trials_blueC_redP;
	TrialPtrContainer_t trials_blueC_blueP;
	TrialPtrContainer_t trials_redC_all;
	TrialPtrContainer_t trials_blueC_all;

	/* Step 1. Generate lists of trials */
	bool genRedC = false, genBlueC = false; 
	bool genRedP = false, genBlueP = false;
	
	switch (m_trialSaccades.getCenterColorRadio()) {
		case CTrialSaccades::COLOR_RANDOM:
		case CTrialSaccades::COLOR_ALTERNATE:
			genRedC = true;
			genBlueC = true;
			break;

		case CTrialSaccades::COLOR_PRIMARY:
			genRedC = true;
			break;

		case CTrialSaccades::COLOR_SECONDARY:
			genBlueC = true;
			break;
	}

	switch (m_trialSaccades.getPeriphColorRadio()) {
		case CTrialSaccades::COLOR_RANDOM:
		case CTrialSaccades::COLOR_ALTERNATE:
			genRedP = true;
			genBlueP = true;
			break;

		case CTrialSaccades::COLOR_PRIMARY:
			genRedP = true;
			break;

		case CTrialSaccades::COLOR_SECONDARY:
			genBlueP = true;
			break;
	}
	
	if (genRedC && genRedP) {
		GenerateLocationsByCode(CSaccadeTrial::CODE_C_RED_P_RED, 
			trials_redC_redP, m_trialSaccades.getNumRedGrids());
	}

	if (genRedC && genBlueP) {
		GenerateLocationsByCode(CSaccadeTrial::CODE_C_RED_P_BLUE, 
			trials_redC_blueP, m_trialSaccades.getNumBlueGrids());
	}

	if (genBlueC && genRedP) {
		GenerateLocationsByCode(CSaccadeTrial::CODE_C_BLUE_P_RED, 
			trials_blueC_redP, m_trialSaccades.getNumRedGrids());
	}

	if (genBlueC && genBlueP) {
		GenerateLocationsByCode(CSaccadeTrial::CODE_C_BLUE_P_BLUE, 
			trials_blueC_blueP, m_trialSaccades.getNumBlueGrids());
	}

	/* Step 2. Sort list according to settings 
	 *
	 * We now have six containers of trials, with each container randomized.
	 * We now stitch them together by alternating or randomizing.  We use the
	 * two _all containers to aggregate.
	 */
	ZipContainers(m_trialSaccades.getPeriphColorRadio(), 1, trials_redC_redP, 
		trials_redC_blueP, trials_redC_all);
	ZipContainers(m_trialSaccades.getPeriphColorRadio(), 1, trials_blueC_redP, 
		trials_blueC_blueP, trials_blueC_all);
	ZipContainers(m_trialSaccades.getCenterColorRadio(), 
		m_trialSaccades.getAlternateBlockSize(), trials_redC_all, 
		trials_blueC_all, trials);
}

/**
 * @brief Generate the four types of trials.
 * @param stimCode The stimulus code for the center and peripheral colors.
 * @param trials The container in which to store the generated trials.
 * @param numGrids Number of full grids to generate.
 */
void CSpatiotopicTrialGenerator::GenerateLocationsByCode(
	CSaccadeTrial::STIM_CODE stimCode, TrialPtrContainer_t& trials, 
	long numGrids)
{
	double ctrXDeg, ctrYDeg;
	etu::GridIter_t end = m_peripheralGrid->end();
	for (int count = 0; count < numGrids; count++) {
		etu::GridIter_t iter = m_peripheralGrid->begin();
		for ( ; iter != end; ++iter) {
			GetRandomCtrLocation(ctrXDeg, ctrYDeg);
			trials.push_back(new CSaccadeTrial(stimCode, ctrXDeg, ctrYDeg,
				iter->xDeg, iter->yDeg, iter->aDeg, iter->rDeg));
		}
	}
	
	std::random_shuffle<TrialPtrContainer_t::iterator>(trials.begin(), 
		trials.end());
}

/**
 * @brief Merge two trial containers together in user-specified fashion.
 * 
 * @param method One of the symbolic integers defined as COLOR_ in CTrialSaccades.
 *        -- COLOR_ALTERNATE: Function takes blockSize elements from left 
 *               container, then blockSize elements from right container, etc.
 *        -- COLOR_RANDOM: Function copies left and right containers to output,
 *               then shuffles the output container.
 *        -- Any other: Function copies left and right containers to output.
 * @param blockSize Size of blocks to alternate, if method is COLOR_ALTERNATE; 
 * ignored otherwise.
 * @param inLeft Left container.
 * @param inRight Right container.
 * @param outZipped The results.
 */
void CSpatiotopicTrialGenerator::ZipContainers(
	CTrialSaccades::COLOR_RADIO_VALUE method, int blockSize, 
	TrialPtrContainer_t& inLeft, TrialPtrContainer_t& inRight, 
	TrialPtrContainer_t& outZipped)
{
	/* Make handling of special cases explicit. */
	if (!inLeft.size() && !inRight.size()) {
		ETU_DEBUG_BLOCK {
			CString msg;
			msg.Format(
				"ZIP\n"
				"Nothing to do\0");
			AfxMessageBox((LPCTSTR)msg);
		}
		return;
	}
	else if (!inLeft.size()) {
		std::copy(inRight.begin(), inRight.end(), std::back_inserter(outZipped));
	}
	else if (!inRight.size()) {
		std::copy(inLeft.begin(), inLeft.end(), std::back_inserter(outZipped));
	}
	else if (method == CTrialSaccades::COLOR_ALTERNATE) {
		TrialPtrContainer_t::iterator leftIter = inLeft.begin();
		TrialPtrContainer_t::iterator leftEnd = inLeft.end();
		TrialPtrContainer_t::iterator rightIter = inRight.begin();
		TrialPtrContainer_t::iterator rightEnd = inRight.end();
		std::back_insert_iterator<TrialPtrContainer_t> outIter = std::back_inserter(outZipped);

		while ((leftIter < leftEnd) || (rightIter < rightEnd)) {
			for (int count = 0; leftIter < leftEnd && count < blockSize; 
				++count, ++leftIter, ++outIter) 
			{
				(*outIter) = (*leftIter);
			}

			for (int count = 0; rightIter < rightEnd && count < blockSize; 
				++count, ++rightIter, ++outIter) 
			{
				(*outIter) = (*rightIter);
			}
		}
	}
	else {
		std::back_insert_iterator<TrialPtrContainer_t> outIter = std::back_inserter(outZipped);
		std::copy(inLeft.begin(), inLeft.end(), outIter);
		std::copy(inRight.begin(), inRight.end(), outIter);
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
	int index = etu::ChooseRandom(0, m_fixationGrid->size() - 1);
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
 * @param stimEye The eye being stimulated.
 * @param stimCode The type of trial to make.
 * @param eyePosition CEyePositions object to calculate eye positions; 
 *        point must be cached..
 * @param perPoint SPoint struct describing the peripheral target.
 * @return The trial object; caller owns new memory.
 */
static CSaccadeTrial* newTrial(CSaccadeTrial::STIM_EYE stimEye,
							   CSaccadeTrial::STIM_CODE stimCode, 
							   const CEyePositions* eyePosition, 
							   const etu::SPoint& perPoint)
{
	double xCrossDeg, yCrossDeg;
	double xTargDeg = perPoint.xDeg;
	double yTargDeg = perPoint.yDeg;

	if (CSaccadeTrial::isCenterRed(stimCode)) {
		eyePosition->GetRightEyeFix(xCrossDeg, yCrossDeg);
	}
	else {
		eyePosition->GetLeftEyeFix(xCrossDeg, yCrossDeg);
	}

	if (g_pTrialSaccades->getUseApprox()) {
		double xSaccEyeDeg, ySaccEyeDeg;
		
		switch (stimEye) {
			case CSaccadeTrial::EYE_RIGHT: 
				if (CSaccadeTrial::isCenterRed(stimCode)) {
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
		xTargDeg, yTargDeg, perPoint.aDeg, perPoint.rDeg));
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

			/* Left-eye centered trials. */
			trials.push_back(newTrial(CSaccadeTrial::EYE_LEFT,
				CSaccadeTrial::CODE_C_RED_P_RED, m_eyePositions.get(), *perIter));
			trials.push_back(newTrial(CSaccadeTrial::EYE_LEFT,
				CSaccadeTrial::CODE_C_RED_P_BLUE, m_eyePositions.get(), *perIter));
			trials.push_back(newTrial(CSaccadeTrial::EYE_LEFT,
				CSaccadeTrial::CODE_C_BLUE_P_RED, m_eyePositions.get(), *perIter));
			trials.push_back(newTrial(CSaccadeTrial::EYE_LEFT,
				CSaccadeTrial::CODE_C_BLUE_P_BLUE, m_eyePositions.get(), *perIter));
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
	out << (trial.isCenterRed() ? "Red" : "Blue") << "\t"
		<< (trial.isPeriphRed() ? "Red" : "Blue") << "\t"
		<< "Deg: (" << trial.getPerXDeg() << ", " << trial.getPerYDeg() << ") \t"
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
