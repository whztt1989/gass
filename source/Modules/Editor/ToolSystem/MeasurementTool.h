#pragma once
#include "Sim/GASSCommon.h"
#include "IMouseTool.h"
#include "CursorInfo.h"

namespace GASS
{
	class MouseToolController;
	class SceneObject;
	class Component;
	typedef GASS_WEAK_PTR<SceneObject> SceneObjectWeakPtr;
	typedef GASS_SHARED_PTR<SceneObject> SceneObjectPtr;
	typedef GASS_WEAK_PTR<Component> ComponentWeakPtr;


	class EditorModuleExport MeasurementTool : public IMouseTool
	{
	public:
		MeasurementTool(MouseToolController* controller);
		virtual ~MeasurementTool(void);

		//IMouseTool interface
		virtual void MouseMoved(const MouseData &data, const SceneCursorInfo &info);
		virtual void MouseDown(const MouseData &data, const SceneCursorInfo &info);
		virtual void MouseUp(const MouseData &data, const SceneCursorInfo &info);
		virtual std::string GetName() {return TID_MEASUREMENT;}
		virtual void Stop() {};
		virtual void Start() {};
	private:
		SceneObjectPtr GetOrCreateRulerObject();
		void UpdateLine(const GASS::Vec3 &start, const GASS::Vec3 &end);
		bool m_MouseIsDown;
		GASS::SceneObjectWeakPtr m_RulerObject;
		MouseToolController* m_Controller;
		GASS::ComponentWeakPtr m_TextComp;
		GASS::Vec3 m_StartPos;

	};
}
