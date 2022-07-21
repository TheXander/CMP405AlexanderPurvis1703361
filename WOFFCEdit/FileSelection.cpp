// FileSelection.cpp : implementation file
//

#include "FileSelection.h"



// FileSelection dialog

IMPLEMENT_DYNAMIC(FileSelection, CDialogEx)

FileSelection::FileSelection(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FileSelectionDialogue, pParent)
{
}

FileSelection::~FileSelection()
{
}

void FileSelection::SetObjectData(CEdit* newCEdit, CString newFileExtension)
{
	m_cEdit = newCEdit;
	m_fileExtension = newFileExtension;

	if (m_fileExtension[0] != '.') {
		m_fileExtension.Insert(0, '.');
	}
}

BOOL FileSelection::OnInitDialog()
{	
	CDialogEx::OnInitDialog();
	m_RootItem = m_FileTree.InsertItem(L"database");
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)EnumFileThread, this, 0, NULL);
	return TRUE;
}

void FileSelection::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FileTree, m_FileTree);
}


BEGIN_MESSAGE_MAP(FileSelection, CDialogEx)
	ON_BN_CLICKED(IDOK, &FileSelection::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &FileSelection::OnBnClickedCancel)
	ON_NOTIFY(TVN_SELCHANGED, IDC_FileTree, &FileSelection::OnTvnSelchangedFiletree)
END_MESSAGE_MAP()


// FileSelection message handlers
std::wstring FileSelection::GetParentDirectory(std::wstring childPath)
{
	return childPath.substr(0, childPath.find_last_of(L"\\/") + 1);
}

DWORD __stdcall FileSelection::EnumFileThread(LPARAM lparam)
{
	FileSelection* parent = (FileSelection*)lparam;
	
	TCHAR sizeBuffer[MAX_PATH] = { 0 };
	
	DWORD dwordLength = GetModuleFileName(NULL, sizeBuffer, MAX_PATH);
	std::wstring directoryPath = parent->GetParentDirectory(sizeBuffer);

	size_t position = directoryPath.rfind(L"Debug");
	if (position != std::wstring::npos)
		directoryPath.replace(position, directoryPath.length(), L"database\\");

	parent->FindFile(directoryPath.c_str(), parent->m_FileTree.GetRootItem());
	
	return 0;
}

void FileSelection::FindFile(CString rootPath, HTREEITEM rootItem)
{
	CFileFind fileFinder;
	CString filePath = rootPath + L"*.*"; // *.* means that we can look for all files

	HTREEITEM currentNode;

	BOOL result = fileFinder.FindFile(filePath);	//Searches the directory for a spisific file name
	
	while (result) {
		result = fileFinder.FindNextFile();	 //Continues the file search from the previous call to find the next file
		if (!fileFinder.IsDots()) {		// "." means file, ".." means a dirctory 
			CString fileName = fileFinder.GetFileName(); // gets the name and extention of the found file
			if (fileFinder.IsDirectory()) {
				CString filePath = fileFinder.GetFilePath() + L"\\*.*";// gets the whole path of the found file. Note: No clue why it has to be redeclared?
				currentNode = m_FileTree.InsertItem(fileName, rootItem);

				FindFile(filePath, currentNode);
			}
			else {
				if (fileName.Find(m_fileExtension) == fileName.GetLength() - m_fileExtension.GetLength()) {
					m_FileTree.InsertItem(fileName, rootItem);
				}
			}
		}
	}
}

void FileSelection::OnBnClickedOk()
{
	if (m_filePath.Find(m_fileExtension) == m_filePath.GetLength() - m_fileExtension.GetLength() && IsWindow(m_cEdit->GetSafeHwnd())) {		
		m_cEdit->SetWindowText(m_filePath);

		CDialogEx::OnOK();
		DestroyWindow();
	}
	else if (IsWindow(m_cEdit->GetSafeHwnd())) {
		MessageBox(L"Please ensure to select a valid "  + m_fileExtension + " file.", L"Warning: Invalid File Extension / No File Selected", MB_OK);
	}
	else {
		DestroyWindow();
	}
}


void FileSelection::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
	DestroyWindow();
}


void FileSelection::OnTvnSelchangedFiletree(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	CString filePath;
	HTREEITEM selectedItem = m_FileTree.GetSelectedItem();
	filePath.Insert(0, m_FileTree.GetItemText(selectedItem));

	while (true) {
		selectedItem = m_FileTree.GetParentItem(selectedItem);
		CString itemText = m_FileTree.GetItemText(selectedItem);

		if (selectedItem == m_RootItem) {
			filePath.Insert(0, itemText + '/');
			break;
		}
		else if (itemText == "") {
			filePath.Empty();
			break;
		}
		else {
			filePath.Insert(0, itemText + '/');
		}
	}
	
	m_filePath = filePath;

	*pResult = 0;
}
