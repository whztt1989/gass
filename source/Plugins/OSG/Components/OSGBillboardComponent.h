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
#include "Sim/GASS.h"
#include "Plugins/OSG/OSGCommon.h"

namespace osg
{
	class Billboard;
	class Image;
	class Drawable;
	class Geometry;
}

namespace GASS
{
	class OSGBillboardComponent : public Reflection<OSGBillboardComponent,BaseSceneComponent> , public IGeometryComponent
	{
	public:
		OSGBillboardComponent (void);
		~OSGBillboardComponent (void);
		static void RegisterReflection();
		virtual void OnInitialize();

		//IGeometryComponent interface
		virtual AABox GetBoundingBox()const;
		virtual Sphere GetBoundingSphere()const;
		virtual void GetMeshData(GraphicsMeshPtr mesh_data);
		virtual GeometryFlags GetGeometryFlags() const;
		virtual void SetGeometryFlags(GeometryFlags flags);
		virtual bool GetCollision() const;
		virtual void SetCollision(bool value);
	protected:
		void OnCollisionSettings(CollisionSettingsRequestPtr message);
		void OnVisibilityMessage(GeometryVisibilityRequestPtr message);
		void OnSetColorMessage(BillboardColorRequestPtr message);
		void OnLocationLoaded(LocationLoadedEventPtr message);
		void OnGeometryScale(GeometryScaleRequestPtr message);

		void SetGeometryFlagsBinder(GeometryFlagsBinder value);
		GeometryFlagsBinder GetGeometryFlagsBinder() const;
		float GetWidth() const;
		void SetWidth(float width);
		float GetHeight() const;
		void SetHeight(float height);
		void SetMaterial(const std::string &mat) {m_Material = mat;}
		std::string GetMaterial()const {return m_Material;}
		bool GetCastShadow()const {return m_CastShadow;}
		void SetCastShadow(bool castShadow);

		//Helpers
		void _UpdateSize(float width,float height);

		osg::ref_ptr<osg::Geometry> CreateSquare(const osg::Vec3& corner,const osg::Vec3& width,const osg::Vec3& height, osg::Image* image=NULL);
		std::string m_Material;
		bool m_CastShadow;
		float m_Width;
		float m_Height;
		osg::ref_ptr<osg::Billboard> m_OSGBillboard;
		osg::Geometry* m_Geom;
		GeometryFlags m_GeomFlags;
		double m_GroundOffset;
		bool m_Collision;
	};
}