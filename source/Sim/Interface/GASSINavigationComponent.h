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
namespace GASS
{
	typedef std::vector<Vec3> NavigationPath;
	class INavigationComponent
	{
	public:
		virtual ~INavigationComponent(){}
		virtual bool GetShortestPath(const Vec3 &from, const Vec3 &to, NavigationPath &path) const = 0;
	};
	typedef GASS_SHARED_PTR<INavigationComponent> NavigationComponentPtr;
	typedef GASS_WEAK_PTR<INavigationComponent> NavigationComponentWeakPtr;
}
