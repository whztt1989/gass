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

#ifndef GASS_POLYGON_H
#define GASS_POLYGON_H

#include "Core/Common.h"
#include <vector>
#include "Core/Math/GASSVector.h"

namespace GASS
{
	class GASSCoreExport Polygon
	{
	public:
		Polygon();
		~Polygon();
		Vec3 Center() const;
		
		/**\var vector<Vec3> m_VertexVector;
		* \brief Contains the vertices of the polygon.
		*/
		/**\var Vec3 m_Normal;
		* \brief The normal of tha polygon plane.
		*/
		std::vector<Vec3> m_VertexVector;
		Vec3 m_Normal;
	};
}

#endif // #ifndef POLYGON_HH
