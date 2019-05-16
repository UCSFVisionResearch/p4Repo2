/**
 * @file FixationStabilityModel.h
 * @brief 
 */

#ifndef FIXATION_STABILITY_MODEL_H
#define FIXATION_STABILITY_MODEL_H

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afx.h>
#include <deque>
#include "comm/SerialComm.h"
#include "sys/sys_concurrency.h"
#include "FixationStability.h"
#include "FixationStabilityTrial.h"

/*****************************************************************************
 * Classes
 *****************************************************************************/

class CFixationStabilityModel : public Mixin_ThreadAware
{
public:
	CString defaultFile;

	CFixationStabilityModel();
	~CFixationStabilityModel();
	
	void LoadFile(bool bDefault);
	void SaveFile(bool bDefault);
	void sendSettings(CSerialComm& theComm);

	int crosshairDiameter() const;
	int crosshairThickness() const;
	int fixationTime() const;

	void crosshairDiameter(int diam);
	void crosshairThickness(int thick);
	void fixationTime(int time);

	const fstab::CEyeParameters& leftEyeParameters() const;
	const fstab::CEyeParameters& rightEyeParameters() const;
	const etu::CNoiseParameters& noiseParameters() const;

	void leftEyeParameters(fstab::CEyeParameters* left);
	void rightEyeParameters(fstab::CEyeParameters* right);
	void noiseParameters(etu::CNoiseParameters* noise);

	void generateTrials();
	const fstab::TrialList_t& trials() const;
	const fstab::CFixationStabilityTrial* getNextTrial();

private:
	void CleanupParameters();
	void generateTrials(fstab::EYE eye, fstab::const_LocationIter_t begin, 
		fstab::const_LocationIter_t end, int deviation, bool occlude);

	int m_crosshairDiameter;
	int m_crosshairThickness;
	int m_fixationTime;

	fstab::CEyeParameters*  m_pLeftEyeParameters;
	fstab::CEyeParameters*  m_pRightEyeParameters;
	etu::CNoiseParameters*  m_pNoiseParameters;

	fstab::TrialList_t m_trials;
	fstab::const_TrialIter_t m_iterCurrent;
	fstab::const_TrialIter_t m_iterEnd;
};

#endif // FIXATION_STABILITY_MODEL_H
