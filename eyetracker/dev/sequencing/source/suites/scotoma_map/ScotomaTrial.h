/**
 * @file ScotomaTrial.h
 * @brief 
 */

#ifndef SCOTOMATRIAL_H
#define SCOTOMATRIAL_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <ostream>

/*****************************************************************************
 * Classes
 *****************************************************************************/

/**
 * @brief Encapsulates info about a Scotoma Mapping trial.
 *
 * Member functions make queries easy.
 */
class CScotomaTrial 
{
public:
	/**
	 * @brief Constants for type of stimulus, by center and peripheral color.
	 */
	enum STIM_CODE {
		CODE_C_RED_P_RED     = 0, /**< Center red,  peripheral red.    */
		CODE_C_RED_P_BLUE    = 1, /**< Center red,  peripheral blue.   */
		CODE_C_BLUE_P_RED    = 2, /**< Center blue, peripheral red.    */
		CODE_C_BLUE_P_BLUE   = 3, /**< Center blue, peripheral blue.   */
		CODE_C_RED_P_PURPLE  = 4, /**< Center red,  peripheral purple. */
		CODE_C_BLUE_P_PURPLE = 5, /**< Center blue, peripheral purple. */
		CODE_C_RED_P_BLANK   = 6, /**< Center red,  peripheral blank.  */
		CODE_C_BLUE_P_BLANK  = 7  /**< Center blue, peripheral blank.  */
	};

	/**
	 * @brief Constants for class of stimulus, by type of coordinate chosen.
	 */
	enum STIM_CLASS {
		CLASS_GRID              = 0, /**< Position from stepwise grid.      */
		CLASS_FOVEA_FIXATING    = 1, /**< Position for fovea, fixating eye. */
		CLASS_FOVEA_DEVIATED    = 2, /**< Position for fovea, deviated eye. */
		CLASS_ENRICHED_LOCATION = 4  /**< Position for enrichment location. */
	};

	CScotomaTrial(STIM_CODE stimCode, STIM_CLASS stimClass, double xDeg, 
		double yDeg);
	~CScotomaTrial();

	CScotomaTrial* clone();

	inline bool isCenterRed() const;
	inline bool isPeriphRed() const;
	inline bool isPeriphBlue() const;
	inline bool isPeriphBlank() const;
	inline bool isPeriphPurple() const;

	inline STIM_CODE getStimCode() const;
	inline void setStimCode(STIM_CODE stimCode);

	inline STIM_CLASS getStimClass() const;
	inline void setStimClass(STIM_CLASS stimClass);

	inline double getXDeg() const;
	inline void   setXDeg(double xx);

	inline double getYDeg() const;
	inline void   setYDeg(double yy);

private:
	STIM_CODE m_stimCode;
	STIM_CLASS m_stimClass;
	double m_xDeg;
	double m_yDeg;
	
	/* Not supported. */
	CScotomaTrial(const CScotomaTrial& other);
	CScotomaTrial& operator=(const CScotomaTrial& other);
};

/*****************************************************************************
 * Prototypes
 *****************************************************************************/

std::ostream& operator<< (std::ostream& out, CScotomaTrial& trial);

/*****************************************************************************
 * Inline Member Functions
 *****************************************************************************/

/**
 * @brief Return true if stim has red fixation target.
 * @return True for all stim codes w/red center target.
 */
bool CScotomaTrial::isCenterRed() const
{
	return (
		(m_stimCode == CODE_C_RED_P_RED)    || 
		(m_stimCode == CODE_C_RED_P_BLUE)   ||
		(m_stimCode == CODE_C_RED_P_PURPLE) ||
		(m_stimCode == CODE_C_RED_P_BLANK));
}

/**
 * @brief Return true if peripheral stim is red.
 * @return True for all stim codes w/red peripheral target.
 */
bool CScotomaTrial::isPeriphRed() const
{
	return (
		(m_stimCode == CODE_C_RED_P_RED) || 
		(m_stimCode == CODE_C_BLUE_P_RED));
}

/**
 * @brief Return true if peripheral stim is blue.
 * @return True for all stim codes w/blue peripheral target.
 */
bool CScotomaTrial::isPeriphBlue() const
{
	return (
		(m_stimCode == CODE_C_RED_P_BLUE) || 
		(m_stimCode == CODE_C_BLUE_P_BLUE));
}

/**
 * @brief Return true if peripheral stim is blank.
 * @return True for all stim codes w/blank peripheral target.
 */
bool CScotomaTrial::isPeriphBlank() const
{
	return (
		(m_stimCode == CODE_C_RED_P_BLANK) || 
		(m_stimCode == CODE_C_BLUE_P_BLANK));
}

/**
 * @brief Return true if peripheral stim is purple.
 * @return True for all stim codes w/purple peripheral target.
 */
bool CScotomaTrial::isPeriphPurple() const
{
	return (
		(m_stimCode == CODE_C_RED_P_PURPLE) || 
		(m_stimCode == CODE_C_BLUE_P_PURPLE));
}

/** Get stim code for this trial. */
CScotomaTrial::STIM_CODE CScotomaTrial::getStimCode() const 
{ return m_stimCode; }

/** Set the stim code. */
void CScotomaTrial::setStimCode(CScotomaTrial::STIM_CODE stimCode) 
{ m_stimCode = stimCode; }

/** Get stim class for this trial. */
CScotomaTrial::STIM_CLASS CScotomaTrial::getStimClass() const 
{ return m_stimClass; }

/** Set the stim class. */
void CScotomaTrial::setStimClass(CScotomaTrial::STIM_CLASS stimClass)
{ m_stimClass = stimClass; }

/** Get X coordinate, in degrees, for this trial. */
double CScotomaTrial::getXDeg() const { return m_xDeg; }

/** Set the X coordinate, in degrees, for this trial. */
void CScotomaTrial::setXDeg(double xx) { m_xDeg = xx; }

/** Get Y coordinate, in degrees, for this trial. */
double CScotomaTrial::getYDeg() const { return m_yDeg; }

/** Set the Y coordinate, in degrees, for this trial. */
void CScotomaTrial::setYDeg(double yy) { m_yDeg = yy; }

/*****************************************************************************
 * Templated Function Prototypes
 *****************************************************************************/

/* Functions bodies must be in header so they are included in the compilation
 * unit where the templates are instantiated; otherwise, code compiles, but
 * no instance of the templated function is generated and a linker error
 * results.
 */

/**
 * @brief Generate trials for the points given by the grid specification.
 *
 * Generates CScotomaTrial objects for the grid points. The grid is centered 
 * at (0, 0), in degrees.  Every grid point is generated.  Does not generate
 * the (0, 0) point.  Trial objects are allocated on the heap; the pointer
 * is written to the output iterator.
 *
 * @param stimCode Code to use for the trial.
 * @param stepsX Number of grid steps in the X direction, per side.
 * @param stepsY Number of grid steps in the Y direction , per side.
 * @param spaceDeg Size of each step, in degrees.
 * @param outIter Forward output iterator to receive the trial objects.
 */
template <class ForwardIterType>
void generateByGrid(CScotomaTrial::STIM_CODE stimCode, int stepsX, int stepsY, 
					double spaceDeg, ForwardIterType outIter)
{
	for (int xx = -stepsX; xx <= stepsX; xx++) {
		for (int yy = -stepsY; yy <= stepsY; yy++) {
			if (xx || yy) {
				/* Not both 0 */
				*outIter++ = new CScotomaTrial(stimCode, 
					CScotomaTrial::CLASS_GRID, xx * spaceDeg, yy * spaceDeg);
			}
		}
	}
}

/**
 * @brief Generate trials randomly for given code, class, and window.
 *
 * Generates CScotomaTrial objects of given code and class.  Objects form a
 * window of given width and height around a central point.  The (0, 0) point
 * is possible.  Trial objects are allocated on the heap; the pointer is 
 * written to the output iterator.
 *
 * @param stimCode Code to use for the trials.
 * @param stimClass Class to use for the trials.
 * @param widthDeg Total width of window around a given point.
 * @param heightDeg Total height of window around a given point.
 * @param totalNum Number of points to generate.
 * @param outIter Forward output iterator to receive the trial objects.
 * @param xDeg X coordinate, in degrees, of center of fixation area; defaults 
 * to 0; supply non-zero value for user-specified enrichment location.
 * @param yDeg Y coordinate, in degrees, of center of fixation area; defaults 
 * to 0; supply non-zero value for user-specified enrichment location.
 */
template <class ForwardIterType>
void generateByClass(CScotomaTrial::STIM_CODE stimCode, 
					 CScotomaTrial::STIM_CLASS stimClass, double widthDeg, 
					 double heightDeg, long totalNum, ForwardIterType outIter,
					 double xDeg = 0, double yDeg = 0)
{
	double dX;
	double dY;

	for (long count = 0; count < totalNum; count++) {
		/* Randomly inside square */
		dX = xDeg + (double)(rand() % 100) * 0.01 * widthDeg - 0.5 * widthDeg;
		dY = yDeg + (double)(rand() % 100) * 0.01 * heightDeg - 0.5 * heightDeg;
		*outIter++ = new CScotomaTrial(stimCode, stimClass, dX, dY);
	}
}

#endif // SCOTOMATRIAL_H
