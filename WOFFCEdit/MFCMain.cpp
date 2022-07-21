#include "MFCMain.h"
#include "resource.h"


BEGIN_MESSAGE_MAP(MFCMain, CWinApp)
	ON_COMMAND(ID_FILE_QUIT,	&MFCMain::MenuFileQuit)
	ON_COMMAND(ID_FILE_SAVETERRAIN, &MFCMain::MenuFileSaveTerrain)
	ON_COMMAND(ID_EDIT_SELECT, &MFCMain::MenuEditSelect)
	ON_COMMAND(ID_EDIT_EDITOR_SETTINGS,	&MFCMain::MenuEditEditorSettings)
	ON_COMMAND(ID_EDIT_OPENINSPECTOR, &MFCMain::MenuEdtOpenInspector)
	ON_COMMAND(ID_GAMEOBJECT_INSERTNEWOBJECT, &MFCMain::MenuGameObjectInsertNewObject)
	ON_COMMAND(ID_BUTTON40001, &MFCMain::ToolBarButton1)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_TOOL, &CMyFrame::OnUpdatePage)

END_MESSAGE_MAP()

BOOL MFCMain::InitInstance()
{
	//instanciate the mfc frame
	m_frame = new CMyFrame();
	m_pMainWnd = m_frame;

	m_frame->Create(	NULL,
					_T("World Of Castle Crafting ~ Editor"),
					WS_OVERLAPPEDWINDOW,
					CRect(100, 100, 1024, 768),
					NULL,
					NULL,
					0,
					NULL
				);

	//show and set the window to run and update. 
	m_frame->ShowWindow(SW_SHOW);
	m_frame->UpdateWindow();


	//get the rect from the MFC window so we can get its dimensions
	m_toolHandle = m_frame->m_DirXView.GetSafeHwnd();				//handle of directX child window
	m_frame->m_DirXView.GetClientRect(&WindowRECT);
	m_width		= WindowRECT.Width();
	m_height	= WindowRECT.Height();

	m_ToolSystem.onActionInitialise(m_toolHandle, m_width, m_height);

	return TRUE;
}

int MFCMain::Run()
{
	MSG msg;
	BOOL bGotMsg;

	PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

	while (WM_QUIT != msg.message)
	{
		if (true)
		{
			bGotMsg = (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) != 0);
		}
		else
		{
			bGotMsg = (GetMessage(&msg, NULL, 0U, 0U) != 0);
		}

		if (bGotMsg)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			m_ToolSystem.UpdateInput(&msg);
		}
		else
		{				
			if (IsWindow(m_ToolInspector.GetSafeHwnd()))
				if (m_SelectedObjectIDs != m_ToolSystem.getCurrentSelectionID())
					m_ToolInspector.UpdateInspector();

			// if vecter of ids larger then 1 the alt message
			//std::wstring statusString = L"Selected Object: " + std::to_wstring(ID);
			std::wstring statusString;
			m_SelectedObjectIDs = m_ToolSystem.getCurrentSelectionID();

			if (m_SelectedObjectIDs.size() > 1) {
	
				// alt message if vecter of ids larger then 1  
				statusString = L"Selected Objects are: " + std::to_wstring(m_SelectedObjectIDs[0]);
				
				for (size_t i = 1; i < m_SelectedObjectIDs.size(); i++)
				{
					statusString = statusString + L", " + std::to_wstring(m_SelectedObjectIDs[i]);
				}
			}
			else if (m_SelectedObjectIDs.size() > 0) {
				
				statusString = L"Selected Object: " + std::to_wstring(m_SelectedObjectIDs[0]);
			}
			else {
				statusString = L"No Currently Selected Object";
			}

			
			m_ToolSystem.Tick(&msg);

			//send current object ID to status bar in The main frame
			m_frame->m_wndStatusBar.SetPaneText(2, statusString.c_str(), 1);	

			if (m_ToolSystem.m_d3dRenderer.m_isEditingObjects) {
				if (m_ToolSystem.m_d3dRenderer.m_isEditingPos) {
					statusString = L"Edit Mode: Position Editing";
				}
				else if (m_ToolSystem.m_d3dRenderer.m_isEditingRot) {
					statusString = L"Edit Mode: Rotation Editing";
				}
				else if (m_ToolSystem.m_d3dRenderer.m_isEditingScale) {
					statusString = L"Edit Mode: Scale Editing";
				}
			}
			else {
				statusString = L"No Edit ModeSelected";
			}
			
			m_frame->m_wndStatusBar.SetPaneText(1, statusString.c_str(), 1);
		}
	}

	return (int)msg.wParam;
}

void MFCMain::MenuFileQuit()
{
	//will post message to the message thread that will exit the application normally
	PostQuitMessage(0);
}

void MFCMain::MenuFileSaveTerrain()
{
	m_ToolSystem.onActionSaveTerrain();
	m_ToolSystem.onActionSave();
}

void MFCMain::MenuEditSelect()
{
	//SelectDialogue m_ToolSelectDialogue(NULL, &m_ToolSystem.m_sceneGraph);		//create our dialoguebox //modal constructor
	//m_ToolSelectDialogue.DoModal();	// start it up modal

	//modeless dialogue must be declared in the class.   If we do local it will go out of scope instantly and destroy itself
	m_ToolSelectDialogue.Create(IDD_SelectDialogue);	//Start up modeless
	m_ToolSelectDialogue.ShowWindow(SW_SHOW);	//show modeless
	m_ToolSelectDialogue.SetObjectData(&m_ToolSystem.m_sceneGraph, &m_ToolSystem.m_selectedObjects, &m_ToolSystem.m_UndoRedoSystem);
}

void MFCMain::MenuEditEditorSettings()
{
	//modeless dialogue must be declared in the class.   If we do local it will go out of scope instantly and destroy itself
	m_ToolEditorSettings.Create(IDD_EditorSettingsDialogue);	//Start up modeless
	m_ToolEditorSettings.ShowWindow(SW_SHOW);	//show modeless
	m_ToolEditorSettings.SetObjectData(&m_ToolSystem);
}

void MFCMain::MenuEdtOpenInspector()
{
	//modeless dialogue must be declared in the class.   If we do local it will go out of scope instantly and destroy itself
	m_ToolInspector.Create(IDD_InspectorParent);	//Start up modeless
	m_ToolInspector.ShowWindow(SW_SHOW);	//show modeless
	m_ToolInspector.SetObjectData(&m_ToolSystem);
}

void MFCMain::MenuGameObjectInsertNewObject()
{
	//modeless dialogue must be declared in the class.   If we do local it will go out of scope instantly and destroy itself
	m_ToolImportObjects.Create(IDD_ImportObjectDialogue);	//Start up modeless
	m_ToolImportObjects.ShowWindow(SW_SHOW);	//show modeless
	m_ToolImportObjects.SetObjectData(&m_ToolSystem);
}

void MFCMain::ToolBarButton1()
{
	
	m_ToolSystem.onActionSave();
}


MFCMain::MFCMain()
{
}


MFCMain::~MFCMain()
{
}
