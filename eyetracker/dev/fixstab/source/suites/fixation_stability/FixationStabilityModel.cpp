/**
 * @file FixationStabilityModel.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <afx.h>
#include <algorithm>
#include "FixationStability.h"
#include "FixationStabilityTrial.h"
#include "FixationStabilityModel.h"
#include "comm/SerialComm.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

CFixationStabilityModel::CFixationStabilityModel()
:   m_crosshairDiameter(3),
	m_crosshairThickness(4),
	m_fixationTime(10000),
	m_pLeftEyeParameters(NULL),
	m_pRightEyeParameters(NULL),
	m_pNoiseParameters(NULL)
{
	double locDoubles[] = {0.0, 0.5, 1};
	defaultFile = CString("C:\\Stimulus Settings\\FixationStability\\default.txt");
	fstab::CColorSpec left(0, 0, 255);
	fstab::CColorSpec right(255, 0, 0);
	fstab::CColorSpec noise(50, 0, 50);
	fstab::CrossLocations_t empty;
	fstab::CrossLocations_t locs(locDoubles, &locDoubles[3]);

	m_pLeftEyeParameters = new fstab::CEyeParameters(fstab::LEFT_EYE, 10, left, locs, empty);
	m_pRightEyeParameters = new fstab::CEyeParameters(fstab::RIGHT_EYE, 10, right, locs, empty);
	m_pNoiseParameters = new fstab::CNoiseParameters(true, true, false, false, 2, 50, noise);

	LoadFile(true);
}

CFixationStabilityModel::~CFixationStabilityModel() 
{
	CleanupParameters();
}

void CFixationStabilityModel::CleanupParameters()
{
	if (m_pLeftEyeParameters != NULL) {
		delete m_pLeftEyeParameters;
		m_pLeftEyeParameters = NULL;
	}

	if (m_pRightEyeParameters != NULL) {
		delete m_pRightEyeParameters;
		m_pRightEyeParameters = NULL;
	}

	if (m_pNoiseParameters != NULL) {
		delete m_pNoiseParameters;
		m_pNoiseParameters = NULL;
	}
}

void CFixationStabilityModel::LoadFile(bool bDefault) 
{
	CString filename = defaultFile;
	if (!bDefault) {
		filename = etu::BrowseForFile(true, ".txt", "Text Files (*.txt)|*.txt", filename, false);
	}

	char buffer[1024];
	sprintf(buffer, "%s", filename);
	FILE* fp = fopen(filename, "rt");
	if (!fp) {
		AfxMessageBox("Error loading file - file or folder not found");
		return;
	}

	fstab::CEyeParameters* leftEyeParams = NULL;
	fstab::CEyeParameters* rightEyeParams = NULL;
	fstab::CNoiseParameters* noiseParams = NULL;

	fscanf(fp, "%d\n", &m_crosshairDiameter);
	fscanf(fp, "%d\n", &m_crosshairThickness);
	fscanf(fp, "%d\n", &m_fixationTime);

	leftEyeParams = fstab::CEyeParameters::getFrom(fp);
	rightEyeParams = fstab::CEyeParameters::getFrom(fp);
	noiseParams = fstab::CNoiseParameters::getFrom(fp);

	if ((leftEyeParams != NULL) && (rightEyeParams != NULL) && 
		(noiseParams != NULL)) 
	{
		CleanupParameters();
		
		m_pLeftEyeParameters = leftEyeParams;
		m_pRightEyeParameters = rightEyeParams;
		m_pNoiseParameters = noiseParams;
	}
	
	fclose(fp);
}

void CFixationStabilityModel::SaveFile(bool bDefault) 
{
	CString filename = defaultFile;
	if (!bDefault) {
		filename = etu::BrowseForFile(false, ".txt", "Text Files (*.txt)|*.txt", filename, false);
	}

	char buffer[1024];
	sprintf(buffer, "%s", filename);
	FILE* fp = fopen(filename, "wt");
	if (!fp) {
		AfxMessageBox("Error saving file - file or folder not found");
		return;
	}

	fprintf(fp, "%d\n", m_crosshairDiameter);
	fprintf(fp, "%d\n", m_crosshairThickness);
	fprintf(fp, "%d\n", m_fixationTime);
	
	m_pLeftEyeParameters->putTo(fp);
	m_pRightEyeParameters->putTo(fp);
	m_pNoiseParameters->putTo(fp);

	fflush(fp);
	fclose(fp);
}

void CFixationStabilityModel::sendSettings(CSerialComm& theComm) 
{
	char buffer[80];
	const int code = 0xC8; // 200
	
	/* time_t is 64 bits.  The %ld conversion expected long, which is only
	 * guaranteed to be 32 bits.  Cast to long so they match. This will
	 * stop working truncate needed info in about 20 years. */
	long tm = (long)time(NULL);

	sprintf(buffer, "FixationStability;PARAM;%%;%ld|0x%02x/\0", /*param,*/ tm, code);		theComm.transmit(buffer, (int)strlen(buffer));
}

void CFixationStabilityModel::generateTrials()
{
	m_trials.clear();

	generateTrials(fstab::LEFT_EYE, m_pLeftEyeParameters->unoccluded().begin(), 
		m_pLeftEyeParameters->unoccluded().end(), 
		m_pLeftEyeParameters->deviation(), false);
	generateTrials(fstab::LEFT_EYE, m_pLeftEyeParameters->occluded().begin(), 
		m_pLeftEyeParameters->occluded().end(), 
		m_pLeftEyeParameters->deviation(), true);

	generateTrials(fstab::RIGHT_EYE, m_pRightEyeParameters->unoccluded().begin(), 
		m_pRightEyeParameters->unoccluded().end(), 
		m_pRightEyeParameters->deviation(), false);
	generateTrials(fstab::RIGHT_EYE, m_pRightEyeParameters->occluded().begin(), 
		m_pRightEyeParameters->occluded().end(), 
		m_pRightEyeParameters->deviation(), true);

	std::random_shuffle<fstab::TrialList_t::iterator>(m_trials.begin(), 
		m_trials.end());
}

/**
 * @brief Generate the trials indicated by parameters.
 * @param eye Left or right eye.
 * @param begin Beginning of locations.
 * @param end End of locations.
 * @param deviation The deviation of the other eye when this eye is fixating.
 * @param occlude Whether to occlude the deviated eye.
 */
void CFixationStabilityModel::generateTrials(fstab::EYE eye, 
											 fstab::const_LocationIter_t begin,
											 fstab::const_LocationIter_t end, 
											 int deviation, bool occlude)
{
	bool occludeLeft = (occlude ? ((eye == fstab::LEFT_EYE) ? false : true) : false);
	bool occludeRight = (occlude ? ((eye == fstab::RIGHT_EYE) ? false : true) : false);
	
	for ( ; begin < end; ++begin) {
		int xDeg = ((int)eye) * (*begin) * deviation;
		m_trials.push_back(fstab::CFixationStabilityTrial(eye, xDeg, 0, 
			occludeLeft, occludeRight));
	}

	m_iterCurrent = m_trials.begin();
	m_iterEnd = m_trials.end();
}

const fstab::TrialList_t& CFixationStabilityModel::trials() const
{
	return (m_trials);
}

const fstab::CFixationStabilityTrial* CFixationStabilityModel::getNextTrial()
{
	const fstab::CFixationStabilityTrial* retval = NULL;
	if (m_trials.size() > 0) {
		if (m_iterCurrent >= m_iterEnd) {
			m_iterCurrent = m_trials.begin();
		}
		retval = &(*m_iterCurrent);
	}
	++m_iterCurrent;
	return retval;
}

const fstab::CEyeParameters& CFixationStabilityModel::leftEyeParameters() const
{ return (*m_pLeftEyeParameters);}

const fstab::CEyeParameters& CFixationStabilityModel::rightEyeParameters() const
{ return (*m_pRightEyeParameters); }

const fstab::CNoiseParameters& CFixationStabilityModel::noiseParameters() const
{ return (*m_pNoiseParameters); }

void CFixationStabilityModel::leftEyeParameters(fstab::CEyeParameters* left)
{
	if (left == NULL) return;
	if (m_pLeftEyeParameters != NULL) delete m_pLeftEyeParameters;
	m_pLeftEyeParameters = left;
}

void CFixationStabilityModel::rightEyeParameters(fstab::CEyeParameters* right)
{
	if (right == NULL) return;
	if (m_pRightEyeParameters != NULL) delete m_pRightEyeParameters;
	m_pRightEyeParameters = right;
}

void CFixationStabilityModel::noiseParameters(fstab::CNoiseParameters* noise)
{
	if (noise == NULL) return;
	if (m_pNoiseParameters != NULL) delete m_pNoiseParameters;
	m_pNoiseParameters = noise;
}

int CFixationStabilityModel::crosshairDiameter() const { return m_crosshairDiameter; }
void CFixationStabilityModel::crosshairDiameter(int diam) { m_crosshairDiameter = diam; }

int CFixationStabilityModel::crosshairThickness() const { return m_crosshairThickness; }
void CFixationStabilityModel::crosshairThickness(int thick) { m_crosshairThickness = thick; }

int CFixationStabilityModel::fixationTime() const { return m_fixationTime; }
void CFixationStabilityModel::fixationTime(int time) { m_fixationTime = time; }
