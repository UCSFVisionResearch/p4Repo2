/**
 * @file SaccadeStimulus.cpp
 * @brief 
 */

#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <math.h>
#include <float.h>
#include "app/AppCore.h"
#include "utils/Utils.h"
#include "vsgext/vsgext.h"

/*!!!!!!! Saccade Timings not perfect -- need to fix that !!!!!!!!!!!! */

/***********************************************************
COMMUNICATIONS:
	VSG Out 1:  Trial Reset pulse. This pulse signals the 1401
				to reset the behavioral contingency count because
				the time out period has expired and thus the stimulus
				has been moved.
	
	VSG IN 0:	Next Stim Pulse. 1 => Behavioral contingency
				has been met, move on to the excursion phase.
				0 => behvioral continengency criteria has not 
				yet been met.

	Analog Out 0:	Horizontal target location
	Analog Out 1:	Vertical target location
************************************************************/

/*** LUT Levels ******************************************/
#define LEVEL_CTR_TARG  (0)
#define LEVEL_BG_CONTR  (2) // Reserve 2-101 and 102-201
#define LEVEL_TIME_OUT                    (202)
#define LEVEL_PRIMARY                     (203)
#define LEVEL_SECONDARY                   (204)
#define LEVEL_PRIMARY_PLUS_NOISE          (206)
#define LEVEL_SECONDARY_PLUS_NOISE        (207)

/*** Page Numbers*****************************************/
#define DRAW_PAGE_REF_BG (2)
#define DRAW_PAGE_BLANK  (3)

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

/***********************************************************
	 Constructor
	 -  Initializes the newly instantiated object. 
************************************************************/
CSaccadeStimulus::CSaccadeStimulus() 
: m_primarySpec(NULL), m_secondarySpec(NULL), m_noiseBgController(NULL), 
  m_noiseColorMap(NULL)
{
	Initialize();
	iScrWidthPix = vsgGetScreenWidthPixels();
	iScrHeightPix = vsgGetScreenHeightPixels();
	bEvenFrame = false;
	iOldLocIndex = 0;
	iOlderLocIndex = 0;
	iPage = 0;
	bDrawNext = true;
	vsgIOWriteDigitalOut(0xFFFF, vsgDIG0);
	bWasPrimary = false;

	vsgSetDrawMode(vsgCENTREXY);
	vsgSetDrawOrigin(iScrWidthPix / 2, iScrHeightPix /2 );

	/* Populate colors generally; per-trial colors are populated in 
	 * DisplayStimulus. */
	InitLutBuffer();
	InitNoiseHandling();
	InitGratings();
	
	g_pSaccade->sendSettings(g_pSettings->theSerialComm);
}

/***********************************************************
	 Destructor
	 -  Destroys the stimulus object. Turns the stimulus
	 running signal off
************************************************************/
CSaccadeStimulus::~CSaccadeStimulus() 
{
	vsgIOWriteDigitalOut(0x0000, vsgDIG0);

	CleanUpNoiseHandling();
	CleanUpGratings();

	//send stop mark
	char buffer[80];
	sprintf(buffer, "Saccade;Stop;%d;%ld|0x%02x/\0", 0, time(NULL), 200);
	g_pSettings->theSerialComm.transmit(buffer, (int)strlen(buffer));
}

/**
 * @brief Set colors in member VSGLUTBUFFER for targets and background.
 */
void CSaccadeStimulus::InitLutBuffer() 
{
	/* For use here and later with BG Controller (if needed) */
	m_bgColor.a    = ((double)g_pSettings->getRedBG()) / 255.0f;
	m_bgColor.b    = ((double)g_pSettings->getGreenBG()) / 255.0f;
	m_bgColor.c    = ((double)g_pSettings->getBlueBG()) / 255.0f;
	m_noiseColor.a = ((double)g_pSaccade->getRedNoise()) / 255.0f;
	m_noiseColor.b = ((double)g_pSaccade->getGreenNoise()) / 255.0f;
	m_noiseColor.c = ((double)g_pSaccade->getBlueNoise()) / 255.0f;

	/* Save copies of all periph colors */
	m_lutBuffer[LEVEL_PRIMARY].a = ((double)g_pSaccade->getRedPrimary())/255.0f;
	m_lutBuffer[LEVEL_PRIMARY].b = ((double)g_pSaccade->getGreenPrimary())/255.0f;
	m_lutBuffer[LEVEL_PRIMARY].c = ((double)g_pSaccade->getBluePrimary())/255.0f;

	m_lutBuffer[LEVEL_SECONDARY].a = ((double)g_pSaccade->getRedSecondary())/255.0f;
	m_lutBuffer[LEVEL_SECONDARY].b = ((double)g_pSaccade->getGreenSecondary())/255.0f;
	m_lutBuffer[LEVEL_SECONDARY].c = ((double)g_pSaccade->getBlueSecondary())/255.0f;

	m_lutBuffer[LEVEL_PRIMARY_PLUS_NOISE].a = m_lutBuffer[LEVEL_PRIMARY].a;
	m_lutBuffer[LEVEL_PRIMARY_PLUS_NOISE].b = 0.0f;
	m_lutBuffer[LEVEL_PRIMARY_PLUS_NOISE].c = m_noiseColor.c;

	m_lutBuffer[LEVEL_SECONDARY_PLUS_NOISE].a = m_noiseColor.a;
	m_lutBuffer[LEVEL_SECONDARY_PLUS_NOISE].b = 0.0f;
	m_lutBuffer[LEVEL_SECONDARY_PLUS_NOISE].c = m_lutBuffer[LEVEL_SECONDARY].c;
}

/**
 * @brief Allocate the NoiseBgController and NoiseColorMap, and initialize
 * them.
 */
void CSaccadeStimulus::InitNoiseHandling()
{
	int size = g_pSaccade->getNoiseSize();
	int percent = g_pSaccade->getNoisePercent();

	CleanUpNoiseHandling();

	m_noiseBgController = new VsgExt::NoiseBgController(size, m_bgColor, 
		m_noiseColor, LEVEL_BG_CONTR, percent, m_lutBuffer);
	m_noiseBgController->initReferenceBg(vsgVIDEOPAGE, DRAW_PAGE_REF_BG);

	m_noiseColorMap = new VsgExt::NoiseColorMap;
	m_noiseColorMap->map(LEVEL_PRIMARY, LEVEL_PRIMARY_PLUS_NOISE);
	m_noiseColorMap->map(LEVEL_SECONDARY, LEVEL_SECONDARY_PLUS_NOISE);
}

/**
 * @brief Deletes noise handling objects.
 */
void CSaccadeStimulus::CleanUpNoiseHandling()
{
	if (m_noiseBgController) delete m_noiseBgController;
	if (m_noiseColorMap)     delete m_noiseColorMap;
}

/**
 * @brief Initializes grating handlers.
 */
void CSaccadeStimulus::InitGratings()
{
	CleanUpGratings();
	m_primarySpec = VsgExt::IGrating::identity(LEVEL_PRIMARY);
	m_secondarySpec = VsgExt::IGrating::identity(LEVEL_SECONDARY);
}

/**
 * @brief Deletes grating objecgts.
 */
void CSaccadeStimulus::CleanUpGratings()
{
	if (m_primarySpec)      delete m_primarySpec;
	if (m_secondarySpec)    delete m_secondarySpec;
}

/***********************************************************
	 DisplayStimulus
	 -   This function calculates displays the dot at the 
	 next point (calculated based on the parameters specified).
************************************************************/
bool CSaccadeStimulus::DisplayStimulus() 
{
	// if the dot advances based on a behvioral contingency, 
	// check for the next stim signal from the 1401
	if (g_pSaccade->useBehavior()) {
		int response = vsgIOReadDigitalIn() & vsgDIG0;//0x001
		// if the next stim signal has come in, signal
		// the function to draw the dot in the next position
		// if the next stim signal has not come in, check to 
		// see if the max wait time period has expired.
		if (response > 0) {
			bDrawNext = true; 	
		} 
		else if (g_pSaccade->getMaxDisplayDuration() <= theMaxWaitTimer.CheckTimer()) {
			// send the stimulus reset signal to the 1401. This signal
			// tells the 1401 that the max wait period has expired, so the 
			// behvioral contingency counter should be reset
			vsgIOWriteDigitalOut(0xFFFF, vsgDIG1);

			// the stimulus shoudl be redrawn at the next location
			bDrawNext = true;
		}
	} 
	else { 
		// if behavioral contingency is not being used, always advance to the next location
		bDrawNext = true;
	}

	// if the stimulus is not advancing, make sure oscillations still are handled
	// if there are no osicillations, sleep for 10 milliseconds to allow the UI
	// thread to run briefly
	if (!bDrawNext) {
		if (g_pSaccade->isOscillationEnabled()) {
			doOscillations(iFrameNum);
			iFrameNum++;
		} 
		else {
			WaitUntil(10);
		}
	} 
	else { // we need to redraw the stimulus
		// create and start the timer
		etu::CTimer SaccadeTimer;
		SaccadeTimer.StartTimer();
		double dDotSize;
		double dPosXPix, dPosYPix;
		double screenXOffset, screenYOffset;
		double dFrameInterval = g_dHalfFrameInterval * 2;

		// set teh dot position to the center initially.
		// The position will be calculated before the drawing occurs
		dPosXPix = 0;
		dPosYPix = 0;
	
		// get the coordinates of the next point to display
		GetNextPoint(dPosXDeg, dPosYDeg, bEvenFrame);
		
		// set the view distance and set the display units
		vsgSetViewDistMM(g_pSettings->getViewDistance());
		vsgSetSpatialUnits(vsgPIXELUNIT);	

		// set the background color
		vsgSetBackgroundColour(&m_lutBuffer[LEVEL_BG_CONTR]);

		// convert the degrees to pixels
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSaccade->getDotSize(), vsgPIXELUNIT, &dDotSize);
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, dPosXDeg, vsgPIXELUNIT, &dPosXPix);
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, dPosYDeg, vsgPIXELUNIT, &dPosYPix);
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getScreenXOffset(), vsgPIXELUNIT, &screenXOffset);
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pSettings->getScreenYOffset(), vsgPIXELUNIT, &screenYOffset);

		// if a blank time was specified, wait for the blank time before changing the stimulus
		if (g_pSaccade->getBlankTime() > 0) {
	        vsgSetDrawPage(vsgVIDEOPAGE, DRAW_PAGE_BLANK, vsgBACKGROUND);
			vsgPresent();
			etu::AnalogOut(0, (float)-9.95);
			etu::AnalogOut(1, (float)-9.95);
			WaitUntil(g_pSaccade->getBlankTime());
			SaccadeTimer.StartTimer();
		}

		DrawDot(2.0 * dDotSize, dPosXPix, dPosYPix, screenXOffset, screenYOffset);
#if 0 /* Compiled out 11/1/11 while hacking in BG.  Above line replaced it. -JVB */
		// Draw the dot with the specified color at the specified position
		if (bEvenFrame) {
			if (g_pSaccade->getPointType()==0) {
				DrawDot(dDotSize, dPosXPix, dPosYPix, screenXOffset, screenYOffset);
			} 
			else {
				double angle = (rand()%4)*3.1415926*0.5;
				if (g_pSaccade->getPointType()==1) {
					DrawLandoltC(dDotSize, dPosXPix, dPosYPix, screenXOffset, screenYOffset, angle);
				} 
				else {
					DrawE(dDotSize, dPosXPix, dPosYPix, screenXOffset, screenYOffset, angle);
				}
			}
		} 
		else {
			DrawDot(dDotSize, dPosXPix, dPosYPix, screenXOffset, screenYOffset);
		}
#endif // #if 0

		// If oscillations are not being used or if behavioral contingency is being applied,
		// then update the analog out. The behavior clause is included becaues the way oscillations are handled
		// in the else clause forces the stimulus to be displayed for a fixed amount of time. The behavioral
		// contingency w/ oscillations case is handled above  (see the previous call to doOscillations above).
		if (!g_pSaccade->isOscillationEnabled() || g_pSaccade->useBehavior()) {	
			etu::AnalogOut(0, (float)dPosXDeg / g_pSettings->getDegPerVolt());
			etu::AnalogOut(1, (float)dPosYDeg / g_pSettings->getDegPerVolt());
					
			// if this is not behaviorally contingent, then wait for the run time delay
			if (!g_pSaccade->useBehavior()) {
				WaitUntil(g_pSaccade->getRunTimeDisplayDuration() - SaccadeTimer.CheckTimer());
			}
		} 
		else {

			// calculate number of frames for this presentation
			int iNumOscillationFrames = (int)((g_pSaccade->getRunTimeDisplayDuration() - SaccadeTimer.CheckTimer()) / dFrameInterval);

			// display the oscillating spot for the duration of the stimulus display at this location
			for (int i = 0; i < iNumOscillationFrames; i++) {
				doOscillations(i);
			}
		}

		bEvenFrame = !bEvenFrame;
	}

	return true;
}

/**************************************************************
	 GetNextPoint
	 -   This function calculates the dot position for the next
	 update. 
***************************************************************/
void CSaccadeStimulus::GetNextPoint(double& dPosXDeg, double& dPosYDeg, bool bEvenFrame) 
{
	int iLocIndex;

	// if this is an even frame and return to point has been set, 
	// then we must return to the point specified -- thus the 
	// next position is that point.
	if (bEvenFrame && g_pSaccade->isReturnToPt()) {
		dPosXDeg = g_pSaccade->getPointX();
		dPosYDeg = g_pSaccade->getPointY();
	} 
	else { // if return to point is not set or if we are in an odd frame
		// set the position to zero
		dPosXDeg = 0;
		dPosYDeg = 0;
		// if we are avoiding the zero point, then choose points
		// repeatedly until the zero point has not been seclected.
		if (g_pSaccade->avoidZero()) {
			// loops until the point chosen is not the last point displayed
			// and is not the zero point
			while (fabs(dPosXDeg) < 0.0001 && fabs(dPosYDeg) < 0.0001) {
				iLocIndex = rand() % g_pSaccade->getNumLocations();
				if (iLocIndex == iOldLocIndex) iLocIndex = (iLocIndex + 1) % g_pSaccade->getNumLocations();

				dPosXDeg = g_pSaccade->getLocationX(iLocIndex);
				dPosYDeg = g_pSaccade->getLocationY(iLocIndex);
			}
		
			// add the offsets into the dot position
			dPosXDeg += g_pSettings->getXOffset();
			dPosYDeg += g_pSettings->getYOffset();
		} 
		else {
			// if we are not avoiding the zero point...
			if (g_pSaccade->getSequentialOrder()) {
				//choose sequential location
				//[ 0  1  2  3 -1 -2 -3]
				if (iOldLocIndex == 0) {
					//zero. figure out which way to go
					if (iOlderLocIndex == 1) {
						//was 1, now move to "-1"		<--
						iLocIndex = g_pSaccade->getNumLocations()/2+1;
					} 
					else {
						//was "-1", now move to 1		-->
						iLocIndex = 1;
					}
				} 
				else if ( (iOldLocIndex == g_pSaccade->getNumLocations()/2+1) && (iOlderLocIndex == g_pSaccade->getNumLocations()/2+2) ) {
					//negative moving in to 0
					iLocIndex = 0;
				} 
				else {
					if ((iOldLocIndex == g_pSaccade->getNumLocations()/2) || (iOldLocIndex == g_pSaccade->getNumLocations()-1)) {
						//reversal
						iLocIndex = iOldLocIndex-1;
					} 
					else {
						//one side or the other. figure out which way to go
						if (iOldLocIndex > iOlderLocIndex) {
							iLocIndex = iOldLocIndex+1;		//	out
						} 
						else {
							iLocIndex = iOldLocIndex-1;		//	in
						}
					}
				}
			} 
			else {
				//choose a random point
				// and make sure it is not the same point as was previously displayed
				iLocIndex = rand() % g_pSaccade->getNumLocations();
				if (iLocIndex == iOldLocIndex) iLocIndex = (iLocIndex + 1) % g_pSaccade->getNumLocations();
			}
			
			// calculate the dot position based on the location just chosen
			dPosXDeg = g_pSaccade->getLocationX(iLocIndex) + g_pSettings->getXOffset();
			dPosYDeg = g_pSaccade->getLocationY(iLocIndex) + g_pSettings->getYOffset();
		}

		// save the index of the current position so we do not choose it again 
		// for the next position
		iOlderLocIndex = iOldLocIndex;
		iOldLocIndex = iLocIndex;

		int iJitterX = 0;
		int iJitterY = 0;
		// calculate the horizonatal jitter component if the 
		// the jitter was set to a value greater than zero
		if (g_pSaccade->getJitterX() > 0) {
			int theSign = (rand() % 2) * 2 -1;
			iJitterX = rand() % g_pSaccade->getJitterX() * theSign;
		}

		// calculate the veritcal jitter component if the 
		// the jitter was set to a value greater than zero
		if (g_pSaccade->getJitterY() > 0) {
			int theSign = (rand() % 2) * 2 -1;
			iJitterY = rand() % g_pSaccade->getJitterY() * theSign;
		}
		
		// add the jitter to the dot position
		dPosXDeg += iJitterX;
		dPosYDeg += iJitterY;
	} 
}

/**
 * @brief Draw fixation dot, respecting background.
 * @param dDotSize Dot diameter, ini pixels.
 * @param dPosXPix X coordinate, in pixels, of dot center.
 * @param dPosYPix Y coordinate, in pixels, of dot center.
 * @param screenXOffset X offset, in pixels, of screen (set globally).
 * @param screenXOffset Y offset, in pixels, of screen (set globally).
 */
void CSaccadeStimulus::DrawDot(double dDotSize, double dPosXPix, 
							   double dPosYPix, double screenXOffset, 
							   double screenYOffset)
{
	char buffer[80];

	VSGTRIVAL& primary = m_lutBuffer[LEVEL_PRIMARY];
	VSGTRIVAL& secondary = m_lutBuffer[LEVEL_SECONDARY];
	
	VsgExt::IGrating* currentColor = NULL;
	VsgExt::NoiseBgController* noiseController = NULL;
	VsgExt::NoiseColorMap* colorMap = NULL;

	//get and reset color palette
	iPage = 1 - iPage;
	vsgSetDrawPage(vsgVIDEOPAGE, iPage, vsgBACKGROUND);
	
	if (g_pSaccade->getDisplayNoiseBg() && g_pSaccade->getSameBgTrial()) {
		m_noiseBgController->drawReferenceBg();
	}
	else if (g_pSaccade->getDisplayNoiseBg()) {
		m_noiseBgController->drawBg();
	}

	// Set the dot color based on the color specified in the dialog
	if (g_pSaccade->getColorSetting() == CSaccade::COLOR_PRIMARY) {
		m_lutBuffer[LEVEL_CTR_TARG] = primary;
		currentColor = m_primarySpec;
		sprintf(buffer, "RGB: %.2f, %.2f, %.2f|0x0%x/", primary.a, primary.b, primary.c, 0x01);
	} 
	else if (g_pSaccade->getColorSetting() == CSaccade::COLOR_SECONDARY) {
		m_lutBuffer[LEVEL_CTR_TARG] = secondary;
		currentColor = m_secondarySpec;
		sprintf(buffer, "RGB: %.2f, %.2f, %.2f|0x0%x/", secondary.a, secondary.b, secondary.c, 0x02);
	} 
	else if (g_pSaccade->getColorSetting() == CSaccade::COLOR_ALTERNATE) {
		if (bWasPrimary) {
			m_lutBuffer[LEVEL_CTR_TARG] = secondary;
			currentColor = m_secondarySpec;
			sprintf(buffer, "RGB: %.2f, %.2f, %.2f|0x0%x/", secondary.a, secondary.b, secondary.c, 0x02);
		} 
		else {
			m_lutBuffer[LEVEL_CTR_TARG] = primary;
			currentColor = m_primarySpec;
			sprintf(buffer, "RGB: %.2f, %.2f, %.2f|0x0%x/", primary.a, primary.b, primary.c, 0x01);
		}

		// flip the bWasPrimary flag
		bWasPrimary = !bWasPrimary;
	} 
	else { // randomize
		int iColor = rand() % 2;
		if (iColor == 0) {
			m_lutBuffer[LEVEL_CTR_TARG] = secondary;
			currentColor = m_secondarySpec;
			sprintf(buffer, "RGB: %.2f, %.2f, %.2f|0x0%x/", secondary.a, secondary.b, secondary.c, 0x02);
		} 
		else {
			m_lutBuffer[LEVEL_CTR_TARG] = primary;
			currentColor = m_primarySpec;
			sprintf(buffer, "RGB: %.2f, %.2f, %.2f|0x0%x/", primary.a, primary.b, primary.c, 0x01);
		}
	}

	if (g_pSaccade->getDisplayNoiseBg() && !g_pSaccade->getOccludeBg()) {
		noiseController = m_noiseBgController;
		colorMap = m_noiseColorMap;
	}
	
	// draw the circle
	VsgExt::drawCircle((int)(dPosXPix + screenXOffset), 
		(int)(-dPosYPix + screenYOffset), (int)dDotSize, *currentColor, 
		colorMap, noiseController);

	// display the newly drawn circle
	vsgLUTBUFFERWrite(0, &m_lutBuffer);
	vsgLUTBUFFERtoPalette(0);
	vsgPresent();

	// make sure the trial reset pulse has been set to 0
	// to prevent the stimulus from automatically advancing
    vsgIOWriteDigitalOut(0x0000, vsgDIG1);

	// reset the maximum wait timer
	theMaxWaitTimer.StartTimer();

	// transmit the text mark
	g_pSettings->theSerialComm.transmit(buffer, (int)strlen(buffer));

	// reset the drawing flag
	bDrawNext = false;

	//reset the frame number used for the oscillations
	iFrameNum = 0;
}

#if 0 /* Compiled out 11/1/11 while hacking in BG. -JVB */

/**************************************************************
	 DrawLandoltC
	 -   This function draws the dot based on the parameters
	 given
***************************************************************/
void CSaccadeStimulus::DrawLandoltC(double dDotSize, double dPosXPix, 
									double dPosYPix, double screenXOffset, 
									double screenYOffset, double angle)
{
	//fix memory leak by avoiding object animation system
	//get and reset color palette
	VSGLUTBUFFER lb;
	vsgPaletteRead(&lb);
	lb[253].a = blackColor.a;
	lb[253].b = blackColor.b;
	lb[253].c = blackColor.c;
	iPage = 1 - iPage;
	vsgSetDrawPage(vsgVIDEOPAGE, iPage, vsgBACKGROUND);
	vsgSetDrawMode(vsgCENTREXY);
	vsgSetDrawOrigin(iScrWidthPix / 2, iScrHeightPix /2 );
	
	char buffer[80];

	// Set the dot color based on the color specified in the dialog
	if (g_pSaccade->getColorSetting() == CSaccade::COLOR_PRIMARY) {
		lb[0].a = primary.a;
		lb[0].b = primary.b;
		lb[0].c = primary.c;
		sprintf(buffer, "RGB: %.2f, %.2f, %.2f|0x0%x/", primary.a, primary.b, primary.c, 0x01);
	} 
	else if (g_pSaccade->getColorSetting() == CSaccade::COLOR_SECONDARY) {
		lb[0].a = secondary.a;
		lb[0].b = secondary.b;
		lb[0].c = secondary.c;
		sprintf(buffer, "RGB: %.2f, %.2f, %.2f|0x0%x/", secondary.a, secondary.b, secondary.c, 0x02);
	} 
	else if (g_pSaccade->getColorSetting() == CSaccade::COLOR_ALTERNATE) {
		// if the dot color is set to alternate, and was previously 
		// set as the primary color, then use the secondary. If it was
		// the secondary color, choose the primary
		if (bWasPrimary) {
			lb[0].a = secondary.a;
			lb[0].b = secondary.b;
			lb[0].c = secondary.c;
			sprintf(buffer, "RGB: %.2f, %.2f, %.2f|0x0%x/", secondary.a, secondary.b, secondary.c, 0x02);
		} 
		else {
			lb[0].a = primary.a;
			lb[0].b = primary.b;
			lb[0].c = primary.c;
			sprintf(buffer, "RGB: %.2f, %.2f, %.2f|0x0%x/", primary.a, primary.b, primary.c, 0x01);
		}

		// flip the bWasPrimary flag
		bWasPrimary = !bWasPrimary;
	} 
	else { // randomize
		int iColor = rand() % 2;
		if (iColor == 0) {
			lb[0].a = secondary.a;
			lb[0].b = secondary.b;
			lb[0].c = secondary.c;
			sprintf(buffer, "RGB: %.2f, %.2f, %.2f|0x0%x/", secondary.a, secondary.b, secondary.c, 0x02);
		} 
		else {
			lb[0].a = primary.a;
			lb[0].b = primary.b;
			lb[0].c = primary.c;
			sprintf(buffer, "RGB: %.2f, %.2f, %.2f|0x0%x/", primary.a, primary.b, primary.c, 0x01);
		}
	}
	int length = (int)strlen(buffer);
	vsgLUTBUFFERWrite(0,&lb);
	vsgLUTBUFFERtoPalette(0);
	vsgSetPen1(0);
	
	// draw the C
	vsgSetDrawMode(vsgCENTREXY);
	vsgDrawOval(dPosXPix + screenXOffset, -dPosYPix + screenYOffset, dDotSize, dDotSize);
	//vsgDrawPieArc(dPosXPix + screenXOffset, -dPosYPix + screenYOffset, dDotSize, dDotSize, angle-1.0, 2.0);//5min of arc
	vsgSetPen1(253);
	vsgDrawOval(dPosXPix + screenXOffset, -dPosYPix + screenYOffset, dDotSize*0.6, dDotSize*0.6);
	//vsgSetPen1(0);
	vsgDrawBar(dPosXPix + screenXOffset+0.5*dDotSize*cos(angle), -dPosYPix + screenYOffset+0.5*dDotSize*sin(angle), dDotSize, dDotSize*0.2, angle*180.0/3.1415926);
	// display the newly drawn C
	vsgPresent();

	// make sure the trial reset pulse has been set to 0
	// to prevent the stimulus from automatically advancing
    vsgIOWriteDigitalOut(0x0000, vsgDIG1);

	theMaxWaitTimer.StartTimer();// reset the maximum wait timer
	g_pSettings->theSerialComm.transmit(buffer, length);// transmit the text mark
	bDrawNext = false;// reset the drawing flag
	iFrameNum = 0;//reset the frame number used for the oscillations
}

/**************************************************************
	 DrawE
	 -   This function draws the dot based on the parameters
	 given
***************************************************************/
void CSaccadeStimulus::DrawE(double dDotSize, double dPosXPix, 
							 double dPosYPix, double screenXOffset, 
							 double screenYOffset, double angle)
{
	//fix memory leak by avoiding object animation system
	//get and reset color palette
	VSGLUTBUFFER lb;
	vsgPaletteRead(&lb);
	lb[253].a = blackColor.a;
	lb[253].b = blackColor.b;
	lb[253].c = blackColor.c;
	iPage = 1 - iPage;
	vsgSetDrawPage(vsgVIDEOPAGE, iPage, vsgBACKGROUND);
	vsgSetDrawMode(vsgCENTREXY);
	vsgSetDrawOrigin(iScrWidthPix / 2, iScrHeightPix /2 );
	
	char buffer[80];

	// Set the dot color based on the color specified in the dialog
	if (g_pSaccade->getColorSetting() == CSaccade::COLOR_PRIMARY) {
		lb[0].a = primary.a;
		lb[0].b = primary.b;
		lb[0].c = primary.c;
		sprintf(buffer, "RGB: %.2f, %.2f, %.2f|0x0%x/", primary.a, primary.b, primary.c, 0x01);
	} 
	else if (g_pSaccade->getColorSetting() == CSaccade::COLOR_SECONDARY) {
		lb[0].a = secondary.a;
		lb[0].b = secondary.b;
		lb[0].c = secondary.c;
		sprintf(buffer, "RGB: %.2f, %.2f, %.2f|0x0%x/", secondary.a, secondary.b, secondary.c, 0x02);
	} 
	else if (g_pSaccade->getColorSetting() == CSaccade::COLOR_ALTERNATE) {
		// if the dot color is set to alternate, and was previously 
		// set as the primary color, then use the secondary. If it was
		// the secondary color, choose the primary
		if (bWasPrimary) {
			lb[0].a = secondary.a;
			lb[0].b = secondary.b;
			lb[0].c = secondary.c;
			sprintf(buffer, "RGB: %.2f, %.2f, %.2f|0x0%x/", secondary.a, secondary.b, secondary.c, 0x02);
		} 
		else {
			lb[0].a = primary.a;
			lb[0].b = primary.b;
			lb[0].c = primary.c;
			sprintf(buffer, "RGB: %.2f, %.2f, %.2f|0x0%x/", primary.a, primary.b, primary.c, 0x01);
		}

		// flip the bWasPrimary flag
		bWasPrimary = !bWasPrimary;
	} 
	else { // randomize
		int iColor = rand() % 2;
		if (iColor == 0) {
			lb[0].a = secondary.a;
			lb[0].b = secondary.b;
			lb[0].c = secondary.c;
			sprintf(buffer, "RGB: %.2f, %.2f, %.2f|0x0%x/", secondary.a, secondary.b, secondary.c, 0x02);
		} 
		else {
			lb[0].a = primary.a;
			lb[0].b = primary.b;
			lb[0].c = primary.c;
			sprintf(buffer, "RGB: %.2f, %.2f, %.2f|0x0%x/", primary.a, primary.b, primary.c, 0x01);
		}
	}
	int length = (int)strlen(buffer);
	vsgLUTBUFFERWrite(0,&lb);
	vsgLUTBUFFERtoPalette(0);
	vsgSetPen1(0);
	
	// draw the E
	vsgSetDrawMode(vsgCENTREXY);
	if (angle < 3*3.14159/4.0 && angle > 3.14159/4.0) {//90//faces downward
		vsgSetDrawMode(vsgSOLIDPEN);
		double ww = dDotSize/5.0;
		vsgSetPenSize(ww, ww);
		vsgDrawLine(dPosXPix + screenXOffset-ww*2, -dPosYPix + screenYOffset-ww,
			dPosXPix + screenXOffset+ww*2, -dPosYPix + screenYOffset-ww);
		vsgDrawLine(dPosXPix + screenXOffset-ww*2, -dPosYPix + screenYOffset-ww,
			dPosXPix + screenXOffset-ww*2, -dPosYPix + screenYOffset+2*ww);
		vsgDrawLine(dPosXPix + screenXOffset, -dPosYPix + screenYOffset-ww,
			dPosXPix + screenXOffset, -dPosYPix + screenYOffset+2*ww);
		vsgDrawLine(dPosXPix + screenXOffset+ww*2, -dPosYPix + screenYOffset-ww,
			dPosXPix + screenXOffset+ww*2, -dPosYPix + screenYOffset+2*ww);
	} 
	else if (angle < 5*3.14159/4.0 && angle > 3*3.14159/4.0) {//180//faces left
		vsgSetDrawMode(vsgSOLIDPEN);
		double ww = dDotSize/5.0;
		vsgSetPenSize(ww, ww);
		vsgDrawLine(dPosXPix + screenXOffset-ww*2, -dPosYPix + screenYOffset-2*ww,
			dPosXPix + screenXOffset+ww, -dPosYPix + screenYOffset-2*ww);
		vsgDrawLine(dPosXPix + screenXOffset-ww*2, -dPosYPix + screenYOffset,
			dPosXPix + screenXOffset+ww, -dPosYPix + screenYOffset);
		vsgDrawLine(dPosXPix + screenXOffset-2*ww, -dPosYPix + screenYOffset+2*ww,
			dPosXPix + screenXOffset+ww, -dPosYPix + screenYOffset+2*ww);
		vsgDrawLine(dPosXPix + screenXOffset+ww, -dPosYPix + screenYOffset-2*ww,
			dPosXPix + screenXOffset+ww, -dPosYPix + screenYOffset+2*ww);
	} 
	else if (angle < 7*3.14159/4.0 && angle > 5*3.14159/4.0) {//270//faces up
		vsgSetDrawMode(vsgSOLIDPEN);
		double ww = dDotSize/5.0;
		vsgSetPenSize(ww, ww);
		vsgDrawLine(dPosXPix + screenXOffset-ww*2, -dPosYPix + screenYOffset+ww,
			dPosXPix + screenXOffset+ww*2, -dPosYPix + screenYOffset+ww);
		vsgDrawLine(dPosXPix + screenXOffset-ww*2, -dPosYPix + screenYOffset+ww,
			dPosXPix + screenXOffset-ww*2, -dPosYPix + screenYOffset-2*ww);
		vsgDrawLine(dPosXPix + screenXOffset, -dPosYPix + screenYOffset+ww,
			dPosXPix + screenXOffset, -dPosYPix + screenYOffset-2*ww);
		vsgDrawLine(dPosXPix + screenXOffset+ww*2, -dPosYPix + screenYOffset+ww,
			dPosXPix + screenXOffset+ww*2, -dPosYPix + screenYOffset-2*ww);
	} 
	else {//0//faces right
		vsgSetDrawMode(vsgSOLIDPEN);
		double ww = dDotSize/5.0;
		vsgSetPenSize(ww, ww);
		vsgDrawLine(dPosXPix + screenXOffset+ww*2, -dPosYPix + screenYOffset-2*ww,
			dPosXPix + screenXOffset-ww, -dPosYPix + screenYOffset-2*ww);
		vsgDrawLine(dPosXPix + screenXOffset+ww*2, -dPosYPix + screenYOffset,
			dPosXPix + screenXOffset-ww, -dPosYPix + screenYOffset);
		vsgDrawLine(dPosXPix + screenXOffset+2*ww, -dPosYPix + screenYOffset+2*ww,
			dPosXPix + screenXOffset-ww, -dPosYPix + screenYOffset+2*ww);
		vsgDrawLine(dPosXPix + screenXOffset-ww, -dPosYPix + screenYOffset-2*ww,
			dPosXPix + screenXOffset-ww, -dPosYPix + screenYOffset+2*ww);
	}
	
	// display the newly drawn E
	vsgPresent();

	// make sure the trial reset pulse has been set to 0
	// to prevent the stimulus from automatically advancing
    vsgIOWriteDigitalOut(0x0000, vsgDIG1);

	theMaxWaitTimer.StartTimer();// reset the maximum wait timer
	g_pSettings->theSerialComm.transmit(buffer, length);// transmit the text mark
	bDrawNext = false;// reset the drawing flag
	iFrameNum = 0;//reset the frame number used for the oscillations
}

#endif // #if 0

/**************************************************************
	 StimulusType()
	 -   Returns a string of the stimulus name
***************************************************************/
CString CSaccadeStimulus::StimulusType() 
{
	return "Saccade Stimulus";
}

/**************************************************************
	 doOscillations()
	 -   This function moves the screen to create the appearance
	 of motion. The screen is moved to the appropriate spot
	 based on the frame number given
***************************************************************/
void CSaccadeStimulus::doOscillations(int iFrameNum) 
{
	// calculate amplitude and frequency
	double dCalculatedFreq, dAmplitudePix;
	dCalculatedFreq = g_pSaccade->getOscillationFreq() * 2 * 3.14159265358979323846 / g_dFramerate;//M_PI
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSaccade->getOscillationAmplitude(), vsgPIXELUNIT, &dAmplitudePix);

	// do the oscillation
	double x, y;
	
	// calculate the x and y coordinate based on teh frequency, amplitude and frame number
	switch (g_pSaccade->getOscillationDirection()) {
		case CSaccade::ORIENT_HORIZ:
				x = SineWave(iFrameNum, dCalculatedFreq, dAmplitudePix);
				y = 0;
				break;
		case CSaccade::ORIENT_VERT: 
				x = 0;
				y = SineWave(iFrameNum, dCalculatedFreq, dAmplitudePix);
				break;
		case CSaccade::ORIENT_45DEG: 
				x = SineWave(iFrameNum, dCalculatedFreq, dAmplitudePix);
				y = x;
					
				break;
		case CSaccade::ORIENT_135DEG: 
				x = SineWave(iFrameNum, dCalculatedFreq, dAmplitudePix);
				y = -x;					
				break;
	}

	// convert the pixel x,y to degrees
	double xDeg, yDeg;
	g_pSettings->tanScreenUnit2Unit(vsgPIXELUNIT, x, vsgDEGREEUNIT, &xDeg);
	g_pSettings->tanScreenUnit2Unit(vsgPIXELUNIT, y, vsgDEGREEUNIT, &yDeg);

	// move the screen in the opposite direction to create the appearance
	// of motion in the correct direction
	vsgMoveScreen(-x, y);

	// update the analog output for the target dot
	etu::AnalogOut(0, (float)(xDeg + dPosXDeg) / g_pSettings->getDegPerVolt());
	etu::AnalogOut(1, (float)(yDeg + dPosYDeg) / g_pSettings->getDegPerVolt());

	// sleep briefly to allow the UI to process any commands that may 
	// come through
	Sleep(0);
}

#endif // APP_EYE_TRACKING
