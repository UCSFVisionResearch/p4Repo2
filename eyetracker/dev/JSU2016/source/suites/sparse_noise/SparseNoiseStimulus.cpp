#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/
#include <algorithm>
#include <vector>
#include "numeric"
#include "app/AppCore.h"
#include "SparseNoiseStimulus.h"
#include "app/EyeTrackingDlg.h"
#include "utils/Utils.h"
#include "utils/Grid.h"
#include "vsgext/vsgext.h"


/*****************************************************************************
 * COMMUNICATIONS:
 *****************************************************************************/

#define A_OUT_CTR_TARG_HORIZ  (0) /* 1401 in channel 3 */
#define A_OUT_CTR_TARG_VERT   (1) /* 1401 in channel 4 */
#define A_OUT_FRAME_SYNC  (2) /* 1401 in channel 7 */
#define A_OUT_CTR_TARG_COLOR  (4) /* VOLTS_COLOR_xxx */


#define LEVEL_PRIMARY (0)
#define LEVEL_SECONDARY (1)
#define LEVEL_TERTIARY (2)
#define LEVEL_CROSS                    (203)
#define VOLTS_EYE_RIGHT     (7.5) /*To just test correct center eye. Usual is 5V*/
#define VOLTS_EYE_LEFT  (-7.5)

#define SYNC_LEVEL_HIGH (5)
#define SYNC_LEVEL_LOW (0)

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

/**
 @brief Initializes the newly instantiated object. 
 */
CSparseNoiseStimulus::CSparseNoiseStimulus() 
:	m_screenWidthPix(vsgGetScreenWidthPixels()), 
	m_screenHeightPix(vsgGetScreenHeightPixels()),
	m_crossXDeg(g_pSparseNoise->getCrossX()),
	m_crossYDeg(g_pSparseNoise->getCrossY()),
	m_crossXPix(0),
	m_crossYPix(0),
	m_crossDiameterPix(0),
	m_crossThicknessPix(g_pSparseNoise->getCrossThickness()),
	m_pageNum(0),
	m_displayTimeMs(g_pSparseNoise->getDisplayTimeMs()),
	m_useBinocular(g_pSparseNoise->getUseBinocular()),
	m_shuttered(false),
	m_crossX2Deg(g_pSparseNoise->getCrossX2()),
	m_crossY2Deg(g_pSparseNoise->getCrossY2()),
	m_screenOffsetXDeg(g_pSettings->getScreenXOffset()),
	m_screenOffsetYDeg(g_pSettings->getScreenYOffset()),
	m_messageSent(false),
	m_pStimLocationGenerator(NULL),
	m_numFrames(g_pSparseNoise->getNumFrames()),
	m_stimWidth(g_pSparseNoise->getStimWidth()),
	m_minDist(g_pSparseNoise->getMinDist()),
	m_primaryCount(g_pSparseNoise->getPrimaryCount()),
	m_secondaryCount(g_pSparseNoise->getSecondaryCount()),
	m_tertiaryCount(g_pSparseNoise->getTertiaryCount()),
	m_swapCounter(0),m_swapInt(g_pSparseNoise->getSwapInt()),
	targetAcquired(true),m_useWait(g_pSparseNoise->getUseWait()),
	m_windowAllowancePix(g_pSparseNoise->getWindowAllowancePix()),
	frameSyncHigh(false),newPt(0,0,0,0),addPt(0,0,0,0),
	m_useGrid(g_pSparseNoise->getUseGrid()),m_fixOffsetPt(0,0,0,0),m_fixCounter(0)
{
	double xxDeg, yyDeg;
	double frameTime;
	char m_priFrameFilePath[256];
	char m_secFrameFilePath[256];
	char m_tertFrameFilePath[256];
	char strPrimaryPts[1024];
	char strSecondaryPts[1024];
	char strTertiaryPts[1024];
	VSGLUTBUFFER lutbuf;
	
	char strBuf[10]; //Use to debug, short term
	fPixelSize = 141.0;
	psuedoPixPerDeg=(floor(tan( PI_DOUBLE / 180.0) * vsgGetViewDistMM()/(fPixelSize / 100.0)));
	
	bEndThread = false;

	if (!g_bIsInitialized) {
		if (vsgInit("") < 0) return;
		g_bIsInitialized = true;
		/* Set page width to 2048; display error if vsg.cfg isn't sync'ed on
		 * green. */
		vsgSetVideoMode(vsgPANSCROLLMODE);
	}
	m_bgColor.a =((double)g_pSparseNoise->getBGRed())/255.0f;
	m_bgColor.b = ((double)g_pSparseNoise->getBGGreen())/255.0f;
	m_bgColor.c =((double)g_pSparseNoise->getBGBlue())/255.0f;

	m_primaryColor.a = ((double)g_pSparseNoise->getPrimaryRed())/255.0f;
	m_primaryColor.b =((double)g_pSparseNoise->getPrimaryGreen())/255.0f;
	m_primaryColor.c = ((double)g_pSparseNoise->getPrimaryBlue())/255.0f;

	m_secondaryColor.a = ((double)g_pSparseNoise->getSecondaryRed())/255.0f;
	m_secondaryColor.b = ((double)g_pSparseNoise->getSecondaryGreen())/255.0f;
	m_secondaryColor.c = ((double)g_pSparseNoise->getSecondaryBlue())/255.0f;

	m_tertiaryColor.a = ((double)g_pSparseNoise->getTertiaryRed())/255.0f;
	m_tertiaryColor.b = ((double)g_pSparseNoise->getTertiaryGreen())/255.0f;
	m_tertiaryColor.c = ((double)g_pSparseNoise->getTertiaryBlue())/255.0f;

	m_crossColor.a = ((double)g_pSettings->getRed())/255.0f;
	m_crossColor.b = ((double)g_pSettings->getGreen())/255.0f;
	m_crossColor.c = ((double)g_pSettings->getBlue())/255.0f;


	memset(lutbuf, 0, 256 * sizeof(VSGTRIVAL));
	lutbuf[LEVEL_PRIMARY] = m_primaryColor;
	lutbuf[LEVEL_SECONDARY] = m_secondaryColor;
	lutbuf[LEVEL_TERTIARY] = m_tertiaryColor;
	lutbuf[LEVEL_CROSS] = m_crossColor;
	lutbuf[vsgBACKGROUND] = m_bgColor;
		
	frameTime = vsgGetSystemAttribute(vsgFRAMETIME);
	m_frameRate = 1000000.0 / frameTime;
	g_dFramerate = m_frameRate;
	g_dHalfFrameInterval = DWORD(floor(frameTime / 2000.0f));
	g_dSleepThreshold = (double)g_dHalfFrameInterval / 2.0f;

	vsgSetCommand(vsgPALETTECLEAR + vsgOVERLAYDRIFT + vsgVIDEODRIFT);
	vsgSetViewDistMM(g_pSettings->getViewDistance());
	vsgSetSpatialUnits(vsgPIXELUNIT);
	vsgSetFixationColour(&m_primaryColor);
	vsgSetBackgroundColour(&m_bgColor);
	vsgSetPageWidth(2048); /* In case we didn't call vsgSetVideoMode. */
	vsgSetDrawMode(vsgCENTREXY);
	vsgSetDrawOrigin(m_screenWidthPix / 2, m_screenHeightPix / 2);
	vsgLUTBUFFERWrite(0, &lutbuf);
	vsgLUTBUFFERtoPalette(0);
	//Mode Calibration
	vsgIOWriteDigitalOut(0xFFFF, vsgDIG0);
	vsgIOWriteDigitalOut(0x0000, vsgDIG2);
	//Reset in case coming from behavior contingent suite
	vsgIOWriteDigitalOut(0xFFFF, vsgDIG1);
	Sleep(100);
	vsgIOWriteDigitalOut(0x0000, vsgDIG1);

	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSparseNoise->getStimWidth(), 
					vsgPIXELUNIT, &widthPix);

	/* Set up cross coordinates for each eye. */
	xxDeg = m_crossXDeg;
	yyDeg = m_crossYDeg;

	/* Analog outs 0 and 1 match with the calibration stimulus. */
	etu::AnalogOut(A_OUT_CTR_TARG_HORIZ, (float)(xxDeg / g_pSettings->getDegPerVolt()));
	etu::AnalogOut(A_OUT_CTR_TARG_VERT, (float)(yyDeg / g_pSettings->getDegPerVolt()));

	m_fixEye = EYE_RIGHT;
	m_fixX = m_crossXDeg;
	m_fixY =m_crossYDeg;

	xxDeg += m_screenOffsetXDeg;
	yyDeg += m_screenOffsetYDeg;

	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, xxDeg, vsgPIXELUNIT, &m_crossXPix);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -1 * yyDeg, vsgPIXELUNIT, &m_crossYPix);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pSparseNoise->getCrossDiameter(), 
		vsgPIXELUNIT, &m_crossDiameterPix);
	
	xxDeg = m_crossX2Deg;
	yyDeg = m_crossY2Deg;

	xxDeg += m_screenOffsetXDeg;
	yyDeg += m_screenOffsetYDeg;

	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, xxDeg, vsgPIXELUNIT, &m_crossX2Pix);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, -1 * yyDeg, vsgPIXELUNIT, &m_crossY2Pix);
	

	if (g_pSparseNoise->getCreateFrames()){
		//Check toSee if PrimaryFramePts already exists (ie. frames had been created)
		sprintf(m_fileName,"C:\\SparseNoise\\%s\\PrimaryFramePts.txt",g_pSparseNoise->getFolderName());
		FILE* fSettings =fopen(m_fileName,"r");
		if(fSettings){
			AfxMessageBox("Folder Already Exists");
			bEndThread = true;
			g_pSparseNoise->setCreateFrames(false);
			fclose(fSettings);
		}
		
	}else{
		m_frameCounter =0;
		std::srand( g_pSparseNoise->getSeed());
		m_pStimLocationGenerator = new StimLocationGenerator;
		newPt = m_pStimLocationGenerator->next();
		if(m_useGrid){
			m_fixOffsetPt = m_pStimLocationGenerator->getFixationLocation();
			m_crossXDeg = g_pSparseNoise->getCrossX()+m_fixOffsetPt.xDeg;
			m_crossYDeg = g_pSparseNoise->getCrossY()+m_fixOffsetPt.yDeg;
			m_crossX2Deg = g_pSparseNoise->getCrossX2()+m_fixOffsetPt.xDeg;
			m_crossY2Deg = g_pSparseNoise->getCrossY2()+m_fixOffsetPt.yDeg;
			g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT,m_crossXDeg+m_screenOffsetXDeg,vsgPIXELUNIT,&m_crossXPix);
			g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT,-1*(m_crossYDeg+m_screenOffsetYDeg),vsgPIXELUNIT,&m_crossYPix);
			g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT,m_crossX2Deg+m_screenOffsetXDeg,vsgPIXELUNIT,&m_crossX2Pix);
			g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT,-1*(m_crossY2Deg+m_screenOffsetYDeg),vsgPIXELUNIT,&m_crossY2Pix);

		}
		
		//initialization for files to write frames to
		char m_primaryFrames[256];
		char m_secondaryFrames[256];
		char m_tertiaryFrames[256];
		char strPrimaryPts[1024];
		char strSecondaryPts[1024];
		char strTertiaryPts[1024];
		

		sprintf(m_folderName,"C:\\SparseNoise\\%s",g_pSparseNoise->getFolderName());
		
		time_t t = time(0); 
		struct tm * now = localtime( & t );
		sprintf(m_priFrameFilePath,"%s\\PrimaryFramePts_%d%d%d_%d%d.txt",m_folderName, now->tm_mon+1,now->tm_mday,now->tm_year+1900,now->tm_hour,now->tm_min);
		sprintf(m_secFrameFilePath,"%s\\SecondaryFramePts_%d%d%d_%d%d.txt",m_folderName, now->tm_mon+1,now->tm_mday,now->tm_year+1900,now->tm_hour,now->tm_min);
		sprintf(m_tertFrameFilePath,"%s\\TertiaryFramePts_%d%d%d_%d%d.txt",m_folderName, now->tm_mon+1,now->tm_mday,now->tm_year+1900,now->tm_hour,now->tm_min);
		primaryFile =fopen(m_priFrameFilePath,"w");
		secondaryFile =fopen(m_secFrameFilePath,"w");
		tertiaryFile =fopen(m_tertFrameFilePath,"w");

		m_sequenceTimer.StartTimer();

		//Show first frame
		vsgSetDrawPage(vsgVIDEOPAGE, m_pageNum, vsgBACKGROUND);
		VsgExt::drawCross((int)m_crossXPix, (int)m_crossYPix, 
			(int)m_crossDiameterPix, (int)m_crossThicknessPix, LEVEL_CROSS);
		etu::AnalogOut(A_OUT_CTR_TARG_HORIZ, (float)(m_crossXDeg/g_pSettings->getDegPerVolt()));
		etu::AnalogOut(A_OUT_CTR_TARG_VERT, (float)(m_crossYDeg/g_pSettings->getDegPerVolt()));
		vsgPresent();
	}
}

/** Trial running signal turned off and tear down. */
CSparseNoiseStimulus::~CSparseNoiseStimulus() 
{
	vsgIOWriteDigitalOut(0x0000, vsgDIG0);
	etu::AnalogOut(A_OUT_CTR_TARG_COLOR, (float)0.0, true);
	delete m_pStimLocationGenerator;
	primaryFramePts.clear();
	secondaryFramePts.clear();
	tertiaryFramePts.clear();
	/* Send stop mark */
	char buffer[80];
	sprintf(buffer, "SparseNoise;Stop;%d;%ld|0x%02x/\0", 0, time(NULL), 200);
	g_pSettings->theSerialComm.transmit(buffer, (int)strlen(buffer));
}

/** Coordinate stimulus display w/pulse. */
bool CSparseNoiseStimulus::DisplayStimulus() 
{
	//Moving create stimuli inside Display stimulus allows for Stopping 
	//in the middle of frame generation
	if (g_pSparseNoise->getCreateFrames()){
		CreateStimuli();
		bEndThread = true;
	}else{
		double xPix, yPix;
		char strBuf[100];
		double currFixEyePosX, currFixEyePosY;
		
		if (!m_messageSent &&!bEndThread) {
			g_pSparseNoise->sendSettings(g_pSettings->theSerialComm);
			m_messageSent = true;
		}
			//If using Wait, Check if on Target if using Grid and Wait
			
		if (m_useWait &&!bEndThread ) {
			g_pEyeTrackingDlg->EyePosSample();
			g_pSparseNoiseDlg->UpdateEyePositions(
				g_pEyeTrackingDlg->avglx, g_pEyeTrackingDlg->avgly,
				g_pEyeTrackingDlg->avgrx, g_pEyeTrackingDlg->avgry);
			if(m_fixEye ==EYE_RIGHT){
				currFixEyePosX = g_pEyeTrackingDlg->avgrx;
				currFixEyePosY = g_pEyeTrackingDlg->avgry;
				m_fixX = m_crossXDeg;
				m_fixY = m_crossYDeg;
			}else{
				currFixEyePosX = g_pEyeTrackingDlg->avglx;
				currFixEyePosY = g_pEyeTrackingDlg->avgly;
				m_fixX = m_crossX2Deg;
				m_fixY = m_crossY2Deg;
			}

			if((abs(currFixEyePosX-m_fixX)<=m_windowAllowancePix)&(abs(currFixEyePosY-m_fixY)<=m_windowAllowancePix)){
				targetAcquired =true;
			}else{
				targetAcquired =false;
			}
		}
		//Stay in loop until target Acquired
		while(!bEndThread && m_useWait && !targetAcquired){
			g_pEyeTrackingDlg->EyePosSample();
			g_pSparseNoiseDlg->UpdateEyePositions(
				g_pEyeTrackingDlg->avglx, g_pEyeTrackingDlg->avgly,
				g_pEyeTrackingDlg->avgrx, g_pEyeTrackingDlg->avgry);
			if(m_fixEye ==EYE_RIGHT){
				currFixEyePosX = g_pEyeTrackingDlg->avgrx;
				currFixEyePosY = g_pEyeTrackingDlg->avgry;
			}else{
				currFixEyePosX = g_pEyeTrackingDlg->avglx;
				currFixEyePosY = g_pEyeTrackingDlg->avgly;
			}		
			if((abs(currFixEyePosX-m_fixX)<=m_windowAllowancePix)&&(abs(currFixEyePosY-m_fixY)<=m_windowAllowancePix)){
				targetAcquired =true;
			}else{
				targetAcquired =false;
			}
		}


		/* Move to the new frame */
		if (m_sequenceTimer.CheckTimer() > m_displayTimeMs && (targetAcquired)&&!bEndThread)
		{
			PrepNextFrame();
			NewDrawingPage();
			m_frameCounter+=1;
			//Draw Elements from text. Does not need conditioning since saved as drawn on vsg.
			vsgSetPen1(LEVEL_PRIMARY);
			for (int j =0;j<m_primaryCount;j++){
				addPt = primaryFramePts.at(j);
				xPix =addPt.xDeg* psuedoPixPerDeg;
				yPix =addPt.yDeg* psuedoPixPerDeg;
				vsgDrawBar(xPix, yPix, widthPix, widthPix, 0);
			}

			// Secondary Blocks
			if (m_secondaryCount>0){
				vsgSetPen1(LEVEL_SECONDARY);
				for (int k =0;k<m_secondaryCount;k++){
					addPt = secondaryFramePts.at(k);
					xPix =addPt.xDeg* psuedoPixPerDeg;
					yPix =addPt.yDeg* psuedoPixPerDeg;
					vsgDrawBar(xPix, yPix, widthPix, widthPix, 0);

				}
			}

			// Tertiary Blocks
			if (m_tertiaryCount>0){
				vsgSetPen1(LEVEL_TERTIARY);
				for (int k =0;k<m_tertiaryCount;k++){
					addPt = tertiaryFramePts.at(k);
					xPix =addPt.xDeg* psuedoPixPerDeg;
					yPix =addPt.yDeg* psuedoPixPerDeg;
					vsgDrawBar(xPix, yPix, widthPix, widthPix, 0);

				}
			}
			//Loop to iterate over fixation Points first 
			if(m_useGrid){
				if ( m_swapCounter % m_swapInt == m_swapInt-1){
					m_fixCounter++;
					m_swapCounter = m_swapCounter-(m_swapInt-1);
					m_fixOffsetPt = m_pStimLocationGenerator->getFixationLocation();
					m_crossXDeg = g_pSparseNoise->getCrossX()+m_fixOffsetPt.xDeg;
					m_crossYDeg = g_pSparseNoise->getCrossY()+m_fixOffsetPt.yDeg;
					m_crossX2Deg = g_pSparseNoise->getCrossX2()+m_fixOffsetPt.xDeg;
					m_crossY2Deg = g_pSparseNoise->getCrossY2()+m_fixOffsetPt.yDeg;
					g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT,m_crossXDeg+m_screenOffsetXDeg,vsgPIXELUNIT,&m_crossXPix);
					g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT,-1*(m_crossYDeg+m_screenOffsetYDeg),vsgPIXELUNIT,&m_crossYPix);
					g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT,m_crossX2Deg+m_screenOffsetXDeg,vsgPIXELUNIT,&m_crossX2Pix);
					g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT,-1*(m_crossY2Deg+m_screenOffsetYDeg),vsgPIXELUNIT,&m_crossY2Pix);
					if (m_fixEye == EYE_RIGHT){
						etu::AnalogOut(A_OUT_CTR_TARG_HORIZ, (float)(m_crossXDeg/g_pSettings->getDegPerVolt()));
						etu::AnalogOut(A_OUT_CTR_TARG_VERT, (float)(m_crossYDeg/g_pSettings->getDegPerVolt()));
					}else{
						etu::AnalogOut(A_OUT_CTR_TARG_HORIZ, (float)(m_crossX2Deg/g_pSettings->getDegPerVolt()));
						etu::AnalogOut(A_OUT_CTR_TARG_VERT, (float)(m_crossY2Deg/g_pSettings->getDegPerVolt()));
						
					}
				} //after going through fix Pts -1, continue on 
				if (m_fixCounter ==((2*g_pSparseNoise->getStepsX()+1)*(2*g_pSparseNoise->getStepsY())+1)+1){
					m_fixCounter =0;
					m_swapCounter +=(m_swapInt);
				}
				
			}
			//Draw Crosshair
			if(!g_pSparseNoise->getUseBinocular()){
				if(!g_pSparseNoise->getUseShutters()){
					if(m_swapCounter<m_swapInt){
						VsgExt::drawCross(m_crossXPix, m_crossYPix, (int)m_crossDiameterPix, (int)m_crossThicknessPix, LEVEL_CROSS);
						m_swapCounter++;
					}else if(m_swapCounter==m_swapInt){
						VsgExt::drawCross(m_crossX2Pix, m_crossY2Pix, (int)m_crossDiameterPix, (int)m_crossThicknessPix, LEVEL_CROSS);
						m_swapCounter++;
						etu::AnalogOut(A_OUT_CTR_TARG_HORIZ, (float)((m_crossX2Deg)/ g_pSettings->getDegPerVolt()));
						etu::AnalogOut(A_OUT_CTR_TARG_VERT, (float)((m_crossY2Deg) / g_pSettings->getDegPerVolt()));						
						m_fixEye = EYE_LEFT;
					}else if(m_swapCounter<2*m_swapInt){
						VsgExt::drawCross(m_crossX2Pix, m_crossY2Pix, (int)m_crossDiameterPix, (int)m_crossThicknessPix, LEVEL_CROSS);
						m_swapCounter++;
					}else{
						VsgExt::drawCross(m_crossXPix, m_crossYPix, (int)m_crossDiameterPix, (int)m_crossThicknessPix, LEVEL_CROSS);
						etu::AnalogOut(A_OUT_CTR_TARG_HORIZ, (float)(m_crossXDeg/g_pSettings->getDegPerVolt()));
						etu::AnalogOut(A_OUT_CTR_TARG_VERT, (float)(m_crossYDeg/g_pSettings->getDegPerVolt()));
						m_fixEye = EYE_RIGHT;
						m_swapCounter=0;
					}
				}else{
					if(m_swapCounter<m_swapInt){
						VsgExt::drawCross(m_crossXPix, m_crossYPix, (int)m_crossDiameterPix, (int)m_crossThicknessPix, LEVEL_CROSS);
						m_swapCounter++;
						g_pEyeTrackingDlg->OpenLeftShutter(true);
						g_pEyeTrackingDlg->OpenRightShutter(true);
					}else if(m_swapCounter==m_swapInt){
						VsgExt::drawCross(m_crossXPix, m_crossYPix, (int)m_crossDiameterPix, (int)m_crossThicknessPix, LEVEL_CROSS);
						m_swapCounter++;
						g_pEyeTrackingDlg->OpenLeftShutter(false);
						g_pEyeTrackingDlg->OpenRightShutter(true);
					}else if(m_swapCounter<2*m_swapInt){
						VsgExt::drawCross(m_crossXPix, m_crossYPix, (int)m_crossDiameterPix, (int)m_crossThicknessPix, LEVEL_CROSS);
						m_swapCounter++;
					}else if(m_swapCounter==2*m_swapInt){
						VsgExt::drawCross(m_crossX2Pix, m_crossY2Pix, (int)m_crossDiameterPix, (int)m_crossThicknessPix, LEVEL_CROSS);
						m_swapCounter++;
						etu::AnalogOut(A_OUT_CTR_TARG_HORIZ, (float)((m_crossX2Deg)/ g_pSettings->getDegPerVolt()));
						etu::AnalogOut(A_OUT_CTR_TARG_VERT, (float)((m_crossY2Deg) / g_pSettings->getDegPerVolt()));						
						m_fixEye = EYE_LEFT;
						g_pEyeTrackingDlg->OpenLeftShutter(true);
						g_pEyeTrackingDlg->OpenRightShutter(true);
					}else if(m_swapCounter<3*m_swapInt){
						VsgExt::drawCross(m_crossX2Pix, m_crossY2Pix, (int)m_crossDiameterPix, (int)m_crossThicknessPix, LEVEL_CROSS);
						m_swapCounter++;
					}else if(m_swapCounter==3*m_swapInt){
						VsgExt::drawCross(m_crossX2Pix, m_crossY2Pix, (int)m_crossDiameterPix, (int)m_crossThicknessPix, LEVEL_CROSS);
						m_swapCounter++;
						g_pEyeTrackingDlg->OpenLeftShutter(true);
						g_pEyeTrackingDlg->OpenRightShutter(false);
					}else if(m_swapCounter<4*m_swapInt){
						VsgExt::drawCross(m_crossX2Pix, m_crossY2Pix, (int)m_crossDiameterPix, (int)m_crossThicknessPix, LEVEL_CROSS);
						m_swapCounter++;
					}else{
						VsgExt::drawCross(m_crossXPix, m_crossYPix, (int)m_crossDiameterPix, (int)m_crossThicknessPix, LEVEL_CROSS);
						etu::AnalogOut(A_OUT_CTR_TARG_HORIZ, (float)(m_crossXDeg/g_pSettings->getDegPerVolt()));
						etu::AnalogOut(A_OUT_CTR_TARG_VERT, (float)(m_crossYDeg/g_pSettings->getDegPerVolt()));
						m_fixEye = EYE_RIGHT;
						m_swapCounter=0;
						g_pEyeTrackingDlg->OpenLeftShutter(true);
						g_pEyeTrackingDlg->OpenRightShutter(true);
					}
				}
			}else{
				if(!g_pSparseNoise->getUseShutters()){
					if(m_swapCounter<m_swapInt){
						VsgExt::drawCross(m_crossXPix, m_crossYPix, (int)m_crossDiameterPix, (int)m_crossThicknessPix, LEVEL_PRIMARY);
						m_swapCounter++;
					}else if(m_swapCounter==m_swapInt){
						VsgExt::drawCross(m_crossX2Pix, m_crossY2Pix, (int)m_crossDiameterPix, (int)m_crossThicknessPix, LEVEL_SECONDARY);
						m_swapCounter++;
						etu::AnalogOut(A_OUT_CTR_TARG_HORIZ, (float)((m_crossX2Deg)/ g_pSettings->getDegPerVolt()));
						etu::AnalogOut(A_OUT_CTR_TARG_VERT, (float)((m_crossY2Deg) / g_pSettings->getDegPerVolt()));
						m_fixEye = EYE_LEFT;
					}else if(m_swapCounter<2*m_swapInt){
						VsgExt::drawCross(m_crossX2Pix, m_crossY2Pix, (int)m_crossDiameterPix, (int)m_crossThicknessPix, LEVEL_SECONDARY);
						m_swapCounter++;
					}else{
						VsgExt::drawCross(m_crossXPix, m_crossYPix, (int)m_crossDiameterPix, (int)m_crossThicknessPix, LEVEL_PRIMARY);
						etu::AnalogOut(A_OUT_CTR_TARG_HORIZ, (float)(m_crossXDeg/g_pSettings->getDegPerVolt()));
						etu::AnalogOut(A_OUT_CTR_TARG_VERT, (float)(m_crossYDeg/g_pSettings->getDegPerVolt()));
						m_fixEye = EYE_RIGHT;
						m_swapCounter=0;
					}
				}else{
					if(m_swapCounter<m_swapInt){
						VsgExt::drawCross(m_crossXPix, m_crossYPix, (int)m_crossDiameterPix, (int)m_crossThicknessPix, LEVEL_PRIMARY);
						g_pEyeTrackingDlg->OpenLeftShutter(true);
						g_pEyeTrackingDlg->OpenRightShutter(true);
						m_swapCounter++;
					}else if(m_swapCounter==m_swapInt){
						VsgExt::drawCross(m_crossXPix, m_crossYPix, (int)m_crossDiameterPix, (int)m_crossThicknessPix, LEVEL_PRIMARY);
						m_swapCounter++;
						g_pEyeTrackingDlg->OpenLeftShutter(false);
						g_pEyeTrackingDlg->OpenRightShutter(true);
					}else if(m_swapCounter<2*m_swapInt){
						VsgExt::drawCross(m_crossXPix, m_crossYPix, (int)m_crossDiameterPix, (int)m_crossThicknessPix, LEVEL_PRIMARY);
						m_swapCounter++;
					}else if(m_swapCounter==2*m_swapInt){
						VsgExt::drawCross(m_crossX2Pix, m_crossY2Pix, (int)m_crossDiameterPix, (int)m_crossThicknessPix, LEVEL_SECONDARY);
						m_swapCounter++;
						etu::AnalogOut(A_OUT_CTR_TARG_HORIZ, (float)((m_crossX2Deg)/ g_pSettings->getDegPerVolt()));
						etu::AnalogOut(A_OUT_CTR_TARG_VERT, (float)((m_crossY2Deg) / g_pSettings->getDegPerVolt()));						
						m_fixEye = EYE_LEFT;
						g_pEyeTrackingDlg->OpenLeftShutter(true);
						g_pEyeTrackingDlg->OpenRightShutter(true);
					}else if(m_swapCounter<3*m_swapInt){
						VsgExt::drawCross(m_crossX2Pix, m_crossY2Pix, (int)m_crossDiameterPix, (int)m_crossThicknessPix, LEVEL_SECONDARY);
						m_swapCounter++;
					}else if(m_swapCounter==3*m_swapInt){
						VsgExt::drawCross(m_crossX2Pix, m_crossY2Pix, (int)m_crossDiameterPix, (int)m_crossThicknessPix, LEVEL_SECONDARY);
						m_swapCounter++;
						g_pEyeTrackingDlg->OpenLeftShutter(true);
						g_pEyeTrackingDlg->OpenRightShutter(false);
					}else if(m_swapCounter<4*m_swapInt){
						VsgExt::drawCross(m_crossX2Pix, m_crossY2Pix, (int)m_crossDiameterPix, (int)m_crossThicknessPix, LEVEL_SECONDARY);
						m_swapCounter++;
					}else{
						VsgExt::drawCross(m_crossXPix, m_crossYPix, (int)m_crossDiameterPix, (int)m_crossThicknessPix, LEVEL_PRIMARY);
						etu::AnalogOut(A_OUT_CTR_TARG_HORIZ, (float)(m_crossXDeg/g_pSettings->getDegPerVolt()));
						etu::AnalogOut(A_OUT_CTR_TARG_VERT, (float)(m_crossYDeg/g_pSettings->getDegPerVolt()));
						m_fixEye = EYE_RIGHT;
						m_swapCounter=0;
						g_pEyeTrackingDlg->OpenLeftShutter(true);
						g_pEyeTrackingDlg->OpenRightShutter(true);
					}

				}
			}
			


			if (m_fixEye==EYE_RIGHT){
				etu::AnalogOut(A_OUT_CTR_TARG_COLOR, VOLTS_EYE_RIGHT);
			}else{
				etu::AnalogOut(A_OUT_CTR_TARG_COLOR, VOLTS_EYE_LEFT);
			}
			//g_pSparseNoiseDlg->printDebug(etu::FloatToString(m_fixCounter));			
			vsgPresent();
			//Sending framesync per change
			if ( frameSyncHigh){
				etu::AnalogOut(A_OUT_FRAME_SYNC, SYNC_LEVEL_HIGH);
			}else{
				etu::AnalogOut(A_OUT_FRAME_SYNC, SYNC_LEVEL_LOW);
			}
			frameSyncHigh=!frameSyncHigh;
			
			char buffer[256];
			const int code = 0x00; // 200
			
			/* time_t is 64 bits.  The %ld conversion expected long, which is only
			 * guaranteed to be 32 bits.  Cast to long so they match.  This will
			 * stop working truncate needed info in about 20 years. */
			long tm = (long)time(NULL);

			sprintf(buffer, "%05f|0x%x/\0",m_frameCounter, code);
			g_pSettings->theSerialComm.transmit(buffer, (int)strlen(buffer));

			g_pSparseNoiseDlg->printDebug(etu::FloatToString(m_frameCounter));
			m_sequenceTimer.CheckTimerWithReset();
		}

		if (bEndThread ){
			if(g_pSparseNoise->getCreateFrames()){//from CreateFrames run
				g_pSparseNoise->setCreateFrames(false);
			}else{//From Display run
				fclose(primaryFile);
				fclose(secondaryFile);
				fclose(tertiaryFile);
			}
		}
	}
	Sleep(1);
	return true;
}


/** Set the current drawing page to a new, blank page. */
void CSparseNoiseStimulus::NewDrawingPage()
{
	vsgSetDrawPage(vsgVIDEOPAGE, m_pageNum, vsgBACKGROUND);
	m_pageNum = 1 - m_pageNum;
}

/** Stop. */
void CSparseNoiseStimulus::StopExecution() 
{
	bEndThread = true;
}

/** Name. */
CString CSparseNoiseStimulus::StimulusType() 
{
	return "SparseNoiseStimulus";
}

/*************************************************************************/

CSparseNoiseStimulus::StimLocationGenerator::StimLocationGenerator()
: m_random(false)
{	float minDistCenter = 1.5*g_pSparseNoise->getMinDist()+g_pSparseNoise->getStimWidth();
	m_pGrid = etu::createCartesianGrid(floor(90/minDistCenter), 
		floor(80/minDistCenter), minDistCenter, true);
	m_iter = m_pGrid->begin();
	m_end = m_pGrid->end();
	if (m_iter >= m_end) {
		AfxMessageBox("Failed assertion: No grid locations generated.");
	}
	else {
		std::random_shuffle(m_iter, m_end);
	}
	m_iter = m_pGrid->begin();
	m_end = m_pGrid->end();

	//Create Fixation Grid Locations if bUseGrid
	if(g_pSparseNoise->getUseGrid()){
		m_fixationGrid = etu::createCartesianGrid(g_pSparseNoise->getStepsX(),g_pSparseNoise->getStepsY(),g_pSparseNoise->getStepSizeDeg(), true);
		m_fixIter = m_fixationGrid->begin();
		m_fixEnd = m_fixationGrid->end();
		if (m_fixIter >= m_fixEnd) {
			AfxMessageBox("Failed assertion: No grid locations generated.");
		}
		else {
			std::random_shuffle(m_fixIter, m_fixEnd);
		}
	}
}

etu::SPoint CSparseNoiseStimulus::StimLocationGenerator::next()
{
	if (m_iter >= m_end) {
		m_iter = m_pGrid->begin();
		std::random_shuffle(m_iter, m_end);

		m_iter = m_pGrid->begin();
		m_end = m_pGrid->end();
	}
	return *m_iter++;
}

int CSparseNoiseStimulus::StimLocationGenerator::remainder()
{
	return std::distance(m_iter, m_end);
}
int CSparseNoiseStimulus::StimLocationGenerator::size()
{
	return std::distance(m_pGrid->begin(), m_end);
}
void CSparseNoiseStimulus::StimLocationGenerator::shuffle()
{
		m_iter = m_pGrid->begin();
		std::random_shuffle(m_iter, m_end);
		m_iter = m_pGrid->begin();
		m_end = m_pGrid->end();
}

etu::SPoint CSparseNoiseStimulus::StimLocationGenerator::getFixationLocation()
{
	//Continualling loops does not shuffle
	if (m_fixIter >= m_fixEnd) {
		m_fixIter = m_fixationGrid->begin();
	}
	return *m_fixIter++;
	
}
void CSparseNoiseStimulus::CreateStimuli(){
	char m_priFrameFilePath[256];
	char m_secFrameFilePath[256];
	char m_tertFrameFilePath[256];
	char strPrimaryPts[1024];
	char strSecondaryPts[1024];
	char strTertiaryPts[1024];
	int primaryPtr =0;
	int secondaryPtr =0;
	int tertiaryPtr =0;

	//Set seed
	std::srand( g_pSparseNoise->getSeed());
	
	g_pSparseNoiseDlg->printDebug("CreatingFrames");
	sprintf(m_folderName,"C:\\SparseNoise\\%s",g_pSparseNoise->getFolderName());
	m_pStimLocationGenerator = new CSparseNoiseStimulus::StimLocationGenerator;
	etu::SPoint newPt = m_pStimLocationGenerator->next();
	
	sprintf(m_priFrameFilePath,"%s\\PrimaryFramePts.txt",m_folderName);
	sprintf(m_secFrameFilePath,"%s\\SecondaryFramePts.txt",m_folderName);
	sprintf(m_tertFrameFilePath,"%s\\TertiaryFramePts.txt",m_folderName);
	//CreateDirectory(m_folderName,NULL);
	
	FILE* primaryFile =fopen(m_priFrameFilePath,"w");
	FILE* secondaryFile =fopen(m_secFrameFilePath,"w");
	FILE* tertiaryFile =fopen(m_tertFrameFilePath,"w");

	if((m_primaryCount>70)||(m_secondaryCount>70)||(m_tertiaryCount>70)){
		AfxMessageBox("Limit of 70 elements of each type");
		bEndThread = true;
		return;
	}
	if((m_primaryCount+m_secondaryCount+m_tertiaryCount)>m_pStimLocationGenerator->size()){
		AfxMessageBox("Too many elements for grid dimensions");
		bEndThread = true;
		return;
	}
	double xPix, yPix,shiftFrameX,shiftFrameY;
	double shiftX=0;
	double shiftY=0;
	for (int i =1;i<=m_numFrames;i++){
		primaryPtr = 0;
		secondaryPtr = 0;
		tertiaryPtr =0;
		sprintf(m_fileName,"%s\\%05d.bmp",m_folderName,i);
		if (m_pStimLocationGenerator->remainder()<(m_primaryCount+m_secondaryCount+m_tertiaryCount)){ //reshuffle if not enough points left for frame
			m_pStimLocationGenerator->shuffle();
		}
		NewDrawingPage();
		shiftFrameX = etu::RandChoice(0,1.5*m_minDist+m_stimWidth-1)-m_minDist-10;
		shiftFrameY = etu::RandChoice(0,1.5*m_minDist+m_stimWidth-1)-m_minDist-15;

		// Primary Blocks 
		vsgSetPen1(LEVEL_PRIMARY);
		for (int j =1;j<=m_primaryCount;j++){
			shiftX = etu::RandChoice(0,m_minDist/4); //Element Jitter
			shiftY = etu::RandChoice(0,m_minDist/4);
			primaryPtr += sprintf(strPrimaryPts+primaryPtr,"%.2f,%.2f;", (newPt.xDeg+shiftX+shiftFrameX), -(newPt.yDeg+shiftY+shiftFrameY));
			xPix =(newPt.xDeg +shiftX+shiftFrameX)* psuedoPixPerDeg;
			yPix =-(newPt.yDeg +shiftY+shiftFrameY)* psuedoPixPerDeg;
			vsgDrawBar(xPix, yPix, widthPix, widthPix, 0);
			newPt= m_pStimLocationGenerator->next();
			
		}
		primaryPtr += sprintf(strPrimaryPts+primaryPtr,"\n");
		fprintf(primaryFile, strPrimaryPts);
		// Secondary Blocks
		if (m_secondaryCount>0){
			vsgSetPen1(LEVEL_SECONDARY);
			for (int k =1;k<=m_secondaryCount;k++){
				shiftX = etu::RandChoice(0,m_minDist/4); //Element Jitter
				shiftY = etu::RandChoice(0,m_minDist/4);
				secondaryPtr += sprintf(strSecondaryPts+secondaryPtr,"%.2f,%.2f;", (newPt.xDeg+shiftX+shiftFrameX), -(newPt.yDeg+shiftY+shiftFrameY));
				xPix =(newPt.xDeg +shiftX+shiftFrameX)* psuedoPixPerDeg;
				yPix =-(newPt.yDeg+shiftY +shiftFrameY)* psuedoPixPerDeg;
				vsgDrawBar(xPix, yPix, widthPix, widthPix, 0);
				newPt= m_pStimLocationGenerator->next();
			}
		}
		secondaryPtr += sprintf(strSecondaryPts+secondaryPtr,"\n");
		fprintf(secondaryFile, strSecondaryPts);
		// Secondary Blocks
		if (m_tertiaryCount>0){
			vsgSetPen1(LEVEL_TERTIARY);
			for (int k =1;k<=m_tertiaryCount;k++){
				shiftX = etu::RandChoice(0,m_minDist/4); //Element Jitter
				shiftY = etu::RandChoice(0,m_minDist/4);
				tertiaryPtr += sprintf(strTertiaryPts+tertiaryPtr,"%.2f,%.2f;", (newPt.xDeg+shiftX+shiftFrameX), -(newPt.yDeg+shiftY+shiftFrameY));
				xPix =(newPt.xDeg +shiftX+shiftFrameX)* psuedoPixPerDeg;
				yPix =-(newPt.yDeg+shiftY +shiftFrameY)* psuedoPixPerDeg;
				vsgDrawBar(xPix, yPix, widthPix, widthPix, 0);
				newPt= m_pStimLocationGenerator->next();
			}
		}
		tertiaryPtr += sprintf(strTertiaryPts+tertiaryPtr,"\n");
		fprintf(tertiaryFile, strTertiaryPts);
		vsgPresent();
		vsgImageExport(vsgBMPPICTURE,0,0,vsgGetScreenWidthPixels(),vsgGetScreenHeightPixels(),m_fileName);
		if(bEndThread){
			g_pSparseNoiseDlg->printDebug("StopCalled");
			fclose(primaryFile);
			fclose(secondaryFile);
			fclose(tertiaryFile);
			return;
		}
		
	}
	fclose(primaryFile);
	fclose(secondaryFile);
	fclose(tertiaryFile);
	bEndThread =true;
}

void CSparseNoiseStimulus::PrepNextFrame(){
	int primaryPtr = 0;
	int secondaryPtr = 0;
	int tertiaryPtr=0;
	double xPix, yPix, shiftX,shiftY,shiftFrameX,shiftFrameY;
	char strPrimaryPts[1024];
	char strSecondaryPts[1024];
	char strTertiaryPts[1024];
	primaryFramePts.clear();
	secondaryFramePts.clear();
	tertiaryFramePts.clear();
	if (m_pStimLocationGenerator->remainder()<(m_primaryCount+m_secondaryCount+m_tertiaryCount)){ //reshuffle if not enough points left for frame
		m_pStimLocationGenerator->shuffle();

	}

	shiftFrameX = etu::RandChoice(0,1.5*m_minDist+m_stimWidth-1)-m_minDist-10;
	shiftFrameY = etu::RandChoice(0,1.5*m_minDist+m_stimWidth-1)-m_minDist-15;
	shiftX=0;
	shiftY=0;
	// Primary Blocks 
	for (int j =1;j<=m_primaryCount;j++){
		shiftX = etu::RandChoice(0,m_minDist/4); //Element Jitter
		shiftY = etu::RandChoice(0,m_minDist/4);
		addPt.xDeg = newPt.xDeg+shiftX+shiftFrameX;
		addPt.yDeg = -(newPt.yDeg +shiftY+shiftFrameY);
		primaryPtr += sprintf(strPrimaryPts+primaryPtr,"%.2f,%.2f;", (newPt.xDeg+shiftX+shiftFrameX), -(newPt.yDeg+shiftY+shiftFrameY));
		primaryFramePts.push_back(addPt);
		newPt= m_pStimLocationGenerator->next();
	}
	primaryPtr += sprintf(strPrimaryPts+primaryPtr,"\n");
	fprintf(primaryFile, strPrimaryPts);
	// Secondary Blocks
	if (m_secondaryCount>0){
		for (int k =1;k<=m_secondaryCount;k++){
			shiftX = etu::RandChoice(0,m_minDist/4); //Element Jitter
			shiftY = etu::RandChoice(0,m_minDist/4);
			addPt.xDeg = newPt.xDeg+shiftX+shiftFrameX;
			addPt.yDeg = -(newPt.yDeg+shiftY +shiftFrameY);
			secondaryPtr += sprintf(strSecondaryPts+secondaryPtr,"%.2f,%.2f;", (newPt.xDeg+shiftX+shiftFrameX), -(newPt.yDeg+shiftY+shiftFrameY));
			secondaryFramePts.push_back(addPt);
			newPt= m_pStimLocationGenerator->next();
		}
	}
	secondaryPtr += sprintf(strSecondaryPts+secondaryPtr,"\n");
	fprintf(secondaryFile, strSecondaryPts);
	// Tertiary Blocks
	if (m_tertiaryCount>0){
		for (int k =1;k<=m_tertiaryCount;k++){
			shiftX = etu::RandChoice(0,m_minDist/4); //Element Jitter
			shiftY = etu::RandChoice(0,m_minDist/4);
			addPt.xDeg = newPt.xDeg+shiftX+shiftFrameX;
			addPt.yDeg = -(newPt.yDeg+shiftY +shiftFrameY);
			tertiaryPtr += sprintf(strTertiaryPts+tertiaryPtr,"%.2f,%.2f;", (newPt.xDeg+shiftX+shiftFrameX), -(newPt.yDeg+shiftY+shiftFrameY));
			tertiaryFramePts.push_back(addPt);
			newPt= m_pStimLocationGenerator->next();
		}
	}
	tertiaryPtr += sprintf(strTertiaryPts+tertiaryPtr,"\n");
	fprintf(tertiaryFile, strTertiaryPts);
}



#endif // APP_EYE_TRACKING
