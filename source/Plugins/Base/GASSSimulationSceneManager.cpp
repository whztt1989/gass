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

#include "GASSSimulationSceneManager.h"
#include "GASSSimulationSystem.h"
#include "Core/Utils/GASSLogManager.h"
#include "Sim/GASSSimEngine.h"
#include "Sim/GASSSimSystemManager.h"

namespace GASS
{
	SimulationSceneManager::SimulationSceneManager() 
	{

	}

	SimulationSceneManager::~SimulationSceneManager()
	{

	}

	void SimulationSceneManager::RegisterReflection()
	{
	}

	void SimulationSceneManager::OnCreate()
	{

	}
	
	void SimulationSceneManager::OnInit()
	{
		SimulationSystemPtr system =  SimEngine::GetPtr()->GetSimSystemManager()->GetFirstSystemByClass<SimulationSystem>();
		SystemListenerPtr listener = shared_from_this();
		system->Register(listener);
	}

	void SimulationSceneManager::OnShutdown()
	{

	}
	
}
