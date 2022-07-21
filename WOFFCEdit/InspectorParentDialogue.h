#pragma once
#include "resource.h"
#include "afxdialogex.h"

// InspectorParentDialogue dialog

class InspectorParentDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(InspectorParentDialogue)

public:
	InspectorParentDialogue(CWnd* pParent = nullptr);   // standard constructor
	virtual ~InspectorParentDialogue();
	void SetObjectData(class ToolMain* toolMain);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_InspectorParent };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	class InspectorChildDialogue* m_inspectorChildDialogue;
	DECLARE_MESSAGE_MAP()

public:

	void UpdateInspector();
	virtual BOOL OnInitDialog() override;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
