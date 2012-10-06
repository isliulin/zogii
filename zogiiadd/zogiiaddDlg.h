// zogiiaddDlg.h : header file
//

#if !defined(AFX_ZOGIIADDDLG_H__096C0B0E_E5CC_41EA_A1B6_05467B4FDF0D__INCLUDED_)
#define AFX_ZOGIIADDDLG_H__096C0B0E_E5CC_41EA_A1B6_05467B4FDF0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CZogiiaddDlg dialog

class CZogiiaddDlg : public CDialog
{
// Construction
public:
	CZogiiaddDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CZogiiaddDlg)
	enum { IDD = IDD_ZOGIIADD_DIALOG };
	CTreeCtrl	m_tree;
	CString	m_SubFamilyEN;
	CString	m_SpNameTW;
	CString	m_SpNameEN;
	CString	m_SpNameCN;
	CString	m_SubFamilyCN;
	CString	m_SubFamilyTW;
	CString	m_TextCNPath;
	CString	m_TextENPath;
	CString	m_TextTWPath;
	CString	m_PupaPicPath;
	CString	m_PupaPicInfo;
	CString	m_OvumPicPath;
	CString	m_OvumPicInfo;
	CString	m_OtherNameA;
	CString	m_OtherNameB;
	CString	m_OtherNameC;
	CString	m_OtherNameD;
	CString	m_OtherNameE;
	CString	m_OtherNameF;
	CString	m_NameCN;
	CString	m_NameEN;
	CString	m_NameTW;
	CString	m_LarvaPicPath;
	CString	m_LarvaPicInfo;
	CString	m_ImagoPicPath;
	CString	m_ImagoPicInfo;
	CTime	m_Date;
	long	m_Code;
	CString	m_GenusTW;
	CString	m_GenusEN;
	CString	m_GenusCN;
	CString	m_FoodNameF;
	CString	m_FoodNameE;
	CString	m_FoodNameD;
	CString	m_FoodNameC;
	CString	m_FoodNameB;
	CString	m_FoodNameA;
	CString	m_DiscoveryPicPath;
	CString	m_DiscoveryPicInfo;
	CString	m_DiscoveryName;
	CString	m_DataOutputPath;
	int		m_Food;
	int		m_ImagoColorA;
	int		m_ImagoColorB;
	int		m_ImagoColorC;
	int		m_ImagoColorD;
	int		m_ImagoColorNum;
	int		m_ImagoNo;
	int		m_ImagoSex;
	int		m_ImagoTexture;
	int		m_ImagoVillus;
	int		m_LarvaCalthrop;
	int		m_LarvaColorA;
	int		m_LarvaColorB;
	int		m_LarvaColorC;
	int		m_LarvaColorD;
	int		m_LarvaColorNum;
	int		m_LarvaInstar;
	int		m_LarvaNo;
	int		m_LarvaSex;
	int		m_OvumColor;
	int		m_OvumNo;
	int		m_OvumSex;
	int		m_PupaColorA;
	int		m_PupaColorB;
	int		m_PupaColorC;
	int		m_PupaColorD;
	int		m_PupaColorNum;
	int		m_PupaNo;
	int		m_PupaSex;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZogiiaddDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CZogiiaddDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBUTTONDiscoveryPicPath();
	afx_msg void OnBUTTONDiscoveryPicView();
	afx_msg void OnBUTTONDeleteData();
	afx_msg void OnBUTTONSaveData();
	afx_msg void OnBUTTONDataOutputPath();
	afx_msg void OnBUTTONDataOutput();
	afx_msg void OnBUTTONTextTWPath();
	afx_msg void OnBUTTONTextTWView();
	afx_msg void OnBUTTONTextCNPath();
	afx_msg void OnBUTTONTextCNView();
	afx_msg void OnBUTTONTextENPath();
	afx_msg void OnBUTTONTextENView();
	afx_msg void OnBUTTONImagoPicView();
	afx_msg void OnBUTTONImagoPicPath();
	afx_msg void OnBUTTONSaveImago();
	afx_msg void OnBUTTONDeleteImago();
	afx_msg void OnBUTTONLarvaPicView();
	afx_msg void OnBUTTONLarvaPicPath();
	afx_msg void OnBUTTONDeleteLarva();
	afx_msg void OnBUTTONSaveLarva();
	afx_msg void OnBUTTONSaveFood();
	afx_msg void OnBUTTONSaveOtherName();
	afx_msg void OnBUTTONOvumPicView();
	afx_msg void OnBUTTONOvumPicPath();
	afx_msg void OnBUTTONSaveOvum();
	afx_msg void OnBUTTONDeleteOvum();
	afx_msg void OnBUTTONPupaPicPath();
	afx_msg void OnBUTTONPupaPicView();
	afx_msg void OnBUTTONSavePupa();
	afx_msg void OnBUTTONDeletePupa();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZOGIIADDDLG_H__096C0B0E_E5CC_41EA_A1B6_05467B4FDF0D__INCLUDED_)
