#pragma once
#include "resource.h"
#include "afxdialogex.h"
#include "SceneObject.h"
#include <vector>

// InspectorChildDialogue dialog

class InspectorChildDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(InspectorChildDialogue)

public:
	InspectorChildDialogue(CWnd* pParent = nullptr);   // standard constructor
	virtual ~InspectorChildDialogue();
	void SetObjectData(class ToolMain* toolMain);
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_InspectorChild };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	template<typename T>
	CString ValueToCString(T inputValue, CString format);
	void NewFileSelectionDialogue();
	DECLARE_MESSAGE_MAP()


private:
	class ToolMain* m_ToolMain;
	CRect m_WindowOriginalRect;
	
	int m_CurrentHeight;
	int m_ScrollPosition;

	class FileSelection* m_FileSelectionDialogue;
    
	BOOL DetectChangeInObject();

	SceneObject m_OriginalObject;
    SceneObject m_ObjectUpdate;

public:
	// game object id and name
	CEdit m_IDBox;
	CEdit m_ChunkID;
	CEdit m_NameInputBox;
	CEdit m_AttachmentParentIDInputBox;

	// Mesh
	CEdit m_ModelFilePathDisplayInputBox;
	// Texture
	CEdit m_TextureFilePathDisplayInputBox;

	// Trasnform
	CEdit m_PosXInputBox;
	CEdit m_PosYInputBox;
	CEdit m_PosZInputBox;
	CEdit m_RotationXInputBox;
	CEdit m_RotationYInputBox;
	CEdit m_RotationZInputBox;
	CEdit m_ScaleXInputBox;
	CEdit m_ScaleYInputBox;
	CEdit m_ScaleZInputBox;

	// snaping / pivits
	CEdit m_PivSnapXInputBox;
	CEdit m_PivSnapYInputBox;
	CEdit m_PivSnapZInputBox;

	CButton m_GroundSnappingCheck;

	// collision   
	CEdit m_CollisionFileSelectionInputBox;
	CButton m_EnableCollisionCheck;
	CButton m_IsCollectableCheck;
	CButton m_IsDestructableCheck;
	CEdit m_HealthValueInputBox;

	// lighting
	CComboBox m_LightingTypeCombo;

	CEdit m_LightingColourRInputBox;
	CEdit m_LightingColourGInputBox;
	CEdit m_LightingColourBInputBox;

	CEdit m_SpecularColourRInputBox;
	CEdit m_SpecularColourGInputBox;
	CEdit m_SpecularColourBInputBox;

	CEdit m_SpotlightAngleInputBox;
	CEdit m_AttenuationConstantInputBox;
	CEdit m_AttenuationLinearInputBox;
	CEdit m_AttenuationQuadraticInputBox;

	// Rendering
	CButton m_RenderCheck;
	CButton m_EditorRenderCheck;
	CButton m_CameraCheck;
	CButton m_EditorWireframeCheck;
	CButton m_TextureVisableCheck;
	CButton m_NormalsVisableCheck;
	CButton m_CollsisonVisableCheck;
	CButton m_PivotVisableCheck;

	// Audio
	CEdit m_AudioFileSelectionInputBox;
	CEdit m_VolumeInputBox;
	CEdit m_PanInputBox;
	CEdit m_PitchInputBox;
	CButton m_AudioOneShotCheck;
	CButton m_AudioPlayOnInitCheck;
	CButton m_AudioInEditorCheck;
	CEdit m_AudioMinDistanceInputBox;
	CEdit m_AudioMaxDistanceInputBox;

	// AI
	CButton m_AINodeCheck;
	CButton m_PathNodeCheck;
	CButton m_PathNodeStartCheck;
	CButton m_PathNodeEndCheck;

	virtual BOOL OnInitDialog() override;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	void UpdateInspector();
	void ApplyChanges();
	void RemoveTralingZeros(CString& inputStr);
	

	afx_msg void OnBnClickedMeshfileselectbutton();
	afx_msg void OnBnClickedTexturefileselectbutton();
	afx_msg void OnBnClickedCollisionfileselectionbutton();
	afx_msg void OnBnClickedAudiofileselectionbutton();
};


