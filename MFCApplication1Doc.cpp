// MFCApplication1Doc.cpp: CMFCApplication1Doc 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFCApplication1.h"
#endif

#include "MFCApplication1Doc.h"

#include <propkey.h>
#include "MFCApplication1View.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMFCApplication1Doc

IMPLEMENT_DYNCREATE(CMFCApplication1Doc, CDocument)

BEGIN_MESSAGE_MAP(CMFCApplication1Doc, CDocument)
	ON_COMMAND(ID_FILE_SAVE_TEXT, &CMFCApplication1Doc::OnFileSaveText)
	ON_COMMAND(ID_FILE_SAVE_MYNOTE, &CMFCApplication1Doc::OnFileSaveMynote)
END_MESSAGE_MAP()

// RAII包装类
class CFileRAII
{
public:
	CFileRAII() : m_pFile(nullptr) {}

	BOOL Open(LPCTSTR lpszFileName, UINT nOpenFlags, CFileException* pError = NULL)
	{
		m_pFile = new CFile();
		return m_pFile->Open(lpszFileName, nOpenFlags, pError);
	}

	~CFileRAII()
	{
		if (m_pFile)
		{
			m_pFile->Close();
			delete m_pFile;
			m_pFile = nullptr;
		}
	}

	CFile* GetFile() { return m_pFile; }
	operator CFile* () { return m_pFile; }

private:
	CFile* m_pFile;
};

class CArchiveRAII
{
public:
	CArchiveRAII(CFile* pFile, UINT nMode, int nBufSize = 4096, void* lpBuf = NULL)
		: m_archive(pFile, nMode, nBufSize, lpBuf) {
	}

	~CArchiveRAII()
	{
		// CArchive析构时会自动关闭，不需要手动调用Close
	}

	CArchive* operator->() { return &m_archive; }
	CArchive& GetArchive() { return m_archive; }

private:
	CArchive m_archive;
};
// CMFCApplication1Doc 构造/析构

CMFCApplication1Doc::CMFCApplication1Doc() noexcept
{
	// TODO: 在此添加一次性构造代码

}

CMFCApplication1Doc::~CMFCApplication1Doc()
{
}

BOOL CMFCApplication1Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CMFCApplication1Doc 序列化

void CMFCApplication1Doc::Serialize(CArchive& ar)
{
	try
	{
		// 设置文档标题显示格式
		CString strPath = GetPathName();
		if (!strPath.IsEmpty())
		{
			if (strPath.Find(_T(".mynote")) != -1)
			{
				SetTitle(_T("[MyNote格式] ") + GetTitle());
			}
			else
			{
				SetTitle(_T("[文本格式] ") + GetTitle());
			}
		}

		POSITION pos = GetFirstViewPosition();
		if (pos != NULL)
		{
			CView* pView = GetNextView(pos);
			if (pView != NULL && pView->IsKindOf(RUNTIME_CLASS(CMFCApplication1View)))
			{
				CMFCApplication1View* pNotepadView = (CMFCApplication1View*)pView;

				// 判断文件格式
				BOOL bIsMyNote = strPath.Find(_T(".mynote")) != -1;

				if (ar.IsStoring())
				{
					// 保存文档
					CString strContent = pNotepadView->GetText();

					if (bIsMyNote)
					{
						// .mynote 格式：添加学号头和AES摘要尾
						CString strMyNoteContent;

						// 第一行：学号占位符
						strMyNoteContent = _T("<STUDENT_ID>\n");

						// 中间：实际文本内容
						strMyNoteContent += strContent;

						// 最后一行：AES-CBC加密的SHA-1摘要占位符
						if (!strContent.IsEmpty() && strContent[strContent.GetLength() - 1] != _T('\n'))
							strMyNoteContent += _T("\n");
						strMyNoteContent += _T("<SECRET_KEY><IV>");

						ar << strMyNoteContent;
					}
					else
					{
						// .txt 格式：直接保存纯文本
						ar << strContent;
					}
				}
				else
				{
					// 加载文档
					CString strContent;
					ar >> strContent;

					if (bIsMyNote)
					{
						// 处理.mynote格式
						// 1. 去掉第一行的学号占位符
						int nFirstNewLine = strContent.Find(_T('\n'));
						if (nFirstNewLine != -1)
						{
							// 去掉第一行
							strContent = strContent.Mid(nFirstNewLine + 1);

							// 2. 去掉最后一行的AES摘要占位符
							int nLastNewLine = strContent.ReverseFind(_T('\n'));
							if (nLastNewLine != -1)
							{
								// 检查是否是AES摘要行
								CString strLastLine = strContent.Mid(nLastNewLine + 1);
								if (strLastLine.Find(_T("<SECRET_KEY>")) != -1)
								{
									// 去掉最后一行
									strContent = strContent.Left(nLastNewLine);
								}
							}
						}
					}

					pNotepadView->SetText(strContent);
				}
			}
		}
	}
	catch (CArchiveException* e)
	{
		CString strError;
		strError.Format(_T("序列化错误: %d"), e->m_cause);
		AfxMessageBox(strError);
		e->Delete();
		throw; // 重新抛出异常，让调用者处理
	}
	catch (CException* e)
	{
		CString strError;
		e->GetErrorMessage(strError.GetBuffer(256), 256);
		strError.ReleaseBuffer();
		AfxMessageBox(strError);
		e->Delete();
		throw;
	}
	catch (...)
	{
		AfxMessageBox(_T("未知序列化错误"));
		throw;
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CMFCApplication1Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CMFCApplication1Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CMFCApplication1Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl* pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMFCApplication1Doc 诊断

#ifdef _DEBUG
void CMFCApplication1Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMFCApplication1Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMFCApplication1Doc 命令

// 保存为文本文件
void CMFCApplication1Doc::OnFileSaveText()
{
	try
	{
		CFileDialog dlg(FALSE, _T("txt"), _T("NewDocument.txt"),
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY,
			_T("Text Files (*.txt)|*.txt||"));

		if (dlg.DoModal() == IDOK)
		{
			CString strPath = dlg.GetPathName();

			// RAII包装
			CFileRAII file;
			CFileException fe;

			if (!file.Open(strPath, CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive, &fe))
			{
				CString strError;
				strError.Format(_T("无法创建文件: %s\n错误代码: %d"), strPath, fe.m_cause);
				AfxMessageBox(strError);
				return;
			}

			CArchiveRAII archive(file.GetFile(), CArchive::store);

			// 获取内容
			POSITION pos = GetFirstViewPosition();
			if (pos != NULL)
			{
				CView* pView = GetNextView(pos);
				if (pView != NULL && pView->IsKindOf(RUNTIME_CLASS(CMFCApplication1View)))
				{
					CMFCApplication1View* pNotepadView = (CMFCApplication1View*)pView;
					CString strContent = pNotepadView->GetText();
					archive.GetArchive() << strContent;
				}
			}

			// 设置文档信息
			SetPathName(strPath, FALSE);
			SetModifiedFlag(FALSE);

			// 更新标题
			SetTitle(_T("[文本格式] ") + GetTitle());

			AfxMessageBox(_T("文件保存成功！"));
		}
	}
	catch (CFileException* e)
	{
		CString strError;
		strError.Format(_T("文件操作错误: %d"), e->m_cause);
		AfxMessageBox(strError);
		e->Delete();
	}
	catch (CArchiveException* e)
	{
		CString strError;
		strError.Format(_T("序列化错误: %d"), e->m_cause);
		AfxMessageBox(strError);
		e->Delete();
	}
	catch (CException* e)
	{
		CString strError;
		e->GetErrorMessage(strError.GetBuffer(256), 256);
		strError.ReleaseBuffer();
		AfxMessageBox(strError);
		e->Delete();
	}
	catch (...)
	{
		AfxMessageBox(_T("未知错误"));
	}
}

// 保存为MyNote文件
void CMFCApplication1Doc::OnFileSaveMynote()
{
	try
	{
		CFileDialog dlg(FALSE, _T("mynote"), _T("NewDocument.mynote"),
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY,
			_T("MyNote Files (*.mynote)|*.mynote||"));

		if (dlg.DoModal() == IDOK)
		{
			CString strPath = dlg.GetPathName();

			// RAII包装
			CFileRAII file;
			CFileException fe;

			if (!file.Open(strPath, CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive, &fe))
			{
				CString strError;
				strError.Format(_T("无法创建文件: %s\n错误代码: %d"), strPath, fe.m_cause);
				AfxMessageBox(strError);
				return;
			}

			CArchiveRAII archive(file.GetFile(), CArchive::store);

			// 获取内容
			POSITION pos = GetFirstViewPosition();
			if (pos != NULL)
			{
				CView* pView = GetNextView(pos);
				if (pView != NULL && pView->IsKindOf(RUNTIME_CLASS(CMFCApplication1View)))
				{
					CMFCApplication1View* pNotepadView = (CMFCApplication1View*)pView;
					CString strContent = pNotepadView->GetText();

					// 构建.mynote格式内容
					CString strMyNoteContent = _T("<STUDENT_ID>\n")
						+ strContent
						+ ((!strContent.IsEmpty() && strContent[strContent.GetLength() - 1] != _T('\n')) ? _T("\n") : _T(""))
						+ _T("<SECRET_KEY><IV>");

					archive.GetArchive() << strMyNoteContent;
				}
			}

			// 设置文档信息
			SetPathName(strPath, FALSE);
			SetModifiedFlag(FALSE);

			// 更新标题
			SetTitle(_T("[MyNote格式] ") + GetTitle());

			AfxMessageBox(_T("文件保存成功！"));
		}
	}
	catch (CFileException* e)
	{
		CString strError;
		strError.Format(_T("文件操作错误: %d"), e->m_cause);
		AfxMessageBox(strError);
		e->Delete();
	}
	catch (CArchiveException* e)
	{
		CString strError;
		strError.Format(_T("序列化错误: %d"), e->m_cause);
		AfxMessageBox(strError);
		e->Delete();
	}
	catch (CException* e)
	{
		CString strError;
		e->GetErrorMessage(strError.GetBuffer(256), 256);
		strError.ReleaseBuffer();
		AfxMessageBox(strError);
		e->Delete();
	}
	catch (...)
	{
		AfxMessageBox(_T("未知错误"));
	}
}