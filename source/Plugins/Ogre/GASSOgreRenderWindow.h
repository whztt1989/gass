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

#include "Sim/Interface/GASSIRenderWindow.h"
#include "Plugins/Ogre/GASSOgreCommon.h"


namespace Ogre
{
	class RenderWindow;
}

namespace GASS
{
	class OgreGraphicsSystem;
	class OgreViewport;
	typedef GASS_SHARED_PTR<OgreViewport> OgreViewportPtr;

	class OgreRenderWindow  : public IRenderWindow
	{
	public:
		typedef std::vector<OgreViewportPtr> OgreViewportVector;
		OgreRenderWindow(OgreGraphicsSystem* system, Ogre::RenderWindow* win) : m_System(system), m_Window(win)
		{

		}
		//IRenderWindow
		virtual unsigned int GetWidth() const;
		virtual unsigned int GetHeight() const;
		virtual void* GetHWND() const;
		OgreGraphicsSystem* GetSystem() const{return m_System;}
		Ogre::RenderWindow* GetOgreWindow() const{return m_Window;}
		ViewportPtr CreateViewport(const std::string &name, float  left, float top, float width, float height);
	private:
		Ogre::RenderWindow* m_Window;
		OgreViewportVector m_Viewports;
		OgreGraphicsSystem* m_System;
	};
	typedef GASS_SHARED_PTR<OgreRenderWindow> OgreRenderWindowPtr;
}