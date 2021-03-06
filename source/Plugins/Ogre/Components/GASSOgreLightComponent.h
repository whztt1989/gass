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
#include "Core/Math/GASSVector.h"
#include "Core/Utils/GASSColorRGB.h"
#include "Sim/Interface/GASSILightComponent.h"
#include "Sim/GASSBaseSceneComponent.h"
#include "Sim/Messages/GASSGraphicsSceneObjectMessages.h"

namespace Ogre
{
	class Light;
}

namespace GASS
{
	class OgreLightComponent : public Reflection<OgreLightComponent,BaseSceneComponent>, public ILightComponent
	{
	public:
		OgreLightComponent();
		virtual ~OgreLightComponent();
		static void RegisterReflection();
		virtual void OnInitialize();
		inline Ogre::Light	* GetOgreLight(){return m_OgreLight;}
		void SetLightType(LightTypeBinder lt);
		LightTypeBinder GetLightType()const {return m_LightType;}
		void SetAttenuationParams(const Vec4 &params);
		Vec4 GetAttenuationParams()const {return m_AttenuationParams;}
		void SetSpotParams(const Vec3 &params);
		Vec3 GetSpotParams()const {return m_SpotParams;}
		void SetDiffuse(const ColorRGB &diffuse);
		ColorRGB GetDiffuse()const {return m_Diffuse;}
		void SetSpecular(const ColorRGB &specular);
		ColorRGB GetSpecular()const {return m_Specular;}
		void SetCastShadow(bool value);
		bool GetCastShadow()const {return m_CastShadow;}

		void SetDir(const Vec3 &dir);
		Vec3 GetDir() const;

	protected:
		//add this dummy prop for ambient to play nice with OSG templates until better solution is found
		ADD_PROPERTY(Vec3,AmbientColor)
		void OnLocationLoaded(LocationLoadedEventPtr message);
		void OnDelete();
		Ogre::Light* m_OgreLight;
		LightTypeBinder m_LightType;

		ColorRGB m_Diffuse;
		ColorRGB m_Specular;
		Vec4 m_AttenuationParams;
		bool m_CastShadow;
		Vec3 m_SpotParams;
	};
}
