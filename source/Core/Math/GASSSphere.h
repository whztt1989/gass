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

#ifndef GASS_SPHERE_H
#define GASS_SPHERE_H

#include "Core/Common.h"
#include "Core/Math/GASSVector.h"


namespace GASS
{

	/** \addtogroup GASSCore
	*  @{
	*/
	/** \addtogroup Math
	*  @{
	*/

	/**
	 Class holding Sphere information, position and radius
	*/

	class AABox;
	class GASSCoreExport Sphere
	{
	public:
		Sphere();
		Sphere(const Vec3& center,float radius);
		virtual ~Sphere();
		AABox GetAABox() const;
		bool AABoxInside(const AABox &box) const;
		bool SphereInside(const Sphere &sphere) const;
		void Union(const Sphere &sphere);

		//public for fast access
		float m_Radius;
		Vec3 m_Pos;
	};
}
#endif // #ifndef SPHERE_HH
