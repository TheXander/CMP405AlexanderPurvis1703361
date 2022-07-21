// EditorSettings.cpp : implementation file
//
#include "EditorSettings.h"

// EditorSettings dialog

IMPLEMENT_DYNAMIC(EditorSettings, CDialogEx)

EditorSettings::EditorSettings(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EditorSettingsDialogue, pParent)
{

}

EditorSettings::~EditorSettings()
{
}

void EditorSettings::SetObjectData(ToolMain* toolMain)
{
	m_Tool = toolMain;

	CString camSpeed, camRotationSpeed, mouseSensitivity;

	camSpeed.Format(_T("%f"), m_Tool->GetMoveSpeed());
	RemoveTralingZeros(camSpeed);
	m_CamSpeedBoxInput.SetWindowText(camSpeed);

	camRotationSpeed.Format(_T("%f"), m_Tool->GetRotationSpeed());
	RemoveTralingZeros(camRotationSpeed);
	m_CamRotationSpeedBoxInput.SetWindowText(camRotationSpeed);

	mouseSensitivity.Format(_T("%f"), m_Tool->GetMouseSensitivity());
	RemoveTralingZeros(mouseSensitivity);
	m_MouseSensitivityBoxInput.SetWindowText(mouseSensitivity);
}

void EditorSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CamSpeed, m_CamSpeedBoxInput);
	DDX_Control(pDX, IDC_CamRotationSpeed, m_CamRotationSpeedBoxInput);
	DDX_Control(pDX, IDC_MouseSensitivity, m_MouseSensitivityBoxInput);
}


BEGIN_MESSAGE_MAP(EditorSettings, CDialogEx)
	ON_BN_CLICKED(IDOK, &EditorSettings::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &EditorSettings::OnBnClickedCancel)
END_MESSAGE_MAP()


// EditorSettings message handlers
void EditorSettings::RemoveTralingZeros(CString& inputStr)
{
	while (true) {
		if (inputStr[CString::StringLength(inputStr) - 3] != '.' && inputStr[CString::StringLength(inputStr) -1] == '0') {
			inputStr.Delete((CString::StringLength(inputStr) - 1), 1);
			continue;
		}
		else {
			break;
		}
	}
}

bool EditorSettings::StrValid(CString inputStr)
{
	for (size_t i = 0; i < CString::StringLength(inputStr); i++)
	{
		if (inputStr[i] < 45 || inputStr[i] == 47 || inputStr[i] > 57) 
			return false;
	}

	return true;
}

BOOL EditorSettings::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	return TRUE;
}

void EditorSettings::OnBnClickedOk()
{
	CString camSpeed, camRotationSpeed, mouseSensitivity;
	
	m_CamSpeedBoxInput.GetWindowText(camSpeed);
	m_CamRotationSpeedBoxInput.GetWindowText(camRotationSpeed);
	m_MouseSensitivityBoxInput.GetWindowText(mouseSensitivity);
	
	if (StrValid(camSpeed) && StrValid(camRotationSpeed) && StrValid(mouseSensitivity)) {
		m_Tool->UpdateCamSettings(_ttof(camSpeed), _ttof(camRotationSpeed), _ttof(mouseSensitivity));
		
		CDialogEx::OnOK();
		DestroyWindow();
	}
	else {
		MessageBox(L"No Change was made. Please enter valid input eg no special characters, letters, or spaces.", L"Warning: Invalid Input", MB_OK);
		// reset values
		SetObjectData(m_Tool);
	}	
}


void EditorSettings::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
	DestroyWindow();
}
