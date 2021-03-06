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

#include "Core/Common.h"
#include "PhysXPhysicsSceneManager.h"
#include "PhysXBaseGeometryComponent.h"
#include "PhysXCommon.h"

namespace GASS
{
	class IGeometryComponent;
	class PhysXPhysicsSceneManager;
	typedef GASS_WEAK_PTR<PhysXPhysicsSceneManager> PhysXPhysicsSceneManagerWeakPtr;
	typedef GASS_SHARED_PTR<IGeometryComponent> GeometryComponentPtr;
	
	class PhysXMeshGeometryComponent : public Reflection<PhysXMeshGeometryComponent,BaseSceneComponent>
	{
	friend class PhysXPhysicsSceneManager;
	public:
		PhysXMeshGeometryComponent();
		virtual ~PhysXMeshGeometryComponent();
		static void RegisterReflection();
		virtual void OnInitialize();
		virtual void OnDelete();
	protected:
		//Message section
		void OnGeometryChanged(GeometryChangedEventPtr message);
		void OnTransformationChanged(TransformationChangedEventPtr message);

		void SetPosition(const Vec3 &pos);
		void SetRotation(const Quaternion &rot);
	protected:
		PhysXTriangleMesh m_TriangleMesh;
		physx::PxRigidStatic* m_Actor;
		physx::PxShape* m_Shape;
		PhysXPhysicsSceneManagerWeakPtr m_SceneManager;
	};
	typedef GASS_SHARED_PTR<PhysXMeshGeometryComponent> PhysXMeshGeometryComponentPtr;
}
