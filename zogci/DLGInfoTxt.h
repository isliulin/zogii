#if !defined(AFX_DLGINFOTXT_H__16513205_F705_40DA_A028_63C8F9D52B71__INCLUDED_)
#define AFX_DLGINFOTXT_H__16513205_F705_40DA_A028_63C8F9D52B71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGInfoTxt.h : header file
//
#include "DLGInfotxtInfo.h"
#include "DLGInfotxtPannel.h"
/////////////////////////////////////////////////////////////////////////////
// CDLGInfoTxt dialog

class CDLGInfoTxt : public CDialog
{
// Construction
public:
	CDLGInfoTxt(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDLGInfoTxt)
	enum { IDD = IDD_INFO_TXT };
	CString	m_edit_txt;
	//}}AFX_DATA

	CDLGInfotxtInfo	DlgInfotxtInfo;
	CDLGInfotxtPannel	DlgInfotxtPannel;

	BOOL	OnInitDialog();
	void AutoSize();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLGInfoTxt)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLGInfoTxt)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGINFOTXT_H__16513205_F705_40DA_A028_63C8F9D52B71__INCLUDED_)
