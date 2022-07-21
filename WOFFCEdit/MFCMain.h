#pragma once

#include <afxwin.h> 
#include <afxext.h>
#include <afx.h>
#include "pch.h"
#include "Game.h"
#include "ToolMain.h"
#include "resource.h"
#include "MFCFrame.h"
#include "SelectDialogue.h"
#include "EditorSettings.h"
#include "ImportObjects.h"
#include "InspectorParentDialogue.h"

class MFCMain : public CWinApp 
{
public:
	MFCMain();
	~MFCMain();
	BOOL InitInstance();
	int  Run();

private:

	CMyFrame * m_frame;	//handle to the frame where all our UI is
	HWND m_toolHandle;	//Handle to the MFC window
	ToolMain m_ToolSystem;	//Instance of Tool System that we interface to. 
	CRect WindowRECT;	//Window area rectangle. 
	SelectDialogue m_ToolSelectDialogue; //for modeless dialogue, declare it here
	EditorSettings m_ToolEditorSettings;
	ImportObjects m_ToolImportObjects;
	InspectorParentDialogue m_ToolInspector;

	std::vector<unsigned int> m_SelectedObjectIDs;
	int m_width;		
	int m_height;
	
	//Interface funtions for menu and toolbar etc requires
	afx_msg void MenuFileQuit();
	afx_msg void MenuFileSaveTerrain();
	afx_msg void MenuEditSelect();
	afx_msg void MenuEditEditorSettings();
	afx_msg void MenuEdtOpenInspector();
	afx_msg	void MenuGameObjectInsertNewObject();
	afx_msg	void ToolBarButton1();


	DECLARE_MESSAGE_MAP()	// required macro for message map functionality  One per class
};