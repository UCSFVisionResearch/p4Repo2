/**
 * @file FixationStabilityDlg.cpp
 * @brief 
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include <afxwin.h>		/* AfxMessageBox */
#include <afx.h>
#include "app/stdafx.h"
#include "app/AppCore.h"
#include "app/EyeTrackingDlg.h"
#include "FixationStabilityDlg.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

IMPLEMENT_DYNAMIC(CFixationStabilityDlg, CDialog)
CFixationStabilityDlg::CFixationStabilityDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFixationStabilityDlg::IDD, pParent)
{}

CFixationStabilityDlg::~CFixationStabilityDlg() { /* Nothing to do */ }

void CFixationStabilityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_fsCrosshairDiameter, CrosshairDiameter);
	DDX_Control(pDX, IDC_fsCrosshairThickness, CrosshairThickness);
	DDX_Control(pDX, IDC_fsFixationTime, FixationTime);

	DDX_Control(pDX, IDC_fsLeftEyeFixatingDeviation, LeftEyeControls.cDeviation);
	DDX_Control(pDX, IDC_fsLeftColorRed,    LeftEyeControls.cColor.cRed);
	DDX_Control(pDX, IDC_fsLeftColorGreen,  LeftEyeControls.cColor.cGreen);
	DDX_Control(pDX, IDC_fsLeftColorBlue,   LeftEyeControls.cColor.cBlue);
	DDX_Control(pDX, IDC_fsLeftColorStatic, LeftEyeControls.cColor.cDisplay);
	DDX_Control(pDX, IDC_fsLeftFixationNoOccAtCtr, LeftEyeControls.cUnOccludedPositions.cCenter);
	DDX_Control(pDX, IDC_fsLeftFixationNoOccAtHalf, LeftEyeControls.cUnOccludedPositions.cHalf);
	DDX_Control(pDX, IDC_fsLeftFixationNoOccAtPeriph, LeftEyeControls.cUnOccludedPositions.cPeriph);
	DDX_Control(pDX, IDC_fsLeftFixationOccAtCtr, LeftEyeControls.cOccludedPositions.cCenter);
	DDX_Control(pDX, IDC_fsLeftFixationOccAtHalf, LeftEyeControls.cOccludedPositions.cHalf);
	DDX_Control(pDX, IDC_fsLeftFixationOccAtPeriph, LeftEyeControls.cOccludedPositions.cPeriph);
	
	DDX_Control(pDX, IDC_fsRightEyeFixatingDeviation, RightEyeControls.cDeviation);
	DDX_Control(pDX, IDC_fsRightColorRed,    RightEyeControls.cColor.cRed);
	DDX_Control(pDX, IDC_fsRightColorGreen,  RightEyeControls.cColor.cGreen);
	DDX_Control(pDX, IDC_fsRightColorBlue,   RightEyeControls.cColor.cBlue);
	DDX_Control(pDX, IDC_fsRightColorStatic, RightEyeControls.cColor.cDisplay);
	DDX_Control(pDX, IDC_fsRightFixationNoOccAtCtr, RightEyeControls.cUnOccludedPositions.cCenter);
	DDX_Control(pDX, IDC_fsRightFixationNoOccAtHalf, RightEyeControls.cUnOccludedPositions.cHalf);
	DDX_Control(pDX, IDC_fsRightFixationNoOccAtPeriph, RightEyeControls.cUnOccludedPositions.cPeriph);
	DDX_Control(pDX, IDC_fsRightFixationOccAtCtr, RightEyeControls.cOccludedPositions.cCenter);
	DDX_Control(pDX, IDC_fsRightFixationOccAtHalf, RightEyeControls.cOccludedPositions.cHalf);
	DDX_Control(pDX, IDC_fsRightFixationOccAtPeriph, RightEyeControls.cOccludedPositions.cPeriph);

	DDX_Control(pDX, IDC_fsUseNoise,        NoiseControls.cUseNoise);
	DDX_Control(pDX, IDC_fsSameBG,          NoiseControls.cSameBG);
	DDX_Control(pDX, IDC_fsInterleaveNoise, NoiseControls.cInterleaveNoise);
	DDX_Control(pDX, IDC_fsOccludeBG,       NoiseControls.cOccludeBG);
	DDX_Control(pDX, IDC_fsNoiseSize, NoiseControls.cNoiseSize);
	DDX_Control(pDX, IDC_fsNoisePct,  NoiseControls.cNoisePct);
	DDX_Control(pDX, IDC_fsNoiseColorRed,    NoiseControls.cColor.cRed);
	DDX_Control(pDX, IDC_fsNoiseColorGreen,  NoiseControls.cColor.cGreen);
	DDX_Control(pDX, IDC_fsNoiseColorBlue,   NoiseControls.cColor.cBlue);
	DDX_Control(pDX, IDC_fsNoiseColorStatic, NoiseControls.cColor.cDisplay);
}

BEGIN_MESSAGE_MAP(CFixationStabilityDlg, CDialog)
	ON_BN_CLICKED(IDC_fsUseNoise, OnBnClickedUseNoise)
	ON_BN_CLICKED(IDC_fsApply, OnBnClickedApply)
	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CFixationStabilityDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	load();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

/** Pull data from model and update dialog. */
void CFixationStabilityDlg::load() 
{
	CrosshairDiameter.SetWindowText(etu::IntToString(g_pFixationStabilityModel->crosshairDiameter()));
	CrosshairThickness.SetWindowText(etu::IntToString(g_pFixationStabilityModel->crosshairThickness()));
	FixationTime.SetWindowText(etu::IntToString(g_pFixationStabilityModel->fixationTime()));

	LeftEyeControls.parameters(g_pFixationStabilityModel->leftEyeParameters());
	RightEyeControls.parameters(g_pFixationStabilityModel->rightEyeParameters());
	NoiseControls.parameters(g_pFixationStabilityModel->noiseParameters());

	LeftEyeControls.cColor.rePaint();
	RightEyeControls.cColor.rePaint();
	NoiseControls.cColor.rePaint();
}

/** Push data from dialog into model. */
void CFixationStabilityDlg::save() 
{
	char buffer[16];

	CrosshairDiameter.GetWindowText(buffer, sizeof buffer);
	g_pFixationStabilityModel->crosshairDiameter(
		etu::StringToInt(buffer, g_pFixationStabilityModel->crosshairDiameter()));
	CrosshairThickness.GetWindowText(buffer, sizeof buffer);
	g_pFixationStabilityModel->crosshairThickness(
		etu::StringToInt(buffer, g_pFixationStabilityModel->crosshairThickness()));
	FixationTime.GetWindowText(buffer, sizeof buffer);
	g_pFixationStabilityModel->fixationTime(
		etu::StringToInt(buffer, g_pFixationStabilityModel->fixationTime()));

	LeftEyeControls.apply();
	RightEyeControls.apply();
	NoiseControls.apply();

	g_pFixationStabilityModel->leftEyeParameters(
		LeftEyeControls.parameters(fstab::LEFT_EYE));
	g_pFixationStabilityModel->rightEyeParameters(
		RightEyeControls.parameters(fstab::RIGHT_EYE));
	g_pFixationStabilityModel->noiseParameters(NoiseControls.parameters());

	g_pFixationStabilityModel->generateTrials();
}

void CFixationStabilityDlg::OnPaint() 
{
	CDialog::OnPaint();

	LeftEyeControls.cColor.rePaint();
	RightEyeControls.cColor.rePaint();
	NoiseControls.cColor.rePaint();
}

void CFixationStabilityDlg::OnBnClickedUseNoise()
{
	bool enabled = (NoiseControls.cUseNoise.GetCheck() == BST_CHECKED);
	NoiseControls.cInterleaveNoise.EnableWindow(enabled);
	NoiseControls.cSameBG.EnableWindow(enabled);
	NoiseControls.cOccludeBG.EnableWindow(enabled);
	NoiseControls.cNoiseSize.EnableWindow(enabled);
	NoiseControls.cNoisePct.EnableWindow(enabled);
	NoiseControls.cColor.cRed.EnableWindow(enabled);
	NoiseControls.cColor.cGreen.EnableWindow(enabled);
	NoiseControls.cColor.cBlue.EnableWindow(enabled);
}

void CFixationStabilityDlg::OnBnClickedApply()
{
	if (g_pActiveStimulus && g_pActiveStimulus->StimulusType() != "Fixation Stability Stimulus") {
		g_pEyeTrackingDlg->OnBnClickedRun();
	}
	save();
	g_pFixationStabilityDlg->Invalidate(FALSE);
}

/****************************************************************************
 ****************************************************************************/

CColorControls::CColorControls()
{
	m_rgb.rr = 0;
	m_rgb.gg = 0;
	m_rgb.bb = 0;
}

CColorControls::~CColorControls() { /* empty */ }

/** Push contents of CEdits into numerical data. Does not repaint. */
void CColorControls::apply()
{
	char buffer[16];

	cRed.GetWindowText(buffer, sizeof buffer);
	m_rgb.rr = etu::StringToInt(buffer, 0);
	cGreen.GetWindowText(buffer, sizeof buffer);
	m_rgb.gg = etu::StringToInt(buffer, 0);
	cBlue.GetWindowText(buffer, sizeof buffer);
	m_rgb.bb = etu::StringToInt(buffer, 0);
}

/** Push numerical data into contents of dlg components. Does not repaint. */
void CColorControls::update()
{
	cRed.SetWindowText(etu::IntToString(m_rgb.rr));
	cGreen.SetWindowText(etu::IntToString(m_rgb.gg));
	cBlue.SetWindowText(etu::IntToString(m_rgb.bb));
}

/** Update the color display square. */
void CColorControls::rePaint() 
{
	CRect rect;
	CPaintDC painter(&cDisplay);
	fstab::CColorSpec colorSpec(m_rgb);
	cDisplay.GetClientRect(rect);
	painter.FillSolidRect(rect, colorSpec.toRef());
}

/** Returns a memberwise copy. */
fstab::RGBTRIVAL CColorControls::get() { return m_rgb; }

/** Memberwise copy. */
void CColorControls::set(const fstab::RGBTRIVAL& rgb) { m_rgb = rgb; }

/****************************************************************************
 ****************************************************************************/

CLocationCheckboxes::CLocationCheckboxes() { /* empty */ }

CLocationCheckboxes::~CLocationCheckboxes() { /* empty */ }

/** Convert state of check boxes into list of positions. */
void CLocationCheckboxes::apply() 
{
	m_locations.clear();
	if (cCenter.GetCheck() == BST_CHECKED) m_locations.push_back(0.0);
	if (cHalf.GetCheck() == BST_CHECKED)   m_locations.push_back(0.5);
	if (cPeriph.GetCheck() == BST_CHECKED) m_locations.push_back(1.0);
}

/** Convert list of positions into check box checks. */
void CLocationCheckboxes::update()
{
	/* Copy it so it isn't cleared before being reset. */
	fstab::CrossLocations_t locations(m_locations);
	this->locations(locations);
}

/** Return copy of list (short list of doubles, sufficiently light-weight). */
const fstab::CrossLocations_t& CLocationCheckboxes::locations() const { return m_locations; }

/** Set list of locations to values in given list, only if the values are 
 *  recognized. */
void CLocationCheckboxes::locations(const fstab::CrossLocations_t& locations)
{
	fstab::const_LocationIter_t end = locations.end();
	fstab::const_LocationIter_t curr;
	
	m_locations.clear();
	cCenter.SetCheck(BST_UNCHECKED);
	cHalf.SetCheck(BST_UNCHECKED);
	cPeriph.SetCheck(BST_UNCHECKED);

	for (curr = locations.begin(); curr < end; ++curr) {
		if ((*curr) == 0.0) {
			cCenter.SetCheck(BST_CHECKED);
			m_locations.push_back(*curr);
		} 
		else if ((*curr) == 0.5) {
			cHalf.SetCheck(BST_CHECKED);
			m_locations.push_back(*curr);
		}
		else if ((*curr) == 1.0) {
			cPeriph.SetCheck(BST_CHECKED);
			m_locations.push_back(*curr);
		}
	}
}

/****************************************************************************
 ****************************************************************************/

CEyeParameterControls::CEyeParameterControls()
: m_deviation(0)
{}

CEyeParameterControls::~CEyeParameterControls() { /* empty */ }

/** Convert dlg state into numerical state. */
void CEyeParameterControls::apply()
{
	char buffer[16];

	cColor.apply();
	cUnOccludedPositions.apply();
	cOccludedPositions.apply();

	cDeviation.GetWindowText(buffer, sizeof buffer);
	m_deviation = etu::StringToInt(buffer, 0);
}

void CEyeParameterControls::update()
{
	cDeviation.SetWindowText(etu::IntToString(m_deviation));
	cColor.update();
	cUnOccludedPositions.update();
	cOccludedPositions.update();
}

int CEyeParameterControls::deviation() const { return m_deviation; }

void CEyeParameterControls::deviation(int dev) 
{ 
	m_deviation = dev; 
	cDeviation.SetWindowText(etu::IntToString(m_deviation));
}

fstab::CEyeParameters* CEyeParameterControls::parameters(fstab::EYE eye)
{
	fstab::CColorSpec color(cColor.get());
	return (new fstab::CEyeParameters(eye, m_deviation, color, 
		cUnOccludedPositions.locations(), cOccludedPositions.locations()));
}

void CEyeParameterControls::parameters(const fstab::CEyeParameters& params)
{
	m_deviation = params.deviation();
	cColor.set(params.color().toRgb());
	cUnOccludedPositions.locations(params.unoccluded());
	cOccludedPositions.locations(params.occluded());
	update();
}

/****************************************************************************
 ****************************************************************************/

CNoiseControls::CNoiseControls()
: m_noiseSize(0),
  m_noisePct(0)
{}

CNoiseControls::~CNoiseControls() { /* empty */ }

void CNoiseControls::apply()
{
	char buffer[16];
	cNoiseSize.GetWindowText(buffer, sizeof buffer);
	m_noiseSize = etu::StringToInt(buffer, 0);
	cNoisePct.GetWindowText(buffer, sizeof buffer);
	m_noisePct = etu::StringToInt(buffer, 0);
	cColor.apply();
}

void CNoiseControls::update()
{
	cNoiseSize.SetWindowText(etu::IntToString(m_noiseSize));
	cNoisePct.SetWindowText(etu::IntToString(m_noisePct));
	cColor.update();
}

bool CNoiseControls::useNoise() const { return (cUseNoise.GetCheck() == BST_CHECKED); }
bool CNoiseControls::sameBg() const { return (cSameBG.GetCheck() == BST_CHECKED); }
bool CNoiseControls::interleaveNoise() const { return (cInterleaveNoise.GetCheck() == BST_CHECKED); }
bool CNoiseControls::occludeBg() const { return (cOccludeBG.GetCheck() == BST_CHECKED); }

void CNoiseControls::useNoise(bool use) { cUseNoise.SetCheck(use ? BST_CHECKED : BST_UNCHECKED); }
void CNoiseControls::sameBg(bool same) {cSameBG.SetCheck(same ? BST_CHECKED : BST_UNCHECKED); }
void CNoiseControls::interleaveNoise(bool inter) {cInterleaveNoise.SetCheck(inter  ? BST_CHECKED : BST_UNCHECKED); }
void CNoiseControls::occludeBg(bool occlude) { cOccludeBG.SetCheck(occlude ? BST_CHECKED : BST_UNCHECKED); }

int  CNoiseControls::noiseSize() const { return m_noiseSize; }
int  CNoiseControls::noisePct() const { return m_noisePct; }

void CNoiseControls::noiseSize(int size) 
{
	m_noiseSize = size;
	cNoiseSize.SetWindowText(etu::IntToString(m_noiseSize));
}

void CNoiseControls::noisePct(int pct) 
{
	m_noisePct = pct;
	cNoisePct.SetWindowText(etu::IntToString(m_noisePct));
}

fstab::CNoiseParameters* CNoiseControls::parameters()
{
	fstab::CColorSpec color(cColor.get());
	return (new fstab::CNoiseParameters(useNoise(), sameBg(), interleaveNoise(), 
		occludeBg(), m_noiseSize, m_noisePct, color));
}

void CNoiseControls::parameters(const fstab::CNoiseParameters& params)
{
	useNoise(params.useNoise());
	sameBg(params.sameBg());
	interleaveNoise(params.interleaveNoise());
	occludeBg(params.occludeBg());
	noiseSize(params.noiseSize());
	noisePct(params.noisePct());
	cColor.set(params.color().toRgb());
	update();
}