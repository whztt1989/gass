#pragma once
#include "Sim/GASSCommon.h"
#include "Modules/Editor/EditorSceneManager.h"
#include "Sim/GASSBaseSceneComponent.h"
#include "Sim/GASSGraphicsMesh.h"
#include "Sim/Messages/GASSGraphicsSystemMessages.h"
#include "Sim/Messages/GASSGraphicsSceneObjectMessages.h"

#define GT_AXIS "GT_AXIS"
#define GT_PLANE "GT_PLANE"
#define GT_GRID "GT_GRID"
#define GT_FIXED_GRID "GT_FIXED_GRID"

namespace GASS
{
	class EditorModuleExport AxisGizmoComponent : public Reflection<AxisGizmoComponent,BaseSceneComponent>
	{
	public:
		AxisGizmoComponent();
		virtual ~AxisGizmoComponent();
		static void RegisterReflection();
		virtual void OnInitialize();
		virtual void OnDelete();
		Vec3 GetPosition(const Ray &ray);
		Quaternion GetRotation(float detla) const;
		void SetActive(bool active)  {m_Active =active;}
		bool GetActive() const {return m_Active;}
		GizmoEditMode GetMode() const {return m_Mode;}
	private:
		void OnChangeGridRequest(ChangeGridRequestPtr message);
		void OnCameraParameter(CameraParameterRequestPtr message);
		void OnLocationLoaded(LocationLoadedEventPtr message);
		void OnNewCursorInfo(CursorMovedOverSceneEventPtr message);
		void OnTransformation(TransformationChangedEventPtr message);
		void OnCameraMoved(TransformationChangedEventPtr message);
		void OnCameraChanged(CameraChangedEventPtr message);
		void OnSceneObjectSelected(ObjectSelectionChangedEventPtr message);
		void OnSelectedTransformation(TransformationChangedEventPtr message);
		void OnWorldPosition(WorldPositionRequestPtr message);
		void OnEditMode(EditModeChangedEventPtr message);

		void SetSelection(std::vector<SceneObjectWeakPtr>  selection);
		void BuildMesh();
		std::string GetType() const {return m_Type;}
		void SetType(const std::string &value) {m_Type = value;}
		float GetSize() const{return m_Size;}
		void SetSize(float value){m_Size =value;}
		ColorRGBA GetColor() const{return m_Color;}
		void SetColor(const ColorRGBA &value){m_Color =value;}
		void UpdateScale();
		Vec3 ProjectPointOnAxis(const Vec3 &axis_origin, const Vec3 &axis_dir, const Vec3 &p) const;
		Float SnapValue(Float value, Float snap);

		Quaternion m_BaseRot;
		GraphicsMeshPtr m_MeshData;
		ColorRGBA m_Color;
		float m_Size;
		std::string m_Type;

		//helpers
		Float m_LastDist;
		bool m_Highlight;
		GASS::SceneObjectWeakPtr m_ActiveCameraObject;
		GASS::SceneObjectWeakPtr m_SelectedObject;
		GizmoEditMode m_Mode;
		Float m_GridDist;
		bool m_Active;
		EditorSceneManagerPtr m_EditorSceneManager;
		std::string m_RegularMat;
		std::string m_HighlightMat;

	};

	typedef GASS_SHARED_PTR<AxisGizmoComponent> AxisGizmoComponentPtr;
}


