/**
 * @file vsgext_grating_spec.cpp
 * @brief Implementation of private GratingSpec class
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "VSGV8.H"
#include "vsgext.h"
#include "vsgext_grating_spec.h"

/* Continued definition. */
namespace VsgExt {

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

/**
 * @brief Create a spec.
 * @param high LUT level for highlight color.
 * @param low LUT level for lowlight color.
 * @param cycles Number of cycles in grating; a cycle is one highlight and one
 * lowlight.
 * @param vertical True for vertical grating, false for horizontal.
 */
GratingSpec::GratingSpec(long high, long low, int cycles, bool vertical)
: m_cycles(cycles), m_highlight(high), m_lowlight(low), m_vertical(vertical),
  m_cacheXPix(-1), m_cacheYPix(-1), m_cacheHeightPix(0), m_cacheWidthPix(0),
  m_screenPix(vertical ? vsgGetScreenWidthPixels() : vsgGetScreenHeightPixels())
{}

/** Destructor. */
GratingSpec::~GratingSpec() { /* Empty */ }

/**
 * @brief Cache the shape area to be used for repeated level queries.
 *
 * Drawing functions take an IGrating so they can use the correct LUT level
 * for each pixel drawn, either highlight or lowlight.  The drawing functions
 * can cache the shape they're drawing, so they make multiple level queries
 * without specifying where the shape is and where they're drawing with each
 * query.
 *
 * Note that it suffices (and simplifies things) to cache a rectangle that 
 * encloses the actual shape, whether a triangle, n-hedron, circle, or cross.
 *
 * The cache can only be overwritten, not merely cleared.
 *
 * @param xPix X coordinate, in pixels, of shape center.
 * @param yPix Y coordinate, in pixels, of shape center.
 * @param widthPix Width, in pixels, of shape, > 0.
 * @param heightPix Height, in pixels, of shape, > 0.
 */
void GratingSpec::cacheShape(int xPix, int yPix, int widthPix, int heightPix) 
{
	if ((heightPix <= 0) || (widthPix <= 0)) {
		/* Grating spec will cause "stipple" pattern for this. */
		m_cacheXPix = m_cacheYPix = 0;
		m_cacheHeightPix = m_cacheWidthPix = 0;
	}
	else {
		m_cacheXPix = xPix;
		m_cacheYPix = yPix;
		m_cacheWidthPix = widthPix;
		m_cacheHeightPix = heightPix;
	}
}

/**
 * @brief Query the LUT level at a pixel.
 *
 * After caching a shape-drawing area, drawing functions make queries for each
 * pixel of the shape to determine if it is grating highlight or grating
 * lowlight.
 *
 * Does not verify that the coordinates specify a point in the cached shape.
 *
 * @param xPix Coordinate in pixels of the pixel being drawn.
 * @param yPix Coordinate in pixels of the pixel being drawn.
 * @return LUT level, either the configured highlight for this spec or its
 * configured lowlight.
 */
long GratingSpec::getLevel(int xPix, int yPix) 
{ 
	/* 
	 * Original algorithm (for equatability with old results), recast to be
	 * generic w/respect to vertical or horizontal orientation.  
	 *
	 * Round-off error -- specifically, truncating a float to an int, rounding 
	 * down -- can lead to partial cycles being added to the requested number.
	 * E.g., if a shape 300 pix wide is supposed to have a 4-cycle vertical 
	 * grating, each cycle is 75 pix, so each bar of the cycle is 37.5 pix, 
	 * rounded to 37 pix, wide.  After 4 cycles, there's an error of
	 * 300 - (37 * 2 * 4) = 4 pix. 
	 *
	 * Adding 'm_screenPix' ensures you don't use a negative w/the modulus
	 * operator, which isn't illegal but doesn't work as intuitively as
	 * positives.
	 */
	int coord = m_vertical ? xPix : yPix;
	int dim   = m_vertical ? m_cacheWidthPix : m_cacheHeightPix;

	if (!dim) {
		/* Speckled pattern to flag error. */
		return ((xPix + yPix) % 2 == 0 ? m_lowlight : m_highlight);
	}
	
	/* Don't divide by 0 */
	if (m_cycles) {
		/* As per original algorithm, truncate, so even 1.667 is rounded to
		 * integer value 1.
		 */
		int thickness = (0.5 * dim) / m_cycles;

		/* Even if m_cycles and dim were non-zero, thickness can be zero from
		 * rounding.  To avoid a divide-by-zero error, bump it to 1 and go; 
		 * user wanted grating. 
		 */
		if (!thickness) {
			thickness = 1;
		}
	
		if (((coord + m_screenPix) / thickness) % 2) {
			return m_lowlight;
		}
	}
	
	/* Return highlight if not lowlight or if 0 cycles (no grating) */
	return m_highlight;
}

/**
 * @brief Create a DualGratingSpec given LUT levels for vertical-horizontal
 * dual grating.
 *
 * @param vHigh Vertical grating highligh color.
 * @param vLow Vertical grating lowlight color.
 * @param vCycles Number of cycles in vertical grating.
 * @param hHigh Horizontal grating highlight color.
 * @param hLow Horizontal grating lowlight color.
 * @param hCycles Horizontal grating number of cycles.
 * @param vHigh_hHigh Color for superposition of vertical highlight and 
 * horizontal highlight.
 * @param vHigh_hLow Color for superposition of vertical highlight and 
 * horizontal lowlight.
 * @param vLow_hHigh Color for superposition of vertical lowlight and 
 * horizontal highlight.
 * @param vLow_hLow Color for superposition of vertical lowlight and 
 * horizontal lowlight.
 */
DualGratingSpec::DualGratingSpec(long vHigh, long vLow, int vCycles, 
								 long hHigh, long hLow, int hCycles, 
								 long vHigh_hHigh, long vHigh_hLow,
								 long vLow_hHigh, long vLow_hLow)
: m_vHigh_hHigh(vHigh_hHigh), m_vHigh_hLow(vHigh_hLow), 
  m_vLow_hHigh(vLow_hHigh), m_vLow_hLow(vLow_hLow), 
  m_verticalSpec(vHigh, vLow, vCycles, true), 
  m_horizontalSpec(hHigh, hLow, hCycles, false)
{}

/** @brief Destructor */
DualGratingSpec::~DualGratingSpec() { /* Empty */}

/**
 * @brief Cache the shape area to be used for repeated level queries.
 *
 * Drawing functions take an IGrating so they can use the correct LUT level
 * for each pixel drawn, either highlight or lowlight.  The drawing functions
 * can cache the shape they're drawing, so they make multiple level queries
 * without specifying where the shape is and where they're drawing with each
 * query.
 *
 * Note that it suffices (and simplifies things) to cache a rectangle that 
 * encloses the actual shape, whether a triangle, n-hedron, circle, or cross.
 *
 * The cache can only be overwritten, not merely cleared.
 *
 * @param xPix X coordinate, in pixels, of shape center.
 * @param yPix Y coordinate, in pixels, of shape center.
 * @param widthPix Width, in pixels, of shape, > 0.
 * @param heightPix Height, in pixels, of shape, > 0.
 */
void DualGratingSpec::cacheShape(int xPix, int yPix, int widthPix, 
								   int heightPix)
{
	m_verticalSpec.cacheShape(xPix, yPix, widthPix, heightPix);
	m_horizontalSpec.cacheShape(xPix, yPix, widthPix, heightPix);
}

/**
 * @brief Query the LUT level at a pixel.
 *
 * After caching a shape-drawing area, drawing functions make queries for each
 * pixel of the shape to determine if it is grating highlight or grating
 * lowlight.
 *
 * Does not verify that the coordinates specify a point in the cached shape.
 *
 * @param xPix Coordinate in pixels of the pixel being drawn.
 * @param yPix Coordinate in pixels of the pixel being drawn.
 * @return LUT level, either the configured highlight for this spec or its
 * configured lowlight, or one of the superpositions.
 */
long DualGratingSpec::getLevel(int xPix, int yPix)
{
	bool vHigh = m_verticalSpec.getLevel(xPix, yPix) == m_verticalSpec.m_highlight;
	long hHigh = m_horizontalSpec.getLevel(xPix, yPix) == m_horizontalSpec.m_highlight;

	if (vHigh) {
		if (hHigh) {
			return m_vHigh_hHigh;
		}
		else { /* hLow */
			return m_vHigh_hLow;
		}
	}
	else { /* vLow */
		if (hHigh) {
			return m_vLow_hHigh;
		}
		else { /* hLow */
			return m_vLow_hLow;
		}
	}
}

}; // namespace VsgExt
