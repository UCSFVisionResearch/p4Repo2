/**
 * @file RFMappingDlg.cpp
 * @brief 
 */

#ifdef APP_EYE_TRACKING

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "app/stdafx.h"
#include "app/AppCore.h"
#include "RFMappingDlg.h"
#include "utils/Utils.h"

/*****************************************************************************
 * Member Functions
 *****************************************************************************/

IMPLEMENT_DYNAMIC(CRFMappingDlg, CDialog)
CRFMappingDlg::CRFMappingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRFMappingDlg::IDD, pParent)
{
	lastPoint.x = 0;
	lastPoint.y = 0;
	currPoint.x = 0;
	currPoint.y = 0;
}

CRFMappingDlg::~CRFMappingDlg()
{}

void CRFMappingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_rfmCrossX, m_rfmCrossX);
	DDX_Control(pDX, IDC_rfmCrossY, m_rfmCrossY);
	DDX_Control(pDX, IDC_rfmCrossDiameter, m_rfmCrossDiameter);
	DDX_Control(pDX, IDC_rfmCrossThickness, m_rfmCrossThickness);

	DDX_Control(pDX, IDC_rfmStimRed, m_rfmStimRed);
	DDX_Control(pDX, IDC_rfmStimGreen, m_rfmStimGreen);
	DDX_Control(pDX, IDC_rfmStimBlue, m_rfmStimBlue);
	DDX_Control(pDX, IDC_rfmStimColorStatic, m_rfmStimColorStatic);

	DDX_Control(pDX, IDC_Circle, m_RadioCircle);
	DDX_Control(pDX, IDC_Bar, m_RadioBar);
	DDX_Control(pDX, IDC_CircleRadius, m_CircleRadius);
	DDX_Control(pDX, IDC_BarWidth, m_BarWidth);
	DDX_Control(pDX, IDC_BarHeight, m_BarHeight);
	DDX_Control(pDX, IDC_Orientation, m_Orientation);

	DDX_Control(pDX, IDC_rfmUseMouse, m_RadioUseMouse);
	DDX_Control(pDX, IDC_rfmUseGrid, m_RadioUseGrid);
	DDX_Control(pDX, IDC_rfmStepSizeDeg, m_StepSizeDeg);
	DDX_Control(pDX, IDC_rfmStepsX, m_StepsX);
	DDX_Control(pDX, IDC_rfmStepsY, m_StepsY);
	DDX_Control(pDX, IDC_rfmDisplayTimeMs, m_DisplayTimeMs);

	DDX_Control(pDX, IDC_rfmBlanks, m_Blanks);
	DDX_Control(pDX, IDC_rfmBlankTimeMs, m_BlankTimeMs);

	DDX_Control(pDX, IDC_Pulse, m_EnablePulse);

	DDX_Control(pDX, IDC_XPosition, m_XPosition);
	DDX_Control(pDX, IDC_YPosition, m_YPosition);
	DDX_Control(pDX, IDC_Width, m_Width);
	DDX_Control(pDX, IDC_Height, m_Height);
		
	DDX_Control(pDX, IDC_StaticLeftEye, m_LeftEyeField);
	DDX_Control(pDX, IDC_StaticRightEye, m_RightEyeField);
	
	DDX_Control(pDX, IDC_SaveAsLeft, m_SaveAsLeft);
	DDX_Control(pDX, IDC_SaveAsRight, m_SaveAsRight);
	DDX_Control(pDX, IDC_APPLY, m_Apply);
}

BEGIN_MESSAGE_MAP(CRFMappingDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_SaveAsLeft, OnBnClickedSaveasleft)
	ON_BN_CLICKED(IDC_SaveAsRight, OnBnClickedSaveasright)
	ON_BN_CLICKED(IDC_Circle, OnBnClickedCircle)
	ON_BN_CLICKED(IDC_Bar, OnBnClickedBar)
	ON_BN_CLICKED(IDC_rfmUseMouse, OnBnClickedUseMouse)
	ON_BN_CLICKED(IDC_rfmUseGrid, OnBnClickedUseGrid)
	ON_BN_CLICKED(IDC_rfmBlanks, OnBnClickedBlanks)
	ON_BN_CLICKED(IDC_APPLY, OnBnClickedApply)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

BOOL CRFMappingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	load();
	OnBnClickedSaveasright();
	OnBnClickedSaveasleft();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CRFMappingDlg::load() 
{
	m_rfmCrossX.SetWindowText(etu::FloatToString(g_pRFMapping->getCrossX()));
	m_rfmCrossY.SetWindowText(etu::FloatToString(g_pRFMapping->getCrossY()));
	m_rfmCrossDiameter.SetWindowText(etu::FloatToString(g_pRFMapping->getCrossDiameter()));
	m_rfmCrossThickness.SetWindowText(etu::FloatToString(g_pRFMapping->getCrossThickness()));

	m_rfmStimRed.SetWindowTextA(etu::IntToString(g_pRFMapping->getStimRed()));
	m_rfmStimGreen.SetWindowTextA(etu::IntToString(g_pRFMapping->getStimGreen()));
	m_rfmStimBlue.SetWindowTextA(etu::IntToString(g_pRFMapping->getStimBlue()));

	m_CircleRadius.SetWindowText(etu::FloatToString(g_pRFMapping->getCircleRadius()));
	m_BarWidth.SetWindowText(etu::FloatToString(g_pRFMapping->getBarWidth()));
	m_BarHeight.SetWindowText(etu::FloatToString(g_pRFMapping->getBarHeight()));
	m_Orientation.SetWindowText(etu::FloatToString(g_pRFMapping->getOrientation()));	

	m_StepSizeDeg.SetWindowText(etu::FloatToString(g_pRFMapping->getStepSizeDeg()));
	m_StepsX.SetWindowText(etu::IntToString(g_pRFMapping->getStepsX()));
	m_StepsY.SetWindowText(etu::IntToString(g_pRFMapping->getStepsY()));
	m_DisplayTimeMs.SetWindowText(etu::IntToString(g_pRFMapping->getDisplayTimeMs()));
	m_Blanks.SetCheck(g_pRFMapping->getUseBlanks() ? BST_CHECKED : BST_UNCHECKED);
	m_BlankTimeMs.SetWindowTextA(etu::IntToString(g_pRFMapping->getBlankTimeMs()));
	OnBnClickedBlanks();

	m_XPosition.SetWindowText(etu::FloatToString(g_pRFMapping->getXPosition()));
	m_YPosition.SetWindowText(etu::FloatToString(g_pRFMapping->getYPosition()));
	m_Width.SetWindowText(etu::FloatToString(g_pRFMapping->getWidth()));
	m_Height.SetWindowText(etu::FloatToString(g_pRFMapping->getHeight()));

	if (g_pRFMapping->useCircle()) {
		OnBnClickedCircle();
	} 
	else {
		OnBnClickedBar();
	}

	if (g_pRFMapping->getUseMouse()) {
		OnBnClickedUseMouse();
	}
	else {
		OnBnClickedUseGrid();
	}

	m_EnablePulse.SetCheck(g_pRFMapping->usePulse());

	PaintColorStatics();
}

void CRFMappingDlg::save()
{
	char buffer[100];

	m_rfmCrossX.GetWindowText(buffer, 100);
	g_pRFMapping->setCrossX(etu::StringToFloat(buffer, g_pRFMapping->getCrossX()));

	m_rfmCrossY.GetWindowText(buffer, 100);
	g_pRFMapping->setCrossY(etu::StringToFloat(buffer, g_pRFMapping->getCrossY()));

	m_rfmCrossDiameter.GetWindowText(buffer, 100);
	g_pRFMapping->setCrossDiameter(etu::StringToFloat(buffer, g_pRFMapping->getCrossDiameter()));

	m_rfmCrossThickness.GetWindowText(buffer, 100);
	g_pRFMapping->setCrossThickness(etu::StringToFloat(buffer, g_pRFMapping->getCrossThickness()));

	m_rfmStimRed.GetWindowTextA(buffer, 100);
	g_pRFMapping->setStimRed(etu::StringToInt(buffer, g_pRFMapping->getStimRed()));

	m_rfmStimGreen.GetWindowTextA(buffer, 100);
	g_pRFMapping->setStimGreen(etu::StringToInt(buffer, g_pRFMapping->getStimGreen()));

	m_rfmStimBlue.GetWindowTextA(buffer, 100);
	g_pRFMapping->setStimBlue(etu::StringToInt(buffer, g_pRFMapping->getStimBlue()));

	m_CircleRadius.GetWindowText(buffer, 100);
	g_pRFMapping->setCircleRadius(etu::StringToFloat(buffer, g_pRFMapping->getCircleRadius()));

	m_BarWidth.GetWindowText(buffer, 100);
	g_pRFMapping->setBarWidth(etu::StringToFloat(buffer, g_pRFMapping->getBarWidth()));

	m_BarHeight.GetWindowText(buffer, 100);
	g_pRFMapping->setBarHeight(etu::StringToFloat(buffer, g_pRFMapping->getBarHeight()));

	m_Orientation.GetWindowText(buffer, 100);
	g_pRFMapping->setOrientation(etu::StringToFloat(buffer, g_pRFMapping->getOrientation()));

	m_StepSizeDeg.GetWindowText(buffer, 100);
	g_pRFMapping->setStepSizeDeg(etu::StringToFloat(buffer, g_pRFMapping->getStepSizeDeg()));

	m_StepsX.GetWindowText(buffer, 100);
	g_pRFMapping->setStepsX(etu::StringToInt(buffer, g_pRFMapping->getStepsX()));

	m_StepsY.GetWindowText(buffer, 100);
	g_pRFMapping->setStepsY(etu::StringToInt(buffer, g_pRFMapping->getStepsY()));

	m_DisplayTimeMs.GetWindowText(buffer, 100);
	g_pRFMapping->setDisplayTimeMs(etu::StringToInt(buffer, g_pRFMapping->getDisplayTimeMs()));

	g_pRFMapping->setUseBlanks(m_Blanks.GetCheck() == BST_CHECKED);

	m_BlankTimeMs.GetWindowTextA(buffer, 100);
	g_pRFMapping->setBlankTimeMs(etu::StringToInt(buffer, g_pRFMapping->getBlankTimeMs()));

	m_CircleRadius.GetWindowText(buffer, 100);
	g_pRFMapping->setCircleRadius(etu::StringToFloat(buffer, g_pRFMapping->getCircleRadius()));

	m_XPosition.GetWindowText(buffer, 100);
	g_pRFMapping->setXPosition(etu::StringToFloat(buffer, g_pRFMapping->getXPosition()));

	m_YPosition.GetWindowText(buffer, 100);
	g_pRFMapping->setYPosition(etu::StringToFloat(buffer, g_pRFMapping->getYPosition()));

	m_Width.GetWindowText(buffer, 100);
	g_pRFMapping->setWidth(etu::StringToFloat(buffer, g_pRFMapping->getWidth()));

	m_Height.GetWindowText(buffer, 100);
	g_pRFMapping->setHeight(etu::StringToFloat(buffer, g_pRFMapping->getHeight()));

	if (m_RadioCircle.GetCheck() == BST_CHECKED) {
		g_pRFMapping->setCircle();
	} 
	else {
		g_pRFMapping->setBar();
	}

	if (m_RadioUseMouse.GetCheck() == BST_CHECKED) {
		g_pRFMapping->setUseMouse();
	}
	else {
		g_pRFMapping->setUseGrid();
	}

	g_pRFMapping->setPulse(m_EnablePulse.GetCheck() == BST_CHECKED);

	PaintColorStatics();
}

void CRFMappingDlg::OnPaint() 
{
	CDialog::OnPaint();
	PaintColorStatics();
}

void CRFMappingDlg::PaintColorStatics()
{
	CRect rect;
	DWORD color = RGB(g_pRFMapping->getStimRed(), g_pRFMapping->getStimGreen(), g_pRFMapping->getStimBlue());

	CPaintDC dc(&m_rfmStimColorStatic);
	m_rfmStimColorStatic.GetClientRect(rect);
	dc.FillSolidRect(rect, color);
}

void CRFMappingDlg::OnBnClickedOk()
{
	OnBnClickedApply();
}

void CRFMappingDlg::OnBnClickedCancel()
{
	OnCancel();
}

void CRFMappingDlg::OnBnClickedSaveasleft()
{
	save();
	g_receptiveFieldLeft.xPos = g_pRFMapping->getXPosition();
	g_receptiveFieldLeft.yPos = g_pRFMapping->getYPosition();
	g_receptiveFieldLeft.width = g_pRFMapping->getWidth();
	g_receptiveFieldLeft.height = g_pRFMapping->getHeight();
	g_receptiveFieldLeft.barWidth = g_pRFMapping->getBarWidth();
	g_receptiveFieldLeft.barHeight = g_pRFMapping->getBarHeight();
	g_receptiveFieldLeft.orientation = g_pRFMapping->getOrientation();
	g_receptiveFieldLeft.circleRadius = g_pRFMapping->getCircleRadius();
	updateFieldString(g_receptiveFieldLeft, true);
}

void CRFMappingDlg::OnBnClickedSaveasright()
{
	save();
	g_receptiveFieldRight.xPos = g_pRFMapping->getXPosition();
	g_receptiveFieldRight.yPos = g_pRFMapping->getYPosition();
	g_receptiveFieldRight.width = g_pRFMapping->getWidth();
	g_receptiveFieldRight.height = g_pRFMapping->getHeight();
	g_receptiveFieldRight.barWidth = g_pRFMapping->getBarWidth();
	g_receptiveFieldRight.barHeight = g_pRFMapping->getBarHeight();
	g_receptiveFieldRight.orientation = g_pRFMapping->getOrientation();
	g_receptiveFieldRight.circleRadius = g_pRFMapping->getCircleRadius();
	updateFieldString(g_receptiveFieldRight, false);
}

void CRFMappingDlg::OnBnClickedCircle()
{
	m_RadioCircle.SetCheck(BST_CHECKED);
	m_RadioBar.SetCheck(BST_UNCHECKED);

	if (m_RadioUseMouse.GetCheck() == BST_UNCHECKED) {
		m_CircleRadius.EnableWindow(true);
		m_BarWidth.EnableWindow(false);
		m_BarHeight.EnableWindow(false);
		m_Orientation.EnableWindow(false);
	}
}

void CRFMappingDlg::OnBnClickedBar()
{
	m_RadioCircle.SetCheck(BST_UNCHECKED);
	m_RadioBar.SetCheck(BST_CHECKED);

	if (m_RadioUseMouse.GetCheck() == BST_UNCHECKED) {
		m_CircleRadius.EnableWindow(false);
		m_BarWidth.EnableWindow(true);
		m_BarHeight.EnableWindow(true);
		m_Orientation.EnableWindow(true);
	}
}

void CRFMappingDlg::OnBnClickedUseMouse()
{
	m_RadioUseMouse.SetCheck(BST_CHECKED);
	m_RadioUseGrid.SetCheck(BST_UNCHECKED);

	m_CircleRadius.EnableWindow(false);
	m_BarWidth.EnableWindow(false);
	m_BarHeight.EnableWindow(false);
	m_Orientation.EnableWindow(false);

	m_StepSizeDeg.EnableWindow(false);
	m_StepsX.EnableWindow(false);
	m_StepsY.EnableWindow(false);
	m_DisplayTimeMs.EnableWindow(false);
	m_Blanks.EnableWindow(false);
	m_BlankTimeMs.EnableWindow(false);
}

void CRFMappingDlg::OnBnClickedUseGrid()
{
	m_RadioUseMouse.SetCheck(BST_UNCHECKED);
	m_RadioUseGrid.SetCheck(BST_CHECKED);

	if (m_RadioCircle.GetCheck() == BST_CHECKED) {
		m_CircleRadius.EnableWindow(true);
	}
	else {
		m_BarWidth.EnableWindow(true);
		m_BarHeight.EnableWindow(true);
		m_Orientation.EnableWindow(true);
	}

	m_StepSizeDeg.EnableWindow(true);
	m_StepsX.EnableWindow(true);
	m_StepsY.EnableWindow(true);
	m_DisplayTimeMs.EnableWindow(true);
	m_Blanks.EnableWindow(true);
	OnBnClickedBlanks();
}

void CRFMappingDlg::OnBnClickedBlanks()
{
	m_BlankTimeMs.EnableWindow(m_Blanks.GetCheck() == BST_CHECKED);
}

void CRFMappingDlg::OnBnClickedApply()
{
	save();
	this->Invalidate(false);
}

void CRFMappingDlg::updateFieldString(CReceptiveField rf, bool bLeft) 
{
	char buffer[200];
	sprintf(buffer, \
		"X Position:\t%.3f deg\n\n" \
		"Y Position:\t%.3f deg\n\n" \
		"Width:\t\t%.3f deg\n\n" \
		"Height:\t\t%.3f deg\n\n" \
		"Bar Width:\t%.3f deg\n\n" \
		"Bar Height:\t%.3f deg\n\n" \
		"Orientation:\t%.3f deg\n\n" \
		"Circle Radius:\t%.3f deg", 
		rf.xPos, rf.yPos, rf.width, rf.height, 
		rf.barWidth, rf.barHeight, rf.orientation, rf.circleRadius);
	if (bLeft) {
		m_LeftEyeField.SetWindowText(buffer);
	} 
	else {
		m_RightEyeField.SetWindowText(buffer);
	}	
}

void CRFMappingDlg::updateFieldResults()
{
	m_XPosition.SetWindowText(etu::FloatToString(g_pRFMapping->getXPosition()));
	m_YPosition.SetWindowText(etu::FloatToString(g_pRFMapping->getYPosition()));
	m_Width.SetWindowText(etu::FloatToString(g_pRFMapping->getWidth()));
	m_Height.SetWindowText(etu::FloatToString(g_pRFMapping->getHeight()));
}

/* Disable controls that should be disabled when actively running stimulus. */
void CRFMappingDlg::disableControls() 
{
	m_XPosition.EnableWindow(false);
	m_YPosition.EnableWindow(false);
	m_Width.EnableWindow(false);
	m_Height.EnableWindow(false);

	m_RadioUseMouse.EnableWindow(false);
	m_RadioUseGrid.EnableWindow(false);
	m_StepSizeDeg.EnableWindow(false);
	m_StepsX.EnableWindow(false);
	m_StepsY.EnableWindow(false);
	m_DisplayTimeMs.EnableWindow(false);

	m_EnablePulse.EnableWindow(false);

	m_Apply.EnableWindow(false);
	
	m_SaveAsLeft.EnableWindow(false);
	m_SaveAsRight.EnableWindow(false);
	
	m_RadioCircle.EnableWindow(false);
	m_RadioBar.EnableWindow(false);
	m_BarWidth.EnableWindow(false);
	m_BarHeight.EnableWindow(false);
	m_Orientation.EnableWindow(false);
	m_CircleRadius.EnableWindow(false);
}

/* Re-enable controls now that the stimulus is no longer running. */
void CRFMappingDlg::enableControls() 
{
	m_XPosition.EnableWindow(true);
	m_YPosition.EnableWindow(true);
	m_Width.EnableWindow(true);
	m_Height.EnableWindow(true);

	m_RadioUseMouse.EnableWindow(true);
	m_RadioUseGrid.EnableWindow(true);

	m_EnablePulse.EnableWindow(true);

	m_Apply.EnableWindow(true);
	m_SaveAsLeft.EnableWindow(true);
	m_SaveAsRight.EnableWindow(true);
	m_RadioCircle.EnableWindow(true);
	m_RadioBar.EnableWindow(true);
	
	if (m_RadioBar.GetCheck() == BST_CHECKED) {
		OnBnClickedBar();
	} 
	else {
		OnBnClickedCircle();
	}

	if (m_RadioUseMouse.GetCheck() == BST_CHECKED) {
		OnBnClickedUseMouse();
	}
	else {
		OnBnClickedUseGrid();
	}
}

void CRFMappingDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (g_pRFMapping->getUseMouse() && g_pActiveStimulus  && g_pActiveStimulus->StimulusType() == "RF Mapping Stimulus") {
		if (MK_LBUTTON & nFlags) {
			float pixelsX = (float)(point.x - lastPoint.x);
			float pixelsY = (float)(point.y - lastPoint.y);
			((CRFMappingStimulus*)g_pActiveStimulus)->updateStimulus((int)pixelsX, (int)pixelsY, true);
			Sleep(1);
			updateFieldResults();
		} 
		else if (MK_RBUTTON & nFlags) {
			float pixelsX = (float)(point.x - lastPoint.x);
			float pixelsY = (float)(point.y - lastPoint.y);
			((CRFMappingStimulus*)g_pActiveStimulus)->updateStimulus((int)pixelsX, (int)pixelsY, false);
			Sleep(1);
			updateFieldResults();

		} 
		else {
			CRect area;
			this->GetClientRect(&area);
			int pixW = area.Width();
			int pixH = area.Height();

			currPoint.x = point.x;
			currPoint.y = point.y;
			
			((CRFMappingStimulus*)g_pActiveStimulus)->updatePosition((float)currPoint.x / (float)pixW, 
				(float)currPoint.y / (float)pixH); 
			
			updateFieldResults();
			
		}

		lastPoint.x = point.x;
		lastPoint.y = point.y;
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CRFMappingDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// locks target
	if (g_pActiveStimulus  && g_pActiveStimulus->StimulusType() == "RF Mapping Stimulus") {
		((CRFMappingStimulus*)g_pActiveStimulus)->StopExecution();
	}
	CDialog::OnLButtonDblClk(nFlags, point);
}

#endif
