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


#include "Plugins/PhysX3/PhysXTerrainGeometryComponent.h"
#include "Plugins/PhysX3/PhysXPhysicsSceneManager.h"
#include "Plugins/PhysX3/PhysXPhysicsSystem.h"
#include "Plugins/PhysX3/PhysXVehicleSceneQuery.h"
#include "Core/ComponentSystem/GASSComponentFactory.h"
#include "Core/ComponentSystem/GASSComponentContainerTemplateManager.h"
#include "Core/MessageSystem/GASSMessageManager.h"
#include "Core/Math/GASSAABox.h"
#include "Sim/GASSScene.h"
#include "Sim/GASSSceneObject.h"
#include "Sim/GASSSceneObjectTemplate.h"
#include "Sim/Interface/GASSIGeometryComponent.h"
#include "Sim/Interface/GASSITerrainComponent.h"
#include "Sim/GASSSimEngine.h"

namespace GASS
{
	PhysXTerrainGeometryComponent::PhysXTerrainGeometryComponent(): m_Debug(false)
	{

	}

	PhysXTerrainGeometryComponent::~PhysXTerrainGeometryComponent()
	{

	}

	void PhysXTerrainGeometryComponent::RegisterReflection()
	{
		ComponentFactory::GetPtr()->Register("PhysicsTerrainGeometryComponent",new Creator<PhysXTerrainGeometryComponent, Component>);
	}

	void PhysXTerrainGeometryComponent::OnInitialize()
	{
		GetSceneObject()->RegisterForMessage(REG_TMESS(PhysXTerrainGeometryComponent::OnCollisionSettings,CollisionSettingsRequest ,0));
		GetSceneObject()->RegisterForMessage(REG_TMESS(PhysXTerrainGeometryComponent::OnGeometryChanged,GeometryChangedEvent,0));
		GetSceneObject()->RegisterForMessage(REG_TMESS(PhysXTerrainGeometryComponent::OnPhysicsDebug,PhysicsDebugRequest,0));
	
		PhysXPhysicsSceneManagerPtr scene_manager = GetSceneObject()->GetScene()->GetFirstSceneManagerByClass<PhysXPhysicsSceneManager>();
		assert(scene_manager);
		m_SceneManager = scene_manager;
	}

	void PhysXTerrainGeometryComponent::OnDelete()
	{
		Reset();
	}

	void PhysXTerrainGeometryComponent::OnGeometryChanged(GeometryChangedEventPtr message)
	{
		Reset();
		m_Shape = CreateTerrain();
	}

	HeightmapTerrainComponentPtr PhysXTerrainGeometryComponent::GetTerrainComponent() const 
	{
		HeightmapTerrainComponentPtr geom;
		if(m_GeometryTemplate != "")
		{
			geom = GASS_DYNAMIC_PTR_CAST<IHeightmapTerrainComponent>(GetSceneObject()->GetComponent(m_GeometryTemplate));
		}
		else geom = GetSceneObject()->GetFirstComponentByClass<IHeightmapTerrainComponent>();
		return geom;
	}

	physx::PxShape* PhysXTerrainGeometryComponent::CreateTerrain()
	{
		PhysXPhysicsSceneManagerPtr scene_manager = GetSceneObject()->GetScene()->GetFirstSceneManagerByClass<PhysXPhysicsSceneManager>();

		PhysXPhysicsSystemPtr system = SimEngine::Get().GetSimSystemManager()->GetFirstSystemByClass<PhysXPhysicsSystem>();
		HeightmapTerrainComponentPtr terrain = GetTerrainComponent();
		//GeometryComponentPtr geom = GASS_DYNAMIC_PTR_CAST<IGeometryComponent>(terrain);
		//save raw point for fast height access, not thread safe!!
		m_TerrainGeom = terrain.get();
		physx::PxShape* shape = NULL;	

		if(terrain)
		{
			//m_TerrainBounds = geom->GetBoundingBox();
			int samples_x = terrain->GetNumSamplesW();
			int samples_z = terrain->GetNumSamplesH();
			m_TerrainBounds = terrain->GetBoundingBox();
			Float size_x = m_TerrainBounds.m_Max.x - m_TerrainBounds.m_Min.x;
			Float size_z = m_TerrainBounds.m_Max.z - m_TerrainBounds.m_Min.z;
			
			Float scale_x = size_x/((Float) samples_x-1);
			Float scale_z = size_z/((Float) samples_z-1);
			//physx::PxHeightFieldSample* samples = (physx::PxHeightFieldSample*) system->GetAllocator()->allocate(sizeof(physx::PxHeightFieldSample)*samples_x*samples_z,0,__FILE__, __LINE__);
			physx::PxHeightFieldSample* samples = (physx::PxHeightFieldSample*) new physx::PxHeightFieldSample[samples_x*samples_z];//(sizeof(physx::PxHeightFieldSample)*(samples_x*samples_z));

			const physx::PxReal heightScale = 0.1f;
			memset(samples,0,samples_x*samples_z*sizeof(physx::PxHeightFieldSample));

			for(int x = 0; x < samples_x; x++)
			{
				for(int z = 0; z < samples_z; z++)
				{
					//Float world_x = x * m_SampleWidth + m_TerrainBounds.m_Min.x;
					//Float world_z = z * m_SampleWidth + m_TerrainBounds.m_Min.z;
					//Float height = m_TerrainGeom->GetHeightAtWorldLocation(world_x,world_z);
					Float height = m_TerrainGeom->GetHeightAtSample(x,z);
					samples[z+x*samples_z].height = (physx::PxI16)(height/heightScale);
					//samples[x+z*samples_x].setTessFlag();
					samples[z+x*samples_z].materialIndex0=0;
					samples[z+x*samples_z].materialIndex1=0;
				}
			}

			physx::PxHeightFieldDesc hfDesc;

			hfDesc.format             = physx::PxHeightFieldFormat::eS16_TM;
			hfDesc.nbColumns          = samples_z;
			hfDesc.nbRows             = samples_x;
			hfDesc.samples.data       = samples;
			hfDesc.samples.stride     = sizeof(physx::PxHeightFieldSample);
			//hfDesc.thickness = 0.1;
			//hfDesc.convexEdgeThreshold = 0;
			//hfDesc.flags = 0;

			physx::PxHeightField* heightField = system->GetPxSDK()->createHeightField(hfDesc);
			physx::PxTransform pose = physx::PxTransform::createIdentity();

			Vec3 position;
			position.x = m_TerrainBounds.m_Min.x;
			position.z = m_TerrainBounds.m_Min.z;
			position.y = 0;

			pose.p = PxConvert::ToPx(position + scene_manager->GetOffset());

			physx::PxRigidStatic* hfActor = system->GetPxSDK()->createRigidStatic(pose);

			physx::PxHeightFieldGeometry hfGeom(heightField, physx::PxMeshGeometryFlags(), static_cast<float>(heightScale), static_cast<float>(scale_x), static_cast<float>(scale_z));
			shape = hfActor->createShape(hfGeom, *system->GetDefaultMaterial());


			physx::PxFilterData collFilterData;
			GeometryFlags against = GeometryFlagManager::GetMask(GEOMETRY_FLAG_GROUND);
			collFilterData.word0 = GEOMETRY_FLAG_GROUND;
			collFilterData.word1 = against;
			shape->setSimulationFilterData(collFilterData);

			PxFilterData queryFilterData;
			VehicleSetupDrivableShapeQueryFilterData(&queryFilterData);
			shape->setQueryFilterData(queryFilterData);

			PhysXPhysicsSceneManagerPtr sm = PhysXPhysicsSceneManagerPtr(m_SceneManager);
			sm->GetPxScene()->addActor(*hfActor);

			
		}
		return shape;
	}

	
	
	void PhysXTerrainGeometryComponent::OnCollisionSettings(CollisionSettingsRequestPtr message)
	{
		bool value = message->EnableCollision();
		if(value)
			Enable();
		else
			Disable();
	}

	void PhysXTerrainGeometryComponent::Reset()
	{

	}

	void PhysXTerrainGeometryComponent::Disable()
	{
	}

	void PhysXTerrainGeometryComponent::Enable()
	{
	}

	void PhysXTerrainGeometryComponent::OnPhysicsDebug(PhysicsDebugRequestPtr message)
	{
		//SetDebug(message->DebugGeometry());
	}
}
