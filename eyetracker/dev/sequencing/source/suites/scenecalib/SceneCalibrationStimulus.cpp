/**
 * @file SceneCalibrationStimulus.cpp
 * @brief 
 */

#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "app/AppCore.h"
#include "utils/Utils.h"

/***********************************************************
COMMUNICATIONS:
	Analog Out 0:	Horizontal target location
	Analog Out 1:	Vertical target location
************************************************************/

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

CSceneCalibrationStimulus::CSceneCalibrationStimulus() 
{
	Initialize();
	iScrWidthPix = vsgGetScreenWidthPixels();
	iScrHeightPix = vsgGetScreenHeightPixels();
	iPage = 0;
	vsgMoveScreen(0,0);
	bDrawn=false;
}

CSceneCalibrationStimulus::~CSceneCalibrationStimulus() 
{
	char buffer[256];
	char buffer2[256];
	sprintf(buffer, "%s", g_pSceneCalibration->getImagePath());
	sprintf(buffer2, "%s|0x0%x/", strrchr(buffer, '\\')+1, 0x01);

	g_pSettings->theSerialComm.transmit(buffer2, (int)strlen(buffer2));
	
	vsgSetDrawPage(vsgVIDEOPAGE,iPage, 0);
	vsgPresent();

	vsgSetCommand(vsgPALETTECLEAR);
	VSGLUTBUFFER lutbuf;
	for (int i=0; i<256; i++) {
		lutbuf[i].a = ((double)g_pSettings->getRedBG()) / 255.0f;
		lutbuf[i].b = ((double)g_pSettings->getGreenBG()) / 255.0f;
		lutbuf[i].c = ((double)g_pSettings->getBlueBG()) / 255.0f;
	}
	vsgSetBackgroundColour(&lutbuf[vsgBACKGROUND]);
	vsgSetFixationColour(&lutbuf[vsgFIXATION]);
	vsgLUTBUFFERWrite(0, &lutbuf);
	vsgLUTBUFFERtoPalette(0);
}

bool CSceneCalibrationStimulus::DisplayStimulus() 
{
	double dsize=0, screenXOffset=0, screenYOffset=0, stimXOffset=0, stimYOffset=0, xpos=0, ypos=0;
	
	if (!bDrawn) {
		//Clear the palette so none of the drawing will be seen.
		vsgSetCommand(vsgPALETTECLEAR);

		//set up palette to draw dot
		VSGLUTBUFFER lutbuf;
		for (int i=0; i<256; i++) {
			lutbuf[i].a = ((double)g_pSettings->getRedBG()) / 255.0f;
			lutbuf[i].b = ((double)g_pSettings->getGreenBG()) / 255.0f;
			lutbuf[i].c = ((double)g_pSettings->getBlueBG()) / 255.0f;
		}
		lutbuf[1].a = ((double)g_pSettings->getRed()) / 255.0f;
		lutbuf[1].b = ((double)g_pSettings->getGreen()) / 255.0f;
		lutbuf[1].c = ((double)g_pSettings->getBlue()) / 255.0f;
		vsgSetBackgroundColour(&lutbuf[vsgBACKGROUND]);
		vsgSetFixationColour(&lutbuf[vsgFIXATION]);
		vsgLUTBUFFERWrite(0, &lutbuf);
		vsgLUTBUFFERtoPalette(0);

		//draw dot until image appears
		vsgSetDrawPage(vsgVIDEOPAGE,iPage, 0);
		vsgSetPen1(1);
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pCalibration->getSize(), vsgPIXELUNIT, &dSize);
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getScreenXOffset(), vsgPIXELUNIT, &screenXOffset);
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pSettings->getScreenYOffset(), vsgPIXELUNIT, &screenYOffset);
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSettings->getXOffset(), vsgPIXELUNIT, &stimXOffset);
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pSettings->getYOffset(), vsgPIXELUNIT, &stimYOffset);
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSceneCalibration->getXPosition(), vsgPIXELUNIT, &xpos);
		g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -g_pSceneCalibration->getYPosition(), vsgPIXELUNIT, &ypos);
		//actually draw the dot
		vsgSetDrawMode(vsgCENTREXY);
		vsgSetDrawOrigin(iScrWidthPix / 2, iScrHeightPix / 2);
		vsgDrawOval(xpos + screenXOffset + stimXOffset, ypos + screenYOffset + stimYOffset, dSize, dSize);

		vsgPresent();
		iPage=1-iPage;
		WaitUntil(500);

		vsgSetDrawPage(vsgVIDEOPAGE,iPage, 0);
		vsgSetDrawOrigin(0, 0);

		char buffer[1024];
		sprintf(buffer, "%s", g_pSceneCalibration->getImagePath());
		
		VSGLUTBUFFER Buff;
		vsgImageGetPalette(0, buffer, (VSGLUTBUFFER*)&Buff);
		if (g_pSceneCalibrationDlg->m_SwapRedBlue.GetCheck()==BST_CHECKED) {
			//swap red and blue channels
			for (int i=0; i<256; i++) {
				double dtemp = Buff[i].a;
				Buff[i].a = Buff[i].c;
				Buff[i].c = dtemp;
			}
		}
		vsgSetBackgroundColour(&Buff[vsgBACKGROUND]);
		vsgSetFixationColour(&Buff[vsgFIXATION]);
		vsgLUTBUFFERWrite(0, (VSGLUTBUFFER*)&Buff);
		vsgLUTBUFFERtoPalette(0);

		vsgDrawImage(0, vsgGetScreenWidthPixels()/2 + screenXOffset + stimXOffset, 
			vsgGetScreenHeightPixels()/2 + screenYOffset + stimYOffset, 
			buffer);

		char buffer2[256];
		sprintf(buffer2, "%s|0x0%x/", strrchr(buffer, '\\')+1, 0x01);

		g_pSettings->theSerialComm.transmit(buffer2, (int)strlen(buffer2));

		vsgPresent();

		bDrawn = true;
		
		etu::AnalogOut(0, (g_pSceneCalibration->getXPosition() + g_pSettings->getXOffset()) / g_pSettings->getDegPerVolt());
		etu::AnalogOut(1, (g_pSceneCalibration->getYPosition() + g_pSettings->getYOffset()) / g_pSettings->getDegPerVolt());
		
		iPage = 1 - iPage;
	}

	WaitUntil(100);
	return true;
}

void CSceneCalibrationStimulus::setDrawingState(bool b) 
{
	bDrawn = b;
}

CString CSceneCalibrationStimulus::StimulusType() 
{
	return "Scene Calibration Stimulus";
}

void CSceneCalibrationStimulus::StopExecution() 
{
	// stop calibration thread
	bEndThread = true;
}

#endif // APP_EYE_TRACKING
