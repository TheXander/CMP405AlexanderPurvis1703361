// ImportObjects.cpp : implementation file
//
#include "ImportObjects.h"
#include "ToolMain.h"
#include "FileSelection.h"
// ImportObjects dialog

IMPLEMENT_DYNAMIC(ImportObjects, CDialogEx)

ImportObjects::ImportObjects(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ImportObjectDialogue, pParent)
{

}

ImportObjects::~ImportObjects()
{
	delete m_ModelDialogue;
	delete m_TextureDialogue;
}

void ImportObjects::SetObjectData(ToolMain* toolMain)
{
	m_Tool = toolMain;

	float defaultPosAndRot = 0.00;
	float defaultScale = 1.00;

	CString defaultPosAndRotString;
	CString defaultScaleString;

	defaultPosAndRotString.Format(_T("%f"), defaultPosAndRot);
	RemoveTralingZeros(defaultPosAndRotString);
	defaultScaleString.Format(_T("%f"), defaultScale);
	RemoveTralingZeros(defaultScaleString);

	m_NewImportPosX.SetWindowText(defaultPosAndRotString);
	m_NewImportPosY.SetWindowText(defaultPosAndRotString);
	m_NewImportPosZ.SetWindowText(defaultPosAndRotString);
	m_NewImportRotationX.SetWindowText(defaultPosAndRotString);
	m_NewImportRotationY.SetWindowText(defaultPosAndRotString);
	m_NewImportRotationZ.SetWindowText(defaultPosAndRotString);
	m_NewImportScaleX.SetWindowText(defaultScaleString);
	m_NewImportScaleY.SetWindowText(defaultScaleString);
	m_NewImportScaleZ.SetWindowText(defaultScaleString);
}

void ImportObjects::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PositionX, m_NewImportPosX);
	DDX_Control(pDX, IDC_PositionY, m_NewImportPosY);
	DDX_Control(pDX, IDC_PositionZ, m_NewImportPosZ);
	DDX_Control(pDX, IDC_RotationX, m_NewImportRotationX);
	DDX_Control(pDX, IDC_RotationY, m_NewImportRotationY);
	DDX_Control(pDX, IDC_RotationZ, m_NewImportRotationZ);
	DDX_Control(pDX, IDC_ScaleX, m_NewImportScaleX);
	DDX_Control(pDX, IDC_ScaleY, m_NewImportScaleY);
	DDX_Control(pDX, IDC_ScaleZ, m_NewImportScaleZ);
	DDX_Control(pDX, IDC_ModelFilePath, m_ModelFilePathDisplay);
	DDX_Control(pDX, IDC_TextureFilePath, m_TextureFilePathDisplay);
}


BEGIN_MESSAGE_MAP(ImportObjects, CDialogEx)
	ON_BN_CLICKED(IDC_ModelSelectBtn, &ImportObjects::OnBnClickedModelselectbtn)
	ON_BN_CLICKED(IDC_TextureSelectBtn, &ImportObjects::OnBnClickedTextureselectbtn)
	ON_BN_CLICKED(IDOK, &ImportObjects::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &ImportObjects::OnBnClickedCancel)
END_MESSAGE_MAP()


BOOL ImportObjects::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_ModelDialogue = new FileSelection();
	m_TextureDialogue = new FileSelection();

	return 0;
}

// ImportObjects message handlers
void ImportObjects::OnBnClickedModelselectbtn()
{
	if (IsWindow(m_ModelDialogue->GetSafeHwnd()))
		m_ModelDialogue->DestroyWindow();

	m_ModelDialogue->Create(IDD_FileSelectionDialogue);	//Start up modeless
	m_ModelDialogue->ShowWindow(SW_SHOW);	//show modeless
	m_ModelDialogue->SetObjectData(&m_ModelFilePathDisplay, L"cmo");
}


void ImportObjects::OnBnClickedTextureselectbtn()
{
	if (IsWindow(m_TextureDialogue->GetSafeHwnd()))
		m_TextureDialogue->DestroyWindow();

	m_TextureDialogue->Create(IDD_FileSelectionDialogue);	//Start up modeless
	m_TextureDialogue->ShowWindow(SW_SHOW);	//show modeless
	m_TextureDialogue->SetObjectData(&m_TextureFilePathDisplay, L"dds");
}

bool ImportObjects::StrValid(CString inputStr)
{
	for (size_t i = 0; i < CString::StringLength(inputStr); i++)
	{
		if (inputStr[i] < 45 || inputStr[i] == 47 || inputStr[i] > 57)
			return false;
	}

	return true;
}

void ImportObjects::OnBnClickedOk()
{
	CString modelPath, texturePath,
			positionX, positionY, positionZ,
			rotationX, rotationY, rotationZ,
			scaleX, scaleY, scaleZ;
	
	m_ModelFilePathDisplay.GetWindowText(modelPath);
	m_TextureFilePathDisplay.GetWindowText(texturePath);

	m_NewImportPosX.GetWindowText(positionX);
	m_NewImportPosY.GetWindowText(positionY);
	m_NewImportPosZ.GetWindowText(positionZ);

	m_NewImportRotationX.GetWindowText(rotationX);
	m_NewImportRotationY.GetWindowText(rotationY);
	m_NewImportRotationZ.GetWindowText(rotationZ);

	m_NewImportScaleX.GetWindowText(scaleX);
	m_NewImportScaleY.GetWindowText(scaleY);
	m_NewImportScaleZ.GetWindowText(scaleZ);

	bool transformValid = StrValid(positionX) && StrValid(positionY) && StrValid(positionZ) &&
						  StrValid(rotationX) && StrValid(rotationY) && StrValid(rotationZ) &&
						  StrValid(scaleX) && StrValid(scaleY) && StrValid(scaleZ);

	if (transformValid){
		
		m_Tool->onActionInsertNewObject(modelPath, texturePath,
			_ttof(positionX), _ttof(positionY), _ttof(positionZ),
			_ttof(rotationX), _ttof(rotationY), _ttof(rotationZ),
			_ttof(scaleX), _ttof(scaleY), _ttof(scaleZ));

		CDialogEx::OnOK();
		DestroyWindow();
	}
	else {
		MessageBox(L"No Change was made. Please enter valid transform input eg no special characters, letters, or spaces.", L"Warning: Invalid Transform", MB_OK);
	}
}


void ImportObjects::OnBnClickedCancel()
{
	
	CDialogEx::OnCancel();
	DestroyWindow();
}

void ImportObjects::RemoveTralingZeros(CString& inputStr)
{
	while (true) {
		if (inputStr[CString::StringLength(inputStr) - 3] != '.' && inputStr[CString::StringLength(inputStr) - 1] == '0') {
			inputStr.Delete((CString::StringLength(inputStr) - 1), 1);
			continue;
		}
		else {
			break;
		}
	}
}
