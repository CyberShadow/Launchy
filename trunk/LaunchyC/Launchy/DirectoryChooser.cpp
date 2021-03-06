// DirectoryChooser.cpp : implementation file
//

#include "stdafx.h"
#include "Launchy.h"
#include "DirectoryChooser.h"
#include "Options.h"
#include "LaunchyDlg.h"

// DirectoryChooser dialog

IMPLEMENT_DYNAMIC(DirectoryChooser, CDialog)

DirectoryChooser::DirectoryChooser(CWnd* pParent /*=NULL*/)
	: CDialog(DirectoryChooser::IDD, pParent)
{

}

DirectoryChooser::~DirectoryChooser()
{
}

void DirectoryChooser::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, Directories);
	DDX_Control(pDX, IDC_LIST2, Types);
	DDX_Control(pDX, IDC_EDIT1, TypeEdit);
	DDX_Control(pDX, IDC_INDEXED, numFiles);
}


BEGIN_MESSAGE_MAP(DirectoryChooser, CDialog)
	ON_BN_CLICKED(IDC_ADD_DIRECTORY, &DirectoryChooser::OnBnClickedAddDirectory)
	ON_BN_CLICKED(IDC_REMOVE_DIRECTORY, &DirectoryChooser::OnBnClickedRemoveDirectory)
	ON_BN_CLICKED(IDC_ADD_TYPE, &DirectoryChooser::OnBnClickedAddType)
	ON_BN_CLICKED(IDC_REMOVE_TYPE, &DirectoryChooser::OnBnClickedRemoveType)
	ON_BN_CLICKED(IDOK, &DirectoryChooser::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &DirectoryChooser::OnBnClickedCancel)
END_MESSAGE_MAP()


// DirectoryChooser message handlers

void DirectoryChooser::OnBnClickedAddDirectory()
{
	shared_ptr<Options> ops = ((CLaunchyDlg*)AfxGetMainWnd())->options;

	// TODO: Add your control notification handler code here
    BROWSEINFO bi = { 0 };
    bi.lpszTitle = _T("Pick a Directory");
	bi.hwndOwner = GetSafeHwnd();
    LPITEMIDLIST pidl = SHBrowseForFolder ( &bi );
    if ( pidl != 0 )
    {
        // get the name of the folder
        TCHAR path[MAX_PATH];
        if ( SHGetPathFromIDList ( pidl, path ) )
        {
            // ( "Selected Folder: %s\n", path );
			ops->Directories.push_back(path);
			Directories.AddString(path);
        }

        // free memory used
        IMalloc * imalloc = 0;
        if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
        {
            imalloc->Free ( pidl );
            imalloc->Release ( );
        }
    }
}

void DirectoryChooser::OnBnClickedRemoveDirectory()
{
	CString name;
	int item = Directories.GetCurSel();
	if (item < 0) {
		return;
	}
	Directories.GetText(Directories.GetCurSel(), name);
	shared_ptr<Options> ops = ((CLaunchyDlg*)AfxGetMainWnd())->options;

	for(vector<CString>::iterator it = ops->Directories.begin(); it != ops->Directories.end(); ) {
		if (*it == name) {
			ops->Directories.erase(it);
		} else {
			it++;
		}
	}

	Directories.DeleteString(item);
}

void DirectoryChooser::OnBnClickedAddType()
{
	shared_ptr<Options> ops = ((CLaunchyDlg*)AfxGetMainWnd())->options;
	CString txt;
	TypeEdit.GetWindowTextW(txt);
	if (txt == "")
		return;
	ops->Types.push_back(txt);
	Types.AddString(txt);
	TypeEdit.SetWindowTextW(_T(""));
}

void DirectoryChooser::OnBnClickedRemoveType()
{
	CString name;
	int item = Types.GetCurSel();
	if (item < 0) {
		return;
	}
	Types.GetText(Types.GetCurSel(), name);
	shared_ptr<Options> ops = ((CLaunchyDlg*)AfxGetMainWnd())->options;

	for(vector<CString>::iterator it = ops->Types.begin(); it != ops->Types.end(); ) {
		if (*it == name) {
			ops->Types.erase(it);
		} else {
			it++;
		}
	}

	Types.DeleteString(item);
}

BOOL DirectoryChooser::OnInitDialog()
{
	CDialog::OnInitDialog();

	shared_ptr<Options> ops = ((CLaunchyDlg*)AfxGetMainWnd())->options;
	shared_ptr<LaunchySmarts> smarts = ((CLaunchyDlg*)AfxGetMainWnd())->smarts;

	CString d;
	d.Format(_T("Indexing %d files"), smarts->catFiles);
	numFiles.SetWindowTextW(d);

	for(int i = 0; i < ops->Directories.size(); i++) {
		Directories.AddString(ops->Directories[i]);
	}

	for(int i = 0; i < ops->Types.size(); i++) {
		Types.AddString(ops->Types[i]);
	}

	dbak = ops->Directories;
	tbak = ops->Types; 

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void DirectoryChooser::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	shared_ptr<LaunchySmarts> smarts = ((CLaunchyDlg*)AfxGetMainWnd())->smarts;
	smarts->LoadCatalog();
	OnOK();
}


void DirectoryChooser::OnBnClickedCancel()
{
	shared_ptr<Options> ops = ((CLaunchyDlg*)AfxGetMainWnd())->options;
	ops->Directories = dbak;
	ops->Types = tbak;

	// TODO: Add your control notification handler code here
	OnCancel();
}

