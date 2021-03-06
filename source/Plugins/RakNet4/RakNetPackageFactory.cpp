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
#include "RakNetPackageFactory.h"

namespace GASS
{
	PackageFactory::PackageFactory()
	{

	}

	PackageFactory::~PackageFactory()
	{

	}

	PackageFactory* PackageFactory::m_Instance = 0;
	PackageFactory* PackageFactory::GetPtr(void)
	{
		//assert(m_Instance);
		if(m_Instance == NULL)
			m_Instance = new PackageFactory();
		return m_Instance;
	}

	PackageFactory& PackageFactory::Get(void)
	{
		//assert(m_Instance);
		if(m_Instance == NULL)
			m_Instance = new PackageFactory();
		return *m_Instance;
	}
}