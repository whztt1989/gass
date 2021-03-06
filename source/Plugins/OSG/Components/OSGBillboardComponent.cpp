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

#include "Plugins/OSG/Components/OSGBillboardComponent.h"
#include "Plugins/OSG/OSGGraphicsSceneManager.h"
#include "Plugins/OSG/Components/OSGLocationComponent.h"
#include "Plugins/OSG/OSGConvert.h"
#include "Plugins/OSG/OSGNodeMasks.h"
#include "Plugins/OSG/OSGNodeData.h"
#include "Core/Math/GASSMath.h"


namespace GASS
{
	OSGBillboardComponent::OSGBillboardComponent() : m_CastShadow(false),
		m_OSGBillboard (NULL),
		m_Width(1.0f),
		m_Height(1.0f),
		m_GroundOffset(0.5),
		m_GeomFlags(GEOMETRY_FLAG_UNKNOWN),
		m_Collision(true),
		m_Geom(NULL)
	{

	}

	OSGBillboardComponent::~OSGBillboardComponent()
	{

	}

	void OSGBillboardComponent::RegisterReflection()
	{
		GASS::ComponentFactory::GetPtr()->Register("BillboardComponent",new GASS::Creator<OSGBillboardComponent, Component>);
		ADD_DEPENDENCY("OSGLocationComponent")
		RegisterProperty<std::string>("Material", &OSGBillboardComponent::GetMaterial, &OSGBillboardComponent::SetMaterial);
		RegisterProperty<bool>("CastShadow", &OSGBillboardComponent::GetCastShadow, &OSGBillboardComponent::SetCastShadow);
		RegisterProperty<float>("Height", &OSGBillboardComponent::GetHeight, &OSGBillboardComponent::SetHeight);
		RegisterProperty<float>("Width", &OSGBillboardComponent::GetWidth, &OSGBillboardComponent::SetWidth);

		RegisterProperty<GeometryFlagsBinder>("GeometryFlags", &OSGBillboardComponent::GetGeometryFlagsBinder, &OSGBillboardComponent::SetGeometryFlagsBinder,
			EnumerationProxyPropertyMetaDataPtr(new EnumerationProxyPropertyMetaData("Geometry Flags",PF_VISIBLE,&GeometryFlagsBinder::GetStringEnumeration, true)));
	}

	void OSGBillboardComponent::SetGeometryFlagsBinder(GeometryFlagsBinder value)
	{
		SetGeometryFlags(value.GetValue());
	}
	GeometryFlagsBinder OSGBillboardComponent::GetGeometryFlagsBinder() const
	{
		return GeometryFlagsBinder(GetGeometryFlags());
	}

	void OSGBillboardComponent::OnInitialize()
	{
		GetSceneObject()->RegisterForMessage(REG_TMESS(OSGBillboardComponent::OnLocationLoaded,LocationLoadedEvent,1));
		GetSceneObject()->RegisterForMessage(REG_TMESS(OSGBillboardComponent::OnGeometryScale,GeometryScaleRequest,0));
		GetSceneObject()->RegisterForMessage(REG_TMESS(OSGBillboardComponent::OnCollisionSettings,CollisionSettingsRequest,0));
		GetSceneObject()->RegisterForMessage(REG_TMESS(OSGBillboardComponent::OnVisibilityMessage,GeometryVisibilityRequest,0));
		GetSceneObject()->RegisterForMessage(REG_TMESS(OSGBillboardComponent::OnSetColorMessage,BillboardColorRequest,0));
	}

	float OSGBillboardComponent::GetWidth() const
	{
		return m_Width;
	}

	void OSGBillboardComponent::SetWidth(float width)
	{
		m_Width = width;
		_UpdateSize(m_Width,m_Height);
	}

	float OSGBillboardComponent::GetHeight() const
	{
		return m_Height;
	}

	void OSGBillboardComponent::SetHeight(float height)
	{
		m_Height = height;
		_UpdateSize(m_Width,m_Height);
	}

	GeometryFlags OSGBillboardComponent::GetGeometryFlags() const
	{
		return m_GeomFlags;
	}

	void OSGBillboardComponent::SetGeometryFlags(GeometryFlags flags)
	{
		m_GeomFlags = flags;
		if(m_OSGBillboard)
			OSGConvert::SetOSGNodeMask(flags, m_OSGBillboard);
	}



	void OSGBillboardComponent::OnLocationLoaded(LocationLoadedEventPtr message)
	{
		ResourceManagerPtr rm = SimEngine::Get().GetResourceManager();
		const std::string  full_path = rm->GetFirstResourceByName(m_Material)->Path().GetFullPath();

		Vec3 up(0,0,m_Height);
		Vec3 east(m_Width,0,0);

		//make offset
		Vec3 corner = -east*0.5;
		osgDB::ReaderWriter::Options* options = new osgDB::ReaderWriter::Options("dds_flip");

		m_OSGBillboard = new osg::Billboard();
		m_OSGBillboard->setMode(osg::Billboard::POINT_ROT_EYE);

		osg::ref_ptr<osg::Geometry> geom = CreateSquare(osg::Vec3(static_cast<float>(corner.x), static_cast<float>(corner.y), static_cast<float>(corner.z)),
														osg::Vec3(static_cast<float>(east.x), static_cast<float>(east.y), static_cast<float>(east.z)),
														osg::Vec3(static_cast<float>(up.x), static_cast<float>(up.y), static_cast<float>(up.z)),
														osgDB::readImageFile(full_path,options));

		m_OSGBillboard->addDrawable(geom);
		m_Geom = geom.get();

		m_OSGBillboard->setPosition(0,osg::Vec3(0,0,static_cast<float>(m_GroundOffset)));
		OSGNodeData* node_data = new OSGNodeData(shared_from_this());
		m_OSGBillboard->setUserData(node_data);

		OSGLocationComponentPtr lc = GetSceneObject()->GetFirstComponentByClass<OSGLocationComponent>();
		lc->GetOSGNode()->addChild(m_OSGBillboard.get());

		osg::ref_ptr<osg::StateSet> nodess (m_OSGBillboard->getOrCreateStateSet());
		nodess->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
		SetCastShadow(m_CastShadow);
		SetGeometryFlags(m_GeomFlags);
		GetSceneObject()->PostEvent(GeometryChangedEventPtr(new GeometryChangedEvent(GASS_DYNAMIC_PTR_CAST<IGeometryComponent>(shared_from_this()))));
	}

	AABox OSGBillboardComponent::GetBoundingBox() const
	{
		if(m_Geom)
		{
			osg::Vec3Array* coords = static_cast<osg::Vec3Array*> (m_Geom->getVertexArray());
			osg::Vec3 p_min = (*coords)[0];
			osg::Vec3 p_max = (*coords)[2];
			osg::Vec3 bb_size = p_max - p_min;
			Float max_size = Math::Max(bb_size.x() ,bb_size.z() )*0.5f;
			Float offset = bb_size.z() * 0.5f;
			AABox box(Vec3(-max_size,-max_size + offset,-max_size),Vec3(max_size,max_size+offset,max_size));
			box.m_Min.y += m_GroundOffset;
			box.m_Max.y += m_GroundOffset;
			return box;
		}
		return AABox();
	}

	Sphere OSGBillboardComponent::GetBoundingSphere() const
	{
		if(m_Geom)
		{
			return GetBoundingBox().GetBoundingSphere();
			/*osg::Vec3Array* coords = static_cast<osg::Vec3Array*> (m_Geom->getVertexArray());
			osg::Vec3 p_min = (*coords)[0];
			osg::Vec3 p_max = (*coords)[2];
			osg::Vec3 bb_size = p_max - p_min;
			Float max_size = Math::Max(bb_size.x() ,bb_size.z() )*0.5f;
			Sphere sphere;
			sphere.m_Pos = Vec3(0,m_GroundOffset,0);
			sphere.m_Radius = max_size;
			return sphere;*/
		}
		return Sphere();
	}

	void OSGBillboardComponent::GetMeshData(GraphicsMeshPtr mesh_data)
	{

	}

	//From OSG billboard example
	osg::ref_ptr<osg::Geometry> OSGBillboardComponent::CreateSquare(const osg::Vec3& corner,const osg::Vec3& width,const osg::Vec3& height, osg::Image* image)
	{
		// set up the Geometry.
		osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
		osg::ref_ptr<osg::Vec3Array> coords = new osg::Vec3Array(4);
		(*coords)[0] = corner;
		(*coords)[1] = corner+width;
		(*coords)[2] = corner+width+height;
		(*coords)[3] = corner+height;
		geom->setVertexArray(coords.get());

		osg::ref_ptr<osg::Vec3Array> norms = new osg::Vec3Array(1);
		(*norms)[0] = width^height;
		(*norms)[0].normalize();

		geom->setNormalArray(norms.get());
		geom->setNormalBinding(osg::Geometry::BIND_OVERALL);

		osg::ref_ptr<osg::Vec2Array> tcoords = new osg::Vec2Array(4);
		(*tcoords)[0].set(0.0f,0.0f);
		(*tcoords)[1].set(1.0f,0.0f);
		(*tcoords)[2].set(1.0f,1.0f);
		(*tcoords)[3].set(0.0f,1.0f);
		geom->setTexCoordArray(0,tcoords.get());

		osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array(4);
		(*colors)[0].set(1.0f,1.0f,1.0f,1.0f);
		(*colors)[1].set(1.0f,1.0f,1.0f,1.0f);
		(*colors)[2].set(1.0f,1.0f,1.0f,1.0f);
		(*colors)[3].set(1.0f,1.0f,1.0f,1.0f);
		geom->setColorArray(colors);
		geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);


		osg::ref_ptr<osg::DrawArrays> arrays = new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4);
		geom->addPrimitiveSet(arrays.get());
		if (image)
		{
			osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet;
			osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
			texture->setImage(image);

			osg::ref_ptr<osg::AlphaFunc> alphaFunc = new osg::AlphaFunc;
			alphaFunc->setFunction(osg::AlphaFunc::GEQUAL,0.05f);
			stateset->setAttributeAndModes( alphaFunc.get(), osg::StateAttribute::ON );

			stateset->setTextureAttributeAndModes(0,texture.get(),osg::StateAttribute::ON);
			geom->setStateSet(stateset.get());
		}
		return geom;
	}

	void OSGBillboardComponent::SetCastShadow(bool value)
	{
		m_CastShadow = value;
		if(m_CastShadow && m_OSGBillboard.valid())
		{
			m_OSGBillboard->setNodeMask(NM_CAST_SHADOWS | m_OSGBillboard->getNodeMask());
			m_OSGBillboard->setNodeMask(NM_RECEIVE_SHADOWS | m_OSGBillboard->getNodeMask());
		}
		else if(m_OSGBillboard.valid())
		{
			m_OSGBillboard->setNodeMask(~NM_CAST_SHADOWS & m_OSGBillboard->getNodeMask());
			m_OSGBillboard->setNodeMask(~NM_RECEIVE_SHADOWS & m_OSGBillboard->getNodeMask());
		}
	}

	void OSGBillboardComponent::OnGeometryScale(GeometryScaleRequestPtr message)
	{
		const Vec3 scale = message->GetScale();
		_UpdateSize(static_cast<float>(m_Width*scale.x), 
			        static_cast<float>(m_Height*scale.y));
	}

	void OSGBillboardComponent::_UpdateSize(float width,float height)
	{
		if(m_OSGBillboard.valid())
		{
			osg::Vec3Array* coords = static_cast<osg::Vec3Array*> (m_Geom->getVertexArray());

			osg::Vec3f osg_height(0.0f,0.0f, height);
			osg::Vec3f osg_width(width,0.0f,0.0f);
			osg::Vec3f osg_corner = -osg_width*0.5f;


			(*coords)[0] = osg_corner;
			(*coords)[1] = osg_corner+osg_width;
			(*coords)[2] = osg_corner+osg_width+osg_height;
			(*coords)[3] = osg_corner+osg_height;
			m_Geom->setVertexArray(coords);
		}
	}

	void OSGBillboardComponent::OnSetColorMessage(BillboardColorRequestPtr message)
	{
		if(m_Geom)
		{
			const ColorRGBA color = message->GetColor();
			osg::Vec4Array* colors = static_cast<osg::Vec4Array*> (m_Geom->getColorArray());
			osg::Vec4 osg_color = OSGConvert::ToOSG(color);
			(*colors)[0]= osg_color;
			(*colors)[1]= osg_color;
			(*colors)[2]= osg_color;
			(*colors)[3]= osg_color;
			m_Geom->setColorArray(colors);

		}
	}

	void OSGBillboardComponent::OnCollisionSettings(CollisionSettingsRequestPtr message)
	{
		SetCollision(message->EnableCollision());
	}

	void OSGBillboardComponent::SetCollision(bool value)
	{
		if(m_OSGBillboard.valid() && m_OSGBillboard->getNodeMask())
		{
			if(value)
			{
				OSGConvert::SetOSGNodeMask(m_GeomFlags, m_OSGBillboard);
			}
			else
			{
				OSGConvert::SetOSGNodeMask(GEOMETRY_FLAG_TRANSPARENT_OBJECT, m_OSGBillboard);
			}
		}
		m_Collision = value;
	}

	bool OSGBillboardComponent::GetCollision() const
	{
		return m_Collision;
	}

	void OSGBillboardComponent::OnVisibilityMessage(GeometryVisibilityRequestPtr message)
	{
		bool visibility = message->GetValue();

		if(m_OSGBillboard)
		{
			if(visibility)
			{
				SetGeometryFlags(m_GeomFlags);
				SetCastShadow(m_CastShadow);
			}
			else
			{
				m_OSGBillboard->setNodeMask(0);
			}
		}
	}
}