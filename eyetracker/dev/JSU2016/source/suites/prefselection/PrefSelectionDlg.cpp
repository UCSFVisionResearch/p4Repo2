// PrefSelectionDlg.cpp : implementation file
//
#ifdef APP_ACUITY


#include "app/stdafx.h"
#include "app/AppCore.h"
#include "prefselectiondlg.h"

// CPrefSelectionDlg dialog

IMPLEMENT_DYNAMIC(CPrefSelectionDlg, CDialog)
CPrefSelectionDlg::CPrefSelectionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPrefSelectionDlg::IDD, pParent)
{	
}

CPrefSelectionDlg::~CPrefSelectionDlg()
{
}

void CPrefSelectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EditTimeOut, m_TimeOut);
	DDX_Control(pDX, IDC_EditFreq, m_SpatialFreq);
	DDX_Control(pDX, IDC_CurrFreq, m_CurrFreqText);
	DDX_Control(pDX, IDC_CurrContrast, m_CurrContrastText);
	DDX_Control(pDX, IDC_CoarseStep, m_CoarseStep);
	DDX_Control(pDX, IDC_FineStep, m_FineStep);
	DDX_Control(pDX, IDC_NumReversals, m_NumReversals);
	DDX_Control(pDX, IDC_NUp, m_NUp);
	DDX_Control(pDX, IDC_NDown, m_NDown);
	DDX_Control(pDX, IDC_GratingHeight, m_GratingHeight);
	DDX_Control(pDX, IDC_GratingWidth, m_GratingWidth);
	DDX_Control(pDX, IDC_DotPosition, m_DotPosition);
	DDX_Control(pDX, IDC_GratingPosition, m_GratingPosition);
	DDX_Control(pDX, IDC_MinContrast, m_MinContrast);
	DDX_Control(pDX, IDC_MaxContrast, m_MaxContrast);
	DDX_Control(pDX, IDC_CurrPosition, m_CurrPosition);
	DDX_Control(pDX, IDC_MEDIUM_STEP, m_MedStep);
	DDX_Control(pDX, IDC_InitialContrasts, m_InitialContrastList);
	DDX_Control(pDX, IDC_EditCorrectTimeOut, m_CorrectTimeOut);
	DDX_Control(pDX, IDC_ReversalsCompleted, m_ReversalsCompleted);
	DDX_Control(pDX, IDC_UBERFINESTEP, m_UberFineStep);
	DDX_Control(pDX, IDC_CurrStepSize, m_CurStepSize);
	DDX_Control(pDX, IDC_DotSize, m_DotSize);
	DDX_Control(pDX, IDC_Edit_TargetDelay, m_TargetDelay);
	DDX_Control(pDX, IDC_TRAINING, m_TrainingMode);
	DDX_Control(pDX, IDC_PCT_TRIALS_RIGHT, m_PctTrialsRight);
}


BEGIN_MESSAGE_MAP(CPrefSelectionDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_ButtonApply, OnBnClickedButtonapply)
	ON_BN_CLICKED(IDC_IncreaseStep, OnBnClickedIncreasestep)
	ON_BN_CLICKED(IDC_DecreaseStep, OnBnClickedDecreasestep)

END_MESSAGE_MAP()



BOOL CPrefSelectionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	load();

	return TRUE;  // return TRUE unless you set the focus to a control
	
}

void CPrefSelectionDlg::load() {
	m_CorrectTimeOut.SetWindowText(etu::IntToString(g_pPrefSelection->getCorrectTimeOut()));
	m_TimeOut.SetWindowText(etu::IntToString(g_pPrefSelection->getTimeOut()));
	m_SpatialFreq.SetWindowText(g_pPrefSelection->getSpatialFreqList());
	m_InitialContrastList.SetWindowText(g_pPrefSelection->getInitialContrastList());
	m_GratingWidth.SetWindowText(etu::FloatToString(g_pPrefSelection->getGratingWidth()));
	m_GratingHeight.SetWindowText(etu::FloatToString(g_pPrefSelection->getGratingHeight()));
	m_GratingPosition.SetWindowText(etu::FloatToString(g_pPrefSelection->getGratingPosition()));
	m_DotPosition.SetWindowText(etu::FloatToString(g_pPrefSelection->getDotPosition()));
	m_DotSize.SetWindowText(etu::FloatToString(g_pPrefSelection->getDotSize()));
	m_TargetDelay.SetWindowText(etu::IntToString(g_pPrefSelection->getTargetDelay()));
	m_PctTrialsRight.SetWindowText(etu::IntToString(g_pPrefSelection->getPctTrialsRight()));
	
	m_CurrFreqText.SetWindowText("N/A");
	m_CurrContrastText.SetWindowText("N/A");
	m_CurrPosition.SetWindowText("N/A");
	m_ReversalsCompleted.SetWindowText("N/A");
	m_CurStepSize.SetWindowText("N/A");

	CStaircaseSettings* pSCSettings = g_pPrefSelection->getStaircaseSettings();

	m_CoarseStep.SetWindowText(etu::FloatToString(pSCSettings->getCoarseStep()));
	m_MedStep.SetWindowText(etu::FloatToString(pSCSettings->getMedStep()));
	m_FineStep.SetWindowText(etu::FloatToString(pSCSettings->getFineStep()));
	m_UberFineStep.SetWindowText(etu::FloatToString(pSCSettings->getUberFineStep()));
	m_NumReversals.SetWindowText(etu::IntToString(pSCSettings->getNumReversals()));
	m_NUp.SetWindowText(etu::IntToString(pSCSettings->getNUp()));
	m_NDown.SetWindowText(etu::IntToString(pSCSettings->getNDown()));
	m_MinContrast.SetWindowText(etu::FloatToString(pSCSettings->getMin()));
	m_MaxContrast.SetWindowText(etu::FloatToString(pSCSettings->getMax()));
	if (g_pPrefSelection->isTrainingMode()) {
        m_TrainingMode.SetCheck(BST_CHECKED);
	} else {
		m_TrainingMode.SetCheck(BST_UNCHECKED);
	}

}


void CPrefSelectionDlg::save() {
	char buffer[100];

	m_CorrectTimeOut.GetWindowText(buffer, 100);
	g_pPrefSelection->setCorrectTimeOut(etu::StringToInt(buffer, g_pPrefSelection->getCorrectTimeOut()));

	m_TimeOut.GetWindowText(buffer, 100);
	g_pPrefSelection->setTimeOut(etu::StringToInt(buffer, g_pPrefSelection->getTimeOut()));

	m_GratingWidth.GetWindowText(buffer, 100);
	g_pPrefSelection->setGratingWidth(etu::StringToFloat(buffer, g_pPrefSelection->getGratingWidth()));

	m_GratingHeight.GetWindowText(buffer, 100);
	g_pPrefSelection->setGratingHeight(etu::StringToFloat(buffer, g_pPrefSelection->getGratingHeight()));

	m_GratingPosition.GetWindowText(buffer, 100);
	g_pPrefSelection->setGratingPosition(etu::StringToFloat(buffer, g_pPrefSelection->getGratingPosition()));

	m_DotPosition.GetWindowText(buffer, 100);
	g_pPrefSelection->setDotPosition(etu::StringToFloat(buffer, g_pPrefSelection->getDotPosition()));

	m_DotSize.GetWindowText(buffer, 100);
	g_pPrefSelection->setDotSize(etu::StringToFloat(buffer, g_pPrefSelection->getDotSize()));

	m_TargetDelay.GetWindowText(buffer, 100);
	g_pPrefSelection->setTargetDelay(etu::StringToInt(buffer, g_pPrefSelection->getTargetDelay()));

	m_SpatialFreq.GetWindowText(buffer, 100);
	g_pPrefSelection->setSpatialFreqList(CString(buffer));

	m_InitialContrastList.GetWindowText(buffer, 100);
	g_pPrefSelection->setInitialContrastList(CString(buffer));

	CStaircaseSettings* pSCSettings = g_pPrefSelection->getStaircaseSettings();
	
	m_CoarseStep.GetWindowText(buffer, 100);
	pSCSettings->setCoarseStep(etu::StringToFloat(buffer, pSCSettings->getCoarseStep()));

	m_MedStep.GetWindowText(buffer, 100);
	pSCSettings->setMedStep(etu::StringToFloat(buffer, pSCSettings->getMedStep()));

	m_FineStep.GetWindowText(buffer, 100);
	pSCSettings->setFineStep(etu::StringToFloat(buffer, pSCSettings->getFineStep()));

	m_UberFineStep.GetWindowText(buffer, 100);
	pSCSettings->setUberFineStep(etu::StringToFloat(buffer, pSCSettings->getUberFineStep()));

	m_NUp.GetWindowText(buffer, 100);
	pSCSettings->setNUp(etu::StringToInt(buffer, pSCSettings->getNUp()));

	m_NDown.GetWindowText(buffer, 100);
	pSCSettings->setNDown(etu::StringToInt(buffer, pSCSettings->getNDown()));

	m_MinContrast.GetWindowText(buffer, 100);
	pSCSettings->setMin(etu::StringToFloat(buffer, pSCSettings->getMin()));

	m_MaxContrast.GetWindowText(buffer, 100);
	pSCSettings->setMax(etu::StringToFloat(buffer, pSCSettings->getMax()));

	m_NumReversals.GetWindowText(buffer, 100);
	pSCSettings->setNumReversals(etu::StringToInt(buffer, pSCSettings->getNumReversals()));

	m_PctTrialsRight.GetWindowText(buffer, 100);
	g_pPrefSelection->setPctTrialsRight(etu::StringToInt(buffer, g_pPrefSelection->getPctTrialsRight()));

	g_pPrefSelection->setTrainingMode(m_TrainingMode.GetCheck() == BST_CHECKED);


}



void CPrefSelectionDlg::OnBnClickedOk()
{
	OnBnClickedButtonapply();
}

void CPrefSelectionDlg::OnBnClickedCancel()
{
	
}

void CPrefSelectionDlg::OnBnClickedButtonapply()
{
	save();
	
}


void CPrefSelectionDlg::setCurrParameters(float spatialFreq, float contrast, int iSide, int numReversals, float currStep) {
	m_CurrFreqText.SetWindowText(etu::FloatToString(spatialFreq));
	m_CurrContrastText.SetWindowText(etu::FloatToString(contrast));
	m_CurrPosition.SetWindowText((iSide > 0) ? "Right" : "Left");
	m_ReversalsCompleted.SetWindowText(etu::IntToString(numReversals));
	m_CurStepSize.SetWindowText(etu::FloatToString(currStep));

}

void CPrefSelectionDlg::OnBnClickedIncreasestep()
{
	((CPrefSelectionStimulus*)g_pActiveStimulus)->increaseStep();
}

void CPrefSelectionDlg::OnBnClickedDecreasestep()
{
	((CPrefSelectionStimulus*)g_pActiveStimulus)->decreaseStep();
}


#endif