/**
 * @file vsgext_draw.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <math.h>
#include "VSGV8.H"
#include "utils/Math.h"
#include "vsgext.h"

/*****************************************************************************
 * Functions
 *****************************************************************************/

static int helper_composeLevel(int xPix, int yPix, VsgExt::IGrating& gratingDesc,
							   VsgExt::NoiseColorMap* colorMap, 
							   VsgExt::NoiseBgController* bgCont)
{
	int composedLevel = gratingDesc.getLevel(xPix, yPix);

	if (colorMap && bgCont) {
		composedLevel = bgCont->getLevel(xPix, yPix, composedLevel, *colorMap);
	}

	return composedLevel;
}

/**
 * @brief Private function to handle pixels of rectangle.
 *
 * Not meant to be of general use; specifically a helper function to 
 * drawRectangle and drawCross.
 *
 * <b>Preconditions</b> 
 * <ul>
 * <li> GratingSpec must have shape cached; whole cross, not each arm, in case
 *     of cross
 * <li> NoiseBgController must have patch saved; whole cross, not each arm, in
 *     case of cross
 * </ul>
 *
 * <b>Note:</b> Paradigm for (xPix, yPix) is <i>top-left</i>, not center.
 * 
 * @param xPix X coordinate of top-left pixel.
 * @param yPix Y coordinate of top-left pixel.
 * @param widthPix Width in pixels.
 * @param heightPix Height in pixels.
 * @param gratingDesc GratingSpec to handle colors.
 * @param colorMap NoiseColorMap with mappings on how to merge background noise
 *        with the shape's colors, highlight and lowlight; or NULL if no noise
 *        background.
 * @param noise Background controller responsible for background, or NULL if
 *        no noise background.
 */
static void helper_drawRectangle(int xPix, int yPix, int widthPix, int heightPix, 
								 VsgExt::IGrating& gratingDesc, 
								 VsgExt::NoiseColorMap* colorMap,
								 VsgExt::NoiseBgController* bgCont)
{
	for (int xx = xPix; xx < xPix + widthPix; xx++) {
		for (int yy = yPix; yy < yPix + heightPix; yy++) {
			vsgSetPen1(
				helper_composeLevel(xx, yy, gratingDesc, colorMap, bgCont));
			vsgDrawPixel(xx, yy);
		}
	}
}

/* Continue the namespace declaration without including the helper 
 * function. 
 */
namespace VsgExt {

/**
 * @brief Draw an equilateral triangle.
 * 
 * If bgCont is non-NULL, it is used to superimpose the background on the 
 * object drawn.
 *
 * @param xPix X pixel coordinate of object center.
 * @param yPix Y pixel coordinate of object center.
 * @param sizePix Height of triangle in pixels
 * @param gratingDesc Grating spec with highlight and lowlight colors.
 * @param colorMap NoiseColorMap with mappings on how to merge background noise
 *        with the shape's colors, highlight and lowlight; or NULL if no noise
 *        background.
 * @param noise Background controller responsible for background, or NULL if
 *        no noise background.
 * @return 0 for success, non-zero for failure or error.
 */
long drawTriangle(int xPix, int yPix, int sizePix, IGrating& gratingDesc, 
				  NoiseColorMap* colorMap, NoiseBgController* bgCont)
{
	/* Height is sizePix.  Equilateral triangle, so sides are 1 : sqrt(3) : 2
	 * ratio.  Thus sides are sizePix/sqrt(3) : sizePix : 2*sizePix/sqrt(3).
	 * Note that 1/sqrt(3) * sqrt(3)/sqrt(3) = sqrt(3)/3; second form works
	 * better for types & numerical comp.
	 *
	 * Slope is tan(a) where a = 60 degrees, thus slope = sqrt(3).  
	 */
	double slope = SQRT3_DOUBLE;
	double halfBase = ((double)sizePix) * SQRT3_DOUBLE / 3.0;
	double halfHeight = ((double)sizePix) / 2;

	int halfBaseInt   = (int)halfBase;
	int halfHeightInt = (int)halfHeight;
	int baseAdj       = (halfBase != ((double)halfBaseInt)) ? 1 : 0;
	int heightAdj     = (halfHeight != ((double)halfHeightInt)) ? 1 : 0;

	gratingDesc.cacheShape(xPix - halfBase, yPix - halfHeight, 2 * halfBase, 
			sizePix);

	for (int xx = xPix - halfBaseInt; xx <= xPix + halfBaseInt + baseAdj; xx++) {
		/* Vertical displacement of y coordinate. */
		int height = ((xx <= xPix) ?
			/* At far left, xx = (xPix - halfBase), thus 
			 *   xx - xPix + halfBase = xPix - halfBase - xPix + halfBase 
			 *                        = 0
			 * And
			 *                 height = slope * 0
			 *                        = 0
			 *
			 * At middle, xx = xPix, thus
			 *   xx - xPix + halfBase = xPix - xPix + halfBase 
			 *                        = halfBase
			 * And
			 *                 height = slope * halfBase 
			 *                        = sqrt(3) * sizePix / sqrt(3) 
			 *                        = sizePix
			 */
			(slope * (xx - xPix + halfBase)) : 
		    /* One after middle, xx = xPix + 1 (call it xPix), thus
			 *   xx - xPix = xPix - xPix 
			 *             = 0
			 * And
			 *      height = sizePix - slope * 0 
			 *             = sizePix
			 *
			 * At far right, xx = (xPix + halfBase), thus 
			 *   xx - xPix = xPix + halfBase - xPix 
			 *             = halfBase
			 * And
			 *      height = sizePix - slope * halfBase 
			 *             = sizePix - sqrt(3) * sizePix / sqrt(3)
			 *             = sizePix - sizePix
			 *             = 0
			 */
			(sizePix - slope * (xx - xPix)));

		/* Loop works backward: from base of triangle to point on edge. */
		for (int yy = yPix + sizePix / 2; yy > yPix + (sizePix / 2) - height; yy--) {
			vsgSetPen1(
				helper_composeLevel(xx, yy, gratingDesc, colorMap, bgCont));
			vsgDrawPixel(xx, yy);
		}
	}

	return 0;
}

/**
 * @brief Draw a circle.
 * 
 * If bgCont is non-NULL, it is used to superimpose the background on the 
 * object drawn.
 *
 * @param xPix X pixel coordinate of object center.
 * @param yPix Y pixel coordinate of object center.
 * @param sizePix Diameter of circle
 * @param gratingDesc Grating spec with highlight and lowlight colors.
 * @param colorMap NoiseColorMap with mappings on how to merge background noise
 *        with the shape's colors, highlight and lowlight; or NULL if no noise
 *        background.
 * @param noise Background controller responsible for background, or NULL if
 *        no noise background.
 * @return 0 for success, non-zero for failure or error.
 */
long drawCircle(int xPix, int yPix, int sizePix, IGrating& gratingDesc, 
				NoiseColorMap* colorMap, NoiseBgController* bgCont)
{
	double dx, dy;
	double radius = ((double)sizePix) / 2.0;
	int    radiusInt = (int)radius;
	int    adj = (radius != ((double)radiusInt)) ? 1 : 0;

	gratingDesc.cacheShape(xPix - radiusInt, yPix - radiusInt + adj, sizePix, sizePix);

	for (int xx = xPix - radiusInt; xx < xPix + radiusInt + adj; xx++) {
		for (int yy = yPix - radiusInt; yy < yPix + radiusInt + adj; yy++) {
			/* Filter out points not in circle. */
			dx = ((double)xx) - xPix;
			dy = ((double)yy) - yPix;
			if ((dx * dx + dy * dy) < (radius * radius)) {
				vsgSetPen1(
					helper_composeLevel(xx, yy, gratingDesc, colorMap, bgCont));
				vsgDrawPixel(xx, yy);
			}
		}
	}

	return 0;
}

/**
 * @brief Draw a rectangle.
 * 
 * If bgCont is non-NULL, it is used to superimpose the background on the 
 * object drawn.
 *
 * @param xPix X pixel coordinate of object center.
 * @param yPix Y pixel coordinate of object center.
 * @param widthPix Width in X direction, in pixels
 * @param heightPix Height in Y direction, in pixels.
 * @param gratingDesc Grating spec with highlight and lowlight colors.
 * @param colorMap NoiseColorMap with mappings on how to merge background noise
 *        with the shape's colors, highlight and lowlight; or NULL if no noise
 *        background.
 * @param noise Background controller responsible for background, or NULL if
 *        no noise background.
 * @return 0 for success, non-zero for failure or error.
 */
long drawRectangle(int xPix, int yPix, int widthPix, int heightPix, 
				   IGrating& gratingDesc, NoiseColorMap* colorMap, 
				   NoiseBgController* bgCont)
{
	int halfWidth = widthPix / 2;
	int halfHeight = heightPix / 2;

	gratingDesc.cacheShape(xPix - halfWidth, yPix - halfHeight, widthPix, 
			heightPix);

	helper_drawRectangle(xPix - halfWidth, yPix - halfHeight, widthPix, 
		heightPix, gratingDesc, colorMap, bgCont);

	return 0;
}

/**
 * @brief Draw a square cross.
 * 
 * If bgCont is non-NULL, it is used to superimpose the background on the 
 * object drawn.
 *
 * @param xPix X pixel coordinate of object center.
 * @param yPix Y pixel coordinate of object center.
 * @param sizePix Edge length in pixels
 * @param thickPix Thickness of the arms of the cross.
 * @param gratingDesc Grating spec with highlight and lowlight colors.
 * @param colorMap NoiseColorMap with mappings on how to merge background noise
 *        with the shape's colors, highlight and lowlight; or NULL if no noise
 *        background.
 * @param noise Background controller responsible for background, or NULL if
 *        no noise background.
 * @return 0 for success, non-zero for failure or error.
 */
long drawCross(int xPix, int yPix, int sizePix, int thickPix, 
			   IGrating& gratingDesc, NoiseColorMap* colorMap, 
			   NoiseBgController* bgCont)
{
	int halfSize = floor(((double)sizePix) / 2.0);
	int halfThickCeil = ceil(((double)thickPix) / 2.0);
	int halfThickFloor = floor(((double)thickPix) / 2.0);
	int xx, yy, armlen;

	gratingDesc.cacheShape(xPix - halfSize, yPix - halfSize, sizePix, 
			sizePix);

	/* First, draw the horizontal bar.  Leftmost point is cross center
	 * moved left by half the cross size (i.e., by the cross radius).  
	 * Topmost point is cross center moved up (subtract) by half the 
	 * cross thickness.
	 */
	xx = xPix - halfSize;
	yy = yPix - halfThickCeil;
	helper_drawRectangle(xx, yy, sizePix, thickPix, gratingDesc, colorMap, bgCont);

	/* Top arm.  Arm length is overall cross height minus bar thickness,
	 * cut in half (have two bars).  Leftmost point is the cross center 
	 * moved left by half the thickness.  Topmost point is the cross 
	 * center moved up (subtract) by arm length and half the thickness 
	 * (with 1 pixel's padding). 
	 */
	armlen = floor(((double)(sizePix - thickPix)) / 2.0);
	xx = xPix - halfThickFloor;
	yy = yPix - halfThickCeil - armlen;
	helper_drawRectangle(xx, yy, thickPix, armlen, gratingDesc, colorMap, bgCont);
	
	/* Bottom arm.  See comment for top arm for main ideas. */
	xx = xPix - halfThickFloor;
	yy = yPix + halfThickFloor;
	helper_drawRectangle(xx, yy, thickPix, armlen, gratingDesc, colorMap, bgCont);

	return 0;
}

}; // namespace VsgExt