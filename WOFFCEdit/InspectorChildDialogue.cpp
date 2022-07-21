// InspectorChildDialogue.cpp : implementation file
//


#include "InspectorChildDialogue.h"
#include "FileSelection.h"
#include "ToolMain.h"



// InspectorChildDialogue dialog

IMPLEMENT_DYNAMIC(InspectorChildDialogue, CDialogEx)

InspectorChildDialogue::InspectorChildDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_InspectorChild, pParent)
{
	m_ToolMain = nullptr;
	m_CurrentHeight = 0;
	m_ScrollPosition = 0;
	m_FileSelectionDialogue = nullptr;
	Create(IDD_InspectorChild, pParent);
}

InspectorChildDialogue::~InspectorChildDialogue()
{
	delete m_FileSelectionDialogue;
}

void InspectorChildDialogue::SetObjectData(ToolMain* toolMain)
{
	m_ToolMain = toolMain;
	UpdateInspector();
}

void InspectorChildDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ObjectID, m_IDBox);
	DDX_Control(pDX, IDC_ChunkID, m_ChunkID);
	DDX_Control(pDX, IDC_ObjectName, m_NameInputBox);
	DDX_Control(pDX, IDC_AttachmentParentID, m_AttachmentParentIDInputBox);
	DDX_Control(pDX, IDC_MeshFileAddress, m_ModelFilePathDisplayInputBox);
	DDX_Control(pDX, IDC_TextureFileAddress, m_TextureFilePathDisplayInputBox);
	DDX_Control(pDX, IDC_PositionX, m_PosXInputBox);
	DDX_Control(pDX, IDC_PositionY, m_PosYInputBox);
	DDX_Control(pDX, IDC_PositionZ, m_PosZInputBox);
	DDX_Control(pDX, IDC_RotationX, m_RotationXInputBox);
	DDX_Control(pDX, IDC_RotationY, m_RotationYInputBox);
	DDX_Control(pDX, IDC_RotationZ, m_RotationZInputBox);
	DDX_Control(pDX, IDC_ScaleX, m_ScaleXInputBox);
	DDX_Control(pDX, IDC_ScaleY, m_ScaleYInputBox);
	DDX_Control(pDX, IDC_ScaleZ, m_ScaleZInputBox);
	DDX_Control(pDX, IDC_PivSnapX, m_PivSnapXInputBox);
	DDX_Control(pDX, IDC_PivSnapY, m_PivSnapYInputBox);
	DDX_Control(pDX, IDC_PivSnapZ, m_PivSnapZInputBox);
	DDX_Control(pDX, IDC_GroundSnapingCheck, m_GroundSnappingCheck);
	DDX_Control(pDX, IDC_CollisionFileSelection, m_CollisionFileSelectionInputBox);
	DDX_Control(pDX, IDC_CollisionCheck, m_EnableCollisionCheck);
	DDX_Control(pDX, IDC_CollectableCheck, m_IsCollectableCheck);
	DDX_Control(pDX, IDC_DestructableCheck, m_IsDestructableCheck);
	DDX_Control(pDX, IDC_HealthValue, m_HealthValueInputBox);
	DDX_Control(pDX, IDC_LightingCombo, m_LightingTypeCombo);
	DDX_Control(pDX, IDC_LightingR, m_LightingColourRInputBox);
	DDX_Control(pDX, IDC_LightingG, m_LightingColourGInputBox);
	DDX_Control(pDX, IDC_LightingB, m_LightingColourBInputBox);
	DDX_Control(pDX, IDC_SpecularR, m_SpecularColourRInputBox);
	DDX_Control(pDX, IDC_SpecularG, m_SpecularColourGInputBox);
	DDX_Control(pDX, IDC_SpecularB, m_SpecularColourBInputBox);
	DDX_Control(pDX, IDC_SpotlightAngle, m_SpotlightAngleInputBox);
	DDX_Control(pDX, IDC_AttenuationConstant, m_AttenuationConstantInputBox);
	DDX_Control(pDX, IDC_AttenuationLinear, m_AttenuationLinearInputBox);
	DDX_Control(pDX, IDC_AttenuationQuadratic, m_AttenuationQuadraticInputBox);
	DDX_Control(pDX, IDC_RenderCheck, m_RenderCheck);
	DDX_Control(pDX, IDC_EditorRenderCheck, m_EditorRenderCheck);
	DDX_Control(pDX, IDC_CameraCheck, m_CameraCheck);
	DDX_Control(pDX, IDC_EditorWireframeCheck, m_EditorWireframeCheck);
	DDX_Control(pDX, IDC_TextureVisableCheck, m_TextureVisableCheck);
	DDX_Control(pDX, IDC_NormalsVisableCheck, m_NormalsVisableCheck);
	DDX_Control(pDX, IDC_CollisionVisableCheck, m_CollsisonVisableCheck);
	DDX_Control(pDX, IDC_PivotVisableCheck, m_PivotVisableCheck);
	DDX_Control(pDX, IDC_AudioFileSelection, m_AudioFileSelectionInputBox);
	DDX_Control(pDX, IDC_Volume, m_VolumeInputBox);
	DDX_Control(pDX, IDC_Pan, m_PanInputBox);
	DDX_Control(pDX, IDC_Pitch, m_PitchInputBox);
	DDX_Control(pDX, IDC_OneShotCheck, m_AudioOneShotCheck);
	DDX_Control(pDX, IDC_PlayOnInitaliseCheck, m_AudioPlayOnInitCheck);
	DDX_Control(pDX, IDC_PlayInEditorCheck, m_AudioInEditorCheck);
	DDX_Control(pDX, IDC_MinDistance, m_AudioMinDistanceInputBox);
	DDX_Control(pDX, IDC_MaxDistance, m_AudioMaxDistanceInputBox);
	DDX_Control(pDX, IDC_AINodeCheck, m_AINodeCheck);
	DDX_Control(pDX, IDC_PathNodeCheck, m_PathNodeCheck);
	DDX_Control(pDX, IDC_PathNodeStartCheck, m_PathNodeStartCheck);
	DDX_Control(pDX, IDC_PathNodeEnd, m_PathNodeEndCheck);
}

void InspectorChildDialogue::NewFileSelectionDialogue()
{
	if (IsWindow(m_FileSelectionDialogue->GetSafeHwnd())) {
		m_FileSelectionDialogue->DestroyWindow();
	}

	m_FileSelectionDialogue->Create(IDD_FileSelectionDialogue);
	m_FileSelectionDialogue->ShowWindow(SW_SHOW);
}

BOOL InspectorChildDialogue::DetectChangeInObject()
{
	if (m_OriginalObject.name != m_ObjectUpdate.name) {
		return TRUE;
	}

	if (m_OriginalObject.parent_id != m_ObjectUpdate.parent_id) {
		return TRUE;
	}

	if (m_OriginalObject.model_path != m_ObjectUpdate.model_path) {
		return TRUE;
	}

	if (m_OriginalObject.tex_diffuse_path != m_ObjectUpdate.tex_diffuse_path) {
		return TRUE;
	}

	if (m_OriginalObject.posX != m_ObjectUpdate.posX) {
		return TRUE;
	}

	if (m_OriginalObject.posY != m_ObjectUpdate.posY) {
		return TRUE;
	}

	if (m_OriginalObject.posZ != m_ObjectUpdate.posZ) {
		return TRUE;
	}

	if (m_OriginalObject.rotX != m_ObjectUpdate.rotX) {
		return TRUE;
	}

	if (m_OriginalObject.rotY != m_ObjectUpdate.rotY) {
		return TRUE;
	}

	if (m_OriginalObject.rotZ != m_ObjectUpdate.rotZ) {
		return TRUE;
	}

	if (m_OriginalObject.scaX != m_ObjectUpdate.scaX) {
		return TRUE;
	}

	if (m_OriginalObject.scaY != m_ObjectUpdate.scaY) {
		return TRUE;
	}

	if (m_OriginalObject.scaZ != m_ObjectUpdate.scaZ) {
		return TRUE;
	}

	if (m_OriginalObject.pivotX != m_ObjectUpdate.pivotX) {
		return TRUE;
	}

	if (m_OriginalObject.pivotY != m_ObjectUpdate.pivotY) {
		return TRUE;
	}

	if (m_OriginalObject.pivotZ != m_ObjectUpdate.pivotZ) {
		return TRUE;
	}

	if (m_OriginalObject.snapToGround != m_ObjectUpdate.snapToGround) {
		return TRUE;
	}

	if (m_OriginalObject.collision_mesh != m_ObjectUpdate.collision_mesh) {
		return TRUE;
	}

	if (m_OriginalObject.collision != m_ObjectUpdate.collision) {
		return TRUE;
	}

	if (m_OriginalObject.collectable != m_ObjectUpdate.collectable) {
		return TRUE;
	}

	if (m_OriginalObject.destructable != m_ObjectUpdate.destructable) {
		return TRUE;
	}

	if (m_OriginalObject.health_amount != m_ObjectUpdate.health_amount) {
		return TRUE;
	}

	if (m_OriginalObject.light_type != m_ObjectUpdate.light_type) {
		return TRUE;
	}

	if (m_OriginalObject.light_diffuse_r != m_ObjectUpdate.light_diffuse_r) {
		return TRUE;
	}

	if (m_OriginalObject.light_diffuse_g != m_ObjectUpdate.light_diffuse_g) {
		return TRUE;
	}

	if (m_OriginalObject.light_diffuse_b != m_ObjectUpdate.light_diffuse_b) {
		return TRUE;
	}

	if (m_OriginalObject.light_specular_r != m_ObjectUpdate.light_specular_r) {
		return TRUE;
	}

	if (m_OriginalObject.light_specular_g != m_ObjectUpdate.light_specular_g) {
		return TRUE;
	}

	if (m_OriginalObject.light_specular_b != m_ObjectUpdate.light_specular_b) {
		return TRUE;
	}

	if (m_OriginalObject.light_spot_cutoff != m_ObjectUpdate.light_spot_cutoff) {
		return TRUE;
	}

	if (m_OriginalObject.light_constant != m_ObjectUpdate.light_constant) {
		return TRUE;
	}

	if (m_OriginalObject.light_linear != m_ObjectUpdate.light_linear) {
		return TRUE;
	}

	if (m_OriginalObject.light_quadratic != m_ObjectUpdate.light_quadratic) {
		return TRUE;
	}

	if (m_OriginalObject.render != m_ObjectUpdate.render) {
		return TRUE;
	}

	if (m_OriginalObject.editor_render != m_ObjectUpdate.editor_render) {
		return TRUE;
	}

	if (m_OriginalObject.camera != m_ObjectUpdate.camera) {
		return TRUE;
	}

	if (m_OriginalObject.editor_wireframe != m_ObjectUpdate.editor_wireframe) {
		return TRUE;
	}

	if (m_OriginalObject.editor_texture_vis != m_ObjectUpdate.editor_texture_vis) {
		return TRUE;
	}

	if (m_OriginalObject.editor_normals_vis != m_ObjectUpdate.editor_normals_vis) {
		return TRUE;
	}

	if (m_OriginalObject.editor_collision_vis != m_ObjectUpdate.editor_collision_vis) {
		return TRUE;
	}

	if (m_OriginalObject.editor_pivot_vis != m_ObjectUpdate.editor_pivot_vis) {
		return TRUE;
	}

	if (m_OriginalObject.audio_path != m_ObjectUpdate.audio_path) {
		return TRUE;
	}

	if (m_OriginalObject.volume != m_ObjectUpdate.volume) {
		return TRUE;
	}

	if (m_OriginalObject.pan != m_ObjectUpdate.pan) {
		return TRUE;
	}

	if (m_OriginalObject.pitch != m_ObjectUpdate.pitch) {
		return TRUE;
	}

	if (m_OriginalObject.one_shot != m_ObjectUpdate.one_shot) {
		return TRUE;
	}

	if (m_OriginalObject.play_on_init != m_ObjectUpdate.play_on_init) {
		return TRUE;
	}

	if (m_OriginalObject.play_in_editor != m_ObjectUpdate.play_in_editor) {
		return TRUE;
	}

	if (m_OriginalObject.min_dist != m_ObjectUpdate.min_dist) {
		return TRUE;
	}

	if (m_OriginalObject.max_dist != m_ObjectUpdate.max_dist) {
		return TRUE;
	}

	if (m_OriginalObject.AINode != m_ObjectUpdate.AINode) {
		return TRUE;
	}

	if (m_OriginalObject.path_node != m_ObjectUpdate.path_node) {
		return TRUE;
	}

	if (m_OriginalObject.path_node_start != m_ObjectUpdate.path_node_start) {
		return TRUE;
	}

	if (m_OriginalObject.path_node_end != m_ObjectUpdate.path_node_end) {
		return TRUE;
	}


	return FALSE;
}


BOOL InspectorChildDialogue::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	GetWindowRect(m_WindowOriginalRect);

	m_FileSelectionDialogue = new FileSelection();

	m_LightingTypeCombo.AddString(L"-");
	m_LightingTypeCombo.AddString(L"Directional Light");
	m_LightingTypeCombo.AddString(L"Spot Light");
	m_LightingTypeCombo.AddString(L"Point Light");

	return TRUE;
}

template<typename T>
CString InspectorChildDialogue::ValueToCString(T inputValue, CString format)
{
	CString outputValue;
	outputValue.Format(format, inputValue);
	return outputValue;
}


BEGIN_MESSAGE_MAP(InspectorChildDialogue, CDialogEx)
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_MeshFileSelectButton, &InspectorChildDialogue::OnBnClickedMeshfileselectbutton)
	ON_BN_CLICKED(IDC_TextureFileSelectButton, &InspectorChildDialogue::OnBnClickedTexturefileselectbutton)
	ON_BN_CLICKED(IDC_CollisionFileSelectionButton, &InspectorChildDialogue::OnBnClickedCollisionfileselectionbutton)
	ON_BN_CLICKED(IDC_AudioFileSelectionButton, &InspectorChildDialogue::OnBnClickedAudiofileselectionbutton)
END_MESSAGE_MAP()


// InspectorChildDialogue message handlers

void InspectorChildDialogue::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int scrollPosChange = 0,
		maxPos = m_WindowOriginalRect.Height() - m_CurrentHeight;


	switch (nSBCode)
	{
	case SB_LINEUP:
		if (m_ScrollPosition <= 0)
			return;

		scrollPosChange = -min(max(maxPos / 20, 5), m_ScrollPosition);
		break;

	case SB_LINEDOWN:
		if (m_ScrollPosition >= maxPos)
			return;

		scrollPosChange = min(max(maxPos / 20, 5), maxPos - m_ScrollPosition);
		break;

	case SB_PAGEUP:
		if (m_ScrollPosition <= 0)
			return;
		scrollPosChange = -min(max(maxPos / 10, 5), m_ScrollPosition);
		break;

	case SB_PAGEDOWN:
		if (m_ScrollPosition >= maxPos)
			return;
		scrollPosChange = min(max(maxPos / 10, 5), maxPos - m_ScrollPosition);
		break;

	case SB_THUMBPOSITION:
		scrollPosChange = (int)nPos - m_ScrollPosition;
		break;

	default:
		return;
	}

	m_ScrollPosition += scrollPosChange;
	SetScrollPos(SB_VERT, m_ScrollPosition, TRUE);
	ScrollWindow(0, -scrollPosChange);

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}


void InspectorChildDialogue::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	m_CurrentHeight = cy;

	SCROLLINFO scrollInfo{};

	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_ALL;
	scrollInfo.nMin = 0;
	scrollInfo.nMax = m_WindowOriginalRect.Height();
	scrollInfo.nPage = cy;
	scrollInfo.nPos = 0;

	SetScrollInfo(SB_VERT, &scrollInfo, TRUE);
}


BOOL InspectorChildDialogue::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	int maxPos = m_WindowOriginalRect.Height() - m_CurrentHeight; 

	if (zDelta < 0) {
		if (m_ScrollPosition < maxPos) {
			zDelta = min(max(maxPos / 20, 5), maxPos - m_ScrollPosition);
			m_ScrollPosition += zDelta;

			SetScrollPos(SB_VERT, m_ScrollPosition, TRUE);
			ScrollWindow(0, -zDelta);
		}
	}
	else
	{
		if (m_ScrollPosition > 0) {
			zDelta = -min(max(maxPos / 20, 5), m_ScrollPosition);
			m_ScrollPosition += zDelta;

			SetScrollPos(SB_VERT, m_ScrollPosition, TRUE);
			ScrollWindow(0, -zDelta);
		}
	}

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

void InspectorChildDialogue::UpdateInspector()
{
	if (m_ToolMain) {
		if (m_ToolMain->m_selectedObjects.size() > 0) {
			// assign m_IDBox
			unsigned int firstObjectIndex = m_ToolMain->GetIndexFromID(m_ToolMain->m_selectedObjects[0]);
			CString compStr = ValueToCString(m_ToolMain->m_sceneGraph[firstObjectIndex].ID, L"%d"); // comparison string 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // If there are mutiple objects they will have mutiple IDs so set compStr = L"-"	
				compStr = L"Multiple Objects Selected";
			}
			m_IDBox.SetWindowText(compStr);

			// assign m_ChunkID
			compStr = ValueToCString(m_ToolMain->m_sceneGraph[firstObjectIndex].chunk_ID, L"%d"); // comparison string 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compStr != ValueToCString(m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].chunk_ID, L"%d")) { //if there are objects with different values make the string a "-"
						compStr = L"-";
						break;
					}
				}
			}
			m_ChunkID.SetWindowText(compStr);

			// assign m_NameInputBox
			compStr = m_ToolMain->m_sceneGraph[firstObjectIndex].name.c_str(); // comparison string 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compStr != m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].name.c_str()) { //if there are objects with different values make the string a "-"
						compStr = L"-";
						break;
					}
				}
			}
			m_NameInputBox.SetWindowText(compStr);

			// assign m_AttachmentParentIDInputBox
			compStr = ValueToCString(m_ToolMain->m_sceneGraph[firstObjectIndex].parent_id, L"%d"); // comparison string 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compStr != ValueToCString(m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].parent_id, L"%d")) { //if there are objects with different values make the string a "-"
						compStr = L"-";
						break;
					}
				}
			}
			m_AttachmentParentIDInputBox.SetWindowText(compStr);

			//Model and texture files

				// assign m_ModelFilePathDisplayInputBox
			compStr = m_ToolMain->m_sceneGraph[firstObjectIndex].model_path.c_str(); // comparison string 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compStr != m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].model_path.c_str()) { //if there are objects with different values make the string a "-"
						compStr = L"-";
						break;
					}
				}
			}
			m_ModelFilePathDisplayInputBox.SetWindowText(compStr);

			// assign m_TextureFilePathDisplayInputBox
			compStr = m_ToolMain->m_sceneGraph[firstObjectIndex].tex_diffuse_path.c_str(); // comparison string 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compStr != m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].tex_diffuse_path.c_str()) { //if there are objects with different values make the string a "-"
						compStr = L"-";
						break;
					}
				}
			}
			m_TextureFilePathDisplayInputBox.SetWindowText(compStr);

			// Transform
				// assign m_PosXInputBox
			compStr = ValueToCString(m_ToolMain->m_sceneGraph[firstObjectIndex].posX, L"%f"); // comparison string 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compStr != ValueToCString(m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].posX, L"%f")) { //if there are objects with different values make the string a "-"
						compStr = L"-";
						break;
					}
				}
			}
			if (compStr != L"-") {
				RemoveTralingZeros(compStr);
			}		
			m_PosXInputBox.SetWindowText(compStr);

			// assign m_PosYInputBox
			compStr = ValueToCString(m_ToolMain->m_sceneGraph[firstObjectIndex].posY, L"%f"); // comparison string 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compStr != ValueToCString(m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].posY, L"%f")) { //if there are objects with different values make the string a "-"
						compStr = L"-";
						break;
					}
				}
			}

			if (compStr != L"-") {
				RemoveTralingZeros(compStr);
			}
			m_PosYInputBox.SetWindowText(compStr);

			// assign m_PosZInputBox
			compStr = ValueToCString(m_ToolMain->m_sceneGraph[firstObjectIndex].posZ, L"%f"); // comparison string 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compStr != ValueToCString(m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].posZ, L"%f")) { //if there are objects with different values make the string a "-"
						compStr = L"-";
						break;
					}
				}
			}
			if (compStr != L"-") {
				RemoveTralingZeros(compStr);
			}
			m_PosZInputBox.SetWindowText(compStr);

			// assign m_RotationXInputBox
			compStr = ValueToCString(m_ToolMain->m_sceneGraph[firstObjectIndex].rotX, L"%f"); // comparison string 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compStr != ValueToCString(m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].rotX, L"%f")) { //if there are objects with different values make the string a "-"
						compStr = L"-";
						break;
					}
				}
			}
			if (compStr != L"-") {
				RemoveTralingZeros(compStr);
			}
			m_RotationXInputBox.SetWindowText(compStr);

			// assign m_RotationYInputBox
			compStr = ValueToCString(m_ToolMain->m_sceneGraph[firstObjectIndex].rotY, L"%f"); // comparison string 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compStr != ValueToCString(m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].rotY, L"%f")) { //if there are objects with different values make the string a "-"
						compStr = L"-";
						break;
					}
				}
			}
			if (compStr != L"-") {
				RemoveTralingZeros(compStr);
			}
			m_RotationYInputBox.SetWindowText(compStr);

			// assign m_RotationZInputBox
			compStr = ValueToCString(m_ToolMain->m_sceneGraph[firstObjectIndex].rotZ, L"%f"); // comparison string 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compStr != ValueToCString(m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].rotZ, L"%f")) { //if there are objects with different values make the string a "-"
						compStr = L"-";
						break;
					}
				}
			}
			RemoveTralingZeros(compStr);
			m_RotationZInputBox.SetWindowText(compStr);

			// assign m_ScaleXInputBox
			compStr = ValueToCString(m_ToolMain->m_sceneGraph[firstObjectIndex].scaX, L"%f"); // comparison string 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compStr != ValueToCString(m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].scaX, L"%f")) { //if there are objects with different values make the string a "-"
						compStr = L"-";
						break;
					}
				}
			}
			if (compStr != L"-") {
				RemoveTralingZeros(compStr);
			}
			m_ScaleXInputBox.SetWindowText(compStr);

			// assign m_ScaleYInputBox
			compStr = ValueToCString(m_ToolMain->m_sceneGraph[firstObjectIndex].scaY, L"%f"); // comparison string 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compStr != ValueToCString(m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].scaY, L"%f")) { //if there are objects with different values make the string a "-"
						compStr = L"-";
						break;
					}
				}
			}
			if (compStr != L"-") {
				RemoveTralingZeros(compStr);
			}
			m_ScaleYInputBox.SetWindowText(compStr);

			// assign m_ScaleZInputBox
			compStr = ValueToCString(m_ToolMain->m_sceneGraph[firstObjectIndex].scaZ, L"%f"); // comparison string 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compStr != ValueToCString(m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].scaZ, L"%f")) { //if there are objects with different values make the string a "-"
						compStr = L"-";
						break;
					}
				}
			}
			if (compStr != L"-") {
				RemoveTralingZeros(compStr);
			}
			m_ScaleZInputBox.SetWindowText(compStr);

			// assign m_PivSnapXInputBox
			compStr = ValueToCString(m_ToolMain->m_sceneGraph[firstObjectIndex].pivotX, L"%f"); // comparison string 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compStr != ValueToCString(m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].pivotX, L"%f")) { //if there are objects with different values make the string a "-"
						compStr = L"-";
						break;
					}
				}
			}
			if (compStr != L"-") {
				RemoveTralingZeros(compStr);
			}
			m_PivSnapXInputBox.SetWindowText(compStr);

			// assign m_PivSnapYInputBox
			compStr = ValueToCString(m_ToolMain->m_sceneGraph[firstObjectIndex].pivotY, L"%f"); // comparison string 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compStr != ValueToCString(m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].pivotY, L"%f")) { //if there are objects with different values make the string a "-"
						compStr = L"-";
						break;
					}
				}
			}
			if (compStr != L"-") {
				RemoveTralingZeros(compStr);
			}
			m_PivSnapYInputBox.SetWindowText(compStr);

			// assign m_PivSnapZInputBox
			compStr = ValueToCString(m_ToolMain->m_sceneGraph[firstObjectIndex].pivotZ, L"%f"); // comparison string 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compStr != ValueToCString(m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].pivotZ, L"%f")) { //if there are objects with different values make the string a "-"
						compStr = L"-";
						break;
					}
				}
			}
			if (compStr != L"-") {
				RemoveTralingZeros(compStr);
			}
			m_PivSnapZInputBox.SetWindowText(compStr);

			// Collision
				// assign m_GroundSnappingCheck
			int compCheck = m_ToolMain->m_sceneGraph[firstObjectIndex].snapToGround; // comparison int 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compCheck != m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].snapToGround) { //if there are objects with different values make the string a "-"
						compCheck = BST_INDETERMINATE;
						break;
					}
				}
			}
			m_GroundSnappingCheck.SetCheck(compCheck);

			// assign m_CollisionFileSelectionInputBox
			compStr = m_ToolMain->m_sceneGraph[firstObjectIndex].collision_mesh.c_str(); // comparison string 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compStr != m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].collision_mesh.c_str()) { //if there are objects with different values make the string a "-"
						compStr = L"-";
						break;
					}
				}
			}
			m_CollisionFileSelectionInputBox.SetWindowText(compStr);

			// assign m_EnableCollisionCheck
			compCheck = m_ToolMain->m_sceneGraph[firstObjectIndex].collision; // comparison int 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compCheck != m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].collision) { //if there are objects with different values make the string a "-"
						compCheck = BST_INDETERMINATE;
						break;
					}
				}
			}
			m_EnableCollisionCheck.SetCheck(compCheck);

			// assign m_IsCollectableCheck
			compCheck = m_ToolMain->m_sceneGraph[firstObjectIndex].collectable; // comparison int 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compCheck != m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].collectable) { //if there are objects with different values make the string a "-"
						compCheck = BST_INDETERMINATE;
						break;
					}
				}
			}
			m_IsCollectableCheck.SetCheck(compCheck);

			// assign m_IsDestructableCheck
			compCheck = m_ToolMain->m_sceneGraph[firstObjectIndex].destructable; // comparison int 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compCheck != m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].destructable) { //if there are objects with different values make the string a "-"
						compCheck = BST_INDETERMINATE;
						break;
					}
				}
			}
			m_IsDestructableCheck.SetCheck(compCheck);

			// assign m_HealthValueInputBox
			compStr = ValueToCString(m_ToolMain->m_sceneGraph[firstObjectIndex].health_amount, L"%d"); // comparison string 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compStr != ValueToCString(m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].health_amount, L"%d")) { //if there are objects with different values make the string a "-"
						compStr = L"-";
						break;
					}
				}
			}
			m_HealthValueInputBox.SetWindowText(compStr);

			// Lighting
				// -----------------------combo Box------------------------------------------
			int compLightType = m_ToolMain->m_sceneGraph[firstObjectIndex].light_type;

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compLightType != m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].light_type) { //if there are objects with different values make the string a "-"
						compLightType = 0;
						break;
					}
				}
			}
			m_LightingTypeCombo.SetCurSel(compLightType);

			// assign m_LightingColourRInputBox
			compStr = ValueToCString(m_ToolMain->m_sceneGraph[firstObjectIndex].light_diffuse_r, L"%f"); // comparison string 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compStr != ValueToCString(m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].light_diffuse_r, L"%f")) { //if there are objects with different values make the string a "-"
						compStr = L"-";
						break;
					}
				}
			}
			if (compStr != L"-") {
				RemoveTralingZeros(compStr);
			}
			m_LightingColourRInputBox.SetWindowText(compStr);

			// assign m_LightingColourGInputBox
			compStr = ValueToCString(m_ToolMain->m_sceneGraph[firstObjectIndex].light_diffuse_g, L"%f"); // comparison string 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compStr != ValueToCString(m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].light_diffuse_g, L"%f")) { //if there are objects with different values make the string a "-"
						compStr = L"-";
						break;
					}
				}
			}
			if (compStr != L"-") {
				RemoveTralingZeros(compStr);
			}
			m_LightingColourGInputBox.SetWindowText(compStr);

			// assign m_LightingColourBInputBox
			compStr = ValueToCString(m_ToolMain->m_sceneGraph[firstObjectIndex].light_diffuse_b, L"%f"); // comparison string 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compStr != ValueToCString(m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].light_diffuse_b, L"%f")) { //if there are objects with different values make the string a "-"
						compStr = L"-";
						break;
					}
				}
			}
			if (compStr != L"-") {
				RemoveTralingZeros(compStr);
			}
			m_LightingColourBInputBox.SetWindowText(compStr);

			// assign m_SpecularColourRInputBox
			compStr = ValueToCString(m_ToolMain->m_sceneGraph[firstObjectIndex].light_specular_r, L"%f"); // comparison string 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compStr != ValueToCString(m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].light_specular_r, L"%f")) { //if there are objects with different values make the string a "-"
						compStr = L"-";
						break;
					}
				}
			}
			if (compStr != L"-") {
				RemoveTralingZeros(compStr);
			}
			m_SpecularColourRInputBox.SetWindowText(compStr);

			// assign m_SpecularColourGInputBox
			compStr = ValueToCString(m_ToolMain->m_sceneGraph[firstObjectIndex].light_specular_g, L"%f"); // comparison string 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compStr != ValueToCString(m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].light_specular_g, L"%f")) { //if there are objects with different values make the string a "-"
						compStr = L"-";
						break;
					}
				}
			}
			if (compStr != L"-") {
				RemoveTralingZeros(compStr);
			}
			m_SpecularColourGInputBox.SetWindowText(compStr);

			// assign m_SpecularColourBInputBox
			compStr = ValueToCString(m_ToolMain->m_sceneGraph[firstObjectIndex].light_specular_b, L"%f"); // comparison string 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compStr != ValueToCString(m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].light_specular_b, L"%f")) { //if there are objects with different values make the string a "-"
						compStr = L"-";
						break;
					}
				}
			}
			if (compStr != L"-") {
				RemoveTralingZeros(compStr);
			}
			m_SpecularColourBInputBox.SetWindowText(compStr);

			// assign m_SpotlightAngleInputBox
			compStr = ValueToCString(m_ToolMain->m_sceneGraph[firstObjectIndex].light_spot_cutoff, L"%f"); // comparison string 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compStr != ValueToCString(m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].light_spot_cutoff, L"%f")) { //if there are objects with different values make the string a "-"
						compStr = L"-";
						break;
					}
				}
			}
			if (compStr != L"-") {
				RemoveTralingZeros(compStr);
			}
			m_SpotlightAngleInputBox.SetWindowText(compStr);

			// assign m_AttenuationConstantInputBox
			compStr = ValueToCString(m_ToolMain->m_sceneGraph[firstObjectIndex].light_constant, L"%f"); // comparison string 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compStr != ValueToCString(m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].light_constant, L"%f")) { //if there are objects with different values make the string a "-"
						compStr = L"-";
						break;
					}
				}
			}
			if (compStr != L"-") {
				RemoveTralingZeros(compStr);
			}
			m_AttenuationConstantInputBox.SetWindowText(compStr);

			// assign m_AttenuationLinearInputBox
			compStr = ValueToCString(m_ToolMain->m_sceneGraph[firstObjectIndex].light_linear, L"%f"); // comparison string 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compStr != ValueToCString(m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].light_linear, L"%f")) { //if there are objects with different values make the string a "-"
						compStr = L"-";
						break;
					}
				}
			}
			if (compStr != L"-") {
				RemoveTralingZeros(compStr);
			}
			m_AttenuationLinearInputBox.SetWindowText(compStr);

			// assign m_AttenuationQuadraticInputBox
			compStr = ValueToCString(m_ToolMain->m_sceneGraph[firstObjectIndex].light_quadratic, L"%f"); // comparison string 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compStr != ValueToCString(m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].light_quadratic, L"%f")) { //if there are objects with different values make the string a "-"
						compStr = L"-";
						break;
					}
				}
			}
			if (compStr != L"-") {
				RemoveTralingZeros(compStr);
			}
			m_AttenuationQuadraticInputBox.SetWindowText(compStr);

			// Rendering

				// assign m_RenderCheck
			compCheck = m_ToolMain->m_sceneGraph[firstObjectIndex].render; // comparison int 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compCheck != m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].render) { //if there are objects with different values make the string a "-"
						compCheck = BST_INDETERMINATE;
						break;
					}
				}
			}
			m_RenderCheck.SetCheck(compCheck);

			// assign m_EditorRenderCheck
			compCheck = m_ToolMain->m_sceneGraph[firstObjectIndex].editor_render; // comparison int 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compCheck != m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].editor_render) { //if there are objects with different values make the string a "-"
						compCheck = BST_INDETERMINATE;
						break;
					}
				}
			}
			m_EditorRenderCheck.SetCheck(compCheck);

			// assign m_CameraCheck
			compCheck = m_ToolMain->m_sceneGraph[firstObjectIndex].camera; // comparison int 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compCheck != m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].camera) { //if there are objects with different values make the string a "-"
						compCheck = BST_INDETERMINATE;
						break;
					}
				}
			}
			m_CameraCheck.SetCheck(compCheck);

			// assign m_EditorWireframeCheck
			compCheck = m_ToolMain->m_sceneGraph[firstObjectIndex].editor_wireframe; // comparison int 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compCheck != m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].editor_wireframe) { //if there are objects with different values make the string a "-"
						compCheck = BST_INDETERMINATE;
						break;
					}
				}
			}
			m_EditorWireframeCheck.SetCheck(compCheck);

			// assign m_TextureVisableCheck
			compCheck = m_ToolMain->m_sceneGraph[firstObjectIndex].editor_texture_vis; // comparison int 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compCheck != m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].editor_texture_vis) { //if there are objects with different values make the string a "-"
						compCheck = BST_INDETERMINATE;
						break;
					}
				}
			}
			m_TextureVisableCheck.SetCheck(compCheck);

			// assign m_NormalsVisableCheck
			compCheck = m_ToolMain->m_sceneGraph[firstObjectIndex].editor_normals_vis; // comparison int 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compCheck != m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].editor_normals_vis) { //if there are objects with different values make the string a "-"
						compCheck = BST_INDETERMINATE;
						break;
					}
				}
			}
			m_NormalsVisableCheck.SetCheck(compCheck);

			// assign m_CollsisonVisableCheck
			compCheck = m_ToolMain->m_sceneGraph[firstObjectIndex].editor_collision_vis; // comparison int 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compCheck != m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].editor_collision_vis) { //if there are objects with different values make the string a "-"
						compCheck = BST_INDETERMINATE;
						break;
					}
				}
			}
			m_CollsisonVisableCheck.SetCheck(compCheck);

			// assign m_PivotVisableCheck
			compCheck = m_ToolMain->m_sceneGraph[firstObjectIndex].editor_collision_vis; // comparison int 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compCheck != m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].editor_collision_vis) { //if there are objects with different values make the string a "-"
						compCheck = BST_INDETERMINATE;
						break;
					}
				}
			}
			m_PivotVisableCheck.SetCheck(compCheck);

			//Audio
				   // assign m_AudioFileSelectionInputBox
			compStr = m_ToolMain->m_sceneGraph[firstObjectIndex].audio_path.c_str(); // comparison string 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compStr != m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].audio_path.c_str()) { //if there are objects with different values make the string a "-"
						compStr = L"-";
						break;
					}
				}
			}
			m_AudioFileSelectionInputBox.SetWindowText(compStr);

			// assign m_VolumeInputBox
			compStr = ValueToCString(m_ToolMain->m_sceneGraph[firstObjectIndex].volume, L"%f"); // comparison string 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compStr != ValueToCString(m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].volume, L"%f")) { //if there are objects with different values make the string a "-"
						compStr = L"-";
						break;
					}
				}
			}
			if (compStr != L"-") {
				RemoveTralingZeros(compStr);
			}
			m_VolumeInputBox.SetWindowText(compStr);

			// assign m_PanInputBox
			compStr = ValueToCString(m_ToolMain->m_sceneGraph[firstObjectIndex].pan, L"%f"); // comparison string 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compStr != ValueToCString(m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].pan, L"%f")) { //if there are objects with different values make the string a "-"
						compStr = L"-";
						break;
					}
				}
			}
			if (compStr != L"-") {
				RemoveTralingZeros(compStr);
			}
			m_PanInputBox.SetWindowText(compStr);

			// assign m_PitchInputBox
			compStr = ValueToCString(m_ToolMain->m_sceneGraph[firstObjectIndex].pitch, L"%f"); // comparison string 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compStr != ValueToCString(m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].pitch, L"%f")) { //if there are objects with different values make the string a "-"
						compStr = L"-";
						break;
					}
				}
			}
			if (compStr != L"-") {
				RemoveTralingZeros(compStr);
			}
			m_PitchInputBox.SetWindowText(compStr);

			// assign m_AudioOneShotCheck
			compCheck = m_ToolMain->m_sceneGraph[firstObjectIndex].one_shot; // comparison int 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compCheck != m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].one_shot) { //if there are objects with different values make the string a "-"
						compCheck = BST_INDETERMINATE;
						break;
					}
				}
			}
			m_AudioOneShotCheck.SetCheck(compCheck);

			// assign m_AudioPlayOnInitCheck
			compCheck = m_ToolMain->m_sceneGraph[firstObjectIndex].play_on_init; // comparison int 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compCheck != m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].play_on_init) { //if there are objects with different values make the string a "-"
						compCheck = BST_INDETERMINATE;
						break;
					}
				}
			}
			m_AudioPlayOnInitCheck.SetCheck(compCheck);

			// assign m_AudioInEditorCheck
			compCheck = m_ToolMain->m_sceneGraph[firstObjectIndex].play_in_editor; // comparison int 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compCheck != m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].play_in_editor) { //if there are objects with different values make the string a "-"
						compCheck = BST_INDETERMINATE;
						break;
					}
				}
			}
			m_AudioInEditorCheck.SetCheck(compCheck);

			// assign m_AudioMinDistanceInputBox
			compStr = ValueToCString(m_ToolMain->m_sceneGraph[firstObjectIndex].min_dist, L"%d"); // comparison string 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compStr != ValueToCString(m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].min_dist, L"%d")) { //if there are objects with different values make the string a "-"
						compStr = L"-";
						break;
					}
				}
			}
			m_AudioMinDistanceInputBox.SetWindowText(compStr);

			// assign m_AudioMaxDistanceInputBox
			compStr = ValueToCString(m_ToolMain->m_sceneGraph[firstObjectIndex].max_dist, L"%d"); // comparison string 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compStr != ValueToCString(m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].max_dist, L"%d")) { //if there are objects with different values make the string a "-"
						compStr = L"-";
						break;
					}
				}
			}
			m_AudioMaxDistanceInputBox.SetWindowText(compStr);

			//AI
				// assign m_AINodeCheck
			compCheck = m_ToolMain->m_sceneGraph[firstObjectIndex].AINode; // comparison int 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compCheck != m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].AINode) { //if there are objects with different values make the string a "-"
						compCheck = BST_INDETERMINATE;
						break;
					}
				}
			}
			m_AINodeCheck.SetCheck(compCheck);

			// assign m_PathNodeCheck
			compCheck = m_ToolMain->m_sceneGraph[firstObjectIndex].path_node; // comparison int 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compCheck != m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].path_node) { //if there are objects with different values make the string a "-"
						compCheck = BST_INDETERMINATE;
						break;
					}
				}
			}
			m_PathNodeCheck.SetCheck(compCheck);

			// assign m_PathNodeStartCheck
			compCheck = m_ToolMain->m_sceneGraph[firstObjectIndex].path_node_start; // comparison int 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compCheck != m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].path_node_start) { //if there are objects with different values make the string a "-"
						compCheck = BST_INDETERMINATE;
						break;
					}
				}
			}
			m_PathNodeStartCheck.SetCheck(compCheck);

			// assign m_PathNodeEndCheck
			compCheck = m_ToolMain->m_sceneGraph[firstObjectIndex].path_node_end; // comparison int 

			if (m_ToolMain->m_selectedObjects.size() > 1) { // cycle through all the objects to verify that they have the same value
				for (unsigned int objID : m_ToolMain->m_selectedObjects) {
					if (compCheck != m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(objID)].path_node_end) { //if there are objects with different values make the string a "-"
						compCheck = BST_INDETERMINATE;
						break;
					}
				}
			}
			m_PathNodeEndCheck.SetCheck(compCheck);
		}
		else {
			m_IDBox.SetWindowText(L"No Objects Selected");
			m_ChunkID.SetWindowText(L"");
			m_NameInputBox.SetWindowText(L"");
			m_AttachmentParentIDInputBox.SetWindowText(L"");
			m_ModelFilePathDisplayInputBox.SetWindowText(L"");
			m_TextureFilePathDisplayInputBox.SetWindowText(L"");
			m_PosXInputBox.SetWindowText(L"");
			m_PosYInputBox.SetWindowText(L"");
			m_PosZInputBox.SetWindowText(L"");
			m_RotationXInputBox.SetWindowText(L"");
			m_RotationYInputBox.SetWindowText(L"");
			m_RotationZInputBox.SetWindowText(L"");
			m_ScaleXInputBox.SetWindowText(L"");
			m_ScaleYInputBox.SetWindowText(L"");
			m_ScaleZInputBox.SetWindowText(L"");
			m_PivSnapXInputBox.SetWindowText(L"");
			m_PivSnapYInputBox.SetWindowText(L"");
			m_PivSnapZInputBox.SetWindowText(L"");
			m_GroundSnappingCheck.SetCheck(0);
			m_CollisionFileSelectionInputBox.SetWindowText(L"");
			m_EnableCollisionCheck.SetCheck(0);
			m_IsCollectableCheck.SetCheck(0);
			m_IsDestructableCheck.SetCheck(0);
			m_HealthValueInputBox.SetWindowText(L"");
			m_LightingTypeCombo.SetCurSel(0);
			m_LightingColourRInputBox.SetWindowText(L"");
			m_LightingColourGInputBox.SetWindowText(L"");
			m_LightingColourBInputBox.SetWindowText(L"");
			m_SpecularColourRInputBox.SetWindowText(L"");
			m_SpecularColourGInputBox.SetWindowText(L"");
			m_SpecularColourBInputBox.SetWindowText(L"");
			m_SpotlightAngleInputBox.SetWindowText(L"");
			m_AttenuationConstantInputBox.SetWindowText(L"");
			m_AttenuationLinearInputBox.SetWindowText(L"");
			m_AttenuationQuadraticInputBox.SetWindowText(L"");
			m_RenderCheck.SetCheck(0);
			m_EditorRenderCheck.SetCheck(0);
			m_CameraCheck.SetCheck(0);
			m_EditorWireframeCheck.SetCheck(0);
			m_TextureVisableCheck.SetCheck(0);
			m_NormalsVisableCheck.SetCheck(0);
			m_CollsisonVisableCheck.SetCheck(0);
			m_PivotVisableCheck.SetCheck(0);
			m_AudioFileSelectionInputBox.SetWindowText(L"");
			m_VolumeInputBox.SetWindowText(L"");
			m_PanInputBox.SetWindowText(L"");
			m_PitchInputBox.SetWindowText(L"");
			m_AudioOneShotCheck.SetCheck(0);
			m_AudioPlayOnInitCheck.SetCheck(0);
			m_AudioInEditorCheck.SetCheck(0);
			m_AudioMinDistanceInputBox.SetWindowText(L"");
			m_AudioMaxDistanceInputBox.SetWindowText(L"");
			m_AINodeCheck.SetCheck(0);
			m_PathNodeCheck.SetCheck(0);
			m_PathNodeStartCheck.SetCheck(0);
			m_PathNodeEndCheck.SetCheck(0);
		}
	}
}

void InspectorChildDialogue::ApplyChanges()
{
	if (m_ToolMain->m_selectedObjects.size() > 0) {
		// game object id and name
		CString name,
			attachmentParentID,

			// Mesh
			modelFilePathDisplay,
			// Texture
			textureFilePathDisplay,

			// Trasnform
			posX,
			posY,
			posZ,
			rotationX,
			rotationY,
			rotationZ,
			scaleX,
			scaleY,
			scaleZ,

			// snapping / pivot
			pivSnapX,
			pivSnapY,
			pivSnapZ,

			// collision   
			collisionFileSelection,
			healthValue,

			// lighting
			lightingColourR,
			lightingColourG,
			lightingColourB,

			specularColourR,
			specularColourG,
			specularColourB,
			spotlightAngle,

			attenuationConstant,
			attenuationLinear,
			attenuationQuadratic,

			// Audio
			audioFileSelection,
			volume,
			pan,
			pitch,
			audioMinDistance,
			audioMaxDistance;


		// retrieve data	 
		m_NameInputBox.GetWindowText(name);
		m_AttachmentParentIDInputBox.GetWindowText(attachmentParentID);

		m_ModelFilePathDisplayInputBox.GetWindowText(modelFilePathDisplay);
		m_TextureFilePathDisplayInputBox.GetWindowText(textureFilePathDisplay);

		m_PosXInputBox.GetWindowText(posX);
		m_PosYInputBox.GetWindowText(posY);
		m_PosZInputBox.GetWindowText(posZ);
		m_RotationXInputBox.GetWindowText(rotationX);
		m_RotationYInputBox.GetWindowText(rotationY);
		m_RotationZInputBox.GetWindowText(rotationZ);
		m_ScaleXInputBox.GetWindowText(scaleX);
		m_ScaleYInputBox.GetWindowText(scaleY);
		m_ScaleZInputBox.GetWindowText(scaleZ);

		m_PivSnapXInputBox.GetWindowText(pivSnapX);
		m_PivSnapYInputBox.GetWindowText(pivSnapY);
		m_PivSnapZInputBox.GetWindowText(pivSnapZ);

		m_CollisionFileSelectionInputBox.GetWindowText(collisionFileSelection);
		m_HealthValueInputBox.GetWindowText(healthValue);

		m_LightingColourRInputBox.GetWindowText(lightingColourR);
		m_LightingColourGInputBox.GetWindowText(lightingColourG);
		m_LightingColourBInputBox.GetWindowText(lightingColourB);

		m_SpecularColourRInputBox.GetWindowText(specularColourR);
		m_SpecularColourGInputBox.GetWindowText(specularColourG);
		m_SpecularColourBInputBox.GetWindowText(specularColourB);

		m_SpotlightAngleInputBox.GetWindowText(spotlightAngle);
		m_AttenuationConstantInputBox.GetWindowText(attenuationConstant);
		m_AttenuationLinearInputBox.GetWindowText(attenuationLinear);
		m_AttenuationQuadraticInputBox.GetWindowText(attenuationQuadratic);


		m_AudioFileSelectionInputBox.GetWindowText(audioFileSelection);
		m_VolumeInputBox.GetWindowText(volume);
		m_PanInputBox.GetWindowText(pan);
		m_PitchInputBox.GetWindowText(pitch);
		m_AudioMinDistanceInputBox.GetWindowText(audioMinDistance);
		m_AudioMaxDistanceInputBox.GetWindowText(audioMaxDistance);


		if (m_ToolMain) {
		
			std::vector<SceneObject> originalObjects;
			std::vector<SceneObject> objectsUpdates;
			
			for (size_t i = 0; i < m_ToolMain->m_selectedObjects.size(); i++)
			{		
				SceneObject updateData = m_ToolMain->m_sceneGraph[m_ToolMain->GetIndexFromID(m_ToolMain->m_selectedObjects[i])];
				originalObjects.push_back(updateData);

				if (name != CString("-"))
					updateData.name = std::string(CT2CA(name));

				if (attachmentParentID != CString("-"))
					updateData.parent_id = _ttoi(attachmentParentID);

				if (modelFilePathDisplay != CString("-"))
					if (modelFilePathDisplay.Find(L".cmo") == modelFilePathDisplay.GetLength() - 4 || modelFilePathDisplay == "")
						updateData.model_path = std::string(CT2CA(modelFilePathDisplay));

				if (textureFilePathDisplay != CString("-"))
					if (textureFilePathDisplay.Find(L".dds") == textureFilePathDisplay.GetLength() - 4 || textureFilePathDisplay == "")
						updateData.tex_diffuse_path = std::string(CT2CA(textureFilePathDisplay));

				if (posX != CString("-"))
					updateData.posX = _ttof(posX);
				if (posY != CString("-"))
					updateData.posY = _ttof(posY);
				if (posZ != CString("-"))
					updateData.posZ = _ttof(posZ);
				if (rotationX != CString("-"))
					updateData.rotX = _ttof(rotationX);
				if (rotationY != CString("-"))
					updateData.rotY = _ttof(rotationY);
				if (rotationZ != CString("-"))
					updateData.rotZ = _ttof(rotationZ);
				if (scaleX != CString("-"))
					updateData.scaX = _ttof(scaleX);
				if (scaleY != CString("-"))
					updateData.scaY = _ttof(scaleY);
				if (scaleZ != CString("-"))
					updateData.scaZ = _ttof(scaleZ);

				if (pivSnapX != CString("-"))
					updateData.pivotX = _ttof(pivSnapX);
				if (pivSnapY != CString("-"))
					updateData.pivotY = _ttof(pivSnapY);
				if (pivSnapZ != CString("-"))
					updateData.pivotZ = _ttof(pivSnapZ);
				if (m_GroundSnappingCheck.GetCheck() != BST_INDETERMINATE)
					updateData.snapToGround = m_GroundSnappingCheck.GetCheck();

				if (collisionFileSelection != CString("-"))
					if (collisionFileSelection.Find(L".cmo") == collisionFileSelection.GetLength() - 4 || collisionFileSelection == "")
						updateData.collision_mesh = std::string(CT2CA(collisionFileSelection));

				if (m_EnableCollisionCheck.GetCheck() != BST_INDETERMINATE)
					updateData.collision = m_EnableCollisionCheck.GetCheck();
				if (m_IsCollectableCheck.GetCheck() != BST_INDETERMINATE)
					updateData.collectable = m_IsCollectableCheck.GetCheck();
				if (m_IsDestructableCheck.GetCheck() != BST_INDETERMINATE)
					updateData.destructable = m_IsDestructableCheck.GetCheck();
				if (healthValue != CString("-"))
					updateData.health_amount = _ttoi(healthValue);

				if (m_LightingTypeCombo.GetCurSel() != 0)
					updateData.light_type = m_LightingTypeCombo.GetCurSel();

				if (lightingColourR != CString("-"))
					updateData.light_diffuse_r = _ttof(lightingColourR);
				if (lightingColourG != CString("-"))
					updateData.light_diffuse_g = _ttof(lightingColourG);
				if (lightingColourB != CString("-"))
					updateData.light_diffuse_b = _ttof(lightingColourB);

				if (specularColourR != CString("-"))
					updateData.light_specular_r = _ttof(specularColourR);
				if (specularColourG != CString("-"))
					updateData.light_specular_g = _ttof(specularColourG);
				if (specularColourB != CString("-"))
					updateData.light_specular_b = _ttof(specularColourB);

				if (spotlightAngle != CString("-"))
					updateData.light_spot_cutoff = _ttof(spotlightAngle);
				if (attenuationConstant != CString("-"))
					updateData.light_constant = _ttof(attenuationConstant);
				if (attenuationLinear != CString("-"))
					updateData.light_linear = _ttof(attenuationLinear);
				if (attenuationQuadratic != CString("-"))
					updateData.light_quadratic = _ttof(attenuationQuadratic);


				if (m_RenderCheck.GetCheck() != BST_INDETERMINATE)
					updateData.render = m_RenderCheck.GetCheck();
				if (m_EditorRenderCheck.GetCheck() != BST_INDETERMINATE)
					updateData.editor_render = m_EditorRenderCheck.GetCheck();
				if (m_CameraCheck.GetCheck() != BST_INDETERMINATE)
					updateData.camera = m_CameraCheck.GetCheck();
				if (m_EditorWireframeCheck.GetCheck() != BST_INDETERMINATE)
					updateData.editor_wireframe = m_EditorWireframeCheck.GetCheck();
				if (m_TextureVisableCheck.GetCheck() != BST_INDETERMINATE)
					updateData.editor_texture_vis = m_TextureVisableCheck.GetCheck();
				if (m_NormalsVisableCheck.GetCheck() != BST_INDETERMINATE)
					updateData.editor_normals_vis = m_NormalsVisableCheck.GetCheck();
				if (m_CollsisonVisableCheck.GetCheck() != BST_INDETERMINATE)
					updateData.editor_collision_vis = m_CollsisonVisableCheck.GetCheck();
				if (m_PivotVisableCheck.GetCheck() != BST_INDETERMINATE)
					updateData.editor_pivot_vis = m_PivotVisableCheck.GetCheck();

				if (audioFileSelection != CString("-"))
					if (audioFileSelection.Find(L".mp3") == audioFileSelection.GetLength() - 4 || audioFileSelection == "")
						updateData.audio_path = std::string(CT2CA(audioFileSelection));

				if (volume != CString("-"))
					updateData.volume = _ttof(volume);
				if (pan != CString("-"))
					updateData.pan = _ttof(pan);
				if (pitch != CString("-"))
					updateData.pitch = _ttof(pitch);
				if (m_AudioOneShotCheck.GetCheck() != BST_INDETERMINATE)
					updateData.one_shot = m_AudioOneShotCheck.GetCheck();
				if (m_AudioPlayOnInitCheck.GetCheck() != BST_INDETERMINATE)
					updateData.play_on_init = m_AudioPlayOnInitCheck.GetCheck();
				if (m_AudioInEditorCheck.GetCheck() != BST_INDETERMINATE)
					updateData.play_in_editor = m_AudioInEditorCheck.GetCheck();
				if (audioMinDistance != CString("-"))
					updateData.min_dist = _ttoi(audioMinDistance);
				if (audioMaxDistance != CString("-"))
					updateData.max_dist = _ttoi(audioMaxDistance);

				if (m_AINodeCheck.GetCheck() != BST_INDETERMINATE)
					updateData.AINode = m_AINodeCheck.GetCheck();
				if (m_PathNodeCheck.GetCheck() != BST_INDETERMINATE)
					updateData.path_node = m_PathNodeCheck.GetCheck();
				if (m_PathNodeStartCheck.GetCheck() != BST_INDETERMINATE)
					updateData.path_node_start = m_PathNodeStartCheck.GetCheck();
				if (m_PathNodeEndCheck.GetCheck() != BST_INDETERMINATE)
					updateData.path_node_end = m_PathNodeEndCheck.GetCheck();
				
				objectsUpdates.push_back(updateData);
			}

			std::vector<SceneObject> alteredObjects;
			std::vector<unsigned int> allUpdatedObjectIDs;


			for (size_t i = 0; i < originalObjects.size(); i++)
			{
				m_OriginalObject = originalObjects[i];
				m_ObjectUpdate = objectsUpdates[i];

				if (DetectChangeInObject()) {
					alteredObjects.push_back(objectsUpdates[i]);
					allUpdatedObjectIDs.push_back(objectsUpdates[i].ID);
				}					
			}

			if (allUpdatedObjectIDs.size() == alteredObjects.size() && allUpdatedObjectIDs.size() > 0) {
				m_ToolMain->onAcionUpdateObjectByID(allUpdatedObjectIDs, alteredObjects);
				m_ToolMain->RebuildDisplayList();
			}	
		}
	}
	else {
		MessageBox(L"Please ensure that at least one Object is selected", L"No Objects Selected", MB_OK);
	}
}



void InspectorChildDialogue::RemoveTralingZeros(CString& inputStr)
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


void InspectorChildDialogue::OnBnClickedMeshfileselectbutton()
{
	if (m_FileSelectionDialogue) {
		NewFileSelectionDialogue();
		m_FileSelectionDialogue->SetObjectData(&m_ModelFilePathDisplayInputBox, L".cmo");
	}	
}


void InspectorChildDialogue::OnBnClickedTexturefileselectbutton()
{
	if (m_FileSelectionDialogue) {
		NewFileSelectionDialogue();
		m_FileSelectionDialogue->SetObjectData(&m_TextureFilePathDisplayInputBox, L".dds");
	}
}


void InspectorChildDialogue::OnBnClickedCollisionfileselectionbutton()
{
	if (m_FileSelectionDialogue) {
		NewFileSelectionDialogue();
		m_FileSelectionDialogue->SetObjectData(&m_CollisionFileSelectionInputBox, L".cmo");
	}
}


void InspectorChildDialogue::OnBnClickedAudiofileselectionbutton()
{
	if (m_FileSelectionDialogue) {
		NewFileSelectionDialogue();
		m_FileSelectionDialogue->SetObjectData(&m_AudioFileSelectionInputBox, L".mp3");
	}
}
