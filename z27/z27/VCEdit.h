#pragma once

/// HEADER //////////////////////////////////////////

class CEditVC : public CEdit
{
public:
    CEditVC();

protected:
    CRect m_rectNCBottom;
    CRect m_rectNCTop;

public:
    virtual ~CEditVC();

protected:
    afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
    afx_msg void OnNcPaint();
    afx_msg UINT OnGetDlgCode();

    DECLARE_MESSAGE_MAP()
};
