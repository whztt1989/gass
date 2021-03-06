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
#ifndef SKY_X_CLOUD_LAYER_COMPONENT_H
#define SKY_X_CLOUD_LAYER_COMPONENT_H

#include "Core/Common.h"
#include "Sim/GASSBaseSceneComponent.h"
#include "Sim/Messages/GASSGraphicsSceneObjectMessages.h"
#include "SkyX.h"

namespace GASS
{
	class SkyXCloudLayerComponent : public Reflection<SkyXCloudLayerComponent,BaseSceneComponent>
	{
	public:
		SkyXCloudLayerComponent(void);
		~SkyXCloudLayerComponent(void);
		static void RegisterReflection();
		virtual void OnInitialize();
		void CreateLayer();
	protected:
		void SetScale(const Float &value);
		Float GetScale() const;
		void SetVolumetricDisplacement(const Float &value);
		Float GetVolumetricDisplacement() const ;
		void SetHeightVolume(const Float &value);
		Float GetHeightVolume() const ;
		void SetNormalMultiplier(const Float &value);
		Float GetNormalMultiplier() const ;
		void SetDetailAttenuation(const Float &value);
		Float GetDetailAttenuation() const ;
		void SetDistanceAttenuation(const Float &value);
		Float GetDistanceAttenuation() const ;
		void SetTimeMultiplier(const Float &value);
		Float GetTimeMultiplier() const ;
		void SetWindDirection(const Vec2 &value);
		Vec2 GetWindDirection() const ;
		void SetHeight(const Float &value);
		Float GetHeight() const ;
		void UpdateOptions();
		
	private:
		SkyX::CloudLayer* m_CloudLayer;
		SkyX::CloudLayer::Options m_Options;

	};
	typedef GASS_SHARED_PTR<SkyXCloudLayerComponent> SkyXCloudLayerComponentPtr; 
}

#endif