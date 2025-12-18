#pragma once

class CMFCApplication1Doc : public CDocument
{
protected:
    CMFCApplication1Doc() noexcept;
    DECLARE_DYNCREATE(CMFCApplication1Doc)

public:
    virtual BOOL OnNewDocument();
    virtual void Serialize(CArchive& ar);

#ifdef SHARED_HANDLERS
    virtual void InitializeSearchContent();
    virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif

public:
    virtual ~CMFCApplication1Doc();
    afx_msg void OnFileSaveText();
    afx_msg void OnFileSaveMynote();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
    void SetSearchContent(const CString& value);
#endif
};