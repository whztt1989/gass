/****************************************************************************
* This file is part of GASS.                                                *
* See https://github.com/leadcoder/gass                                     *
*                                                                           *
* Copyright (c) 2008-2016 GASS team. See Contributors.txt for details.      *
*                                                                           *
* GASS is free software: you can redistribute it and/or modify              *
* it under the terms of the GNU Lesser General Public License as published  *
* by the Free Software Foundation, either version 3 of the License, or      *
* (at your option) any later version.                                       *
*                                                                           *
* GASS is distributed in the hope that it will be useful,                   *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU Lesser General Public License for more details.                       *
*                                                                           *
* You should have received a copy of the GNU Lesser General Public License  *
* along with GASS. If not, see <http://www.gnu.org/licenses/>.              *
*****************************************************************************/



#include "Plugins/ODE/ODEBaseGeometryComponent.h"
#include "Plugins/ODE/ODEPhysicsSceneManager.h"
#include "Plugins/ODE/ODEBodyComponent.h"
#include "Core/ComponentSystem/GASSComponentFactory.h"
#include "Core/ComponentSystem/GASSComponentContainerTemplateManager.h"

#include "Core/MessageSystem/GASSMessageManager.h"
#include "Sim/GASSScene.h"
#include "Sim/GASSSceneObject.h"

#include "Sim/GASSSceneObjectTemplate.h"
#include "Sim/Interface/GASSIGeometryComponent.h"
#include "Sim/Interface/GASSILocationComponent.h"
#include "Sim/GASSSimEngine.h"
#ifdef _MSC_VER
#define NOMINMAX
//#include <algorithm>
#endif

namespace GASS
{
	ODEBaseGeometryComponent::ODEBaseGeometryComponent():
		m_ODESpaceID (NULL),
		m_Body (NULL),
		m_Friction(1),
		m_Offset(0,0,0),
		m_CollisionCategory(1),
		m_CollisionBits(1),
		m_GeomID(0),
		m_TransformGeomID(0),
		m_SizeFromMesh(true),
		m_Debug(false)
	{

	}

	ODEBaseGeometryComponent::~ODEBaseGeometryComponent()
	{
		
	}

	void ODEBaseGeometryComponent::RegisterReflection()
	{
		RegisterProperty<Vec3>("Offset", &GASS::ODEBaseGeometryComponent::GetOffset, &GASS::ODEBaseGeometryComponent::SetOffset);
		RegisterProperty<float>("Friction", &GASS::ODEBaseGeometryComponent::GetFriction, &GASS::ODEBaseGeometryComponent::SetFriction);
		RegisterProperty<bool>("SizeFromMesh", &GASS::ODEBaseGeometryComponent::GetSizeFromMesh, &GASS::ODEBaseGeometryComponent::SetSizeFromMesh);
		RegisterProperty<unsigned long>("CollisionBits", &GASS::ODEBaseGeometryComponent::GetCollisionBits, &GASS::ODEBaseGeometryComponent::SetCollisionBits);
		RegisterProperty<unsigned long>("CollisionCategory", &GASS::ODEBaseGeometryComponent::GetCollisionCategory, &GASS::ODEBaseGeometryComponent::SetCollisionCategory);
		RegisterProperty<bool>("Debug", &GASS::ODEBaseGeometryComponent::GetDebug, &GASS::ODEBaseGeometryComponent::SetDebug);
	}

	void ODEBaseGeometryComponent::OnInitialize()
	{
		//Try to figure out when to load
		m_Body = GetSceneObject()->GetFirstComponentByClass<ODEBodyComponent>().get();
		LocationComponentPtr location  = GetSceneObject()->GetFirstComponentByClass<ILocationComponent>();
		GeometryComponentPtr geom  = GetSceneObject()->GetFirstComponentByClass<IGeometryComponent>();
		
		if(m_Body)
		{
			if(m_SizeFromMesh)
				GetSceneObject()->RegisterForMessage(REG_TMESS(ODEBaseGeometryComponent::OnGeometryChanged,GeometryChangedEvent,0));
			else
				GetSceneObject()->RegisterForMessage(REG_TMESS(ODEBaseGeometryComponent::OnBodyLoaded,PhysicsBodyLoadedEvent,1));
		}
		else
		{
			if(m_SizeFromMesh && geom)
				GetSceneObject()->RegisterForMessage(REG_TMESS(ODEBaseGeometryComponent::OnGeometryChanged,GeometryChangedEvent,0));
			else
			{
				if(location)
					GetSceneObject()->RegisterForMessage(REG_TMESS(ODEBaseGeometryComponent::OnLocationLoaded,LocationLoadedEvent,1));
				else
				{
					ODEPhysicsSceneManagerPtr scene_manager = GetSceneObject()->GetScene()->GetFirstSceneManagerByClass<ODEPhysicsSceneManager>();
					assert(scene_manager);
					m_SceneManager = scene_manager;
					UpdateODEGeom();
					if(m_Debug) 
						SetDebug(true);
				}
			}
		}
		GetSceneObject()->RegisterForMessage(REG_TMESS(ODEBaseGeometryComponent::OnTransformationChanged,TransformationChangedEvent ,0));
		GetSceneObject()->RegisterForMessage(REG_TMESS(ODEBaseGeometryComponent::OnCollisionSettings,CollisionSettingsRequest ,0));
		GetSceneObject()->RegisterForMessage(REG_TMESS(ODEBaseGeometryComponent::OnPhysicsDebug,PhysicsDebugRequest,0));
	}

	void ODEBaseGeometryComponent::OnDelete()
	{
		Reset();
	}


	void ODEBaseGeometryComponent::OnBodyLoaded(PhysicsBodyLoadedEventPtr message)
	{
		ODEPhysicsSceneManagerPtr scene_manager = GetSceneObject()->GetScene()->GetFirstSceneManagerByClass<ODEPhysicsSceneManager>();
		assert(scene_manager);
		m_SceneManager = scene_manager;
		UpdateODEGeom();
		if(m_Debug) 
			SetDebug(true);
	}

	void ODEBaseGeometryComponent::OnLocationLoaded(LocationLoadedEventPtr message)
	{
		ODEPhysicsSceneManagerPtr scene_manager = GetSceneObject()->GetScene()->GetFirstSceneManagerByClass<ODEPhysicsSceneManager>();
		assert(scene_manager);
		m_SceneManager = scene_manager;
		UpdateODEGeom();
		if(m_Debug) 
			SetDebug(true);
	}

	void ODEBaseGeometryComponent::OnGeometryChanged(GeometryChangedEventPtr message)
	{
		ODEPhysicsSceneManagerPtr scene_manager = GetSceneObject()->GetScene()->GetFirstSceneManagerByClass<ODEPhysicsSceneManager>();
		assert(scene_manager);
		m_SceneManager = scene_manager;
		UpdateODEGeom();
		SetSizeFromMesh(true);
	}

	void ODEBaseGeometryComponent::OnTransformationChanged(TransformationChangedEventPtr message)
	{
		if(m_Body == 0) //only update position for static geometry 
		{
			
			//Reflect scaling			
			SetSizeFromMesh(m_SizeFromMesh);

			Vec3 pos = message->GetPosition();
			SetPosition(pos);
			Quaternion rot = message->GetRotation();
			SetRotation(rot);
		}
	}

	bool  ODEBaseGeometryComponent::GetSizeFromMesh() const
	{
		return m_SizeFromMesh;
	}

	void ODEBaseGeometryComponent::UpdateODEGeom()
	{
		Reset();
		m_Body = GetSceneObject()->GetFirstComponentByClass<ODEBodyComponent>().get();
		dSpaceID space = GetSpace();

		m_GeomID  = CreateODEGeom();

		m_TransformGeomID = dCreateGeomTransform(space);
		dGeomTransformSetCleanup(m_TransformGeomID, 1 );
		dGeomTransformSetGeom(m_TransformGeomID,m_GeomID);
		dGeomSetPosition(m_GeomID, m_Offset.x, m_Offset.y, m_Offset.z);

		if(m_Body)
		{
			dGeomSetBody(m_TransformGeomID, m_Body->GetODEBodyComponent());
		}
		else
		{
			dGeomSetBody(m_TransformGeomID, NULL);
		}
		dGeomSetData(m_TransformGeomID, (void*)this);
		UpdateBodyMass();
		SetCollisionBits(m_CollisionBits);
		SetCollisionCategory(m_CollisionCategory);
		SetOffset(m_Offset);
	}
	
	void ODEBaseGeometryComponent::OnCollisionSettings(CollisionSettingsRequestPtr message)
	{
		bool value = message->EnableCollision();
		if(value)
			Enable();
		else
			Disable();
	}

	void ODEBaseGeometryComponent::Reset()
	{
		if(m_TransformGeomID) 
			dGeomDestroy(m_TransformGeomID);
		if(m_ODESpaceID) 
			dSpaceDestroy(m_ODESpaceID);
		m_TransformGeomID = NULL;
		m_ODESpaceID = NULL;
		m_GeomID = NULL;
	}

	void ODEBaseGeometryComponent::SetOffset(const Vec3 &value)
	{
		m_Offset = value;
		if(m_GeomID)
		{
			dGeomSetPosition(m_GeomID, m_Offset.x, m_Offset.y, m_Offset.z);

			//also reset position, know why but offset change is not reflected otherwise
			const dReal* pos = dGeomGetPosition(m_TransformGeomID);
			dGeomSetPosition(m_TransformGeomID, pos[0], pos[1], pos[2]);

			UpdateDebug();
		}
	}

	void ODEBaseGeometryComponent::SetPosition(const Vec3 &pos)
	{
		if(m_Body == NULL && m_TransformGeomID)
		{
			dGeomSetPosition(m_TransformGeomID, pos.x, pos.y, pos.z);
		}
	}

	void ODEBaseGeometryComponent::SetRotation(const Quaternion &rot)
	{
		if(m_Body == NULL && m_TransformGeomID)
		{
			dReal ode_rot_mat[12];
			Mat4 rot_mat;
			rot_mat.Identity();
			rot.ToRotationMatrix(rot_mat);
			ODEPhysicsSceneManager::CreateODERotationMatrix(rot_mat,ode_rot_mat);
			dGeomSetRotation(m_TransformGeomID, ode_rot_mat);
		}
	}

	void ODEBaseGeometryComponent::Disable()
	{
		if(m_TransformGeomID)
			dGeomDisable(m_TransformGeomID);
	}

	void ODEBaseGeometryComponent::Enable()
	{
		if(m_TransformGeomID)
			dGeomEnable(m_TransformGeomID);
	}

	GeometryComponentPtr ODEBaseGeometryComponent::GetGeometry() const 
	{
		GeometryComponentPtr geom;
		if(m_GeometryTemplate != "")
		{
			geom = GASS_DYNAMIC_PTR_CAST<IGeometryComponent>(GetSceneObject()->GetComponent(m_GeometryTemplate));
		}
		else geom = GetSceneObject()->GetFirstComponentByClass<IGeometryComponent>();
		return geom;
	}

	unsigned long ODEBaseGeometryComponent::GetCollisionBits() const 
	{
		return m_CollisionBits;
	}

	void ODEBaseGeometryComponent::SetCollisionBits(unsigned long value)
	{
		m_CollisionBits = value;
		if(m_GeomID && m_TransformGeomID)
		{
			dGeomSetCollideBits (m_GeomID,m_CollisionBits);
			dGeomSetCollideBits (m_TransformGeomID, m_CollisionBits);
		}
	}

	bool ODEBaseGeometryComponent::IsInitialized() const
	{
		return (m_GeomID == 0) ? false:true;
	}

	unsigned long  ODEBaseGeometryComponent::GetCollisionCategory() const 
	{
		return m_CollisionCategory;
	}

	void ODEBaseGeometryComponent::SetCollisionCategory(unsigned long value)
	{
		m_CollisionCategory =value;
		if(m_GeomID && m_TransformGeomID)
		{
			dGeomSetCategoryBits(m_GeomID, m_CollisionCategory );
			dGeomSetCategoryBits(m_TransformGeomID, m_CollisionCategory );
		}
	}

	dSpaceID ODEBaseGeometryComponent::GetSpace()
	{
		if(m_Body)
		{
			return m_Body->GetSpace();
		}
		else
		{
			if(m_ODESpaceID == NULL)
			{
				m_ODESpaceID = dSimpleSpaceCreate(ODEPhysicsSceneManagerPtr(m_SceneManager)->GetPhysicsSpace());
			}
			return m_ODESpaceID;
		}
	}

	void ODEBaseGeometryComponent::OnPhysicsDebug(PhysicsDebugRequestPtr message)
	{
		SetDebug(message->DebugGeometry());
	}

	void ODEBaseGeometryComponent::SetDebug(bool value)
	{
		m_Debug = value;
		if(IsInitialized())
		{
			if(m_Debug)
			{
				UpdateDebug();
			}
			else
			{
				SceneObjectPtr obj = GetDebugObject();
				obj->UnregisterForMessage(UNREG_TMESS(ODEBaseGeometryComponent::OnDebugTransformation,TransformationChangedEvent));
				obj->SendRemoveRequest(0);
			}
		}
	}

	bool ODEBaseGeometryComponent::GetDebug() const
	{
		return m_Debug;
	}


	SceneObjectPtr ODEBaseGeometryComponent::GetDebugObject()
	{
		SceneObjectPtr scene_object;
		ComponentContainer::ComponentContainerIterator children = GetSceneObject()->GetChildren();
		while(children.hasMoreElements())
		{
			SceneObjectPtr child = GASS_STATIC_PTR_CAST<SceneObject>(children.getNext());
			std::string::size_type pos = child->GetName().find(GetName() + "DebugPhysics");
			if(pos  != std::string::npos)
			{
				scene_object = child;
			}
		}

		if(!scene_object)
		{
			scene_object = SimEngine::Get().CreateObjectFromTemplate("DebugPhysics");
			if(!scene_object)
			{
				SceneObjectTemplatePtr debug_template (new SceneObjectTemplate);
				debug_template->SetName("DebugPhysics");

				ComponentPtr location_comp = ComponentFactory::Get().Create("LocationComponent");
				location_comp->SetName("LocationComp");
				location_comp->SetPropertyByType("AttachToParent",true);


				ComponentPtr mesh_comp = ComponentFactory::Get().Create("ManualMeshComponent");
				mesh_comp->SetName("MeshComp");
				mesh_comp->SetPropertyByType("CastShadows",false);

				debug_template->AddComponent(location_comp);
				debug_template->AddComponent(mesh_comp );
				SimEngine::Get().GetSceneObjectTemplateManager()->AddTemplate(debug_template);
				//scene_object = GetSceneObject()->GetScene()->LoadObjectFromTemplate("DebugPhysics",GetSceneObject());
				scene_object = SimEngine::Get().CreateObjectFromTemplate("DebugPhysics");
			}
			scene_object->SetName(GetName() + scene_object->GetName());
			scene_object->RegisterForMessage(REG_TMESS(ODEBaseGeometryComponent::OnDebugTransformation,TransformationChangedEvent,0));
			GetSceneObject()->AddChildSceneObject(scene_object,true);
		}
		return scene_object;
	}


	void ODEBaseGeometryComponent::OnDebugTransformation(TransformationChangedEventPtr message)
	{
		SceneObjectPtr obj = GetDebugObject();
		Vec3 pos  =obj->GetFirstComponentByClass<ILocationComponent>()->GetPosition();
		if(pos != m_Offset)
		{
			m_Offset = pos;
			if(m_GeomID)
			{
				dGeomSetPosition(m_GeomID, m_Offset.x, m_Offset.y, m_Offset.z);

				//also reset position, know why but offset change is not reflected otherwise
				const dReal* ode_pos = dGeomGetPosition(m_TransformGeomID);
				dGeomSetPosition(m_TransformGeomID, ode_pos[0], ode_pos[1], ode_pos[2]);

			}
		}
	}
}
