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

#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <osg/PositionAttitudeTransform>
#include <osgGA/NodeTrackerManipulator>
#include <osgGA/TrackballManipulator>
#include <osg/MatrixTransform>
#include <osgShadow/ShadowTechnique>
#include <osgEarth/MapNode>
#include <osgEarth/ECEF>

#include <osgEarthUtil/AnnotationEvents>
#include <osgEarthUtil/ExampleResources>
#include <osgEarthAnnotation/AnnotationEditing>
#include <osgEarthAnnotation/AnnotationRegistry>
#include <osgEarthAnnotation/ImageOverlay>
#include <osgEarthAnnotation/ImageOverlayEditor>
#include <osgEarthAnnotation/CircleNode>
#include <osgEarthAnnotation/RectangleNode>
#include <osgEarthAnnotation/EllipseNode>
#include <osgEarthAnnotation/PlaceNode>
#include <osgEarthAnnotation/LabelNode>
#include <osgEarthAnnotation/LocalGeometryNode>
#include <osgEarthAnnotation/FeatureNode>
//#include <osgEarthAnnotation/Decluttering>
#include <osgEarthAnnotation/HighlightDecoration>
#include <osgEarthAnnotation/ScaleDecoration>

#include <osgEarthSymbology/GeometryFactory>
#include <osg/Camera>

//#include <GL/gl.h>
//#include <GL/glu.h>

#include "OSGEarthLocationComponent.h"

#include "Plugins/OSG/OSGNodeMasks.h"
#include "Plugins/OSG/OSGConvert.h"
#include "Plugins/OSG/IOSGGraphicsSceneManager.h"
#include "Plugins/OSG/IOSGGraphicsSystem.h"


namespace GASS
{
	OSGEarthLocationComponent::OSGEarthLocationComponent() :m_Latitude(0),
		m_Longitude(0),
		m_DebugNode(NULL),
		m_Offset(0)
	{

	}

	OSGEarthLocationComponent::~OSGEarthLocationComponent()
	{

	}

	void OSGEarthLocationComponent::RegisterReflection()
	{
		ComponentFactory::GetPtr()->Register("OSGEarthLocationComponent",new Creator<OSGEarthLocationComponent, Component>);
		GetClassRTTI()->SetMetaData(ClassMetaDataPtr(new ClassMetaData("Component used to handle object position, rotation", OF_VISIBLE)));
		RegisterProperty<double>("Latitude", &OSGEarthLocationComponent::GetLatitude, &OSGEarthLocationComponent::SetLatitude,
			BasePropertyMetaDataPtr(new BasePropertyMetaData("", PF_VISIBLE | PF_EDITABLE)));
		RegisterProperty<double>("Longitude", &OSGEarthLocationComponent::GetLongitude, &OSGEarthLocationComponent::SetLongitude,
			BasePropertyMetaDataPtr(new BasePropertyMetaData("", PF_VISIBLE | PF_EDITABLE)));
		RegisterProperty<double>("Offset", &OSGEarthLocationComponent::GetOffset, &OSGEarthLocationComponent::SetOffset,
			BasePropertyMetaDataPtr(new BasePropertyMetaData("", PF_VISIBLE | PF_EDITABLE)));
	}
	
	void OSGEarthLocationComponent::OnDelete()
	{

	}

	void OSGEarthLocationComponent::OnInitialize()
	{
		IOSGGraphicsSceneManagerPtr osg_sm  = GetSceneObject()->GetScene()->GetFirstSceneManagerByClass<IOSGGraphicsSceneManager>();
		osg::ref_ptr<osg::Group> root = osg_sm->GetOSGRootNode();
		//osg::Group* root = static_cast<osg::Group*> (message->GetUserData());
		osgEarth::MapNode* mapNode = osgEarth::MapNode::findMapNode(root);
		if(mapNode)
		{
			m_MapNode = mapNode;
			const osgEarth::SpatialReference* geoSRS = mapNode->getMapSRS()->getGeographicSRS();
			/*osgEarth::Style labelStyle;
			labelStyle.getOrCreate<osgEarth::TextSymbol>()->alignment() = osgEarth::TextSymbol::ALIGN_CENTER_CENTER;
			labelStyle.getOrCreate<osgEarth::TextSymbol>()->fill()->color() = osgEarth::Color::Yellow;
			m_DebugNode = new osgEarth::Annotation::PlaceNode(mapNode, osgEarth::GeoPoint(geoSRS, -80.28, 25.82), "Miami", labelStyle);
			m_DebugNode->setDynamic(true);
			root->addChild(m_DebugNode);
			*/
			//Get lat lon from location
			UpdateNode();
			//HACK: send a request with delay to give osgearth some time to load better height data:(
			//GetSceneObject()->PostRequest(GeoLocationRequestPtr(new GeoLocationRequest(m_Latitude,m_Longitude,-1,10)));
		}

		GetSceneObject()->RegisterForMessage(REG_TMESS(OSGEarthLocationComponent::OnGeoLocationRequest,GeoLocationRequest,0));
		GetSceneObject()->RegisterForMessage(REG_TMESS(OSGEarthLocationComponent::OnTransformation, TransformationChangedEvent, 0));
		
		if(m_DebugNode)
		{
			GetSceneObject()->RegisterForMessage(REG_TMESS(OSGEarthLocationComponent::OnNameChanged,SceneObjectNameMessage,0));
			GetSceneObject()->RegisterForMessage(REG_TMESS(OSGEarthLocationComponent::OnCaptionChanged,TextCaptionRequest,0));
		}
	}

	double OSGEarthLocationComponent::GetLatitude() const
	{ 
		return m_Latitude;
	}

	void OSGEarthLocationComponent::SetLatitude(double lat)
	{ 
		m_Latitude = lat;
		UpdateNode();
	}

	double OSGEarthLocationComponent::GetLongitude() const
	{ 
		return m_Longitude;
	}

	void OSGEarthLocationComponent::SetLongitude(double lat)
	{ 
		m_Longitude = lat;
		UpdateNode();
	}

	void OSGEarthLocationComponent::OnGeoLocationRequest(GeoLocationRequestPtr message)
	{
		m_Latitude = message->GetLatitude();
		m_Longitude = message->GetLongitude();
		UpdateNode();
	}

	void OSGEarthLocationComponent::OnTransformation(TransformationChangedEventPtr message)
	{
		const osgEarth::SpatialReference* geoSRS = m_MapNode->getMapSRS()->getGeographicSRS();
		const osgEarth::SpatialReference* mapSRS = m_MapNode->getMapSRS();

		osg::Vec3d pos = OSGConvert::ToOSG(message->GetPosition());
		osgEarth::GeoPoint mapPos;
		mapPos.fromWorld(geoSRS,pos);
		m_Latitude = mapPos.y();
		m_Longitude = mapPos.x();
	}

	void OSGEarthLocationComponent::UpdateNode()
	{
		if(m_MapNode)
		{
			 const osgEarth::SpatialReference* geoSRS = m_MapNode->getMapSRS()->getGeographicSRS();
			 const osgEarth::SpatialReference* mapSRS = m_MapNode->getMapSRS();
			 
			 double height = 0;

			 //Create geocentric coordinates from lat long, use  Geographic-SRS!
			 m_MapNode->getTerrain()->getHeight(0L, geoSRS,m_Longitude,m_Latitude, &height, 0L);
			 osgEarth::GeoPoint mapPoint(geoSRS, m_Longitude, m_Latitude, height,osgEarth::ALTMODE_ABSOLUTE);

			 height += m_Offset;
			 
			 //Transform geocentric coordinates to map-space using map-SRS!
			 osgEarth::GeoPoint mapPos = mapPoint.transform(mapSRS);
			 osg::Matrixd out_local2world;
			 mapPos.createLocalToWorld(out_local2world);
			 
			 osg::Quat osg_rot = out_local2world.getRotate();
			 osg::Vec3d osg_pos = out_local2world.getTrans();

			 
			 //GetSceneObject()->PostRequest(WorldPositionRequestPtr(new WorldPositionRequest(Vec3(osg_pos.x(),osg_pos.z(),-osg_pos.y()))));
			 //GetSceneObject()->PostRequest(WorldRotationRequestPtr(new WorldRotationRequest(Quaternion(osg_rot.w(),-osg_rot.x(),-osg_rot.z(),osg_rot.y()))));
			 GetSceneObject()->PostRequest(WorldPositionRequestPtr(new WorldPositionRequest(OSGConvert::ToGASS(osg_pos))));
			 GetSceneObject()->PostRequest(WorldRotationRequestPtr(new WorldRotationRequest(OSGConvert::ToGASS(osg_rot))));

			 if (m_DebugNode)
				 m_DebugNode->setPosition(mapPoint);
		}
	}

	void OSGEarthLocationComponent::OnNameChanged(SceneObjectNameMessagePtr message)
	{
		if(m_DebugNode)
			m_DebugNode->setText(message->GetName());
	}

	void OSGEarthLocationComponent::OnCaptionChanged(TextCaptionRequestPtr message)
	{
		if(m_DebugNode)
			m_DebugNode->setText(message->GetCaption());
	}
}

