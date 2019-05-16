/**
 * @file RFMapping.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <afx.h>
#include <math.h>
#include "RFMapping.h"
#include "comm/SerialComm.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

CRFMapping::CRFMapping()
:   m_CrossX(0.0), m_CrossY(0.0),
	m_useBinocular(false), m_CrossX2(0.0), m_CrossY2(0.0), m_SwapInt(0),
	m_CrossDiameter(2.5), m_CrossThickness(2.0),
	m_eyeMode(CRFTrial::CODE_UNI),
	m_colorMode(CRFTrial::CODE_C_RED),
	m_primaryRed(255), m_primaryBlue(0),m_primaryGreen(0),
	m_secondaryRed(0), m_secondaryBlue(255),m_secondaryGreen(0),
	m_tertiaryRed(255), m_tertiaryBlue(255),m_tertiaryGreen(0),
	m_noiseRed(255), m_noiseBlue(255),m_noiseGreen(0),
	bUsePrimaryL(false),bUsePrimaryR(true),
	bUseSecondaryL(false), bUseSecondaryR(false),
	bUseTertiaryL(false), bUseTertiaryR(false),
	bUseCircle(false), bUseBar(true),
	fCircleRadius(2.0),
	fBarWidth(2.0),
	fBarHeight(6.0),
	fOrientation(0.0),
	bUseMouse(true),
	bUseGrid(false),
	bUsePulse(false),
	bUseNoise(false),
	bSameBG(false),
	bOccludeBG(false),
	bInterleaveNoise(false),
	m_NoisePct(0),
	m_NoiseSize(0),
	m_stepSizeDeg(5),
	m_stepsX(6),
	m_stepsY(3),
	m_displayTimeMs(500),
	m_useBlanks(true),
	m_blankTimeMs(200),
	fXPosition(0.0),
	fYPosition(0.0),
	fWidth(2.0),
	fHeight(2.0),
	m_trialGenerator(NULL),
	fWindowAllowancePix(1.0),m_useWait(false)
{
	m_currentTrial = m_trials.begin();
	LoadFile(true);
}

CRFMapping::~CRFMapping() 
{	 
	CleanUpTrials();
	delete m_trialGenerator;
	m_trialGenerator = NULL;
}

void CRFMapping::LoadFile(bool bDefault) 
{
	CString defaultFile = CString("C:\\Stimulus Settings\\RFMapping\\default.txt");
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

	fscanf(fp, "%f\n", &fXPosition);
	fscanf(fp, "%f\n", &fYPosition);
	fscanf(fp, "%f\n", &fWidth);
	fscanf(fp, "%f\n", &fHeight);
	fscanf(fp, "%d\n", &bUseMouse);
	bUseGrid = !bUseMouse;
	fscanf(fp, "%d\n", &bUsePulse);
	fscanf(fp, "%d\n", &bUseCircle);
	fscanf(fp, "%d\n", &bUseBar);
	fscanf(fp, "%f\n", &fBarWidth);
	fscanf(fp, "%f\n", &fBarHeight);
	fscanf(fp, "%f\n", &fOrientation);
	fscanf(fp, "%f\n", &fCircleRadius);
	fscanf(fp, "%f\n", &m_CrossX);
	fscanf(fp, "%f\n", &m_CrossY);
	fscanf(fp, "%f\n", &m_CrossDiameter);
	fscanf(fp, "%f\n", &m_CrossThickness);
	fscanf(fp, "%f\n", &m_stepSizeDeg);
	fscanf(fp, "%d\n", &m_stepsX);
	fscanf(fp, "%d\n", &m_stepsY);
	fscanf(fp, "%d\n", &m_displayTimeMs);
	fscanf(fp, "%d\n", &m_useBlanks);
	fscanf(fp, "%d\n", &m_blankTimeMs);

	fscanf(fp, "%f\n", &m_CrossX2);
	fscanf(fp, "%f\n", &m_CrossY2);
	fscanf(fp, "%d\n", &m_useBinocular);
	fscanf(fp, "%d\n", &m_SwapInt);
	fscanf(fp, "%d\n", &m_primaryRed);
	fscanf(fp, "%d\n", &m_primaryBlue);
	fscanf(fp, "%d\n", &m_primaryGreen);
	fscanf(fp, "%d\n", &m_secondaryRed);
	fscanf(fp, "%d\n", &m_secondaryBlue);
	fscanf(fp, "%d\n", &m_secondaryGreen);
	fscanf(fp, "%d\n", &m_tertiaryRed);
	fscanf(fp, "%d\n", &m_tertiaryBlue);
	fscanf(fp, "%d\n", &m_tertiaryGreen);
	fscanf(fp, "%d\n", &m_noiseRed);
	fscanf(fp, "%d\n", &m_noiseBlue);
	fscanf(fp, "%d\n", &m_noiseGreen);

	fscanf(fp, "%d\n", &bUseNoise);
	fscanf(fp, "%d\n", &bSameBG);
	fscanf(fp, "%d\n", &bOccludeBG);
	fscanf(fp, "%d\n", &bInterleaveNoise);
	fscanf(fp, "%d\n", &m_NoisePct);
	fscanf(fp, "%d\n", &m_NoiseSize);

	fscanf(fp, "%f\n", &fWindowAllowancePix);
	fscanf(fp, "%d\n", &m_useWait);
	fclose(fp);
}

void CRFMapping::SaveFile(bool bDefault) 
{
	CString defaultFile = CString("C:\\Stimulus Settings\\RFMapping\\default.txt");
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
	
	fprintf(fp, "%f\n", fXPosition);
	fprintf(fp, "%f\n", fYPosition);
	fprintf(fp, "%f\n", fWidth);
	fprintf(fp, "%f\n", fHeight);
	fprintf(fp, "%d\n", bUseMouse);
	fprintf(fp, "%d\n", bUsePulse);
	fprintf(fp, "%d\n", bUseCircle);
	fprintf(fp, "%d\n", bUseBar);
	fprintf(fp, "%f\n", fBarWidth);
	fprintf(fp, "%f\n", fBarHeight);
	fprintf(fp, "%f\n", fOrientation);
	fprintf(fp, "%f\n", fCircleRadius);
	fprintf(fp, "%f\n", m_CrossX);
	fprintf(fp, "%f\n", m_CrossY);
	
	fprintf(fp, "%f\n", m_CrossDiameter);
	fprintf(fp, "%f\n", m_CrossThickness);
	fprintf(fp, "%f\n", m_stepSizeDeg);
	fprintf(fp, "%d\n", m_stepsX);
	fprintf(fp, "%d\n", m_stepsY);
	fprintf(fp, "%d\n", m_displayTimeMs);
	fprintf(fp, "%d\n", m_useBlanks);
	fprintf(fp, "%d\n", m_blankTimeMs);

	fprintf(fp, "%f\n", m_CrossX2);
	fprintf(fp, "%f\n", m_CrossY2);
	fprintf(fp, "%d\n", m_useBinocular);
	fprintf(fp, "%d\n", m_SwapInt);

	fprintf(fp, "%d\n", m_primaryRed);
	fprintf(fp, "%d\n", m_primaryBlue);
	fprintf(fp, "%d\n", m_primaryGreen);
	fprintf(fp, "%d\n", m_secondaryRed);
	fprintf(fp, "%d\n", m_secondaryBlue);
	fprintf(fp, "%d\n", m_secondaryGreen);
	fprintf(fp, "%d\n", m_tertiaryRed);
	fprintf(fp, "%d\n", m_tertiaryBlue);
	fprintf(fp, "%d\n", m_tertiaryGreen);
	fprintf(fp, "%d\n", m_noiseRed);
	fprintf(fp, "%d\n", m_noiseBlue);
	fprintf(fp, "%d\n", m_noiseGreen);

	fprintf(fp, "%d\n", bUseNoise);
	fprintf(fp, "%d\n", bSameBG);
	fprintf(fp, "%d\n", bOccludeBG);
	fprintf(fp, "%d\n", bInterleaveNoise);
	fprintf(fp, "%d\n", m_NoisePct);
	fprintf(fp, "%d\n", m_NoiseSize);

	fprintf(fp, "%f\n", fWindowAllowancePix);
	fprintf(fp, "%d\n", m_useWait);
	fclose(fp);
}

/**
 * @brief Sends settings over serial port.
 * @param theComm Communications object to use for settings.
 */
void CRFMapping::sendSettings(CSerialComm& theComm) 
{
	char buffer[256];
	const int code = 0xC8; // 200
	
	/* time_t is 64 bits.  The %ld conversion expected long, which is only
	 * guaranteed to be 32 bits.  Cast to long so they match.  This will
	 * stop working truncate needed info in about 20 years. */
	long tm = (long)time(NULL);

	sprintf(buffer, "RF Mapping Started;%ld|0x%x/\0", tm, code); theComm.transmit(buffer, (int)strlen(buffer));
}

void CRFMapping::GenerateTrials()
{
	etu::pGrid_t peripheralGrid = etu::createCartesianGrid(m_stepsX, 
		m_stepsY, m_stepSizeDeg, false);
	
	if(m_useBinocular){
		m_eyeMode = CRFTrial::CODE_BI;
	}else{
		m_eyeMode = CRFTrial::CODE_UNI;
	}
	CRFTrialGenerator generator= CRFTrialGenerator( m_eyeMode, m_colorMode,
		peripheralGrid);
	CleanUpTrials();

	generator.GenerateTrials(m_trials);
	m_currentTrial = m_trials.begin();

}

CRFTrial* CRFMapping::GetNextTrial()
{
	CRFTrial* retval = NULL;

	if (m_trials.size()) {
		if (m_currentTrial >= m_trials.end()) {
			m_currentTrial = m_trials.begin();
		}
		
		retval = (*m_currentTrial);
		++m_currentTrial;
	} 
	
	return retval;
}

void CRFMapping::CleanUpTrials()
{
	RFTrialPtrContainer_t::iterator iter = m_trials.begin();
	RFTrialPtrContainer_t::iterator endIter = m_trials.end();
	for ( ; iter != endIter; ++iter) {
		delete (*iter);
	}
	m_trials.clear();
}