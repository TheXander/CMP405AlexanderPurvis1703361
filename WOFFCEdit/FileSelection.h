#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include <string>

// FileSelection dialog
class FileSelection : public CDialogEx
{
	DECLARE_DYNAMIC(FileSelection)

public:
	FileSelection(CWnd* pParent = nullptr);   // standard constructor
	virtual ~FileSelection();
	void SetObjectData(CEdit* newCEdit, CString fileExtension);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FileSelectionDialogue };
#endif

protected:
	CEdit* m_cEdit;
	CString m_filePath, m_fileExtension;

	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support	

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_FileTree;
	HTREEITEM m_RootItem, m_TreeItem;

	static std::wstring GetParentDirectory(std::wstring childPath);
	static DWORD WINAPI EnumFileThread(LPARAM lparam);

	void FindFile(CString rootPath, HTREEITEM rootItem);

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnTvnSelchangedFiletree(NMHDR* pNMHDR, LRESULT* pResult);
};
