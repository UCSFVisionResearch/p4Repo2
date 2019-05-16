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

	DDX_Control(pDX, IDC_XPosition, m_XPosition);
	DDX_Control(pDX, IDC_YPosition, m_YPosition);
	DDX_Control(pDX, IDC_Width, m_Width);
	DDX_Control(pDX, IDC_Height, m_Height);
	DDX_Control(pDX, IDC_Circle, m_RadioCircle);
	DDX_Control(pDX, IDC_Bar, m_RadioBar);
	DDX_Control(pDX, IDC_MouseControl, m_EnableMouse);
	DDX_Control(pDX, IDC_Pulse, m_EnablePulse);
	DDX_Control(pDX, IDC_StaticLeftEye, m_LeftEyeField);
	DDX_Control(pDX, IDC_StaticRightEye, m_RightEyeField);
	DDX_Control(pDX, IDC_BarWidth, m_BarWidth);
	DDX_Control(pDX, IDC_BarHeight, m_BarHeight);
	DDX_Control(pDX, IDC_Orientation, m_Orientation);
	DDX_Control(pDX, IDC_CircleRadius, m_CircleRadius);
	DDX_Control(pDX, IDC_APPLY, m_Apply);
	DDX_Control(pDX, IDC_SaveAsLeft, m_SaveAsLeft);
	DDX_Control(pDX, IDC_SaveAsRight, m_SaveAsRight);
}

BEGIN_MESSAGE_MAP(CRFMappingDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_SaveAsLeft, OnBnClickedSaveasleft)
	ON_BN_CLICKED(IDC_SaveAsRight, OnBnClickedSaveasright)
	ON_BN_CLICKED(IDC_Circle, OnBnClickedCircle)
	ON_BN_CLICKED(IDC_Bar, OnBnClickedBar)
	ON_BN_CLICKED(IDC_MouseControl, OnBnClickedMousecontrol)
	ON_BN_CLICKED(IDC_Pulse, OnBnClickedPulse)
	ON_BN_CLICKED(IDC_APPLY, OnBnClickedApply)
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
	m_XPosition.SetWindowText(etu::FloatToString(g_pRFMapping->getXPosition()));
	m_YPosition.SetWindowText(etu::FloatToString(g_pRFMapping->getYPosition()));
	m_Width.SetWindowText(etu::FloatToString(g_pRFMapping->getWidth()));
	m_Height.SetWindowText(etu::FloatToString(g_pRFMapping->getHeight()));
	m_BarWidth.SetWindowText(etu::FloatToString(g_pRFMapping->getBarWidth()));
	m_BarHeight.SetWindowText(etu::FloatToString(g_pRFMapping->getBarHeight()));
	m_Orientation.SetWindowText(etu::FloatToString(g_pRFMapping->getOrientation()));
	m_CircleRadius.SetWindowText(etu::FloatToString(g_pRFMapping->getCircleRadius()));
	
	if (g_pRFMapping->useCircle() ) {
		OnBnClickedCircle();
	} 
	else {
		OnBnClickedBar();
	}

	OnBnClickedMousecontrol();
	OnBnClickedPulse();
}

void CRFMappingDlg::save()
{
	char buffer[100];

	m_XPosition.GetWindowText(buffer, 100);
	g_pRFMapping->setXPosition(etu::StringToFloat(buffer, g_pRFMapping->getXPosition()));

	m_YPosition.GetWindowText(buffer, 100);
	g_pRFMapping->setYPosition(etu::StringToFloat(buffer, g_pRFMapping->getYPosition()));

	m_Width.GetWindowText(buffer, 100);
	g_pRFMapping->setWidth(etu::StringToFloat(buffer, g_pRFMapping->getWidth()));

	m_Height.GetWindowText(buffer, 100);
	g_pRFMapping->setHeight(etu::StringToFloat(buffer, g_pRFMapping->getHeight()));

	m_BarWidth.GetWindowText(buffer, 100);
	g_pRFMapping->setBarWidth(etu::StringToFloat(buffer, g_pRFMapping->getBarWidth()));

	m_BarHeight.GetWindowText(buffer, 100);
	g_pRFMapping->setBarHeight(etu::StringToFloat(buffer, g_pRFMapping->getBarHeight()));

	m_Orientation.GetWindowText(buffer, 100);
	g_pRFMapping->setOrientation(etu::StringToFloat(buffer, g_pRFMapping->getOrientation()));

	m_CircleRadius.GetWindowText(buffer, 100);
	g_pRFMapping->setCircleRadius(etu::StringToFloat(buffer, g_pRFMapping->getCircleRadius()));

	if (m_RadioCircle.GetCheck() == BST_CHECKED) {
		g_pRFMapping->setCircle(true);
	} 
	else {
		g_pRFMapping->setBar(true);
	}

	g_pRFMapping->setMouse(m_EnableMouse.GetCheck() == BST_CHECKED);
	g_pRFMapping->setPulse(m_EnablePulse.GetCheck() == BST_CHECKED);
}

void CRFMappingDlg::OnBnClickedOk()
{
	OnBnClickedApply();
}

void CRFMappingDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
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

	m_CircleRadius.EnableWindow(true);
	m_BarWidth.EnableWindow(false);
	m_BarHeight.EnableWindow(false);
	m_Orientation.EnableWindow(false);
}

void CRFMappingDlg::OnBnClickedBar()
{
	m_RadioCircle.SetCheck(BST_UNCHECKED);
	m_RadioBar.SetCheck(BST_CHECKED);

	m_CircleRadius.EnableWindow(false);
	m_BarWidth.EnableWindow(true);
	m_BarHeight.EnableWindow(true);
	m_Orientation.EnableWindow(true);
}

void CRFMappingDlg::OnBnClickedMousecontrol()
{
	if (m_EnableMouse.GetCheck() == BST_CHECKED) {
		m_EnablePulse.EnableWindow(true);
	} 
	else {
		m_EnablePulse.EnableWindow(false);
	}
	
}

void CRFMappingDlg::OnBnClickedPulse()
{
	// TODO: Add your control notification handler code here
}

void CRFMappingDlg::OnBnClickedApply()
{
	save();
}

void CRFMappingDlg::updateFieldString(CReceptiveField rf, bool bLeft) 
{
	char buffer[200];
	sprintf(buffer, "X Position:\t%.3f deg\n\nY Position:\t%.3f deg\n\nWidth:\t\t%.3f deg\n\nHeight:\t\t%.3f deg\n\nBar Width:\t%.3f deg\n\nBar Height:\t%.3f deg\n\nOrientation:\t%.3f deg\n\nCircle Radius:\t%.3f deg", rf.xPos, rf.yPos, rf.width, rf.height, rf.barWidth, rf.barHeight, rf.orientation, rf.circleRadius);
	if (bLeft) {
		m_LeftEyeField.SetWindowText(buffer);
	} 
	else {
		m_RightEyeField.SetWindowText(buffer);
	}	
}

void CRFMappingDlg::disableControls() 
{
	m_XPosition.EnableWindow(false);
	m_YPosition.EnableWindow(false);
	m_Width.EnableWindow(false);
	m_Height.EnableWindow(false);
	m_EnableMouse.EnableWindow(false);
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

void CRFMappingDlg::enableControls() 
{
	m_XPosition.EnableWindow(true);
	m_YPosition.EnableWindow(true);
	m_Width.EnableWindow(true);
	m_Height.EnableWindow(true);
	m_EnableMouse.EnableWindow(true);
	m_EnablePulse.EnableWindow(m_EnableMouse.GetCheck() == BST_CHECKED);
	m_Apply.EnableWindow(true);
	m_SaveAsLeft.EnableWindow(true);
	m_SaveAsRight.EnableWindow(true);
	m_RadioCircle.EnableWindow(true);
	m_RadioBar.EnableWindow(true);
	
	if (m_RadioCircle.GetCheck() == BST_UNCHECKED) {
		m_BarWidth.EnableWindow(true);
		m_BarHeight.EnableWindow(true);
		m_Orientation.EnableWindow(true);
        m_CircleRadius.EnableWindow(false);
	} 
	else {
		m_BarWidth.EnableWindow(false);
		m_BarHeight.EnableWindow(false);
		m_Orientation.EnableWindow(false);
		m_CircleRadius.EnableWindow(true);
	}
}

void CRFMappingDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (g_pRFMapping->useMouse() && g_pActiveStimulus  && g_pActiveStimulus->StimulusType() == "RF Mapping Stimulus") {
		if (MK_LBUTTON & nFlags) {
			float pixelsX = (float)(point.x - lastPoint.x);
			float pixelsY = (float)(point.y - lastPoint.y);
			((CRFMappingStimulus*)g_pActiveStimulus)->updateStimulus((int)pixelsX, (int)pixelsY, true);
			Sleep(1);
			m_XPosition.SetWindowText(etu::FloatToString(g_pRFMapping->getXPosition()));
			m_YPosition.SetWindowText(etu::FloatToString(g_pRFMapping->getYPosition()));
			m_BarWidth.SetWindowText(etu::FloatToString(g_pRFMapping->getBarWidth()));
			m_BarHeight.SetWindowText(etu::FloatToString(g_pRFMapping->getBarHeight()));
			m_Orientation.SetWindowText(etu::FloatToString(g_pRFMapping->getOrientation()));
			m_CircleRadius.SetWindowText(etu::FloatToString(g_pRFMapping->getCircleRadius()));
		} 
		else if (MK_RBUTTON & nFlags) {
			float pixelsX = (float)(point.x - lastPoint.x);
			float pixelsY = (float)(point.y - lastPoint.y);
			((CRFMappingStimulus*)g_pActiveStimulus)->updateStimulus((int)pixelsX, (int)pixelsY, false);
			Sleep(1);
			m_XPosition.SetWindowText(etu::FloatToString(g_pRFMapping->getXPosition()));
			m_YPosition.SetWindowText(etu::FloatToString(g_pRFMapping->getYPosition()));
			m_BarWidth.SetWindowText(etu::FloatToString(g_pRFMapping->getBarWidth()));
			m_BarHeight.SetWindowText(etu::FloatToString(g_pRFMapping->getBarHeight()));
			m_Orientation.SetWindowText(etu::FloatToString(g_pRFMapping->getOrientation()));
			m_CircleRadius.SetWindowText(etu::FloatToString(g_pRFMapping->getCircleRadius()));

		} 
		else {
			CRect area;
			this->GetClientRect(&area);
			int pixW=area.Width();
			int pixH=area.Height();

			currPoint.x = min(max(currPoint.x + point.x - lastPoint.x, 0), pixW);
			currPoint.y = min(max(currPoint.y + point.y - lastPoint.y, 0), pixH);
			lastPoint.x = currPoint.x;
			lastPoint.y = currPoint.y;
			
			((CRFMappingStimulus*)g_pActiveStimulus)->updatePosition((float)currPoint.x / (float)pixW, (float)currPoint.y / (float)pixH); 
			
			m_XPosition.SetWindowText(etu::FloatToString(g_pRFMapping->getXPosition()));
			m_YPosition.SetWindowText(etu::FloatToString(-g_pRFMapping->getYPosition()));
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
		g_pRFMapping->setMouse(false);
		((CRFMappingStimulus*)g_pActiveStimulus)->StopExecution();
		m_EnableMouse.SetCheck(BST_UNCHECKED);
		OnBnClickedMousecontrol();

	}
	CDialog::OnLButtonDblClk(nFlags, point);
}

#endif
