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
#ifndef HEIGHTMAP_COMPONENT_H
#define HEIGHTMAP_COMPONENT_H

#include "Sim/GASSCommon.h"
#include "Sim/GASSBaseSceneComponent.h"
#include "Sim/Interface/GASSITerrainComponent.h"
#include "Sim/Interface/GASSIGeometryComponent.h"

namespace GASS
{

	/**
		Component that impl. IHeightmapTerrainComponent interface used for terrain interaction
	*/

	class HeightField;
	class HeightmapComponent : public Reflection<HeightmapComponent,BaseSceneComponent> , public IHeightmapTerrainComponent, public IGeometryComponent
	{
	public:
		HeightmapComponent(void);
		~HeightmapComponent(void);
		static void RegisterReflection();
		virtual void OnInitialize();
		virtual void SaveXML(tinyxml2::XMLElement *obj_elem);

		//IHeightmapTerrainComponent
		virtual Float GetHeightAtSample(int x, int z) const;
		virtual Float GetHeightAtWorldLocation(Float x, Float z) const;
		virtual unsigned int GetNumSamplesW() const;
		virtual unsigned int GetNumSamplesH() const;
		//virtual float* GetHeightData() const;
		virtual AABox GetBoundingBox() const;
		virtual Sphere GetBoundingSphere() const;
		virtual GeometryFlags GetGeometryFlags() const;
		virtual void SetGeometryFlags(GeometryFlags flags){(void) flags;}
		virtual bool GetCollision() const;
		virtual void SetCollision(bool value);
	protected:
		void SetUpdate(bool value);
		bool  GetUpdate() const;
		bool GetAutoBBoxGeneration() const;
		void SetAutoBBoxGeneration(bool value);

		ADD_PROPERTY(Vec2,Size)
		ADD_PROPERTY(Float,Resolution)

		//internal
		void _UpdateData();
		FilePath _GetFilePath() const;
		AABox _GetTerrainBoundingBox() const;
	private:
		HeightField* m_HM;
		bool m_AutoBBoxGeneration;
	};
}
#endif
