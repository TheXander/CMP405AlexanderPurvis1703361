#pragma once
#include "afxdialogex.h"
#include "resource.h"

class ToolMain;

// ImportObjects dialog

class ImportObjects : public CDialogEx
{
	DECLARE_DYNAMIC(ImportObjects)

public:
	ImportObjects(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ImportObjects();

	void SetObjectData(ToolMain* toolMain);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ImportObjectDialogue };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	ToolMain* m_Tool;
	bool StrValid(CString inputStr);
	class FileSelection* m_ModelDialogue, *m_TextureDialogue;

public:
	// new import variables  
	CEdit m_ModelFilePathDisplay;
	CEdit m_TextureFilePathDisplay;

	CEdit m_NewImportPosX;
	CEdit m_NewImportPosY;
	CEdit m_NewImportPosZ;
	CEdit m_NewImportRotationX;
	CEdit m_NewImportRotationY;
	CEdit m_NewImportRotationZ;
	CEdit m_NewImportScaleX;
	CEdit m_NewImportScaleY;
	CEdit m_NewImportScaleZ;

	virtual BOOL OnInitDialog() override;

	afx_msg void OnBnClickedModelselectbtn();
	afx_msg void OnBnClickedTextureselectbtn();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	void RemoveTralingZeros(CString& inputStr);
};
