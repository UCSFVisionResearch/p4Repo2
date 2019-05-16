/**
 * @file ReverseFixationStimulus.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <fstream>
#include "app/AppCore.h"
#include "ReverseFixationStimulus.h"
#include "utils/Utils.h"
#include "vsgext/vsgext.h"
#include "app/EyeTrackingDlg.h"

/*****************************************************************************
 * COMMUNICATIONS:
 *****************************************************************************/

#define A_OUT_STIM_HORIZ  (0) /* 1401 in channel 3 */
#define A_OUT_STIM_VERT   (1) /* 1401 in channel 4 */
#define A_OUT_STIM_COLOR   (4)

#define PULSE_ON_VOLTS   (5)
#define PULSE_OFF_VOLTS (-5)

#define LEVEL_PRIMARY (0)
#define LEVEL_SECONDARY (1)
#define	VOLTS_COLOR_RED (5)
#define VOLTS_COLOR_BLUE (-5)

#define MODE_FOVEA (0)
#define MODE_INIT (1)
#define MODE_TEST (2)

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

/***********************************************************
	 Constructor     
	 -  Initializes the newly instantiated object. 
************************************************************/

CReverseFixationStimulus::CReverseFixationStimulus() 
:	m_screenWidthPix(vsgGetScreenWidthPixels()), 
	m_screenHeightPix(vsgGetScreenHeightPixels()),
	m_initLeftXDeg(g_pReverseFixation->getInitLeftX()),
	m_initLeftYDeg(g_pReverseFixation->getInitLeftY()),
	m_initRightXDeg(g_pReverseFixation->getInitRightX()),
	m_initRightYDeg(g_pReverseFixation->getInitRightY()),
	m_screenOffsetXDeg(g_pSettings->getScreenXOffset()),
	m_screenOffsetYDeg(g_pSettings->getScreenYOffset()),
	m_stimSizePix(g_pReverseFixation->getDotSize()),
	m_pageNum(0),
	m_initLevel(g_pReverseFixation->getInitLeft()? LEVEL_SECONDARY : LEVEL_PRIMARY),
	m_mode(g_pReverseFixation->getMode()),
	m_displayTime(g_pReverseFixation->getDisplayTime()),
	targetAcquired(false), 
	m_windowAllowancePix(g_pRFMapping->getWindowAllowancePix()), 
	m_useWait(g_pRFMapping->getUseWait()),
	m_messageSent(false)
{
	double frameTime;
	VSGLUTBUFFER lutbuf;
	char bf[1024];

	bEndThread = false;

	if (!g_bIsInitialized) {
		if (vsgInit("") < 0) return;
		g_bIsInitialized = true;
		/* Set page width to 2048; display error if vsg.cfg isn't sync'ed on
		 * green. */
		vsgSetVideoMode(vsgPANSCROLLMODE);
	}

	m_PrimaryColor.a = ((double)g_pReverseFixation->getPrimaryRed()) / 255.0f;
	m_PrimaryColor.b = ((double)g_pReverseFixation->getPrimaryGreen()) / 255.0f;
	m_PrimaryColor.c = ((double)g_pReverseFixation->getPrimaryBlue()) / 255.0f;

	m_bgColor.a = ((double)g_pSettings->getRedBG()) / 255.0f;
	m_bgColor.b = ((double)g_pSettings->getGreenBG()) / 255.0f;
	m_bgColor.c = ((double)g_pSettings->getBlueBG()) / 255.0f;

	m_SecondaryColor.a = ((double)g_pReverseFixation->getSecondaryRed()) / 255.0f;
	m_SecondaryColor.b = ((double)g_pReverseFixation->getSecondaryGreen()) / 255.0f;
	m_SecondaryColor.c = ((double)g_pReverseFixation->getSecondaryBlue()) / 255.0f;

	memset(lutbuf, 0, 256 * sizeof(VSGTRIVAL));
	lutbuf[LEVEL_SECONDARY] = m_SecondaryColor;
	lutbuf[LEVEL_PRIMARY] = m_PrimaryColor;
	lutbuf[vsgBACKGROUND] = m_bgColor;
		
	frameTime = vsgGetSystemAttribute(vsgFRAMETIME);
	m_frameRate = 1000000.0 / frameTime;
	g_dFramerate = m_frameRate;
	g_dHalfFrameInterval = DWORD(floor(frameTime / 2000.0f));
	g_dSleepThreshold = (double)g_dHalfFrameInterval / 2.0f;

	vsgSetCommand(vsgPALETTECLEAR + vsgOVERLAYDRIFT + vsgVIDEODRIFT);
	vsgSetViewDistMM(g_pSettings->getViewDistance());
	vsgSetSpatialUnits(vsgPIXELUNIT);
	vsgSetFixationColour(&m_PrimaryColor);
	vsgSetBackgroundColour(&m_bgColor);
	vsgSetPageWidth(2048); /* In case we didn't call vsgSetVideoMode. */
	vsgSetDrawMode(vsgCENTREXY);
	vsgSetDrawOrigin(m_screenWidthPix / 2, m_screenHeightPix / 2);
	vsgLUTBUFFERWrite(0, &lutbuf);
	vsgLUTBUFFERtoPalette(0);

	vsgIOWriteDigitalOut(0xFFFF, vsgDIG0);
	vsgIOWriteDigitalOut(0x0000, vsgDIG1);
	

	/*Set up first image */
	if (m_initLevel==LEVEL_PRIMARY){
		m_stimXDeg = m_initRightXDeg;
		m_stimYDeg = m_initRightYDeg;
		m_currEye = LEVEL_PRIMARY;
	}else{
		m_stimXDeg = m_initLeftXDeg;
		m_stimYDeg = m_initLeftYDeg;
		m_currEye = LEVEL_SECONDARY;
	}
	/* Analog outs 0 and 1 match with the calibration stimulus. */
	etu::AnalogOut(A_OUT_STIM_HORIZ, (float)(m_stimXDeg / g_pSettings->getDegPerVolt()));
	etu::AnalogOut(A_OUT_STIM_VERT, (float)(m_stimYDeg / g_pSettings->getDegPerVolt()));

	m_stimXDeg = m_stimXDeg+g_pSettings->getXOffset();
	m_stimYDeg = m_stimYDeg+g_pSettings->getYOffset();
	

	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, (m_stimXDeg+m_screenOffsetXDeg), vsgPIXELUNIT, &m_stimXPix);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -1 * (m_stimYDeg+m_screenOffsetYDeg), vsgPIXELUNIT, &m_stimYPix);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pReverseFixation->getDotSize(), 
		vsgPIXELUNIT, &m_stimSizePix);

	m_pageNum = 1 - m_pageNum;
	vsgSetDrawPage(vsgVIDEOPAGE, m_pageNum, vsgBACKGROUND);
	vsgSetPen1(m_currEye);
	vsgDrawOval(m_stimXPix, m_stimYPix, m_stimSizePix * 2, m_stimSizePix * 2);

	vsgPresent();
	
	m_timer.StartTimer();

}

/*******************************************************************
	Destructor
		- Destroys any objecs that were instantiated on the heap.
		In this case, no objects were created within this stimulus
		thus nothing needs to be done. 
********************************************************************/
CReverseFixationStimulus::~CReverseFixationStimulus() 
{
	vsgIOWriteDigitalOut(0x0000, vsgDIG0);
	vsgObjDestroy(g_stim);
	vsgObjDestroy(g_fixation);
	vsgObjDestroy(g_grating);
	vsgObjDestroy(g_stim2);
	//send stop mark
	char buff[80];
	sprintf(buff, "ReverseFixation;Stop;%d;%ld|0x%02x/\0", 0, time(NULL), 200);
	g_pSettings->theSerialComm.transmit(buff, (int)strlen(buff));
}


/*******************************************************************
	DisplayStimulus
		- Updates the position of the target spot based on mode selected

********************************************************************/
bool CReverseFixationStimulus::DisplayStimulus() 
{
	double currFixEyePosX, currFixEyePosY; //Current Location of fixation Eye
	char bf[1024];
	Sleep(1);
	if (!m_messageSent) {
		sprintf(bf,"Sending Settings");
		g_pReverseFixationDlg->printDebug(bf);
		g_pReverseFixation->sendSettings(g_pSettings->theSerialComm);
		m_messageSent = true;
		
	}
	targetAcquired=false;
	m_timer.CheckTimerWithReset();
	//Checking for fixation and displayTime
	//if not fixating, time resets unless in Test Mode
	sprintf(bf,"RightX:%f\n LeftX:%f\n MCurr:%s\n,mStimX: %f\n",m_initRightXDeg,m_initLeftXDeg,m_currEye==LEVEL_PRIMARY?"primary":"secondary",m_stimXDeg);
	g_pReverseFixationDlg->printDebug(bf);
	while(!bEndThread && (!targetAcquired || (m_timer.CheckTimer()<m_displayTime))){
		g_pEyeTrackingDlg->EyePosSample();
		g_pReverseFixationDlg->UpdateEyePositions(
			g_pEyeTrackingDlg->avglx, g_pEyeTrackingDlg->avgly,
			g_pEyeTrackingDlg->avgrx, g_pEyeTrackingDlg->avgry);
		if(m_currEye ==LEVEL_PRIMARY){
			currFixEyePosX = g_pEyeTrackingDlg->avgrx;
			currFixEyePosY = g_pEyeTrackingDlg->avgry;
		}else{
			currFixEyePosX = g_pEyeTrackingDlg->avglx;
			currFixEyePosY = g_pEyeTrackingDlg->avgly;
		}
		if (m_mode == MODE_TEST){
			targetAcquired = true;
		}else if((abs(currFixEyePosX-m_stimXDeg)<=m_windowAllowancePix)&&(abs(currFixEyePosY-m_stimYDeg)<=m_windowAllowancePix)){
			targetAcquired =true;
		}else{
			targetAcquired =false;
			m_timer.CheckTimerWithReset();
		}
	}

	//Change to next position depending on mode
	if (m_mode ==MODE_FOVEA){
		if(m_currEye == LEVEL_SECONDARY){
			m_currEye =LEVEL_PRIMARY;
			g_pEyeTrackingDlg->EyePosSample();
			g_pReverseFixationDlg->UpdateEyePositions(
				g_pEyeTrackingDlg->avglx, g_pEyeTrackingDlg->avgly,
				g_pEyeTrackingDlg->avgrx, g_pEyeTrackingDlg->avgry);
			m_stimXDeg = g_pEyeTrackingDlg->avgrx;
			m_stimYDeg = g_pEyeTrackingDlg->avgry;
		}else{
			m_currEye =LEVEL_SECONDARY;
			g_pEyeTrackingDlg->EyePosSample();
			g_pReverseFixationDlg->UpdateEyePositions(
				g_pEyeTrackingDlg->avglx, g_pEyeTrackingDlg->avgly,
				g_pEyeTrackingDlg->avgrx, g_pEyeTrackingDlg->avgry);
			m_stimXDeg = g_pEyeTrackingDlg->avglx;
			m_stimYDeg = g_pEyeTrackingDlg->avgly;
		}
	}else if(m_mode ==MODE_INIT){
		if(m_initLevel==LEVEL_SECONDARY){
			if(m_currEye == LEVEL_SECONDARY){
				m_currEye =LEVEL_PRIMARY;
				g_pEyeTrackingDlg->EyePosSample();
				g_pReverseFixationDlg->UpdateEyePositions(
					g_pEyeTrackingDlg->avglx, g_pEyeTrackingDlg->avgly,
					g_pEyeTrackingDlg->avgrx, g_pEyeTrackingDlg->avgry);
				m_stimXDeg = g_pEyeTrackingDlg->avgrx;
				m_stimYDeg = g_pEyeTrackingDlg->avgry;
			}else{
				m_currEye = LEVEL_SECONDARY;
				m_stimXDeg = m_initLeftXDeg;
				m_stimYDeg = m_initLeftYDeg;
			}
		}else{//initLevel == PRIMARY
			if(m_currEye == LEVEL_PRIMARY){
				m_currEye =LEVEL_SECONDARY;
				g_pEyeTrackingDlg->EyePosSample();
				g_pReverseFixationDlg->UpdateEyePositions(
					g_pEyeTrackingDlg->avglx, g_pEyeTrackingDlg->avgly,
					g_pEyeTrackingDlg->avgrx, g_pEyeTrackingDlg->avgry);
				m_stimXDeg = g_pEyeTrackingDlg->avglx;
				m_stimYDeg = g_pEyeTrackingDlg->avgly;
			}else{
				m_currEye = LEVEL_PRIMARY;
				m_stimXDeg = m_initRightXDeg;
				m_stimYDeg = m_initRightYDeg;
			}
		}
	}else{//MODE_TEST
		if(m_currEye == LEVEL_SECONDARY){
			m_currEye = LEVEL_PRIMARY;
			m_stimXDeg = m_initRightXDeg;
			m_stimYDeg = m_initRightYDeg;
		}else{//m_currEye ==LEVEL_PRIMARY
			m_currEye = LEVEL_SECONDARY;
			m_stimXDeg = m_initLeftXDeg;
			m_stimYDeg = m_initLeftYDeg;
			Sleep(5);
		}
	}
	DrawStimulus();
	vsgPresent();
	Sleep(1);
	return true;
}

/*******************************************************************
	StimulusType
		- Returns a string containing "Reverse Fixation Stimulus" to identify
		the current stimulus.

********************************************************************/
CString CReverseFixationStimulus::StimulusType() 
{
	return "Reverse Fixation Stimulus";
}
void CReverseFixationStimulus::DrawStimulus() 
{	char bf[1024];
	m_pageNum = 1 - m_pageNum;	
	double xPix, yPix;
	int color;
	//Add Stimulus Offsets from settings
	m_stimXDeg = m_stimXDeg+g_pSettings->getXOffset();
	m_stimYDeg = m_stimYDeg+g_pSettings->getYOffset();
	/* Add offsets, take starting point as cross (not necessarily 0,0) and flip
	 * the Y coordinate. */
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, m_screenOffsetXDeg + m_stimXDeg, 
		vsgPIXELUNIT, &xPix);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -(m_screenOffsetYDeg + m_stimYDeg), 
		vsgPIXELUNIT, &yPix);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pReverseFixation->getDotSize(), 
		vsgPIXELUNIT, &m_stimSizePix);
	//m_currEye =0 primary, 1 secondary
	if (m_currEye ==LEVEL_PRIMARY){
		color = VOLTS_COLOR_RED;
	}else{
		color = VOLTS_COLOR_BLUE;
	}

	vsgSetDrawPage(vsgVIDEOPAGE, m_pageNum, vsgBACKGROUND);
	vsgSetPen1(m_currEye);
	vsgDrawOval(xPix, yPix, m_stimSizePix * 2, m_stimSizePix * 2);

	/* Reported location only reflects the offset. It ignores the screen offset 
	 * as per usual. It is interpreted offline as being relative to the cross 
	 * location so that loc isn't added either. */
	etu::AnalogOut(A_OUT_STIM_HORIZ, ((float)m_stimXDeg) / g_pSettings->getDegPerVolt());
	etu::AnalogOut(A_OUT_STIM_VERT, ((float)m_stimYDeg) / g_pSettings->getDegPerVolt());
	etu::AnalogOut(A_OUT_STIM_COLOR, color);

}
/** Stop. */
void CReverseFixationStimulus::StopExecution() 
{
	bEndThread = true;
}