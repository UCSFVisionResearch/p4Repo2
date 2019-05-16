/**
 * @file vsgext_noisebg.cpp
 * @brief Implementation of noise background drawing class.
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <map>
#include "VSGV8.H"
#include "vsgext.h"

#define BOUNDED_PERCENT(val) (((val) > (100)) ?              \
                                  (100)       :              \
					              (((val) < (0)) ?           \
                                       (0)       :           \
					                   (val)))

/*** Continue definition of VsgExt namespace. ************/
namespace VsgExt {

/*****************************************************************************
 * Classes
 *****************************************************************************/

/**
 * @brief A saved copy of a patch of pixels from the framestore.
 * 
 * Helper class, private to NoiseBgController, to save a copy of a patch
 * of pixels in the frame store.
 */
class NoiseBgController::PatchCopy
{
public:
	PatchCopy(int xPix, int yPix, int width, int height);
	~PatchCopy();

	int get(int xPix, int yPix);

	int getX();
	int getY();
	int getWidth();
	int getHeight();

private:
	int m_xPix;
	int m_yPix;
	int m_width;
	int m_height;
	int m_size;
	int* m_grid;

	int xyIx(int xPix, int yPix);

	/** Not supported */
	PatchCopy(const PatchCopy&);
};

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

/**
 * @brief Create BG Controller for given values.
 *
 * The Noise controller will modify the LUT based upon the bgColor, 
 * noiseColor, and lutBase params.  You should not modify the LUT from index 
 * lutBase to index (lutBase + 199) after creating this controller.
 *
 * BG Controller will default to a full-screen background.  If you need to
 * controll the background area, use the other constructor.
 * 
 * @param size Edge size of a block of noise pixels, in number of pixels.
 * @param bgColor Color for background pixels; need not be the same as
 * lutBuffer[vsgBACKGROUND].
 * @param noiseColor Color for noise pixels against background.
 * @param lutBase Index in LUT to use.  Controller requires 200 indices
 * starting from this index, thus this index must be < 50 (allowing room
 * for reserved indices in the 250+ range).
 * @param percent The percent of the area to color with the noise color.
 * @param lutBuffer Reference to the LUT buffer to be used.
 */
NoiseBgController::NoiseBgController(int size, const VSGTRIVAL& bgColor,
		const VSGTRIVAL& noiseColor, int lutBase, int percent, 
		VSGLUTBUFFER& lutBuffer)
: m_xPos(0), m_yPos(0), m_size(size), m_lutBase(lutBase), 
  m_binocPercent(percent), m_leftPercent(0), m_rightPercent(0), 
  m_lutBuffer(lutBuffer), m_occlude(false), 
  m_patch(0), m_refPageZone(0xFFFF), m_refPageNum(0xFFFF)
{
	m_width = vsgGetScreenWidthPixels(); 
	m_height = vsgGetScreenHeightPixels();

	/* Calls this->configureLut(), which is necessary. */
	this->setColors(bgColor, noiseColor);
}

/**
 * @brief Create BG Controller for given values.
 *
 * The Noise controller will modify the LUT based upon the bgColor, 
 * noiseColor, and lutBase params.  You should not modify the LUT from index 
 * lutBase to index (lutBase + 199) after creating this controller.
 * 
 * @param xPos X position of top-left corner of noise.
 * @param yPos Y position of top-left corner of noise.
 * @param width Width of noise, in pixels.
 * @param height Height of noise, in pixels.
 * @param size Edge size of a block of noise pixels, in number of pixels.
 * @param bgColor Color for background pixels; need not be the same as
 * lutBuffer[vsgBACKGROUND].
 * @param noiseColor Color for noise pixels against background.
 * @param lutBase Index in LUT to use.  Controller requires 200 indices
 * starting from this index, thus this index must be < 50 (allowing room
 * for reserved indices in the 250+ range).
 * @param percent The percent of the area to color with the noise color.
 * @param lutBuffer Reference to the LUT buffer to be used.
 */
NoiseBgController::NoiseBgController(int xPos, int yPos, int width, int height,
									 int size, const VSGTRIVAL& bgColor,
									 const VSGTRIVAL& noiseColor, int lutBase,
									 int percent, VSGLUTBUFFER& lutBuffer)
: m_xPos(xPos), m_yPos(yPos), m_width(width), m_height(height), m_size(size),
  m_lutBase(lutBase), m_binocPercent(percent), m_leftPercent(0), m_rightPercent(0),
  m_lutBuffer(lutBuffer), m_occlude(false), m_patch(0), m_refPageZone(0xFFFF), 
  m_refPageNum(0xFFFF)  
{
	/* Calls this->configureLut(), which is necessary. */
	this->setColors(bgColor, noiseColor);
}

/**
 * @brief Create BG Controller for given values.
 *
 * The Noise controller will modify the LUT based upon the bgColor, 
 * binocNoiseColor, inferred monocular cololrs, and lutBase params.  You 
 * should not modify the LUT from index lutBase to index (lutBase + 199) 
 * after creating this controller.
 *
 * BG Controller will default to a full-screen background.  If you need to
 * controll the background area, use the other constructor.
 * 
 * @param size Edge size of a block of noise pixels, in number of pixels.
 * @param bgColor Color for background pixels; need not be the same as
 * lutBuffer[vsgBACKGROUND].
 * @param binocNoiseColor Color for binocular noise pixels against background.
 * @param lutBase Index in LUT to use.  Controller requires 200 indices
 * starting from this index, thus this index must be < 50 (allowing room
 * for reserved indices in the 250+ range).
 * @param binocPercent The percent of the area to color with the binocular 
 *        noise color.
 * @param leftPercent The percent of the area to color with the inferred 
 *        left-eye noise (blue value of binocular noise).
 * @param rightPercent The percent of the area to color with the inferred 
 *        right-eye noise (red value of binocular noise).
 * @param lutBuffer Reference to the LUT buffer to be used.
 */
NoiseBgController::NoiseBgController(int size, const VSGTRIVAL& bgColor,
		const VSGTRIVAL& binocNoiseColor, int lutBase, int binocPercent, 
		int leftPercent, int rightPercent, VSGLUTBUFFER& lutBuffer)
: m_xPos(0), m_yPos(0), m_size(size), m_lutBase(lutBase), 
  m_binocPercent(binocPercent), m_leftPercent(leftPercent), 
  m_rightPercent(rightPercent), m_lutBuffer(lutBuffer), m_occlude(false), 
  m_patch(0), m_refPageZone(0xFFFF), m_refPageNum(0xFFFF)
{
	m_width = vsgGetScreenWidthPixels(); 
	m_height = vsgGetScreenHeightPixels();

	/* Calls this->configureLut(), which is necessary. */
	this->setColors(bgColor, binocNoiseColor);
}

/**
 * @brief Create BG Controller for given values.
 *
 * The Noise controller will modify the LUT based upon the bgColor, 
 * binocNoiseColor, inferred monocular cololrs, and lutBase params.  You 
 * should not modify the LUT from index lutBase to index (lutBase + 199) 
 * after creating this controller.
 * 
 * @param xPos X position of top-left corner of noise.
 * @param yPos Y position of top-left corner of noise.
 * @param width Width of noise, in pixels.
 * @param height Height of noise, in pixels.
 * @param size Edge size of a block of noise pixels, in number of pixels.
 * @param bgColor Color for background pixels; need not be the same as
 *        lutBuffer[vsgBACKGROUND].
 * @param binocNoiseColor Color for binocular noise pixels against background.
 * @param lutBase Index in LUT to use.  Controller requires 200 indices
 *        starting from this index, thus this index must be < 50 (allowing room
 *        for reserved indices in the 250+ range).
 * @param binocPercent The percent of the area to color with the binocular 
 *        noise color.
 * @param leftPercent The percent of the area to color with the inferred 
 *        left-eye noise (blue value of binocular noise).
 * @param rightPercent The percent of the area to color with the inferred 
 *        right-eye noise (red value of binocular noise).
 * @param lutBuffer Reference to the LUT buffer to be used.
 */
NoiseBgController::NoiseBgController(int xPos, int yPos, int width, int height, int size, 
		const VSGTRIVAL& bgColor, const VSGTRIVAL& binocNoiseColor, 
		int lutBase, int binocPercent, int leftPercent, int rightPercent,
		VSGLUTBUFFER& lutBuffer)
: m_xPos(xPos), m_yPos(yPos), m_width(width), m_height(height), m_size(size),
  m_lutBase(lutBase), m_binocPercent(binocPercent), m_leftPercent(leftPercent), 
  m_rightPercent(rightPercent), m_lutBuffer(lutBuffer), m_occlude(false), 
  m_patch(0), m_refPageZone(0xFFFF), m_refPageNum(0xFFFF)
{
	/* Calls this->configureLut(), which is necessary. */
	this->setColors(bgColor, binocNoiseColor);
}

/**
 * @brief Cleanup
 */
NoiseBgController::~NoiseBgController() 
{ 
	delete m_patch;
	m_patch = 0;
}

/**
 * @brief Draw the background on current drawing page according to configured 
 * parameters.
 */
void NoiseBgController::drawBg()
{
	/* See configureLut(...) function for notes on LUT range. */
	vsgDrawNoise(m_xPos, m_yPos, m_width, m_height, m_size, m_size, 0, 
		m_lutBase, 100);
}

/**
 * @brief Draw the BG on the given page.
 *
 * Saves current drawing page and restores it when returning.
 *
 * @param srcPageZone vsgVIDEOPAGE etc.
 * @param srcPage Number page within given zone.
 */
void NoiseBgController::drawBg(DWORD srcPageZone, DWORD srcPage)
{
	/* Space to save current page zone and page num; initializing _StructSize
	 * is required by VSG documentation. */
	VSGPAGEDESCRIPTOR savedPage;
	savedPage._StructSize = sizeof(VSGPAGEDESCRIPTOR);
	vsgGetCurrentDrawPage(&savedPage);

	/* Switch to user-requested drawing page; draw BG; then restore previous
	 * page. vsgNOCLEAR is self-explanatory but necessary. */
	vsgSetDrawPage(srcPageZone, srcPage, vsgBACKGROUND);
	this->drawBg();
	vsgSetDrawPage(savedPage.PageZone, savedPage.Page, vsgNOCLEAR);
}

/**
 * @brief Creates a reference noise BG on the given page.
 * 
 * Current draw page is not affected.  To display the reference BG, call
 * drawReferenceBg().  Repeated calls display the same BG; that's the point of
 * having the feature.
 *
 * @param srcPageZone vsgVIDEOPAGE etc.
 * @param srcPage Number page within given zone.
 */
void NoiseBgController::initReferenceBg(DWORD srcPageZone, DWORD srcPage)
{
	m_refPageZone = srcPageZone;
	m_refPageNum = srcPage;
	/* Draws on given page but restores previous page, so drawn noise is just
	 * waiting in the wings. */
	this->drawBg(m_refPageZone, m_refPageNum);
}

/**
 * @brief Causes reference noise page to be drawn.
 *
 * Uses vsgDrawMoveRectangle.  Error if reference noise isn't generated yet.
 */
void NoiseBgController::drawReferenceBg()
{
	if ((m_refPageZone != 0xFFFF) && (m_refPageNum != 0xFFFF)) {
		vsgDrawMoveRect(m_refPageZone, m_refPageNum, m_xPos, m_yPos, m_width, 
			m_height, m_xPos, m_yPos, m_width, m_height);
	}
}

/**
 * @brief Draw a pixel without clobbering the noise BG.
 * @param xPix X-coordinate of the pixel.
 * @param yPix Y-coordinate of the pixel.
 * @param fgLutVal LUT index of color to draw normally.
 * @param fgbgLutVal LUT index of color to draw if pixel contains noise.
 */
void NoiseBgController::drawPixelRespectingBg(int xPix, int yPix, 
											  int fgLutVal, int fgbgLutVal)
{
	if (m_occlude) {
		vsgSetPen1(fgLutVal);
	}
	else {
		int lutindex = vsgReadPixel(xPix, yPix);
		if (this->isNoise(lutindex)) {
			vsgSetPen1(fgbgLutVal);
		} 
		else {
			vsgSetPen1(fgLutVal);
		}
	}
	vsgDrawPixel(xPix, yPix);
}

/**
 * @brief Sets the LUT to the configuration needed for controller to work.
 *
 * Useful in case something modifies the LUT and then you need to draw BG 
 * noise.
 */
void NoiseBgController::configureLut()
{
	m_binocPercent = BOUNDED_PERCENT(m_binocPercent);
	m_leftPercent  = BOUNDED_PERCENT(m_leftPercent);
	m_rightPercent = BOUNDED_PERCENT(m_rightPercent);
	m_bgPercent    = BOUNDED_PERCENT(100 - m_binocPercent - m_leftPercent - m_rightPercent);

	/* Set up LUT with 100 levels at the specified percentages. */
	for (int cnt = 0, ix = m_lutBase; cnt < 100; ++cnt, ++ix) {
		if (cnt < m_bgPercent) {
			m_lutBuffer[ix] = m_bgColor;
		}
		else if (cnt  < (m_bgPercent + m_rightPercent)) {
			m_lutBuffer[ix] = m_rightNoiseColor;
		}
		else if (cnt  < (m_bgPercent + m_rightPercent + m_leftPercent)) {
			m_lutBuffer[ix] = m_leftNoiseColor;
		}
		else {
			m_lutBuffer[ix] = m_binocNoiseColor;
		}
	}
}

/**
 * @brief Causes BG controller to save patch of framestore.
 *
 * Useful if you've drawn BG with this controller, then want to draw an obj,
 * but don't want to draw the object pixel-wise to respect the BG.
 *
 * Warning: this action is pretty time-consuming.
 *
 * If this controller has already saved a patch, the old one is deleted.
 *
 * @param xPix The X coordinate of the top-left pixel.
 * @param yPix The Y coordinate of the top-left pixel.
 * @param height The height, i.e. range of Y values, in pixels.
 * @param width The width, i.e. range of X values, in pixels.
 */
void NoiseBgController::savePatch(int xPix, int yPix, int width, int height)
{
	if (m_patch) {
		delete m_patch;
	}
	m_patch = new PatchCopy(xPix, yPix, width, height);
}

/**
 * @brief Restore BG pattern to framestore.
 *
 * A saved patch remembers where in the framestore it was copied from.  This
 * function copies the BG pattern back while preserving the dichroic shape.
 *
 * Warning: this action is very costly.
 *
 * @param colorMap Mapping of fg colors to "fg + noise" colors.
 */
void NoiseBgController::restorePatch(const NoiseColorMap& colorMap)
{
	if (!m_patch) {
		return;
	}

	long pen1 = vsgGetPen1();
	for (int xx = m_patch->getX(); xx < m_patch->getX() + m_patch->getWidth(); xx++) {
		for (int yy = m_patch->getY(); yy < m_patch->getY() + m_patch->getHeight(); yy++) {
			int newLevel = colorMap.get(vsgReadPixel(xx, yy));
			int oldLevel = m_patch->get(xx, yy);
			if ((newLevel != -1) && isNoise(oldLevel)) {
				/* If pixel has a mapped fg color and pixel used to be noise, 
				 * replace with the mapped color. */
				vsgSetPen1(newLevel);
				vsgDrawPixel(xx, yy);
			}
			else if (newLevel == -1) {
				/* If pixel is not part of the shape, copy back noise 
				 * pattern just to be sure. */
				vsgSetPen1(oldLevel);
				vsgDrawPixel(xx, yy);
			}
		}
	}

	/* Restore pen */
	vsgSetPen1(pen1);
}

int NoiseBgController::getLevel(int xPix, int yPix, int fgLevel, 
								const NoiseColorMap& colorMap)
{
	int newLevel = colorMap.get(fgLevel);
	int oldLevel = vsgReadPixel(xPix, yPix);
	/* If fg color has a mapped fg+noise color and pixel is presently noise, 
	 * color, replace w/mapped fg+noise color. */
	return ((isNoise(oldLevel) && (newLevel != -1)) ? newLevel : fgLevel);
}

/**
 * @brief Whether the given LUT level is noise.
 * @param level LUT level.
 * @return True if the level is the noise color; false otherwise.
 */
bool NoiseBgController::isNoise(int level)
{
	return (((m_lutBase + m_bgPercent) <= level) && 
		(level < (m_lutBase + 100)));
}

/*** Accessors *****************************************************/

void NoiseBgController::setXPos(int xPos) { m_xPos = xPos; }
int NoiseBgController::getXPos() { return m_xPos; }

void NoiseBgController::setYPos(int yPos) { m_yPos = yPos; }
int NoiseBgController::getYPos() { return m_yPos; }

void NoiseBgController::setWidth(int width) { m_width = width; }
int NoiseBgController::getWidth() { return m_width; }

void NoiseBgController::setHeight(int height) { m_height = height; }
int NoiseBgController::getHeight() { return m_height; }

void NoiseBgController::setSize(int size) { m_size = size; }
int NoiseBgController::getSize() { return m_size; }

/**
 * @brief Set new BG and noise colors.
 *
 * Causes class to reconfigure the LUT.
 *
 * @param bgColor Values to set for BG color; param not modified.
 * @param binocNoiseColor Values to set for noise color; param not modified.
 */
void NoiseBgController::setColors(const VSGTRIVAL& bgColor, 
								  const VSGTRIVAL& binocNoiseColor)
{
	m_bgColor = bgColor;
	m_binocNoiseColor = binocNoiseColor;
	m_leftNoiseColor = binocNoiseColor;  m_leftNoiseColor.a = 0.0;
	m_rightNoiseColor = binocNoiseColor; m_rightNoiseColor.c = 0.0;
	this->configureLut();
}

/**
 * @brief Read BG and noise colors into variables.
 * @param[out] bgColor Receives BG color values.
 * @param[out] binocNoiseColor Receives noise color values.
 */
void NoiseBgController::getColors(VSGTRIVAL& bgColor, 
								  VSGTRIVAL& binocNoiseColor) const
{
	bgColor = m_bgColor;
	binocNoiseColor = m_binocNoiseColor;
}

/**
 * @brief Set new BG and noise colors.
 *
 * Causes class to reconfigure the LUT.
 *
 * @param bgColor Values to set for BG color; param not modified.
 * @param binocNoiseColor Values to set for noise color; param not modified.
 * @param leftNoiseColor Values for noise visible only to left eye; param not 
 *        modified.
 * @param rightNoiseColor Values for noise visible only to right eye; param 
 *        not modified.
 */
void NoiseBgController::setColors(const VSGTRIVAL& bgColor, 
								  const VSGTRIVAL& binocNoiseColor,
								  const VSGTRIVAL& leftNoiseColor,
								  const VSGTRIVAL& rightNoiseColor)
{
	m_bgColor = bgColor;
	m_binocNoiseColor = binocNoiseColor;
	m_leftNoiseColor = leftNoiseColor;
	m_rightNoiseColor = rightNoiseColor;
	this->configureLut();
}

/**
 * @brief Read BG and noise colors into variables.
 * @param[out] bgColor Receives BG color values.
 * @param[out] binocNoiseColor Receives noise color values.
 */
void NoiseBgController::getColors(VSGTRIVAL& bgColor, VSGTRIVAL& binocNoiseColor, 
								  VSGTRIVAL& leftNoiseColor, 
								  VSGTRIVAL& rightNoiseColor) const
{
	bgColor = m_bgColor;
	binocNoiseColor = m_binocNoiseColor;
	leftNoiseColor = m_leftNoiseColor;
	rightNoiseColor = m_rightNoiseColor;
}

/**
 * @brief Set new location within LUT for class to use.
 *
 * Causes class to reconfigure the LUT.
 * 
 * @param lutBase New index; must be < 50.
 */
void NoiseBgController::setLutBase(int lutBase)
{
	m_lutBase = lutBase;
	this->configureLut();
}

int NoiseBgController::getLutBase() { return m_lutBase; }

void NoiseBgController::setBinocPercent(int percent) { m_binocPercent = percent; }
int NoiseBgController::getBinocPercent() { return m_binocPercent; }

void NoiseBgController::setLeftPercent(int percent) { m_leftPercent = percent; }
int NoiseBgController::getLeftPercent() { return m_leftPercent; }

void NoiseBgController::setRightPercent(int percent) { m_rightPercent = percent; }
int NoiseBgController::getRightPercent() { return m_rightPercent; }

void NoiseBgController::setOcclude(bool occlude) { m_occlude = occlude; }
bool NoiseBgController::getOcclude() { return m_occlude; }

/****************************************************************************/

/**
 * @brief Create copy of a patch of pixels.
 *
 * Allocates memory for the copy.
 *
 * @param xPix The X coordinate of the top-left pixel.
 * @param yPix The Y coordinate of the top-left pixel.
 * @param height The height, i.e. range of Y values, in pixels.
 * @param width The width, i.e. range of X values, in pixels.
 */
NoiseBgController::PatchCopy::PatchCopy(int xPix, int yPix, int width, int height)
: m_xPix(xPix), m_yPix(yPix), m_width(width), m_height(height), m_grid(0)
{
	m_size = m_width * m_height;
	m_grid = new int[m_size];

	for (int xx = 0; xx < m_width; xx++) {
		for (int yy = 0; yy < m_height; yy++) {
			m_grid[xyIx(xx, yy)] = vsgReadPixel(xx + m_xPix, yy + m_yPix);
		}
	}
}

/**
 * @brief Cleanup allocated internal storage.
 */
NoiseBgController::PatchCopy::~PatchCopy()
{
	if (m_grid) {
		delete m_grid;
	}
}

/**
 * @brief Get LUT level for pixel at coordinates.
 *
 * Use pixel coordinates relative to the framestore, not the patch.  That is, 
 * if you copy a patch 100 pixels wide and 250 pixels tall starting at pixel
 * 25, 25, your call for the middle pixel should be get(75, 150), <b>not</b>
 * get(50, 125).
 *
 * @param xPix X coordinate of pixel.
 * @param yPix Y coordinate of pixel.
 * @return The LUT level, [0,255], or < 0 for an invalid reference.
 */
int NoiseBgController::PatchCopy::get(int xPix, int yPix)
{
	int index = xyIx(xPix - m_xPix, yPix - m_yPix);
	if ((index < 0) || (index >= m_size)) {
		return -1;
	}
	else {
		return m_grid[index];
	}
}

int NoiseBgController::PatchCopy::xyIx(int xPix, int yPix) { return (xPix * m_height + yPix); }

int NoiseBgController::PatchCopy::getX() { return m_xPix; }
int NoiseBgController::PatchCopy::getY() { return m_yPix; }
int NoiseBgController::PatchCopy::getWidth() { return m_width; }
int NoiseBgController::PatchCopy::getHeight() { return m_height; }

/****************************************************************************/

/**
 * @brief Create empty NoiseColorMap.
 */
NoiseColorMap::NoiseColorMap()
: m_map()
{}

/**
 * @brief Create copy of other map.
 * Just a matter of copying the other's m_map field.
 * @param other NoiseColorMap to copy.
 */
NoiseColorMap::NoiseColorMap(const NoiseColorMap& other)
: m_map(other.m_map)
{}

/**
 * @brief Clean up map, including any storage.
 */
NoiseColorMap::~NoiseColorMap() 
{}

/**
 * @brief Sets this map equal to the other.
 * Future changes are independent, i.e. no references are preserved.
 * @param other NoiseColorMap to copy.
 * @return This NoiseColorMap, set to equal the param.
 */
NoiseColorMap& NoiseColorMap::operator=(const NoiseColorMap& other)
{
	if (this != &other) {
		m_map.clear();
		m_map.insert(other.m_map.begin(), other.m_map.end());
	}
	return (*this);
}

/**
 * @brief Map the LUT levels.
 *
 * For example, if "red" is LUT level 1 and "red plus noise" is LUT level 2,
 * you would call "map(1, 2)".
 *
 * @param key The LUT level to be mapped.
 * @param value The LUT level to which key should be mapped.
 */
void NoiseColorMap::map(long key, long value) { m_map[key] = value; }

/**
 * @brief Return the value mapped to key.
 * @param key The LUT level to access.
 * @return The value mapped to key, or -1 if unmapped.
 */
long NoiseColorMap::get(long key) const
{
	std::map<long, long>::const_iterator val = m_map.find(key);
	if (val == m_map.end()) {
		return -1;
	}
	else {
		return (val->second);
	}
}

}; /* namespace VsgExt */
