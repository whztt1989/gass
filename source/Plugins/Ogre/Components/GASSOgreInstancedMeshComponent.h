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
#pragma once

#include "Sim/GASSCommon.h"
#include "Plugins/Ogre/GASSOgreCommon.h"
#include "Sim/Interface/GASSIMeshComponent.h"
#include "Sim/Interface/GASSIGeometryComponent.h"
#include "Sim/Interface/GASSICollisionComponent.h"
#include "Sim/GASSBaseSceneComponent.h"
#include "Core/Math/GASSVector.h"
#include "Core/Math/GASSAABox.h"
#include "Core/Math/GASSSphere.h"
#include "Sim/Messages/GASSGraphicsSceneObjectMessages.h"
#include "Plugins/Ogre/GASSOgreGraphicsSceneManager.h"

namespace Ogre
{
	class Entity;
	class Bone;
	class IndexData;
	class VertexData;
	class InstancedGeometry;
}

namespace GASS
{
	struct MeshInstance
	{
		std::string m_Filename;
		Vec3 m_Position;
		Quaternion m_Rotation;
		Vec3 m_Scale;
	};

	class OgreInstancedMeshComponent : public Reflection<OgreInstancedMeshComponent,BaseSceneComponent>, public IMeshComponent , public IGeometryComponent
	{
	public:
		OgreInstancedMeshComponent (void);
		~OgreInstancedMeshComponent (void);
		static void RegisterReflection();
		virtual void OnInitialize();

		//IGeometryComponent
		virtual AABox GetBoundingBox()const;
		virtual Sphere GetBoundingSphere()const;
		virtual GeometryFlags GetGeometryFlags() const;
		virtual void SetGeometryFlags(GeometryFlags flags);
		virtual bool GetCollision() const;
		virtual void SetCollision(bool value);
		
		//IMeshComponent
		virtual ResourceHandle GetMeshResource()const {return m_MeshResource;}
		virtual GraphicsMesh GetMeshData() const;

		//override from Component
		virtual void LoadXML(tinyxml2::XMLElement *elem);
	protected:
		std::string GetRenderQueue()const {return m_RenderQueue;}
		void SetRenderQueue(const std::string &rq) {m_RenderQueue = rq;}
		bool GetCastShadow()const {return m_CastShadow;}
		void SetCastShadow(bool castShadow) {m_CastShadow = castShadow;}
		void SetRegionSize(Float size) {m_RegionSize = size;}
		Float GetRegionSize() const {return m_RegionSize;}
		void SetGlobalScale(const Vec3 &scale) {m_GlobalScale = scale;}
		Vec3 GetGlobalScale() const {return m_GlobalScale;}
		void OnLocationLoaded(LocationLoadedEventPtr message);
		void OnDelete();
		//void OnMeshFileNameMessage(MeshFileRequestPtr message);
		void setupInstancedMaterialToEntity(Ogre::Entity*ent);
		Ogre::String buildInstancedMaterial(const Ogre::String &originalMaterialName);

		std::string m_RenderQueue;
		bool m_CastShadow;
		bool m_ReadyToLoadMesh;
		bool m_UniqueMaterialCreated;

		typedef std::vector<MeshInstance> MeshInstanceVector;
		typedef std::map<std::string, MeshInstanceVector> MeshMap;
		MeshMap m_MeshInstances;
		std::vector<Ogre::InstancedGeometry*> m_InstancedGeometries;
		OgreGraphicsSceneManagerWeakPtr m_OgreSceneManager;
		ResourceHandle m_MeshResource;
		Float m_RegionSize;
		Vec3 m_GlobalScale;
		GeometryFlags m_GeomFlags;
		CollisionComponentPtr m_Collision;
	};
	typedef GASS_SHARED_PTR<OgreInstancedMeshComponent> OgreInstancedMeshComponentPtr;
}
