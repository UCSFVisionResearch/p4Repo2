/**
 * @file Gratings.h
 * @brief 
 */

#ifndef GRATINGS_H
#define GRATINGS_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afx.h>
#include <vector>
#include <utility>
#include "GratingsTrial.h"
#include "comm/SerialComm.h"
#include "sys/sys_concurrency.h"
#include "suites/settings/Settings.h"

/*****************************************************************************
 * Statics and Globals
 *****************************************************************************/

extern CSettings* g_pSettings;

/*****************************************************************************
 * DEFINES
 *****************************************************************************/

/**
 * @brief Length of string buffers for strings specifying fixation coordinates.
 */
#define FIXSTR_BUFLEN (1024)

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CGratings : public Mixin_ThreadAware
{
public:
	static CString const defaultFile;
	CString unitTag;

	const static int TYPE_RANDOM = 1;
	const static int TYPE_STATIC = 2;
	const static int TYPE_ROTATE = 3;

	const static int STYLE_SQUARE = 1;
	const static int STYLE_SINE = 2;
	const static int STYLE_GABOR = 3;
	const static int STYLE_RFMAP = 4;
	
	const static int SHUTTER_MANUAL = 1;
	const static int SHUTTER_INTERLEAVED = 2;
	const static int SHUTTER_RANDOM = 3;

	int numTrials;
	CGratingsTrial **trials;

	CGratings();
	~CGratings();

	void LoadFile(bool bDefault);
	void SaveFile(bool bDefault);
	void GenerateTrials();
	void GenerateRFMappingTrials();
	void sendSettings(CSerialComm& theComm);

	int getPhase() {return iPhase;}
	bool setPhase(int i) {bool changed = iPhase != i; iPhase=i; return changed;}

	int getAngle() {return iAngle;}
	bool setAngle(int i) {bool changed = iAngle != i; iAngle=i; return changed;}

	int getType() {return iType;}
	bool setType(int i) {bool changed = iType != i; iType=i; return changed;}

	const CSmartCoord* getRandomLeftFixation();
	const CSmartCoord* getRandomRightFixation();

	void setLeftFixations(const char* fixationStr);
	void setRightFixations(const char* fixationStr);

	CString getLeftFixationString();
	CString getRightFixationString();

	float getDotSize() {return fDotSize;}
	void setDotSize(float f) {fDotSize=f;}

	float getGratingWidth() {return fGratingWidth;}
	void setGratingWidth(float f) {fGratingWidth=f;}

	int getNumAngles() {return iNumAngles;}
	bool setNumAngles(int i) {bool changed = iNumAngles != i; iNumAngles=i; return changed;}

	float getGaborSD() {return fGaborSD;}
	void setGaborSD(float f) {fGaborSD=f;}

	float getStaticPhase() {return fStaticPhase;}
	bool setStaticPhase(float f) {bool changed = fStaticPhase != f; fStaticPhase=f; return changed;}

	float getStaticAngle() {return fStaticAngle;}
	bool setStaticAngle(float f) {bool changed = fStaticAngle != f; fStaticAngle=f; return changed;}

	int getDelay() {return iDelay;}
	void setDelay(int i) {iDelay=i;}
	
	int getBlank() {return iBlank;}
	void setBlank(int i) {iBlank=i;}
			
	int getNumPhases() {return iNumPhases;}
	bool setNumPhases(int i) {bool changed = iNumPhases != i; iNumPhases=i; return changed;}
	
	int getGratingRed() {return iGratingRed;}
	void setGratingRed(int i) {iGratingRed=i;}
	
	int getGratingGreen() {return iGratingGreen;}
	void setGratingGreen(int i) {iGratingGreen=i;}
	
	int getGratingBlue() {return iGratingBlue;}
	void setGratingBlue(int i) {iGratingBlue=i;}

	int getGratingRed2() {return iGratingRed2;}
	void setGratingRed2(int i) {iGratingRed2=i;}
	
	int getGratingGreen2() {return iGratingGreen2;}
	void setGratingGreen2(int i) {iGratingGreen2=i;}
	
	int getGratingBlue2() {return iGratingBlue2;}
	void setGratingBlue2(int i) {iGratingBlue2=i;}
	
	int getNumRepeats() {return iNumRepeats;}
	bool setNumRepeats(int i) {bool changed = iNumRepeats != i; iNumRepeats=i; return changed;}
	
	const CString getUnitTag();
	void setUnitTag(CString cstr);

	bool getBehavior() {return bBehavior;}
	void setBehavior(bool b) {bBehavior=b;}
	
	int getInitiationDelay() {return iInitiation;}
	void setInitiationDelay(int i) {iInitiation=i;}

	int getRewardDelay() {return iReward;}
	void setRewardDelay(int i) {iReward=i;}

	int getHSteps() {return iHSteps;}
	void setHSteps(int i) {iHSteps=i;}

	int getVSteps() {return iVSteps;}
	void setVSteps(int i) {iVSteps=i;}

	float getStepSize() {return fStepSize;}
	void setStepSize(float f) {fStepSize=f;}

	bool getUseSaccades() {return bUseSaccades;}
	void setUseSaccades(bool b) {bUseSaccades=b;}

	int getGratingsPerSaccade() {return iGratingsPerSaccade;}
	void setGratingsPerSaccade(int i) {iGratingsPerSaccade=i;}

	int getShutters() {return iShutters;}
	bool setShutters(int i) {bool changed = iShutters != i; iShutters=i; return changed;}

	int getShutterInterleavedN() {return iShutterInterleavedN;}
	bool setShutterInterleavedN(int i) {bool changed = iShutterInterleavedN != i; iShutterInterleavedN=i; return changed;}

	int getShutterDelay() {return iShutterDelay;}
	void setShutterDelay(int i) {iShutterDelay=i;}

	float getRFX() {return fRFX;}
	bool setRFX(float f) {bool changed = fRFX != f; fRFX=f; return changed;}

	float getRFY() {return fRFY;}
	bool setRFY(float f) {bool changed = fRFY != f; fRFY=f; return changed;}

	float getDevX() {return fDevX;}
	bool setDevX(float f) {bool changed = fDevX != f; fDevX=f; return changed;}

	float getDevY() {return fDevY;}
	bool setDevY(float f) {bool changed = fDevY != f; fDevY=f; return changed;}

	float getSpeed() {return fSpeed;}
	bool setSpeed(float f) {bool changed = fSpeed != f; fSpeed=f; return changed;}

	int getRFEye() {return iRFEye;}
	bool setRFEye(int i) {bool changed = iRFEye != i; iRFEye=i; return changed;}

	float getFinalSize() {return fFinalSize;}
	bool setFinalSize(float f) {bool changed = fFinalSize != f; fFinalSize=f; return changed;}

	float getInitialSize() {return fInitialSize;}
	bool setInitialSize(float f) {bool changed = fInitialSize != f; fInitialSize=f; return changed;}

private:
	typedef CSmartCoord** CoordPtrArray_t;

	int iPhase, iAngle, iType, iNumAngles, iDelay, iBlank, iNumPhases,
		iGratingRed, iGratingGreen, iGratingBlue, iGratingRed2, iGratingGreen2, iGratingBlue2,
		iNumRepeats, iInitiation, iReward, iHSteps, iVSteps, iGratingsPerSaccade,
		iShutters, iShutterInterleavedN, iShutterDelay, iRFEye;
	float fDotSize, fGratingWidth, fGaborSD, fStaticPhase, fStaticAngle, fStepSize,
		fRFX, fRFY, fDevX, fDevY, fSpeed, fInitialSize, fFinalSize;
	bool bBehavior, bUseSaccades;

	CoordPtrArray_t m_leftFixationPoints;
	CoordPtrArray_t m_rightFixationPoints;
	size_t m_numLeftFixationPoints;
	size_t m_numRightFixationPoints;

	CString m_leftStringFromFile;
	CString m_rightStringFromFile;

	void populatePoints(const char* fixationStr, CoordPtrArray_t& coords, size_t& numCoords);
	void clearPoints(CoordPtrArray_t& coords, size_t& numCoords);
	CString constructString(CoordPtrArray_t& coords, size_t numCoords);
};

#endif
