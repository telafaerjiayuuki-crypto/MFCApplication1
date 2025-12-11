#pragma once

// 最简单的搜索/替换对话框
class CSearchReplaceDlg : public CDialog
{
public:
    CSearchReplaceDlg(CWnd* pParent = nullptr);
    virtual ~CSearchReplaceDlg();

    // 对话框数据
    enum { IDD = IDD_SEARCH_REPLACE_DIALOG };

    CString m_strSearchText;     // 搜索文本
    CString m_strReplaceText;    // 替换文本
    BOOL m_bIsSearchOnly;        // 是否为只搜索模式

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    virtual void OnOK();

    DECLARE_MESSAGE_MAP()
};