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

#include "Plugins/Ogre/Components/GASSOgreBillboardComponent.h"
#include "Core/ComponentSystem/GASSComponentFactory.h"
#include "Core/MessageSystem/GASSMessageManager.h"
#include "Core/MessageSystem/GASSIMessage.h"
#include "Core/Math/GASSMath.h"
#include "Sim/GASSSimEngine.h"
#include "Sim/GASSScene.h"
#include "Sim/GASSSceneObject.h"
#include "Sim/GASSSimSystemManager.h"
#include "Plugins/Ogre/GASSOgreGraphicsSceneManager.h"
#include "Plugins/Ogre/Components/GASSOgreLocationComponent.h"
#include "Plugins/Ogre/GASSOgreConvert.h"


using namespace Ogre;

namespace GASS
{
	OgreBillboardComponent::OgreBillboardComponent() : m_CastShadow(true),
		m_BillboardSet (NULL),
		m_Billboard(NULL),
		m_Width(1.0f),
		m_Height(1.0f),
		m_GeomFlags(GEOMETRY_FLAG_EDITOR)
	{

	}

	OgreBillboardComponent::~OgreBillboardComponent()
	{

	}

	void OgreBillboardComponent::RegisterReflection()
	{
		ADD_DEPENDENCY("OgreLocationComponent")

		GASS::ComponentFactory::GetPtr()->Register("BillboardComponent",new GASS::Creator<OgreBillboardComponent, Component>);
		RegisterProperty<std::string>("RenderQueue", &GASS::OgreBillboardComponent::GetRenderQueue, &GASS::OgreBillboardComponent::SetRenderQueue);
		RegisterProperty<OgreMaterial>("Material", &GASS::OgreBillboardComponent::GetMaterial, &GASS::OgreBillboardComponent::SetMaterial);
		RegisterProperty<bool>("CastShadow", &GASS::OgreBillboardComponent::GetCastShadow, &GASS::OgreBillboardComponent::SetCastShadow);
		RegisterProperty<float>("Height", &GASS::OgreBillboardComponent::GetHeight, &GASS::OgreBillboardComponent::SetHeight);
		RegisterProperty<float>("Width", &GASS::OgreBillboardComponent::GetWidth, &GASS::OgreBillboardComponent::SetWidth);
		RegisterProperty<GeometryFlagsBinder>("GeometryFlags", &OgreBillboardComponent::GetGeometryFlagsBinder, &OgreBillboardComponent::SetGeometryFlagsBinder,
			EnumerationProxyPropertyMetaDataPtr(new EnumerationProxyPropertyMetaData("Geometry Flags",PF_VISIBLE,&GeometryFlagsBinder::GetStringEnumeration, true)));
	}

	void OgreBillboardComponent::SetGeometryFlagsBinder(GeometryFlagsBinder value)
	{
		SetGeometryFlags(value.GetValue());
	}
	GeometryFlagsBinder OgreBillboardComponent::GetGeometryFlagsBinder() const
	{
		return GeometryFlagsBinder(GetGeometryFlags());
	}

	void OgreBillboardComponent::OnInitialize()
	{
		GetSceneObject()->RegisterForMessage(REG_TMESS(OgreBillboardComponent::OnLocationLoaded,LocationLoadedEvent,0));
		GetSceneObject()->RegisterForMessage(REG_TMESS(OgreBillboardComponent::OnGeometryScale,GeometryScaleRequest,0));
		GetSceneObject()->RegisterForMessage(REG_TMESS(OgreBillboardComponent::OnSetColorMessage,BillboardColorRequest,0));
		GetSceneObject()->RegisterForMessage(REG_TMESS(OgreBillboardComponent::OnVisibilityMessage,GeometryVisibilityRequest,0));
		m_Collision = GetSceneObject()->GetFirstComponentByClass<ICollisionComponent>();
	}

	float OgreBillboardComponent::GetWidth() const
	{
		return m_Width;
	}

	void OgreBillboardComponent::SetWidth(float width)
	{
		m_Width = width;
		if(m_Billboard)
			m_Billboard->setDimensions(m_Width,m_Height);
	}

	float OgreBillboardComponent::GetHeight() const
	{
		return m_Height;
	}

	void OgreBillboardComponent::SetHeight(float height)
	{
		m_Height = height;
		if(m_Billboard)
			m_Billboard->setDimensions(m_Width,m_Height);
	}

	void OgreBillboardComponent::OnLocationLoaded(LocationLoadedEventPtr message)
	{
		OgreGraphicsSceneManagerPtr ogsm =  GetSceneObject()->GetScene()->GetFirstSceneManagerByClass<OgreGraphicsSceneManager>();
		GetClassRTTI()->SetMetaData(ClassMetaDataPtr(new ClassMetaData("Billboard", OF_VISIBLE)));

		OgreLocationComponent * lc = GetSceneObject()->GetFirstComponentByClass<OgreLocationComponent>().get();

		static unsigned int obj_id = 0;
		obj_id++;
		std::stringstream ss;
		std::string name;
		ss << GetName() << obj_id;
		ss >> name;

		std::string material_name = m_Material.GetName();
		Ogre::MaterialPtr material;
		if(Ogre::MaterialManager::getSingleton().resourceExists(material_name)) material = Ogre::MaterialManager::getSingleton().getByName(material_name);
		else
		{
			material = Ogre::MaterialManager::getSingleton().create(material_name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,false, 0); // Manual, loader
			// Remove all defaults techniques
			material->removeAllTechniques();
			// Create technique, pass and texture unit
			Ogre::Technique * technique = material->createTechnique();
			Ogre::Pass* pass = technique->createPass();
			material->setLightingEnabled(false);
			material->setDepthWriteEnabled(true);
			material->setCullingMode(Ogre::CULL_NONE);
			std::string fullpath;
			/*Ogre::TextureUnitState * textureUnit = */pass->createTextureUnitState(m_Material.GetName(),0);
			pass->setAlphaRejectSettings(Ogre::CMPF_GREATER_EQUAL, 128);
		}

		m_BillboardSet = ogsm->GetOgreSceneManager()->createBillboardSet(name);
		m_BillboardSet->setMaterialName(material_name);

		Vec3 pos = Vec3(0,m_Height/2.0,0);
		Ogre::ColourValue color = Ogre::ColourValue::White;
		Ogre::Billboard* billboard = m_BillboardSet->createBillboard(OgreConvert::ToOgre(pos),color);
		billboard->mPosition = OgreConvert::ToOgre(pos);
		billboard->setColour(color);
		billboard->setDimensions(m_Width,m_Height);
		billboard->setTexcoordRect(0, 0,1, 1);
		m_Billboard = billboard;
		float bbsize = m_Height;
		if(m_Width > m_Height)
			bbsize = m_Width;
		bbsize *=  0.5f;
		m_BillboardSet->setBounds(Ogre::AxisAlignedBox(Ogre::Vector3(-bbsize,-bbsize + static_cast<float>(pos.y),-bbsize),Ogre::Vector3(bbsize,bbsize + static_cast<float>(pos.y), bbsize)),bbsize*2);
		lc->GetOgreNode()->attachObject((Ogre::MovableObject*) m_BillboardSet);
		GetSceneObject()->PostEvent(GeometryChangedEventPtr(new GeometryChangedEvent(GASS_DYNAMIC_PTR_CAST<IGeometryComponent>(shared_from_this()))));
	}

	AABox OgreBillboardComponent::GetBoundingBox() const
	{
		Float max_size = Math::Max(m_Width,m_Height);
		Float offset = 0;
		if(m_Billboard)
		{
			max_size = Math::Max(m_Billboard->getOwnHeight(),m_Billboard->getOwnWidth());
			Ogre::Vector3 pos = m_Billboard->getPosition();
			offset = pos.y;
		}
		max_size *= 0.5f;
		AABox box(Vec3(-max_size,-max_size+offset,-max_size),Vec3(max_size,max_size+offset,max_size));
		return box;
	}

	Sphere OgreBillboardComponent::GetBoundingSphere() const
	{
		Sphere sphere;
		sphere.m_Pos = Vec3(0,0,0);
		sphere.m_Radius = static_cast<float>(Math::Max(m_Width,m_Height)/2.0);
		return sphere;
	}

	void OgreBillboardComponent::OnSetColorMessage(BillboardColorRequestPtr message)
	{
		const ColorRGBA color = message->GetColor();
		if(m_Billboard)
			m_Billboard->setColour(OgreConvert::ToOgre(color));
	}

	void OgreBillboardComponent::OnGeometryScale(GeometryScaleRequestPtr message)
	{
		const Vec3 scale = message->GetScale();
		if(m_Billboard)
		{
			m_Billboard->setPosition(Ogre::Vector3(0, static_cast<float>(scale.y)*m_Height/2.0f,0));
			m_Billboard->setDimensions(m_Width * static_cast<float>(scale.x), m_Height * static_cast<float>(scale.y));
		}
	}

	GeometryFlags OgreBillboardComponent::GetGeometryFlags() const
	{
		return m_GeomFlags;
	}

	void OgreBillboardComponent::SetGeometryFlags(GeometryFlags flags)
	{
		m_GeomFlags = flags;
	}

	bool OgreBillboardComponent::GetCollision() const
	{
		if(m_Collision)
			return m_Collision->GetActive();
		return false;
	}

	void OgreBillboardComponent::SetCollision(bool value)
	{
		if(m_Collision)
			m_Collision->SetActive(value);
	}

	void OgreBillboardComponent::OnVisibilityMessage(GeometryVisibilityRequestPtr message)
	{
		if(m_BillboardSet)
		{
			m_BillboardSet->setVisible(message->GetValue());
		}
	}
}