#include "pch.h"
#include "MFCApplication1.h"
#include "SearchReplaceDlg.h"
#include "InputDlg.h"
IMPLEMENT_DYNAMIC(CSearchReplaceDlg, CDialog)

CSearchReplaceDlg::CSearchReplaceDlg(CWnd* pParent /*=nullptr*/)
    : CDialog(IDD_SEARCH_REPLACE_DIALOG, pParent)
    , m_strSearchText(_T(""))
    , m_strReplaceText(_T(""))
    , m_bIsSearchOnly(TRUE)
{
}

CSearchReplaceDlg::~CSearchReplaceDlg()
{
}

void CSearchReplaceDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_SEARCH, m_strSearchText);
    DDX_Text(pDX, IDC_EDIT_REPLACE, m_strReplaceText);
}

BEGIN_MESSAGE_MAP(CSearchReplaceDlg, CDialog)
END_MESSAGE_MAP()

BOOL CSearchReplaceDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // 根据模式设置标题
    if (m_bIsSearchOnly)
    {
        SetWindowText(_T("搜索"));
        GetDlgItem(IDC_STATIC_REPLACE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_REPLACE)->ShowWindow(SW_HIDE);
        SetDlgItemText(IDOK, _T("查找(&F)"));
    }
    else
    {
        SetWindowText(_T("替换"));
        SetDlgItemText(IDOK, _T("替换(&R)"));
    }

    return TRUE;
}

void CSearchReplaceDlg::OnOK()
{
    UpdateData(TRUE);

    if (m_strSearchText.IsEmpty())
    {
        AfxMessageBox(_T("请输入要搜索的内容"));
        GetDlgItem(IDC_EDIT_SEARCH)->SetFocus();
        return;
    }

    CDialog::OnOK();
}