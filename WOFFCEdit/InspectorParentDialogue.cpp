// InspectorParentDialogue.cpp : implementation file
//

#include "InspectorParentDialogue.h"
#include "InspectorChildDialogue.h"


// InspectorParentDialogue dialog

IMPLEMENT_DYNAMIC(InspectorParentDialogue, CDialogEx)

InspectorParentDialogue::InspectorParentDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_InspectorParent, pParent)
{
	m_inspectorChildDialogue = nullptr;
}

InspectorParentDialogue::~InspectorParentDialogue()
{
	delete m_inspectorChildDialogue;
}

void InspectorParentDialogue::SetObjectData(ToolMain* toolMain)
{
	m_inspectorChildDialogue->SetObjectData(toolMain);
}

void InspectorParentDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(InspectorParentDialogue, CDialogEx)
	ON_BN_CLICKED(IDOK, &InspectorParentDialogue::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &InspectorParentDialogue::OnBnClickedCancel)
END_MESSAGE_MAP()


// InspectorParentDialogue message handlers
void InspectorParentDialogue::UpdateInspector()
{
	m_inspectorChildDialogue->UpdateInspector();
}

BOOL InspectorParentDialogue::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_inspectorChildDialogue = new InspectorChildDialogue(this);
	CRect windowRect;
	GetDlgItem(IDC_InspectorParent)->GetWindowRect(windowRect);
	ScreenToClient(&windowRect);
	m_inspectorChildDialogue->MoveWindow(windowRect);

	return TRUE;
}

void InspectorParentDialogue::OnBnClickedOk()
{
	m_inspectorChildDialogue->ApplyChanges();
}


void InspectorParentDialogue::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
	DestroyWindow();
}
