/****************************************************************************
* This file is part of GASS.                                                *
* See https://github.com/leadcoder/gass                                     *
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

#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <osgGA/NodeTrackerManipulator>
#include <osgGA/TrackballManipulator>
#include <osg/MatrixTransform>
#include <osgShadow/ShadowTechnique>
#include <osgEarth/MapNode>
#include <osgEarth/DateTime>
#include <osgEarthUtil/Sky>
#include <osg/Camera>

//#include <GL/gl.h>
//#include <GL/glu.h>

#include "OSGEarthSkyComponent.h"

#include "Plugins/OSG/OSGNodeMasks.h"
#include "Plugins/OSG/OSGConvert.h"
#include "Plugins/OSG/IOSGGraphicsSceneManager.h"
#include "Plugins/OSG/IOSGGraphicsSystem.h"

namespace GASS
{
	OSGEarthSkyComponent::OSGEarthSkyComponent() 
	{

	}

	OSGEarthSkyComponent::~OSGEarthSkyComponent()
	{

	}

	void OSGEarthSkyComponent::RegisterReflection()
	{
		ComponentFactory::GetPtr()->Register("OSGEarthSkyComponent",new Creator<OSGEarthSkyComponent, Component>);
		//RegisterProperty<std::string>("EarthFile", &OSGEarthSkyComponent::GetEarthFile, &OSGEarthSkyComponent::SetEarthFile);
	}
	
	void OSGEarthSkyComponent::OnDelete()
	{

	}

	void OSGEarthSkyComponent::OnInitialize()
	{
		IOSGGraphicsSceneManagerPtr osg_sm  = GetSceneObject()->GetScene()->GetFirstSceneManagerByClass<IOSGGraphicsSceneManager>();

		osg::ref_ptr<osg::Group> root = osg_sm->GetOSGRootNode();
		//osg::Group* root = static_cast<osg::Group*> (message->GetUserData());
		osgEarth::MapNode* mapNode = osgEarth::MapNode::findMapNode(root);
		if(mapNode)
		{
			osgEarth::Util::SkyNode* sky = osgEarth::Util::SkyNode::create( mapNode);
			sky->setDateTime(osgEarth::DateTime(2013, 1, 6, 17.0));
			root->addChild( sky );

			//hack
			IOSGGraphicsSystemPtr osg_sys = SimEngine::Get().GetSimSystemManager()->GetFirstSystemByClass<IOSGGraphicsSystem>();
			osgViewer::ViewerBase::Views views;
			osg_sys->GetViewer()->getViews(views);
			sky->attach( views[0] );
		}
		//else
		//	Log::Warning("No MapNode found for OSGEarthSkyComponent");
	}

	std::string OSGEarthSkyComponent::GetEarthFile() const
	{ 
		return m_EarthFile;
	}

	void OSGEarthSkyComponent::SetEarthFile(const std::string &earthfile)
	{ 
		m_EarthFile = earthfile;
	}

	
}

