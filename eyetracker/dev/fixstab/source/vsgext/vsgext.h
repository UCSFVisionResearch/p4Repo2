/**
 * @file vsgext.h
 * @brief Extension module for VSG APIs.
 */

#ifndef VSGEXT_H
#define VSGEXT_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <map>
#include "VSGV8.H"

/*****************************************************************************
 * Defines
 *****************************************************************************/

#define LUTBUFFER_SIZE (256)

/**
 * @brief Extensions to VSG APIs, residing in namespace VsgExt.
 */
namespace VsgExt {

/*****************************************************************************
 * Classes
 *****************************************************************************/

/**
 * @brief Maps LUT levels for foreground colors and fg + noise colors.
 *
 * This map gives details on how to superimpose a noise background on shapes
 * defined by their LUT levels.  If a pixel is at an LUT level k and level k
 * is a key mapped to value v, then the NoiseBackgroundController can
 * replace the pixel level with v if the pixel is also noise, or leave the
 * pixel level at k otherwise.  This way, superimposing the noise BG is a
 * parametrized transform of the LUT.
 *
 * Expected usage:
 * 
 *   Key LUT Level |         Value LUT Level
 *  ---------------+-----------------------------------
 *        Red      |   Red + Blue component of noise
 *        Blue     |   Blue + Red component of noise
 *     Red + Blue  |            Red + Blue
 *      Dim Red    | Dim Red + Blue component of noise
 *      Dim Blue   | Dim Blue + Red component of noise
 *
 * More mappings can be added to support noise-over-grating display.
 */
class NoiseColorMap
{
public:
	NoiseColorMap();
	NoiseColorMap(const NoiseColorMap& other);
	~NoiseColorMap();

	NoiseColorMap& operator=(const NoiseColorMap& other);

	void map(long key, long value);
	long get(long key) const;

private:
	std::map<long, long> m_map;
};

/**
 * @brief Abstract interface to drawing a noise background and drawing other
 * objects on it.
 *
 * A noise background displays noise intended to eliminate cross talk with
 * dichroic dichoptic stimuli.  Drawing objects on the background requires 
 * that the noise pattern be superimposed on the object, or the object will be
 * a shadow for the non-selected eye.  This class encapsulates drawing the
 * background at a given percent and drawing objects against the background.
 *
 * All drawing functions in this object require a subsequent call to
 * vsgPresent() to take effect.
 *
 * Drawing function calls depend on the current hardware palette.  This class
 * holds a reference to an LUT buffer and configures part of that buffer for
 * display of the background.  If a client writes a different LUT buffer to 
 * the hardware palette or modifies the LUT buffer in the portion reserved by 
 * this class, then the pixels this class writes to the framestore will be
 * display incorrectly upon a call to vsgPresent().
 */
class NoiseBgController
{
public:
	NoiseBgController(int size, const VSGTRIVAL& bgColor, 
		const VSGTRIVAL& noiseColor, int lutBase, int percent, 
		VSGLUTBUFFER& lutBuffer);
	NoiseBgController(int xPos, int yPos, int width, int height, int size, 
		const VSGTRIVAL& bgColor, const VSGTRIVAL& noiseColor, int lutBase, 
		int percent, VSGLUTBUFFER& lutBuffer);
	~NoiseBgController();

	void drawBg();
	void drawBg(DWORD srcPageZone, DWORD srcPage);
	void initReferenceBg(DWORD srcPageZone, DWORD srcPage);
	void drawReferenceBg();
	void configureLut();

	void savePatch(int xPix, int yPix, int width, int height);
	void restorePatch(const NoiseColorMap& colorMap);

	void drawPixelRespectingBg(int xPix, int yPix, int fgLutVal, 
		int fgbgLutVal);

	int getLevel(int xPix, int yPix, int fgLevel, 
		const NoiseColorMap& colorMap);

	bool isNoise(int level);

	/*** Accessors *************************************************/
	void setXPos(int xPos);
	int getXPos();

	void setYPos(int yPos);
	int getYPos();

	void setWidth(int width);
	int getWidth();

	void setHeight(int height);
	int getHeight();

	void setSize(int size);
	int getSize();

	void setColors(const VSGTRIVAL& bgColor, const VSGTRIVAL& noiseColor);
	void getColors(VSGTRIVAL& bgColor, VSGTRIVAL& noiseColor);

	void setBgColor(const VSGTRIVAL& color);
	void getBgColor(VSGTRIVAL& color);

	void setNoiseColor(const VSGTRIVAL& color);
	void getNoiseColor(VSGTRIVAL& color);

	void setLutBase(int lutBase);
	int getLutBase();

	void setPercent(int percent);
	int getPercent();

	void setOcclude(bool occlude);
	bool getOcclude();

private:
	class PatchCopy;

	int m_xPos;
	int m_yPos;
	int m_width;
	int m_height;
	int m_size;
	VSGTRIVAL m_bgColor;
	VSGTRIVAL m_noiseColor;
	int m_lutBase;
	int m_percent;
	VSGLUTBUFFER& m_lutBuffer;
	bool m_occlude;
	PatchCopy* m_patch;
	DWORD m_refPageZone;
	DWORD m_refPageNum;

	/* Not supported */
	NoiseBgController(const NoiseBgController&);
	NoiseBgController& operator=(const NoiseBgController&);
};

/**
 * @brief Interface to drawing a grating pattern.
 *
 * This class specifies contrast, number of cycles, and orientation for 
 * drawing gratings on colored shapes.  It encapsulates a lot of dense logic, 
 * assumptions, and external dependencies.
 *
 * To create one, first create a factory.  Then, create a spec via a factory
 * method.  Finally, cache the shape and go.
 */
class IGrating
{
public:
	virtual ~IGrating();

	virtual void cacheShape(int x, int y, int width, int height) = 0;
	virtual long getLevel(int x, int y) = 0;

	static IGrating* identity(long level);
	static IGrating* gratingVertical(long high, long low, int cycles);
	static IGrating* gratingHorizontal(long high, long low, 
		int cycles);
	static IGrating* gratingDual(long vHigh, long vLow, int vCycles, 
		long hHigh, long hLow, int hCycles, long vHigh_hHigh, long vHigh_hLow,
		long vLow_hHigh, long vLow_hLow);
};

/*****************************************************************************
 * Prototypes
 *****************************************************************************/

double getLowlight(double inH, double contrast);

double getHighlight(double inL, double contrast);

long drawTriangle(int xPix, int yPix, int sizePix, IGrating& gratingDesc,
	NoiseColorMap* colorMap, NoiseBgController* bgCont);

long drawCircle(int xPix, int yPix, int sizePix, IGrating& gratingDesc,
	NoiseColorMap* colorMap, NoiseBgController* bgCont);

long drawRectangle(int xPix, int yPix, int widthPix, int heightPix, 
	IGrating& gratingDesc, NoiseColorMap* colorMap, NoiseBgController* bgCont);

long drawCross(int xPix, int yPix, int sizePix, int thickPix, 
	IGrating& gratingDesc, NoiseColorMap* colorMap, NoiseBgController* bgCont);

long drawCross(int xPix, int yPix, int sizePix, int thickPix, long level);

}; // namespace VsgExt

#endif // VSGEXT_H
