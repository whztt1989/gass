/****************************************************************************
* This file is part of GASS.                                                *
* See http://code.google.com/p/gass/                                        *
*                                                                           *
* Copyright (c) 2008-2015 GASS team. See Contributors.txt for details.      *
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

#include "ProjectileComponent.h"
#include "Plugins/Game/GameSceneManager.h"
#include "Plugins/Game/GameMessages.h"
#include "Core/Math/GASSQuaternion.h"
#include "Core/ComponentSystem/GASSComponentFactory.h"
#include "Core/MessageSystem/GASSMessageManager.h"
#include "Core/MessageSystem/GASSIMessage.h"
#include "Core/Utils/GASSLogManager.h"
#include "Sim/GASSScene.h"
#include "Sim/GASSSceneObject.h"


#include "Sim/GASSSimEngine.h"
#include "Sim/GASSSimSystemManager.h"




namespace GASS
{
	ProjectileComponent::ProjectileComponent() : m_MaxDamage(10000), m_ImpactForce(0,0,0)
	{
		m_TotSquaredDist = 0;
		m_ExplodeNearEnemyDistance = -1;
		m_TimeLeft = 0;
		m_TimeToLive = 4;
		m_DamgeRadius = -1;
		m_DieAfterColl = 1;
		m_Velocity.Set(0,-0.1,0);
		m_HasColHandle = false;
		m_PhysicsDeltaTime = 0;
	}

	ProjectileComponent::~ProjectileComponent()
	{

	}

	void ProjectileComponent::RegisterReflection()
	{
		ComponentFactory::GetPtr()->Register("ProjectileComponent",new Creator<ProjectileComponent, Component>);
		RegisterProperty<std::string>("EndEffectTemplate", &ProjectileComponent::GetEndEffectTemplateName, &ProjectileComponent::SetEndEffectTemplateName);
		RegisterProperty<Vec3>("ImpactForce", &ProjectileComponent::GetImpactForce, &ProjectileComponent::SetImpactForce);
		RegisterProperty<float>("MaxDamage", &ProjectileComponent::GetMaxDamage, &ProjectileComponent::SetMaxDamage);
	}

	void ProjectileComponent::OnInitialize()
	{
		GetSceneObject()->RegisterForMessage(REG_TMESS(ProjectileComponent::OnPositionMessage,PositionRequest,0));
		GetSceneObject()->RegisterForMessage(REG_TMESS(ProjectileComponent::OnRotationMessage,RotationRequest,0));
		GetSceneObject()->RegisterForMessage(REG_TMESS(ProjectileComponent::OnVelocityRequest,PhysicsBodyVelocityRequest,0));

		m_TimeLeft = m_TimeToLive;
		//save for fast access
		m_ColSM = GetSceneObject()->GetScene()->GetFirstSceneManagerByClass<ICollisionSceneManager>();
		//register fot ticks
		GetSceneObject()->GetScene()->GetFirstSceneManagerByClass<GameSceneManager>()->Register(shared_from_this());
	}

	void ProjectileComponent::OnDelete()
	{

	}

	void ProjectileComponent::OnVelocityRequest(PhysicsBodyVelocityRequestPtr message)
	{
		m_Velocity = message->GetVelocity();
	}


	void ProjectileComponent::OnPositionMessage(PositionRequestPtr message)
	{
		if(message->GetSenderID() != PTR_TO_INT(this))
			m_Pos = message->GetPosition();
	}

	void ProjectileComponent::OnRotationMessage(RotationRequestPtr message)
	{
		if(message->GetSenderID() != PTR_TO_INT(this))
			m_Rot = message->GetRotation();
	}

	void ProjectileComponent::StepPhysics(double time)
	{
		Vec3 gravity;
		gravity.Set(0,-9.82,0);

		Vec3 ray_start;
		Vec3 ray_end;
		Vec3 ray_dir;
		Vec3 right,up;


			bool impact = false;
			if(m_TimeLeft < time && m_ExplodeNearEnemyDistance < 0) //do not explode on time stop if mine
			{
				//Check for expolsion
				if(m_DamgeRadius > 0)
				{
					impact = true;
				}
				else
				{
					GetSceneObject()->GetScene()->PostMessage(RemoveSceneObjectRequestPtr(new RemoveSceneObjectRequest(GetSceneObject())));
					return;
				}
			}

		//calc some basic physics
		if(m_Velocity.SquaredLength() > 0.000001)
		{
			Mat4 rot_mat;
			//Create rotation matrix
			Vec3 drag = -m_Velocity;
			drag.Normalize();
			Vec3 dir = -drag;

			//right
			right.x = dir.z;
			right.y = 0;
			right.z = -dir.x;
			if(right.SquaredLength() > 0.00001)
			{
				right.FastNormalize();
			}
			else right.Set(0,0,1);
			//up
			up = Math::Cross(dir,right);
			up.Normalize();



			rot_mat.Identity();
			rot_mat.SetZAxis(-dir);
			rot_mat.SetXAxis(-right);
			rot_mat.SetYAxis(up);

			//air drag ~ v^2*k
			drag = drag* m_Velocity.SquaredLength()*time;

			m_Velocity = m_Velocity + gravity*time;// + drag*time;
			Vec3 new_pos = m_Pos + m_Velocity*time;

			//Send collision request
			ray_start = m_Pos;
			ray_end = new_pos;
			ray_dir = ray_end - ray_start;
			GASS::CollisionResult result;
			m_ColSM->Raycast(ray_start,ray_dir,GEOMETRY_FLAG_SCENE_OBJECTS,result);
			m_Pos = new_pos;
			m_Rot.FromRotationMatrix(rot_mat);


			if(result.Coll)
			{
				if(m_DieAfterColl)
					impact = true;
					//correct postition
				m_Pos = result.CollPosition;
			}
				//send position and rotaion update
			int id = PTR_TO_INT(this);


			GetSceneObject()->PostRequest(PositionRequestPtr(new PositionRequest(m_Pos,id)));
			GetSceneObject()->PostRequest(RotationRequestPtr(new RotationRequest(m_Rot,id)));


			if(impact)
			{
				if(m_DamgeRadius > 0)
				{

				}
				else
				{
					Vec3 proj_dir = m_Velocity;
					proj_dir.FastNormalize();

					float angle_falloff = fabs(Math::Dot(proj_dir,result.CollNormal));
					float damage_value = angle_falloff*m_MaxDamage;

					SceneObjectPtr(result.CollSceneObject)->PostEvent(HitMessagePtr(new HitMessage(damage_value,m_Pos,proj_dir)));

					//Send force message to indicate hit
					Vec3 force = proj_dir*m_ImpactForce;
					SceneObjectPtr(result.CollSceneObject)->PostRequest(PhysicsBodyAddForceRequestPtr(new PhysicsBodyAddForceRequest(force)));
				}
				SceneMessagePtr remove_msg(new RemoveSceneObjectRequest(GetSceneObject()));
				GetSceneObject()->GetScene()->PostMessage(remove_msg);

				if(m_EndEffectTemplateName != "")
					SpawnEffect(m_EndEffectTemplateName);
			}
		}
	}

	void ProjectileComponent::SceneManagerTick(double time)
	{
		//std::cout << "Update proj:" << GetSceneObject()->GetName() << std::endl;

		m_TimeLeft -= time;
		m_PhysicsDeltaTime += time;
		StepPhysics(time);



		/*if(m_HasColHandle)
		{
			if(m_ColSM->Check(m_ColHandle,result))
			{
				if(result.Coll)
				{
					if(m_DieAfterColl)
						impact = true;
					//correct postition
					m_Pos = result.CollPosition;
				}
				//send position and rotaion update
				int id = PTR_TO_INT(this);
				MessagePtr pos_msg(new PositionRequest(m_Pos,id));
				MessagePtr rot_msg(new RotationRequest(m_Rot,id));
				GetSceneObject()->PostMessage(pos_msg);
				GetSceneObject()->PostMessage(rot_msg);
			}
			else
				//wait for collision results
				return;
		}

		if(impact)
		{
			if(m_DamgeRadius > 0)
			{

			}
			else
			{
				Vec3 proj_dir = m_Velocity;
				proj_dir.FastNormalize();

				float angle_falloff = fabs(Math::Dot(proj_dir,result.CollNormal));
				float damage_value = angle_falloff*m_MaxDamage;

				MessagePtr hit_msg(new HitMessage(damage_value,m_Pos,proj_dir));
				SceneObjectPtr(result.CollSceneObject)->PostMessage(hit_msg);

				//Send force message to indicate hit
				Vec3 force = proj_dir*m_ImpactForce;
				MessagePtr force_msg(new PhysicsBodyAddForceRequest(force));
				SceneObjectPtr(result.CollSceneObject)->PostMessage(force_msg);
			}
			SceneMessagePtr remove_msg(new RemoveSceneObjectRequest(GetSceneObject()));
			GetSceneObject()->GetScene()->PostMessage(remove_msg);

			if(m_EndEffectTemplateName != "")
				SpawnEffect(m_EndEffectTemplateName);
			return;
		}
		StepPhysics(m_PhysicsDeltaTime);
		m_PhysicsDeltaTime = 0;*/
	}

	void ProjectileComponent::SpawnEffect(const std::string &effect)
	{
		Vec3 vel(0,0,0);
		GetSceneObject()->GetScene()->PostMessage(SpawnObjectFromTemplateRequestPtr(new SpawnObjectFromTemplateRequest(effect,m_Pos,m_Rot,vel)));
	}

	void ProjectileComponent::SetEndEffectTemplateName(const std::string &effect)
	{
		m_EndEffectTemplateName = effect;
	}

	std::string ProjectileComponent::GetEndEffectTemplateName() const
	{
		return m_EndEffectTemplateName;
	}


	Vec3 ProjectileComponent::GetImpactForce() const
	{
		return m_ImpactForce;
	}
	void ProjectileComponent::SetImpactForce(const Vec3 &value)
	{
		m_ImpactForce = value;
	}

	float ProjectileComponent::GetMaxDamage() const
	{
		return m_MaxDamage;
	}

	void ProjectileComponent::SetMaxDamage(float value)
	{
		m_MaxDamage = value;
	}
}
