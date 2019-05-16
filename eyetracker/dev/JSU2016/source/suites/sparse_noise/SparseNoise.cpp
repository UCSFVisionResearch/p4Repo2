/**
 * @file SparseNoise.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <afx.h>
#include <math.h>
#include "comm/SerialComm.h"
#include "utils/Utils.h"
#include "SparseNoise.h"
/*****************************************************************************
 * Member Functions
 *****************************************************************************/

CSparseNoise::CSparseNoise()
:   m_CrossX(0.0), m_CrossY(0.0),m_CreateFrames(false),
	m_useBinocular(false), m_useShutters(false), m_CrossX2(0.0), m_CrossY2(0.0), m_SwapInt(0),
	m_CrossDiameter(2.5), m_CrossThickness(2.0),
	m_displayTimeMs(500),
	m_fixX(0.0),
	m_fixY(0.0),
	fWindowAllowancePix(1.0),m_useWait(false),
	m_NumFrames(10),m_StimWidth(2), m_MinDist(5), m_PrimaryCount(5),m_SecondaryCount(5),m_TertiaryCount(5), m_Seed(0),
	m_primaryRed(255), m_primaryBlue(0),m_primaryGreen(0),
	m_secondaryRed(0), m_secondaryBlue(255),m_secondaryGreen(0),
	m_tertiaryRed(255), m_tertiaryBlue(255),m_tertiaryGreen(0),
	m_bgRed(125), m_bgBlue(125),m_bgGreen(125),
	bUseGrid(false),m_stepSizeDeg(5),m_stepsX(1),m_stepsY(1)
{
	LoadFile(true);
}

CSparseNoise::~CSparseNoise() 
{	 
	m_NumFrames = NULL;
}

void CSparseNoise::LoadFile(bool bDefault) 
{
	CString defaultFile = CString("C:\\Stimulus Settings\\SparseNoise\\default.txt");
	if (!bDefault) {
		defaultFile = etu::BrowseForFile(true, ".txt", "Text Files (*.txt)|*.txt", defaultFile, false);
	}

	char buffer[1024];
	sprintf(buffer, "%s", defaultFile);
	FILE* fp = fopen(buffer, "rt");
	if (!fp) {
		AfxMessageBox("Error loading file - file or folder not found");
		return;
	}

	fscanf(fp, "%f\n", &m_CrossX);
	fscanf(fp, "%f\n", &m_CrossY);
	fscanf(fp, "%f\n", &m_CrossDiameter);
	fscanf(fp, "%f\n", &m_CrossThickness);
	fscanf(fp, "%d\n", &m_displayTimeMs);
	
	fscanf(fp, "%f\n", &m_CrossX2);
	fscanf(fp, "%f\n", &m_CrossY2);
	fscanf(fp, "%d\n", &m_useBinocular);
	fscanf(fp, "%d\n", &m_useShutters);
	fscanf(fp, "%d\n", &m_SwapInt);
	
	fscanf(fp, "%f\n", &fWindowAllowancePix);
	fscanf(fp, "%d\n", &m_useWait);

	fscanf(fp, "%d\n", &m_primaryRed);
	fscanf(fp, "%d\n", &m_primaryGreen);
	fscanf(fp, "%d\n", &m_primaryBlue);
	fscanf(fp, "%d\n", &m_secondaryRed);
	fscanf(fp, "%d\n", &m_secondaryBlue);
	fscanf(fp, "%d\n", &m_secondaryGreen);
	fscanf(fp, "%d\n", &m_tertiaryRed);
	fscanf(fp, "%d\n", &m_tertiaryBlue);
	fscanf(fp, "%d\n", &m_tertiaryGreen);
	fscanf(fp, "%d\n", &m_bgRed);
	fscanf(fp, "%d\n", &m_bgBlue);
	fscanf(fp, "%d\n", &m_bgGreen);
	
	fscanf(fp, "%d\n", &bUseGrid);
	fscanf(fp, "%f\n", &m_stepSizeDeg);
	fscanf(fp, "%d\n", &m_stepsX);
	fscanf(fp, "%d\n", &m_stepsY);

	fclose(fp);
}

void CSparseNoise::SaveFile(bool bDefault) 
{
	CString defaultFile = CString("C:\\Stimulus Settings\\SparseNoise\\default.txt");
	if (!bDefault) {
		defaultFile = etu::BrowseForFile(false, ".txt", "Text Files (*.txt)|*.txt", defaultFile, false);
	}

	char buffer[1024];
	sprintf(buffer, "%s", defaultFile);
	FILE* fp = fopen(buffer, "wt");
	if (!fp) {
		AfxMessageBox("Error saving file - file or folder not found");
		return;
	}
	
	fprintf(fp, "%f\n", m_CrossX);
	fprintf(fp, "%f\n", m_CrossY);
	fprintf(fp, "%f\n", m_CrossDiameter);
	fprintf(fp, "%f\n", m_CrossThickness);
	fprintf(fp, "%d\n", m_displayTimeMs);
	
	fprintf(fp, "%f\n", m_CrossX2);
	fprintf(fp, "%f\n", m_CrossY2);
	fprintf(fp, "%d\n", m_useBinocular);
	fprintf(fp, "%d\n", m_useShutters);
	fprintf(fp, "%d\n", m_SwapInt);

	fprintf(fp, "%f\n", fWindowAllowancePix);
	fprintf(fp, "%d\n", m_useWait);

	fprintf(fp, "%d\n", m_primaryRed);
	fprintf(fp, "%d\n", m_primaryBlue);
	fprintf(fp, "%d\n", m_primaryGreen);
	fprintf(fp, "%d\n", m_secondaryRed);
	fprintf(fp, "%d\n", m_secondaryBlue);
	fprintf(fp, "%d\n", m_secondaryGreen);
	fprintf(fp, "%d\n", m_tertiaryRed);
	fprintf(fp, "%d\n", m_tertiaryBlue);
	fprintf(fp, "%d\n", m_tertiaryGreen);
	fprintf(fp, "%d\n", m_bgRed);
	fprintf(fp, "%d\n", m_bgBlue);
	fprintf(fp, "%d\n", m_bgGreen);

	fprintf(fp, "%d\n", bUseGrid);
	fprintf(fp, "%f\n", m_stepSizeDeg);
	fprintf(fp, "%d\n", m_stepsX);
	fprintf(fp, "%d\n", m_stepsY);

	fclose(fp);
}
void CSparseNoise::CreateStimFolder()
{	
	//Test if new Name for Stim folder (tests for settings file)
	char m_fileName[256];
	sprintf(m_fileName,"C:\\SparseNoise\\%s\\Settings.txt",getFolderName());
	FILE* fp =fopen(m_fileName,"rt");
	if(fp){
		AfxMessageBox("This Folder Already exists, please check C:\\SparseNoise.");
		fclose(fp);
	}else{

		//If not, CreateFrames
		fp =fopen(m_fileName,"wt");
		fprintf(fp, "%f\n", m_NumFrames);
		fprintf(fp, "%f\n", m_StimWidth);
		fprintf(fp, "%f\n", m_MinDist);
		fprintf(fp, "%d\n", m_PrimaryCount);
		fprintf(fp, "%d\n", m_SecondaryCount);
		fprintf(fp, "%d\n", m_TertiaryCount);
		fprintf(fp, "%d\n", m_Seed);
		fclose(fp);
		setCreateFrames(true);
		
	}

	
}
void CSparseNoise::LoadStimFolder()
{
	//Test if new Name for Stim folder (tests for settings file)
	char m_fileName[256];
	sprintf(m_fileName,"C:\\SparseNoise\\%s\\Settings.txt",getFolderName());
	FILE* fp =fopen(m_fileName,"r");
	if(fp){
		fscanf(fp, "%f\n", &m_NumFrames);
		fscanf(fp, "%f\n", &m_StimWidth);
		fscanf(fp, "%f\n", &m_MinDist);
		fscanf(fp, "%d\n", &m_PrimaryCount);
		fscanf(fp, "%d\n", &m_SecondaryCount);
		fscanf(fp, "%d\n", &m_TertiaryCount);
		fscanf(fp, "%d\n", &m_Seed);
		fclose(fp);
			
	}else{
		AfxMessageBox("This Folder Does Not exist,please check C:\\SparseNoise.");
	}
}


/**
 * @brief Sends settings over serial port.
 * @param theComm Communications object to use for settings.
 */
void CSparseNoise::sendSettings(CSerialComm& theComm) 
{
	char buffer[256];
	const int code = 0xC8; // 200
	
	/* time_t is 64 bits.  The %ld conversion expected long, which is only
	 * guaranteed to be 32 bits.  Cast to long so they match.  This will
	 * stop working truncate needed info in about 20 years. */
	long tm = (long)time(NULL);

	sprintf(buffer, "SparseNoise Started;%ld|0x%x/\0", tm, code); theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "SparseNoise;m_FolderName;%s;%ld|0x%x/\0",m_FolderName, tm, code); theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "SparseNoise;m_NumFrames;%f;%ld|0x%x/\0",m_NumFrames, tm, code); theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "SparseNoise;m_StimWidth;%f;%ld|0x%x/\0",m_StimWidth, tm, code); theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "SparseNoise;m_MinDist;%f;%ld|0x%x/\0",m_MinDist, tm, code); theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "SparseNoise;m_PrimaryCount;%d;%ld|0x%x/\0",m_PrimaryCount, tm, code); theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "SparseNoise;m_SecondaryCount;%d;%ld|0x%x/\0",m_SecondaryCount, tm, code); theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "SparseNoise;m_TertiaryCount;%d;%ld|0x%x/\0",m_TertiaryCount, tm, code); theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "SparseNoise;m_Seed;%d;%ld|0x%x/\0",m_Seed, tm, code); theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "SparseNoise;m_CrossX;%f;%ld|0x%x/\0",m_CrossX, tm, code); theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "SparseNoise;m_CrossY;%f;%ld|0x%x/\0",m_CrossY, tm, code); theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "SparseNoise;m_CrossDiameter;%f;%ld|0x%x/\0",m_CrossDiameter, tm, code); theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "SparseNoise;m_CrossThickness;%f;%ld|0x%x/\0",m_CrossThickness, tm, code); theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "SparseNoise;m_displayTimeMs;%d;%ld|0x%x/\0",m_displayTimeMs, tm, code); theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "SparseNoise;m_CrossX2;%f;%ld|0x%x/\0",m_CrossX2, tm, code); theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "SparseNoise;m_CrossY2;%f;%ld|0x%x/\0",m_CrossY2, tm, code); theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "SparseNoise;m_useBinocular;%d;%ld|0x%x/\0",m_useBinocular, tm, code); theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "SparseNoise;m_useShutters;%d;%ld|0x%x/\0",m_useShutters, tm, code); theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "SparseNoise;m_SwapInt;%d;%ld|0x%x/\0",m_SwapInt, tm, code); theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "SparseNoise;fWindowAllowancePix;%f;%ld|0x%x/\0",fWindowAllowancePix, tm, code); theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "SparseNoise;m_useWait;%d;%ld|0x%x/\0",m_useWait, tm, code); theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "SparseNoise;m_PrimaryRed;%d;%ld|0x%x/\0",m_primaryRed, tm, code); theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "SparseNoise;m_PrimaryGreen;%d;%ld|0x%x/\0",m_primaryGreen, tm, code); theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "SparseNoise;m_PrimaryBlue;%d;%ld|0x%x/\0",m_primaryBlue, tm, code); theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "SparseNoise;m_SecondaryRed;%d;%ld|0x%x/\0",m_secondaryRed, tm, code); theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "SparseNoise;m_SecondaryGreen;%d;%ld|0x%x/\0",m_secondaryGreen, tm, code); theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "SparseNoise;m_SecondaryBlue;%d;%ld|0x%x/\0",m_secondaryBlue, tm, code); theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "SparseNoise;m_TertiaryRed;%d;%ld|0x%x/\0",m_tertiaryRed, tm, code); theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "SparseNoise;m_TertiartGreen;%d;%ld|0x%x/\0",m_tertiaryGreen, tm, code); theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "SparseNoise;m_TertiaryBlue;%d;%ld|0x%x/\0",m_tertiaryBlue, tm, code); theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "SparseNoise;m_BGRed;%d;%ld|0x%x/\0",m_bgRed, tm, code); theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "SparseNoise;m_BGGreen;%d;%ld|0x%x/\0",m_bgGreen, tm, code); theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "SparseNoise;m_BGBlue;%d;%ld|0x%x/\0",m_bgBlue, tm, code); theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "SparseNoise;bUseGrid;%d;%ld|0x%x/\0",bUseGrid, tm, code); theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "SparseNoise;m_stepSizeDeg;%f;%ld|0x%x/\0",m_stepSizeDeg, tm, code); theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "SparseNoise;m_stepsX;%d;%ld|0x%x/\0",m_stepsX, tm, code); theComm.transmit(buffer, (int)strlen(buffer));
	sprintf(buffer, "SparseNoise;m_stepsY;%d;%ld|0x%x/\0",m_stepsY, tm, code); theComm.transmit(buffer, (int)strlen(buffer));

}
