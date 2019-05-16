#ifdef APP_EYE_TRACKING

#include "app/stdafx.h"
#include "app/AppCore.h"
#include "app/AppHandler.h"
#include "RememberDlg.h"
#include "RememberAnswerDlg.h"






#include "RememberAnswerDlg.h"

CRememberAnswerDlg::CRememberAnswerDlg() : CDialog(CRememberAnswerDlg::IDD) {}

void CRememberAnswerDlg::DoDataExchange(CDataExchange* pDX) { CDialog::DoDataExchange(pDX); }

BEGIN_MESSAGE_MAP(CRememberAnswerDlg, CDialog)
	ON_WM_KEYUP()
	ON_COMMAND(IDCLOSE, CRememberAnswerDlg::OnCloseButton)
	ON_COMMAND(IDCANCEL, CRememberAnswerDlg::OnCloseButton)
	ON_COMMAND(IDOK, CRememberAnswerDlg::OnOkButton)
END_MESSAGE_MAP()

BOOL CRememberAnswerDlg::OnInitDialog() {
	CDialog::OnInitDialog();

	//CPrefsDlg::SetDlgItemInt(IDC_FRAMERATE, framerate, true);
	//CPrefsDlg::CheckDlgButton(IDC_CHECK_SHOWTARGET, showtarget);
	
	//sprintf(buff, "R:%d G:%d B:%d\0", GetRValue(lefteyecolor), GetGValue(lefteyecolor), GetBValue(lefteyecolor));
	//CPrefsDlg::SetDlgItemText(IDC_LeftEyeColor, buff);
	//display icon or bitmap
	//CPrefsDlg::SendDlgItemMessage(IDC_LeftEyeColor, changeyourpicturetothiscolor, thiscolor, thiscolor);
	//HBITMAP h;
	//makeColorBitmap(32,32,lefteyecolor,h);
	//((CButton*)(CPrefsDlg::GetDlgItem(IDC_LeftEyeColor)))->SetBitmap(h);
	//delete h;
	return TRUE;
}

void CRememberAnswerDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Beep(200 + 5*nChar,500);
	AfxMessageBox("key up");
	CDialog::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CRememberAnswerDlg::OnCloseButton() {
	EndDialog(1);
}

void CRememberAnswerDlg::OnOkButton() {
	OnCloseButton();
}




#endif