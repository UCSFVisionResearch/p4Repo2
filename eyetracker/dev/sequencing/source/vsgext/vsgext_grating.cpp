/**
 * @file vsgext_grating.cpp
 * @brief Implementation of gratings controller
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

/* Required */
IGrating::~IGrating() {}

/**
 * @brief Create a lowlight R, B, or G gun value based on factory contrast.
 * @param inH The highlight value, based on which to make the lowlight.
 * @param contrast The contrast percentage, entered as a whole number.
 * @return New gun value, equal to the input value scaled down to have the
 * desired contrast with inH.
 */
double getLowlight(double inH, double contrast)
{
	return (inH * (1.0 - 0.01 * contrast));
}

/**
 * @brief Create a highlight R, B, or G gun value based on factory contrast.
 * @param inL The lowlight value, based on which to make the highlight.
 * @param contrast The contrast percentage, entered as a whole number.
 * @return New gun value, equal to the input value scaled up to have the
 * desired contrast with inL.
 */
double getHighlight(double inL, double contrast)
{
	double scale = (1.0 - 0.01 * contrast);
	return (scale ? (inL / scale) : 1.0);
}

/**
 * @brief Create an IGrating that returns the given level always.
 * @param level The LUT level for the identity pixel.
 * @return A new IGrating object configured always to return the given level on
 * calls to its getLevel function.  Caller owns the heap memory.
 */
IGrating* IGrating::identity(long level)
{
	return new GratingSpec(level, level, 0, true);
}

/**
 * @brief Create an IGrating using given LUT levels for a vertical 
 * grating.
 * @param high LUT level of the highlight color.  Configure the VSGTRIVAL for 
 * this LUT level via the makeHighlight function, or use a user-defined value.
 * @param low LUT level of the lowlight color. Configure this VSGTRIVAL via 
 * the makeLowlight function or use a user-defined value.
 * @param cycles The number of grating cycles to fit into the shape.  A cycle
 * is one highlight and one lowlight.
 * @return A new IGrating object configured for a vertical gratihg with given 
 * highlight, lowlight.  Caller owns the heap memory.
 */
IGrating* IGrating::gratingVertical(long high, long low, int cycles)
{
	return new GratingSpec(high, low, cycles, true);
}

/**
 * @brief Create an IGrating using given LUT levels for a horizontal 
 * grating.
 * @param high LUT level of the highlight color.  Configure the VSGTRIVAL for 
 * this LUT level via the makeHighlight function, or use a user-defined value.
 * @param low LUT level of the lowlight color. Configure this VSGTRIVAL via 
 * the makeLowlight function or use a user-defined value.
 * @param cycles The number of grating cycles to fit into the shape.  A cycle
 * is one highlight and one lowlight.
 * @return A new IGrating object configured for a horizontal gratihg with given 
 * highlight, lowlight.  Caller owns the heap memory.
 */
IGrating* IGrating::gratingHorizontal(long high, long low, int cycles)
{
	return new GratingSpec(high, low, cycles, false);
}

/**
 * @brief Create an IGrating using given LUT levels for vertical-
 * horizontal dual grating.
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
 * @return A new IGrating object configured for a dual gratihg with given 
 * highlights and lowlights.  Caller owns the heap memory.
 */
IGrating* IGrating::gratingDual(long vHigh, long vLow, int vCycles, long hHigh,
								long hLow, int hCycles, long vHigh_hHigh,
								long vHigh_hLow, long vLow_hHigh, long vLow_hLow)
{
	return new DualGratingSpec(vHigh, vLow, vCycles, hHigh, hLow, hCycles, 
		vHigh_hHigh, vHigh_hLow, vLow_hHigh, vLow_hLow);
}

}; /* namespace VsgExt */
