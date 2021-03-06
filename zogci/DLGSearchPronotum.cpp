// DLGSearchPronotum.cpp : implementation file
//

#include "stdafx.h"
#include "zogci.h"
#include "DLGSearchPronotum.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLGSearchPronotum dialog


CDLGSearchPronotum::CDLGSearchPronotum(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGSearchPronotum::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLGSearchPronotum)
	m_pronotum_any = FALSE;
	m_pronotum_other = FALSE;
	m_colornum_1 = FALSE;
	m_colornum_2 = FALSE;
	m_colornum_3 = FALSE;
	m_colornum_4 = FALSE;
	m_colornum_any = FALSE;
	m_colornum_m4 = FALSE;
	m_spot_12 = FALSE;
	m_spot_34 = FALSE;
	m_spot_56 = FALSE;
	m_spot_78 = FALSE;
	m_spot_910 = FALSE;
	m_spot_any = FALSE;
	m_spot_m10 = FALSE;
	//}}AFX_DATA_INIT
}


void CDLGSearchPronotum::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGSearchPronotum)
	DDX_Control(pDX, IDC_CHECK_PRONOTUM_SPOTS, m_pronotum_spots);
	DDX_Control(pDX, IDC_CHECK_PRONOTUM_SOLID, m_pronotum_solid);
	DDX_Control(pDX, IDC_CHECK_PRONOTUM_2BIG, m_pronotum_2big);
	DDX_Check(pDX, IDC_CHECK_PRONOTUM_ANY, m_pronotum_any);
	DDX_Check(pDX, IDC_CHECK_PRONOTUM_OTHER, m_pronotum_other);
	DDX_Check(pDX, IDC_CHECK_COLOR_NUM_1, m_colornum_1);
	DDX_Check(pDX, IDC_CHECK_COLOR_NUM_2, m_colornum_2);
	DDX_Check(pDX, IDC_CHECK_COLOR_NUM_3, m_colornum_3);
	DDX_Check(pDX, IDC_CHECK_COLOR_NUM_4, m_colornum_4);
	DDX_Check(pDX, IDC_CHECK_COLOR_NUM_ANY, m_colornum_any);
	DDX_Check(pDX, IDC_CHECK_COLOR_NUM_M4, m_colornum_m4);
	DDX_Check(pDX, IDC_CHECK_SPOT_12, m_spot_12);
	DDX_Check(pDX, IDC_CHECK_SPOT_34, m_spot_34);
	DDX_Check(pDX, IDC_CHECK_SPOT_56, m_spot_56);
	DDX_Check(pDX, IDC_CHECK_SPOT_78, m_spot_78);
	DDX_Check(pDX, IDC_CHECK_SPOT_910, m_spot_910);
	DDX_Check(pDX, IDC_CHECK_SPOT_ANY, m_spot_any);
	DDX_Check(pDX, IDC_CHECK_SPOT_M10, m_spot_m10);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLGSearchPronotum, CDialog)
	//{{AFX_MSG_MAP(CDLGSearchPronotum)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGSearchPronotum message handlers
// CDLGSearchElytron message handlers
BOOL CDLGSearchPronotum::OnInitDialog()
{
	Language_SetWndStaticText(this);
	CDialog::OnInitDialog();

	HINSTANCE hi=AfxGetApp()->m_hInstance;
	HBITMAP hbitmap;
/////////////////////////////
	hbitmap = (HBITMAP)LoadBitmap(hi,	MAKEINTRESOURCE(IDB_PRONOTUM_SPOTS));
	m_pronotum_spots.SetBitmap(hbitmap);

	hbitmap = (HBITMAP)LoadBitmap(hi,	MAKEINTRESOURCE(IDB_PRONOTUM_SOLID));
	m_pronotum_solid.SetBitmap(hbitmap);

	hbitmap = (HBITMAP)LoadBitmap(hi,	MAKEINTRESOURCE(IDB_PRONOTUM_2BIG));
	m_pronotum_2big.SetBitmap(hbitmap);

	return TRUE;  // return TRUE  unless you set the focus to a control
}
