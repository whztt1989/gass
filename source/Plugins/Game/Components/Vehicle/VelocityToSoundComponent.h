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

#ifndef VELOCITY_TO_SOUND_COMPONENT_H
#define VELOCITY_TO_SOUND_COMPONENT_H


#include "Sim/GASSCommon.h"
#include "Sim/GASSBaseSceneComponent.h"
#include "Sim/Messages/GASSGraphicsSceneObjectMessages.h"
#include "Sim/Messages/GASSPhysicsSceneObjectMessages.h"
#include "Sim/Messages/GASSCoreSceneObjectMessages.h"
#include "Sim/Interface/GASSIControlSettingsSystem.h"
#include "Plugins/Game/GameMessages.h"


namespace GASS
{

	class SceneObject;
	typedef SPTR<SceneObject> SceneObjectPtr;
	typedef WPTR<SceneObject> SceneObjectWeakPtr;

	class VelocityToSoundComponent :  public Reflection<VelocityToSoundComponent,BaseSceneComponent>
	{
	public:
		VelocityToSoundComponent();
		virtual ~VelocityToSoundComponent();
		static void RegisterReflection();
		virtual void OnInitialize();
		virtual void SceneManagerTick(double delta_time);
	private:
		ADD_PROPERTY(Vec2,MinMaxVolume)
		ADD_PROPERTY(Vec2,MinMaxPitch)
		ADD_PROPERTY(Float,VelocityLimit)
		void OnHingeReport(PhysicsHingeJointReportEventPtr message);
		Float m_Volume;
		Float m_Pitch;
		Float m_TargetVolume;
		Float m_TargetPitch;
		Float m_MaxVelRequest;
		//Float m_MaxForce;
	};
}
#endif
