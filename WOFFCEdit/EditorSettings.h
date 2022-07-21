#pragma once
#include "resource.h"
#include "afxdialogex.h"
#include "ToolMain.h"

// EditorSettings dialog
class EditorSettings : public CDialogEx
{
	DECLARE_DYNAMIC(EditorSettings)

public:
	EditorSettings(CWnd* pParent = nullptr);   // standard constructor
	virtual ~EditorSettings();
	void SetObjectData(ToolMain* toolMain);


// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EditorSettingsDialogue };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:

	ToolMain* m_Tool;
	void RemoveTralingZeros(CString& inputStr);
	bool StrValid(CString inputStr);

public:
	CEdit m_CamSpeedBoxInput;
	CEdit m_CamRotationSpeedBoxInput;
	CEdit m_MouseSensitivityBoxInput;

	virtual BOOL OnInitDialog() override;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
