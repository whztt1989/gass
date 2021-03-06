/****************************************************************************
* This file is part of GASS.                                                *
* See http://code.google.com/p/gass/                                 *
*                                                                           *
* Copyright (c) 2008-2009 GASS team. See Contributors.txt for details.      *
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

#include "Sim/Scheduling/GASSTBBUpdateTask.h"
#include <iostream>


namespace GASS
{
	TBBUpdateTask::TBBUpdateTask(double delta_time, const TaskListenerVector &update_vec): m_DeltaTime(delta_time),m_UpdateVec(update_vec)
	{
	
	}

	tbb::task* TBBUpdateTask::execute()
	{
		for(int i = 0; i < m_UpdateVec.size(); i++)
		{
			m_UpdateVec[i]->Update(m_DeltaTime);
		}
		return NULL;
		//add addinonal tasks added by system
	}
}
