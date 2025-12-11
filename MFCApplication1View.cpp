#include "pch.h"
#include "framework.h"
#ifndef SHARED_HANDLERS
#include "MFCApplication1.h"
#endif

#include "MFCApplication1Doc.h"
#include "MFCApplication1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "InputDlg.h"
IMPLEMENT_DYNCREATE(CMFCApplication1View, CScrollView)

BEGIN_MESSAGE_MAP(CMFCApplication1View, CScrollView)
    ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
    ON_WM_CHAR()
    ON_WM_KEYDOWN()
    ON_WM_LBUTTONDOWN()
    ON_WM_TIMER()
    ON_WM_VSCROLL()
    ON_WM_HSCROLL()
    ON_WM_SETFOCUS()
    ON_WM_KILLFOCUS()
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_COMMAND(ID_EDIT_CUT, OnEditCut)
    ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
    ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
    ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
    ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
    ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
    ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
    ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
    ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
    ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateEditSelectAll)
    ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
    ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
    ON_COMMAND(ID_EDIT_FIND, &CMFCApplication1View::OnEditFind)
    ON_COMMAND(ID_EDIT_REPLACE, &CMFCApplication1View::OnEditReplace)
    ON_COMMAND(ID_VIEW_DARK_THEME, &CMFCApplication1View::OnViewDarkTheme)
    ON_COMMAND(ID_VIEW_LIGHT_THEME, &CMFCApplication1View::OnViewLightTheme)
    ON_UPDATE_COMMAND_UI(ID_VIEW_DARK_THEME, &CMFCApplication1View::OnUpdateViewDarkTheme)
    ON_UPDATE_COMMAND_UI(ID_VIEW_LIGHT_THEME, &CMFCApplication1View::OnUpdateViewLightTheme)
END_MESSAGE_MAP()

CMFCApplication1View::CMFCApplication1View()
{
    m_nCursorPos = 0;
    m_bCursorVisible = TRUE;
    m_nCursorBlinkCounter = 0;
    m_nLineNumberWidth = 50;

    // 设置初始主题为亮色
    m_bDarkTheme = FALSE;

    // 初始化亮色主题颜色
    m_crLightText = RGB(0, 0, 0);          // 黑色文字
    m_crLightBg = RGB(255, 255, 255);      // 白色背景
    m_crLightLineNumberText = RGB(100, 100, 100);      // 深灰色行号文字
    m_crLightLineNumberBg = RGB(240, 240, 240);        // 浅灰色行号背景

    // 初始化暗色主题颜色
    m_crDarkText = RGB(255, 255, 255);     // 白色文字
    m_crDarkBg = RGB(30, 30, 30);          // 深灰色背景
    m_crDarkLineNumberText = RGB(180, 180, 180);       // 浅灰色行号文字
    m_crDarkLineNumberBg = RGB(50, 50, 50);            // 深灰色行号背景

    // 应用初始主题
    m_crTextColor = m_crLightText;
    m_crBackgroundColor = m_crLightBg;
    m_crLineNumberText = m_crLightLineNumberText;
    m_crLineNumberBg = m_crLightLineNumberBg;
}
void CMFCApplication1View::ApplyTheme(BOOL bDarkTheme)
{
    m_bDarkTheme = bDarkTheme;

    if (m_bDarkTheme)
    {
        // 应用暗色主题
        m_crTextColor = m_crDarkText;
        m_crBackgroundColor = m_crDarkBg;
        m_crLineNumberText = m_crDarkLineNumberText;
        m_crLineNumberBg = m_crDarkLineNumberBg;
    }
    else
    {
        // 应用亮色主题
        m_crTextColor = m_crLightText;
        m_crBackgroundColor = m_crLightBg;
        m_crLineNumberText = m_crLightLineNumberText;
        m_crLineNumberBg = m_crLightLineNumberBg;
    }

    // 重绘视图
    Invalidate();
}
void CMFCApplication1View::OnViewDarkTheme()
{
    ApplyTheme(TRUE);
}

// 切换为亮色主题
void CMFCApplication1View::OnViewLightTheme()
{
    ApplyTheme(FALSE);
}

// 更新暗色主题菜单状态
void CMFCApplication1View::OnUpdateViewDarkTheme(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_bDarkTheme);
}

// 更新亮色主题菜单状态
void CMFCApplication1View::OnUpdateViewLightTheme(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(!m_bDarkTheme);
}
CMFCApplication1View::~CMFCApplication1View()
{
}

int CMFCApplication1View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CScrollView::OnCreate(lpCreateStruct) == -1)
        return -1;

    // 设置光标闪烁定时器
    SetTimer(1, 500, NULL); // 500ms闪烁一次

    return 0;
}

void CMFCApplication1View::OnDestroy()
{
    // 清除定时器
    KillTimer(1);
    CScrollView::OnDestroy();
}

void CMFCApplication1View::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == 1)
    {
        // 光标闪烁
        m_bCursorVisible = !m_bCursorVisible;
        m_nCursorBlinkCounter++;

        // 每10次闪烁（5秒）重置一次，避免长时间运行后计数器溢出
        if (m_nCursorBlinkCounter > 10)
            m_nCursorBlinkCounter = 0;

        // 如果当前有焦点，重绘光标
        if (GetFocus() == this)
        {
            CRect rect;
            GetClientRect(&rect);
            InvalidateRect(&rect);
        }
    }

    CScrollView::OnTimer(nIDEvent);
}

BOOL CMFCApplication1View::PreCreateWindow(CREATESTRUCT& cs)
{
    return CScrollView::PreCreateWindow(cs);
}

void CMFCApplication1View::OnDraw(CDC* pDC)
{
    CMFCApplication1Doc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    CRect rect;
    GetClientRect(&rect);

    // 绘制行号区域
    DrawLineNumbers(pDC, rect);

    // 设置文本区域
    CRect rectText = rect;
    rectText.left = m_nLineNumberWidth;

    // 填充文本背景
    CBrush brushBg(m_crBackgroundColor);
    pDC->FillRect(rectText, &brushBg);

    // 设置文本字体和颜色
    CFont* pOldFont = pDC->SelectObject(&m_fontText);
    COLORREF crOldText = pDC->SetTextColor(m_crTextColor);
    int nOldBkMode = pDC->SetBkMode(TRANSPARENT);

    // 获取文本度量
    TEXTMETRIC tm;
    pDC->GetTextMetrics(&tm);
    int nCharHeight = tm.tmHeight + tm.tmExternalLeading;
    int nCharWidth = tm.tmAveCharWidth;

    // 计算可见区域
    CPoint scrollPos = GetScrollPosition();
    CRect rectClip;
    pDC->GetClipBox(rectClip);

    // 分割文本为行
    CStringArray lines;
    int nStart = 0;
    int nEnd = m_strContent.Find(_T('\n'));

    while (nEnd != -1)
    {
        lines.Add(m_strContent.Mid(nStart, nEnd - nStart));
        nStart = nEnd + 1;
        nEnd = m_strContent.Find(_T('\n'), nStart);
    }

    // 添加最后一行
    lines.Add(m_strContent.Mid(nStart));

    // 计算可见行范围
    int nFirstVisibleLine = scrollPos.y / nCharHeight;
    int nLastVisibleLine = min((scrollPos.y + rect.Height() + nCharHeight - 1) / nCharHeight, lines.GetSize());

    // 绘制可见文本行
    for (int i = nFirstVisibleLine; i < nLastVisibleLine; i++)
    {
        CPoint pt;
        pt.x = m_nLineNumberWidth + 5 - scrollPos.x;
        pt.y = i * nCharHeight - scrollPos.y;

        pDC->TextOut(pt.x, pt.y, lines[i]);
    }

    // 绘制光标
    if (GetFocus() == this && m_bCursorVisible)
    {
        // 获取光标位置
        int nCursorLine, nCursorColumn;
        CharIndexToLineColumn(m_nCursorPos, nCursorLine, nCursorColumn);

        // 计算光标屏幕坐标
        int nCursorX = m_nLineNumberWidth + 5 + nCursorColumn * nCharWidth - scrollPos.x;
        int nCursorY = nCursorLine * nCharHeight - scrollPos.y;

        // 确保光标在可见区域内
        if (nCursorY >= -nCharHeight && nCursorY <= rect.Height())
        {
            // 绘制光标
            CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
            CPen* pOldPen = pDC->SelectObject(&pen);

            pDC->MoveTo(nCursorX, nCursorY);
            pDC->LineTo(nCursorX, nCursorY + nCharHeight);

            pDC->SelectObject(pOldPen);
        }
    }

    // 恢复原始设置
    pDC->SetBkMode(nOldBkMode);
    pDC->SetTextColor(crOldText);
    pDC->SelectObject(pOldFont);
}

void CMFCApplication1View::DrawLineNumbers(CDC* pDC, CRect& rect)
{
    int nSaveDC = pDC->SaveDC();

    CRect rectLineNumbers = rect;
    rectLineNumbers.right = m_nLineNumberWidth;

    // 填充背景
    CBrush brushBg(m_crLineNumberBg);
    pDC->FillRect(rectLineNumbers, &brushBg);

    // 绘制分隔线
    CPen penLine(PS_SOLID, 1, RGB(200, 200, 200));
    CPen* pOldPen = pDC->SelectObject(&penLine);
    pDC->MoveTo(rectLineNumbers.right, rect.top);
    pDC->LineTo(rectLineNumbers.right, rect.bottom);

    // 设置字体和颜色
    pDC->SelectObject(&m_fontLineNumber);
    pDC->SetTextColor(m_crLineNumberText);
    pDC->SetBkColor(m_crLineNumberBg);

    // 获取文本度量
    TEXTMETRIC tm;
    pDC->GetTextMetrics(&tm);
    int nCharHeight = tm.tmHeight + tm.tmExternalLeading;

    // 计算可见区域
    CPoint scrollPos = GetScrollPosition();
    CRect rectClip;
    pDC->GetClipBox(rectClip);

    // 计算总行数
    int nTotalLines = 0;
    int nPos = 0;
    while ((nPos = m_strContent.Find(_T('\n'), nPos)) != -1)
    {
        nTotalLines++;
        nPos++;
    }
    nTotalLines++; // 最后一行

    // 计算可见行范围
    int nFirstVisibleLine = scrollPos.y / nCharHeight;
    int nLastVisibleLine = min((scrollPos.y + rect.Height() + nCharHeight - 1) / nCharHeight, nTotalLines);

    // 绘制可见行号
    for (int i = nFirstVisibleLine; i < nLastVisibleLine; i++)
    {
        CRect rectLine;
        rectLine.left = rectLineNumbers.left + 5;
        rectLine.right = rectLineNumbers.right - 5;
        rectLine.top = i * nCharHeight - scrollPos.y;
        rectLine.bottom = rectLine.top + nCharHeight;

        CString strLine;
        strLine.Format(_T("%d"), i + 1);
        pDC->DrawText(strLine, rectLine, DT_RIGHT | DT_SINGLELINE | DT_VCENTER);
    }

    pDC->SelectObject(pOldPen);
    pDC->RestoreDC(nSaveDC);
}

void CMFCApplication1View::OnInitialUpdate()
{
    CScrollView::OnInitialUpdate();

    // 创建字体
    LOGFONT lf = { 0 };
    lf.lfHeight = -MulDiv(10, GetDeviceCaps(GetDC()->GetSafeHdc(), LOGPIXELSY), 72);
    lf.lfWeight = FW_NORMAL;
    lf.lfCharSet = DEFAULT_CHARSET;
    lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
    lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    lf.lfQuality = DEFAULT_QUALITY;
    lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
    _tcscpy_s(lf.lfFaceName, _T("Consolas"));

    // 创建文本字体
    m_fontText.CreateFontIndirect(&lf);

    // 创建行号字体
    lf.lfHeight = -MulDiv(9, GetDeviceCaps(GetDC()->GetSafeHdc(), LOGPIXELSY), 72);
    m_fontLineNumber.CreateFontIndirect(&lf);

    // 更新视图度量
    UpdateViewMetrics();

    // 更新文档标题
    CString strPath = GetDocument()->GetPathName();
    if (!strPath.IsEmpty())
    {
        CString strTitle;
        int nPos = strPath.ReverseFind('\\');
        if (nPos != -1)
            strTitle = strPath.Mid(nPos + 1);
        else
            strTitle = strPath;
        GetDocument()->SetTitle(strTitle);
    }
}

void CMFCApplication1View::UpdateViewMetrics()
{
    CClientDC dc(this);
    CFont* pOldFont = dc.SelectObject(&m_fontText);

    // 计算文本度量
    TEXTMETRIC tm;
    dc.GetTextMetrics(&tm);
    int nCharHeight = tm.tmHeight + tm.tmExternalLeading;
    int nCharWidth = tm.tmAveCharWidth;

    // 计算总行数
    int nTotalLines = 0;
    int nPos = 0;
    while ((nPos = m_strContent.Find(_T('\n'), nPos)) != -1)
    {
        nTotalLines++;
        nPos++;
    }
    nTotalLines++; // 最后一行

    // 计算最大行长度
    int nMaxLineLength = 0;
    nPos = 0;
    int nPrevPos = 0;
    while ((nPos = m_strContent.Find(_T('\n'), nPrevPos)) != -1)
    {
        int nLineLength = nPos - nPrevPos;
        if (nLineLength > nMaxLineLength)
            nMaxLineLength = nLineLength;
        nPrevPos = nPos + 1;
    }

    // 最后一行
    int nLastLineLength = m_strContent.GetLength() - nPrevPos;
    if (nLastLineLength > nMaxLineLength)
        nMaxLineLength = nLastLineLength;

    // 更新行号宽度
    CalculateLineNumberWidth();

    // 设置滚动大小
    CSize sizeTotal;
    sizeTotal.cx = m_nLineNumberWidth + 5 + nMaxLineLength * nCharWidth + 20; // 加一些边距
    sizeTotal.cy = nTotalLines * nCharHeight + 20;

    SetScrollSizes(MM_TEXT, sizeTotal);

    dc.SelectObject(pOldFont);
}

void CMFCApplication1View::CalculateLineNumberWidth()
{
    CClientDC dc(this);
    CFont* pOldFont = dc.SelectObject(&m_fontLineNumber);

    // 计算总行数
    int nTotalLines = 0;
    int nPos = 0;
    while ((nPos = m_strContent.Find(_T('\n'), nPos)) != -1)
    {
        nTotalLines++;
        nPos++;
    }
    nTotalLines++; // 最后一行

    // 计算最大行号所需的宽度
    CString strMaxLine;
    strMaxLine.Format(_T("%d"), nTotalLines);

    CSize size = dc.GetTextExtent(strMaxLine);
    m_nLineNumberWidth = size.cx + 20; // 增加一些边距

    dc.SelectObject(pOldFont);
}

void CMFCApplication1View::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // 在修改前保存当前状态（包括光标位置）
    m_undoStack.push_back(EditAction(m_strContent, m_nCursorPos));
    m_redoStack.clear();  // 清空重做栈

    switch (nChar)
    {
    case 8:  // Backspace
        if (m_nCursorPos > 0)
        {
            m_strContent.Delete(m_nCursorPos - 1, 1);
            m_nCursorPos--;
            Invalidate();
        }
        break;

    case 13: // Enter
        m_strContent.Insert(m_nCursorPos, _T('\n'));
        m_nCursorPos++;
        UpdateViewMetrics();
        Invalidate();
        break;

    default:
        if (nChar >= 32)
        {
            m_strContent.Insert(m_nCursorPos, (TCHAR)nChar);
            m_nCursorPos++;
            UpdateViewMetrics();
            Invalidate();
        }
        break;
    }

    CScrollView::OnChar(nChar, nRepCnt, nFlags);
}

void CMFCApplication1View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // 检查Ctrl+Z和Ctrl+Y快捷键
    BOOL bCtrlPressed = (GetKeyState(VK_CONTROL) & 0x8000) != 0;

    if (bCtrlPressed)
    {
        if (nChar == 'Z')  // Ctrl+Z 撤销
        {
            if (!m_undoStack.empty())
            {
                OnEditUndo();
                return;
            }
        }
        else if (nChar == 'Y')  // Ctrl+Y 重做
        {
            if (!m_redoStack.empty())
            {
                OnEditRedo();
                return;
            }
        }
    }
    switch (nChar)
    {
    case VK_LEFT:
        if (m_nCursorPos > 0)
        {
            m_nCursorPos--;
            Invalidate();
        }
        break;

    case VK_RIGHT:
        if (m_nCursorPos < m_strContent.GetLength())
        {
            m_nCursorPos++;
            Invalidate();
        }
        break;

    case VK_UP:
    {
        int nCurrentLine, nCurrentColumn;
        CharIndexToLineColumn(m_nCursorPos, nCurrentLine, nCurrentColumn);

        if (nCurrentLine > 0)
        {
            // 移动到上一行相同列位置
            m_nCursorPos = LineColumnToCharIndex(nCurrentLine - 1, nCurrentColumn);
            Invalidate();
        }
    }
    break;

    case VK_DOWN:
    {
        int nCurrentLine, nCurrentColumn;
        CharIndexToLineColumn(m_nCursorPos, nCurrentLine, nCurrentColumn);

        // 计算总行数
        int nTotalLines = 0;
        int nPos = 0;
        while ((nPos = m_strContent.Find(_T('\n'), nPos)) != -1)
        {
            nTotalLines++;
            nPos++;
        }
        nTotalLines++; // 最后一行

        if (nCurrentLine < nTotalLines - 1)
        {
            // 移动到下一行相同列位置
            m_nCursorPos = LineColumnToCharIndex(nCurrentLine + 1, nCurrentColumn);
            Invalidate();
        }
    }
    break;

    case VK_HOME:
    {
        int nCurrentLine, nCurrentColumn;
        CharIndexToLineColumn(m_nCursorPos, nCurrentLine, nCurrentColumn);

        // 移动到当前行开头
        m_nCursorPos = LineColumnToCharIndex(nCurrentLine, 0);
        Invalidate();
    }
    break;

    case VK_END:
    {
        int nCurrentLine, nCurrentColumn;
        CharIndexToLineColumn(m_nCursorPos, nCurrentLine, nCurrentColumn);

        // 计算当前行长度
        int nLineStart = LineColumnToCharIndex(nCurrentLine, 0);
        int nLineEnd = m_strContent.Find(_T('\n'), nLineStart);
        if (nLineEnd == -1)
            nLineEnd = m_strContent.GetLength();

        // 移动到当前行末尾
        m_nCursorPos = nLineEnd;
        Invalidate();
    }
    break;
    }

    CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMFCApplication1View::OnLButtonDown(UINT nFlags, CPoint point)
{
    SetFocus();  // 获取焦点

    // 获取滚动位置
    CPoint scrollPos = GetScrollPosition();

    // 获取文本度量
    CClientDC dc(this);
    CFont* pOldFont = dc.SelectObject(&m_fontText);
    TEXTMETRIC tm;
    dc.GetTextMetrics(&tm);
    int nCharHeight = tm.tmHeight + tm.tmExternalLeading;
    int nCharWidth = tm.tmAveCharWidth;

    // 计算点击位置对应的行和列
    int nClickLine = (point.y + scrollPos.y) / nCharHeight;
    int nClickColumn = (point.x + scrollPos.x - m_nLineNumberWidth - 5) / nCharWidth;

    // 确保列不小于0
    if (nClickColumn < 0)
        nClickColumn = 0;

    // 将行和列转换为字符索引
    m_nCursorPos = LineColumnToCharIndex(nClickLine, nClickColumn);

    // 重置光标闪烁
    m_bCursorVisible = TRUE;
    m_nCursorBlinkCounter = 0;

    dc.SelectObject(pOldFont);
    Invalidate();

    CScrollView::OnLButtonDown(nFlags, point);
}

void CMFCApplication1View::OnSetFocus(CWnd* pOldWnd)
{
    CScrollView::OnSetFocus(pOldWnd);

    // 获得焦点时显示光标
    m_bCursorVisible = TRUE;
    m_nCursorBlinkCounter = 0;
    Invalidate();
}

void CMFCApplication1View::OnKillFocus(CWnd* pNewWnd)
{
    CScrollView::OnKillFocus(pNewWnd);
    Invalidate();
}

void CMFCApplication1View::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
    Invalidate();
}

void CMFCApplication1View::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
    Invalidate();
}

void CMFCApplication1View::SetText(LPCTSTR lpszText)
{
    m_strContent = lpszText;
    m_nCursorPos = 0;
    UpdateViewMetrics();
    Invalidate();
}

void CMFCApplication1View::InsertChar(TCHAR ch)
{
    m_strContent.Insert(m_nCursorPos, ch);
    m_nCursorPos++;
    UpdateViewMetrics();
    Invalidate();
}

void CMFCApplication1View::DeleteChar()
{
    if (m_nCursorPos > 0)
    {
        m_strContent.Delete(m_nCursorPos - 1, 1);
        m_nCursorPos--;
        UpdateViewMetrics();
        Invalidate();
    }
}

void CMFCApplication1View::InsertString(LPCTSTR lpszText)
{
    m_strContent.Insert(m_nCursorPos, lpszText);
    m_nCursorPos += (int)_tcslen(lpszText);
    UpdateViewMetrics();
    Invalidate();
}

void CMFCApplication1View::MoveCursor(int nCharOffset)
{
    int nNewPos = m_nCursorPos + nCharOffset;
    if (nNewPos >= 0 && nNewPos <= m_strContent.GetLength())
    {
        m_nCursorPos = nNewPos;
        Invalidate();
    }
}

void CMFCApplication1View::MoveCursorTo(int nLine, int nColumn)
{
    m_nCursorPos = LineColumnToCharIndex(nLine, nColumn);
    Invalidate();
}

void CMFCApplication1View::GetCursorPosition(int& nLine, int& nColumn) const
{
    CharIndexToLineColumn(m_nCursorPos, nLine, nColumn);
}

int CMFCApplication1View::CharIndexToLineColumn(int nCharIndex, int& nLine, int& nColumn) const
{
    nLine = 0;
    nColumn = 0;

    int nCurrentPos = 0;
    int nPrevLineStart = 0;

    // 遍历查找换行符
    while (nCurrentPos < nCharIndex && nCurrentPos < m_strContent.GetLength())
    {
        if (m_strContent[nCurrentPos] == _T('\n'))
        {
            nLine++;
            nPrevLineStart = nCurrentPos + 1;
        }
        nCurrentPos++;
    }

    // 计算列
    nColumn = nCharIndex - nPrevLineStart;

    return nCharIndex;
}

int CMFCApplication1View::LineColumnToCharIndex(int nLine, int nColumn) const
{
    int nCurrentLine = 0;
    int nCurrentPos = 0;

    // 移动到指定行
    while (nCurrentLine < nLine && nCurrentPos < m_strContent.GetLength())
    {
        if (m_strContent[nCurrentPos] == _T('\n'))
            nCurrentLine++;
        nCurrentPos++;
    }

    // 如果在查找过程中到达文本末尾
    if (nCurrentLine < nLine)
        return m_strContent.GetLength();

    // 移动到指定列
    int nLineLength = 0;
    while (nCurrentPos < m_strContent.GetLength() && m_strContent[nCurrentPos] != _T('\n') && nLineLength < nColumn)
    {
        nCurrentPos++;
        nLineLength++;
    }

    // 如果列位置超过行长度，调整到行末
    if (nLineLength < nColumn)
    {
        // 已经到达行末，不需要调整
    }

    return nCurrentPos;
}

void CMFCApplication1View::OnEditCut()
{
    AfxMessageBox(_T("剪切功能待实现"));
}

void CMFCApplication1View::OnEditCopy()
{
    AfxMessageBox(_T("复制功能待实现"));
}

void CMFCApplication1View::OnEditPaste()
{
    AfxMessageBox(_T("粘贴功能待实现"));
}

void CMFCApplication1View::OnEditSelectAll()
{
    AfxMessageBox(_T("全选功能待实现"));
}

void CMFCApplication1View::OnEditUndo()
{
    if (!m_undoStack.empty())
    {
        // 将当前状态保存到重做栈
        m_redoStack.push_back(EditAction(m_strContent, m_nCursorPos));
        
        // 恢复撤销栈顶的状态
        EditAction& action = m_undoStack.back();
        m_strContent = action.strContent;
        m_nCursorPos = action.nCursorPos;
        
        m_undoStack.pop_back();

        UpdateViewMetrics();
        Invalidate();
    }
}

void CMFCApplication1View::OnEditRedo()
{
    if (!m_redoStack.empty())
    {
        // 将当前状态保存到撤销栈
        m_undoStack.push_back(EditAction(m_strContent, m_nCursorPos));

        // 恢复重做栈顶的状态
        EditAction& action = m_redoStack.back();
        m_strContent = action.strContent;
        m_nCursorPos = action.nCursorPos;

        m_redoStack.pop_back();

        UpdateViewMetrics();
        Invalidate();
    }
}

void CMFCApplication1View::OnUpdateEditUndo(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(!m_undoStack.empty());
}

void CMFCApplication1View::OnUpdateEditRedo(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(!m_redoStack.empty());
}

void CMFCApplication1View::OnUpdateEditCut(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(FALSE);
}

void CMFCApplication1View::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(FALSE);
}

void CMFCApplication1View::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(TRUE);
}

void CMFCApplication1View::OnUpdateEditSelectAll(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(TRUE);
}

BOOL CMFCApplication1View::OnPreparePrinting(CPrintInfo * pInfo)
{
    return DoPreparePrinting(pInfo);
}

void CMFCApplication1View::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
}

void CMFCApplication1View::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
}

#ifdef _DEBUG
void CMFCApplication1View::AssertValid() const
{
    CScrollView::AssertValid();
}

void CMFCApplication1View::Dump(CDumpContext& dc) const
{
    CScrollView::Dump(dc);
}

CMFCApplication1Doc* CMFCApplication1View::GetDocument() const
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication1Doc)));
    return (CMFCApplication1Doc*)m_pDocument;
}
#endif


// 最简单的搜索函数
void CMFCApplication1View::SearchText(const CString& strSearch)
{
    if (strSearch.IsEmpty())
    {
        AfxMessageBox(_T("请输入要搜索的内容"));
        return;
    }
    
    if (m_strContent.IsEmpty())
    {
        AfxMessageBox(_T("文档为空，无可搜索内容"));
        return;
    }
    
    // 从当前位置开始搜索
    int nFindPos = m_strContent.Find(strSearch, m_nCurrentSearchPos);
    
    if (nFindPos == -1)
    {
        // 从头开始搜索
        nFindPos = m_strContent.Find(strSearch);
        if (nFindPos == -1)
        {
            AfxMessageBox(_T("未找到匹配的文本"));
            m_nCurrentSearchPos = 0;
            return;
        }
    }
    
    // 找到匹配
    m_nCurrentSearchPos = nFindPos;
    m_nCursorPos = nFindPos;
    
    // 更新显示
    UpdateViewMetrics();
    Invalidate();
    
    // 简单滚动到光标位置
    CPoint pt = GetScrollPosition();
    SetScrollPos(SB_HORZ, 0);
    SetScrollPos(SB_VERT, 0);
}

// 最简单的替换函数
int CMFCApplication1View::ReplaceText(const CString& strSearch, const CString& strReplace)
{
    if (strSearch.IsEmpty())
    {
        AfxMessageBox(_T("请输入要搜索的内容"));
        return 0;
    }
    
    if (m_strContent.IsEmpty())
    {
        AfxMessageBox(_T("文档为空"));
        return 0;
    }
    
    // 找到匹配位置
    int nFindPos = m_strContent.Find(strSearch, m_nCurrentSearchPos);
    if (nFindPos == -1)
    {
        nFindPos = m_strContent.Find(strSearch);
        if (nFindPos == -1)
        {
            AfxMessageBox(_T("未找到匹配的文本"));
            return 0;
        }
    }
    
    // 保存到撤销栈
    m_undoStack.push_back(EditAction(m_strContent, m_nCursorPos));
    m_redoStack.clear();
    
    // 执行替换
    m_strContent.Delete(nFindPos, strSearch.GetLength());
    m_strContent.Insert(nFindPos, strReplace);
    
    // 更新光标位置
    m_nCursorPos = nFindPos;
    m_nCurrentSearchPos = nFindPos;
    
    // 更新显示
    UpdateViewMetrics();
    Invalidate();
    
    AfxMessageBox(_T("替换完成"));
    return 1;
}

// 查找菜单命令
void CMFCApplication1View::OnEditFind()
{
    CString strSearch;

    // 使用简单的输入对话框
    BOOL bOK = FALSE;
    CString strTemp;

    // 创建临时对话框
    CDialog dlgInput;

    // 创建一个简单的输入对话框
    CString strTitle = _T("查找");

    // 使用最简单的方法：使用 AfxGetApp()->GetMainWnd() 来获取父窗口
    CWnd* pParent = AfxGetMainWnd();

    // 创建一个简单输入框
    CInputDlg dlg;
    dlg.m_strPrompt = _T("请输入要查找的内容:");
    dlg.m_strInput = _T("");

    if (dlg.DoModal() == IDOK)
    {
        strSearch = dlg.m_strInput;
        if (!strSearch.IsEmpty())
        {
            // 最简单的搜索实现
            int nFindPos = m_strContent.Find(strSearch);
            if (nFindPos != -1)
            {
                m_nCursorPos = nFindPos;
                UpdateViewMetrics();
                Invalidate();
                AfxMessageBox(_T("找到匹配文本"));
            }
            else
            {
                AfxMessageBox(_T("未找到匹配文本"));
            }
        }
    }
}


// 替换菜单命令
void CMFCApplication1View::OnEditReplace()
{
    CString strSearch, strReplace;

    // 获取查找内容
    {
        CInputDlg dlg1;
        dlg1.m_strPrompt = _T("请输入要查找的内容:");
        dlg1.m_strInput = _T("");

        if (dlg1.DoModal() == IDOK)
        {
            strSearch = dlg1.m_strInput;
        }
        else
        {
            return; // 用户取消
        }
    }

    // 获取替换内容
    {
        CInputDlg dlg2;
        dlg2.m_strPrompt = _T("请输入替换的内容:");
        dlg2.m_strInput = _T("");

        if (dlg2.DoModal() == IDOK)
        {
            strReplace = dlg2.m_strInput;
        }
        else
        {
            return; // 用户取消
        }
    }

    // 执行替换
    if (!strSearch.IsEmpty())
    {
        int nFindPos = m_strContent.Find(strSearch);
        if (nFindPos != -1)
        {
            // 保存到撤销栈
            m_undoStack.push_back(EditAction(m_strContent, m_nCursorPos));
            m_redoStack.clear();

            m_strContent.Delete(nFindPos, strSearch.GetLength());
            m_strContent.Insert(nFindPos, strReplace);

            UpdateViewMetrics();
            Invalidate();
            AfxMessageBox(_T("替换完成"));
        }
        else
        {
            AfxMessageBox(_T("未找到匹配文本"));
        }
    }
}