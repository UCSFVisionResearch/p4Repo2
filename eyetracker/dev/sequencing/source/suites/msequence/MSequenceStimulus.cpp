/**
 * @file MSequenceStimulus.cpp
 * @brief 
 */

#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "app/AppCore.h"
#include "MSequenceStimulus.h"
#include "utils/Utils.h"

/***********************************************************
COMMUNICATIONS:
	Analog Out 0:	frames
************************************************************/

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

/***********************************************************
	 Constructor     
	 -  Instantiates the newly created object
************************************************************/
CMSequenceStimulus::CMSequenceStimulus() 
{
	Initialize();

	vsgSetCommand(vsgPALETTECLEAR + vsgOVERLAYDRIFT + vsgVIDEODRIFT);
	iScrWidthPix = vsgGetScreenWidthPixels();
	iScrHeightPix = vsgGetScreenHeightPixels();
	
	iPage = 0;
	iOn = 1;
	bOn = true;

	iSeqOffset=0;
	piSeq = NULL;
	
	vsgIOWriteDigitalOut(0xFFFF, vsgDIG0);
	vsgIOWriteDigitalOut(0x0000, vsgDIG1);

	// set the drawing mode, and set the origin to the center of the screen
	vsgSetDrawMode(vsgCENTREXY);
	vsgSetDrawOrigin(iScrWidthPix  /2, iScrHeightPix / 2);

	// set the background color
	VSGTRIVAL bgColor;
	bgColor.a = ((double)g_pSettings->getRedBG()) / 255.0f;
	bgColor.b = ((double)g_pSettings->getGreenBG()) / 255.0f;
	bgColor.c = ((double)g_pSettings->getBlueBG()) / 255.0f;

	vsgSetBackgroundColour(&bgColor);

	vsgLUTBUFFERRead(1,&lutbuffer);
	int i;
	for (i=0; i<256; i++) {
		lutbuffer[i].a = 0;
		lutbuffer[i].b = 0;
		lutbuffer[i].c = 0;
	}
	vsgLUTBUFFERWrite(0,&lutbuffer);

	vsgSetDrawPage(vsgVIDEOPAGE,0,vsgBACKGROUND);//horiz

	vsgSetPen1(1);
	vsgDrawRect(100, 0, 100, 100);
	vsgSetPen1(2);
	vsgDrawRect(-100, 0, 100, 100);

	//draw each bar with a unique color id
	int x,y;
	double dx,dy;
	//convert degrees to pixels
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pMSequence->getHCenter(), vsgPIXELUNIT, &dx);
	g_pSettings->tanScreenUnit2Unit(vsgDEGREEUNIT, g_pMSequence->getVCenter(), vsgPIXELUNIT, &dy);
	x=(int)dx;
	y=(int)-dy;
	for (i=0; i<g_pMSequence->getNumBars(); i++) {
		vsgSetPen1(i);
		//draw horizontal bars vertically
		vsgDrawRect( x, y+(i - g_pMSequence->getNumBars()/2)*g_pMSequence->getWidth() + g_pMSequence->getWidth()/2, g_pMSequence->getHeight(), g_pMSequence->getWidth());
	}
	vsgSetDrawPage(vsgVIDEOPAGE,1,vsgBACKGROUND);//vert
	for (i=0; i<g_pMSequence->getNumBars(); i++) {
		vsgSetPen1(i);
		//draw vertical bars horizontally
		vsgDrawRect( x+(i - g_pMSequence->getNumBars()/2)*g_pMSequence->getWidth() + g_pMSequence->getWidth()/2, y, g_pMSequence->getWidth(), g_pMSequence->getHeight());
	}

	if (g_pMSequence->getOrientation()==1) iPage=1;
	else iPage =0;
	vsgSetZoneDisplayPage(vsgVIDEOPAGE, iPage);
	vsgPresent();//draw

	theComm.init(false, CBR_115200, 1);
	char buffer2[256];
	sprintf(buffer2, "m-seq start. orderR=%d tapR=%d orderG=%d tapG=%d orderB=%d tapB=%d|0x00/", g_pMSequence->getOrder(), g_pMSequence->getTap(), g_pMSequence->getOrder2(), g_pMSequence->getTap2(), g_pMSequence->getOrder3(), g_pMSequence->getTap3());
	theComm.transmit(buffer2, (int)strlen(buffer2) );//send textmark
	piSeq = g_pMSequence->generateSeq(g_pMSequence->getOrder(), g_pMSequence->getTap());
	piSeq2 = g_pMSequence->generateSeq(g_pMSequence->getOrder2(), g_pMSequence->getTap2());
	piSeq3 = g_pMSequence->generateSeq(g_pMSequence->getOrder3(), g_pMSequence->getTap3());
	if (false) {
		for (i=0; i < ((int)pow(2.0F,g_pMSequence->getOrder())) / g_pMSequence->getNumBars(); i++) {
			for (int j=0; j < g_pMSequence->getNumBars(); j++) {
				buffer2[j] = ((piSeq[i*g_pMSequence->getNumBars()+j] & 1)==0)? '0': '1';
			}
			buffer2[g_pMSequence->getNumBars()]=0;
			AfxMessageBox(buffer2, 0,0);
		}
	}

	vsgIOWriteDigitalOut(0xFFFF, vsgDIG1);
	FlashTimer.StartTimer();
}

/*****************************************************************
	 Destructor
	 -  Cleans up when the object is deleted. The trial running
	 signal is turned off.
*****************************************************************/
CMSequenceStimulus::~CMSequenceStimulus() 
{
	vsgIOWriteDigitalOut(0x0000, vsgDIG1);
	free(piSeq);
	free(piSeq2);
	free(piSeq3);
	theComm.stop();
}

/*************************************************************************
	 DisplayStimulus()
	 -  Handles the display of the stimulus. 
*************************************************************************/
bool CMSequenceStimulus::DisplayStimulus() 
{
	char buf[1024];
	bool textmarks = g_pMSequence->getTextmarks();
	
	//piSeq = g_pMSequence->generateSeq(g_pMSequence->getOrder(), g_pMSequence->getTap());

	int i;

	char buffer[256];
	
	for (i=0; i<g_pMSequence->getNumBars(); i++) {
		//construct color based on combination of 3 mseqs
		lutbuffer[i].a = (double)(
			(g_pMSequence->getRedR()*(piSeq[iSeqOffset+i] & 1) +
			g_pMSequence->getGreenR()*(piSeq2[iSeqOffset+i] & 1) +
			g_pMSequence->getBlueR()*(piSeq3[iSeqOffset+i] & 1))%256
			)/255.0f;
		lutbuffer[i].b = (double)(
			(g_pMSequence->getRedG()*(piSeq[iSeqOffset+i] & 1) +
			g_pMSequence->getGreenG()*(piSeq2[iSeqOffset+i] & 1) +
			g_pMSequence->getBlueG()*(piSeq3[iSeqOffset+i] & 1))%256
			)/255.0f;
		lutbuffer[i].c = (double)(
			(g_pMSequence->getRedB()*(piSeq[iSeqOffset+i] & 1) +
			g_pMSequence->getGreenB()*(piSeq2[iSeqOffset+i] & 1) +
			g_pMSequence->getBlueB()*(piSeq3[iSeqOffset+i] & 1))%256
			)/255.0f;
		if (i<240 && textmarks) buffer[i]='a' + (piSeq[iSeqOffset+i] & 1) + 2*(piSeq2[iSeqOffset+i] & 1) + 4*(piSeq3[iSeqOffset+i] & 1);
	}

	if (textmarks) buffer[(g_pMSequence->getNumBars() >= 240)? 240 : g_pMSequence->getNumBars() ]=0;//null terminator
	g_pMSequence->setCurrentOffset(iSeqOffset);
	if (textmarks) {
		sprintf(buf, "Current offset: %d\0", g_pMSequence->getCurrentOffset());
		g_pMSequenceDlg->m_CurrentOffset.SetWindowText(buf);
		char buffer2[256];
		sprintf(buffer2, "%s|0x%x/", buffer, g_pMSequence->getCurrentOffset());
		theComm.transmit(buffer2, (int)strlen(buffer2) );//send textmark
	}

	//mark first frame of sequence
	int nextOffset = (iSeqOffset+g_pMSequence->getNumBars())%((int)pow(2.0F,g_pMSequence->getOrder()));
	if (iSeqOffset < g_pMSequence->getNumBars()) {
		if (g_pMSequence->getOrientation()==0) {
			iPage = 0;
		} 
		else if (g_pMSequence->getOrientation()==1) {
			iPage = 1;
		} 
		else {
			iPage = 1-iPage;
		}
		vsgSetZoneDisplayPage(vsgVIDEOPAGE, iPage);
		etu::AnalogOut(0, -5.0*(iPage+1), true);
	} 
	else {
		etu::AnalogOut(0, 0.0, true);
	}

	WaitUntil(g_pMSequence->getDelay() - FlashTimer.CheckTimer());

	//"draw"
	vsgSetBackgroundColour(&lutbuffer[vsgBACKGROUND]);
	vsgSetFixationColour(&lutbuffer[vsgFIXATION]);
	vsgLUTBUFFERWrite(0,&lutbuffer);//iPage
	vsgLUTBUFFERtoPalette(0);

	//move along sequence
	iSeqOffset = nextOffset;
	//free(piSeq);

	FlashTimer.StartTimer();

	return true;
}

/***************************************************************
	 StimulusType()
	 -  Inherited method that returns the stimulus type string.
****************************************************************/
CString CMSequenceStimulus::StimulusType() 
{
	return "MSequence Stimulus";
}

#endif // APP_EYE_TRACKING
