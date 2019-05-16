/**
 * @file vsgext_grating_spec.h
 * @brief Internal header file for GratingSpec class
 */

#ifndef VSGEXT__GRATING__SPEC_H
#define VSGEXT__GRATING__SPEC_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "VSGV8.H"
#include "vsgext.h"

/* Continued definition. */
namespace VsgExt {

/*****************************************************************************
 * Classes
 *****************************************************************************/

class DualGratingSpec;

/**
 * @brief Concrete, <b>private</b> implementation of IGrating.
 */
class GratingSpec : public IGrating
{
	friend IGrating;
	friend DualGratingSpec;

public:
	virtual ~GratingSpec();

	void cacheShape(int xPix, int yPix, int widthPix, int heightPix);
	long getLevel(int xPix, int yPix);

private:
	/* Only created by the factory type, a friend. */
	GratingSpec(long high, long low, int cycles, bool vertical);

	/* All declared 'const' so they're unchanged for life of obj */
	const int m_cycles;
	const long m_highlight;
	const long m_lowlight;
	const bool m_vertical;
	const int m_screenPix;

	int m_cacheXPix;
	int m_cacheYPix;
	int m_cacheHeightPix;
	int m_cacheWidthPix;

	/* Not supported. */
	GratingSpec(const GratingSpec& other);
	GratingSpec& operator=(const GratingSpec& other);
};

/**
 * @brief Concrete, <b>private</b> aggregator.
 *
 * Class aggregates two GratingSpec objects -- a vertical and a horizontal --
 * to implement a double grating.
 */
class DualGratingSpec : public IGrating
{
	friend IGrating;
public:
	virtual ~DualGratingSpec();

	void cacheShape(int xPix, int yPix, int widthPix, int heightPix);
	long getLevel(int xPix, int yPix);

private:
	/* Only created by the factory type, a friend. */
	DualGratingSpec(long vHigh, long vLow, int vCycles, long hHigh, 
		long hLow, int hCycles, long vHigh_hHigh, long vHigh_hLow, 
		long vLow_hHigh, long vLow_hLow);

	/* All declared 'const' so they're unchanged for life of obj */
	const long m_vHigh_hHigh;
	const long m_vHigh_hLow;
	const long m_vLow_hHigh;
	const long m_vLow_hLow;

	GratingSpec m_verticalSpec;
	GratingSpec m_horizontalSpec;

	/* Not supported. */
	DualGratingSpec(const DualGratingSpec& other);
	DualGratingSpec& operator=(const DualGratingSpec& other);
};

}; // namespace VsgExt

#endif // VSGEXT__GRATING__SPEC_H
