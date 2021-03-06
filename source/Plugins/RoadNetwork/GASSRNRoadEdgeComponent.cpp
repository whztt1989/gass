#include <limits>
#include "GASSRNRoadEdgeComponent.h"
#include "GASSRNRoadNodeComponent.h"
#include "Plugins/Base/CoreMessages.h"
#include "Sim/Messages/GASSPlatformMessages.h"
#include "Sim/Interface/GASSIWaypointListComponent.h"
#include "Sim/Interface/GASSIGraphComponent.h"

namespace GASS
{
	RNRoadEdgeComponent::RNRoadEdgeComponent(void) : m_LaneWidth(2)
	{

	}	

	RNRoadEdgeComponent::~RNRoadEdgeComponent(void)
	{

	}

	void RNRoadEdgeComponent::RegisterReflection()
	{
		ComponentFactory::GetPtr()->Register("RNRoadEdgeComponent",new Creator<RNRoadEdgeComponent, Component>);
		GetClassRTTI()->SetMetaData(ClassMetaDataPtr(new ClassMetaData("RNRoadEdgeComponent", OF_VISIBLE)));
		RegisterProperty<Float>("LaneWidth", &RNRoadEdgeComponent::GetLaneWidth, &RNRoadEdgeComponent::SetLaneWidth,
			BasePropertyMetaDataPtr(new BasePropertyMetaData("Lane width",PF_VISIBLE | PF_EDITABLE)));
	}

	void RNRoadEdgeComponent::OnInitialize()
	{

	}

	void RNRoadEdgeComponent::OnDelete()
	{
		GraphNodeComponentPtr start_node = GASS_DYNAMIC_PTR_CAST<IGraphNodeComponent>(GetStartNode());
		GraphNodeComponentPtr end_node = GASS_DYNAMIC_PTR_CAST<IGraphNodeComponent>(GetEndNode());
		GraphEdgeComponentPtr edge = GASS_DYNAMIC_PTR_CAST<IGraphEdgeComponent>(shared_from_this());
		if(start_node)
			start_node->RemoveEdge(edge);
		if(end_node)
			end_node->RemoveEdge(edge);
	}

	WaypointListComponentPtr RNRoadEdgeComponent::GetWaypointList() const
	{
		return GetSceneObject()->GetChildByID("EDGE_WAYPOINTS")->GetFirstComponentByClass<IWaypointListComponent>();
	}

	std::vector<Vec3> RNRoadEdgeComponent::GetWaypoints() const 
	{
		std::vector<Vec3> road_wps;
		if(GetStartNode())
		{
			RNRoadNodeComponentPtr start_rn = GASS_DYNAMIC_PTR_CAST<RNRoadNodeComponent>(GetStartNode());
			Vec3 start_pos  = start_rn->GetSceneObject()->GetFirstComponentByClass<ILocationComponent>()->GetWorldPosition();
			road_wps.push_back(start_pos);
		}
		std::vector<SceneObjectPtr> objects;
		GetSceneObject()->GetChildrenByID(objects,"ROAD_WP",true,true);
		
		for(size_t i = 0; i < objects.size(); i++)
		{
			LocationComponentPtr wp_location = objects[i]->GetFirstComponentByClass<ILocationComponent>();
			road_wps.push_back(wp_location->GetWorldPosition());
		}
		
		if(GetEndNode())
		{
			RNRoadNodeComponentPtr end_rn = GASS_DYNAMIC_PTR_CAST<RNRoadNodeComponent>(GetEndNode());
			Vec3 end_pos  = end_rn->GetSceneObject()->GetFirstComponentByClass<ILocationComponent>()->GetWorldPosition();
			road_wps.push_back(end_pos);
		}
		return road_wps;
	}

	SceneObjectPtr RNRoadEdgeComponent::GetWaypointListObject() const
	{
		return GetSceneObject()->GetChildByID("EDGE_WAYPOINTS");
	}

	void RNRoadEdgeComponent::SetWaypoints(const std::vector<Vec3> &wps)
	{
		//Add waypoints
		if(wps.size() > 2)
		{
			for(size_t i = 1; i < wps.size()-1; i++)
			{
				SceneObjectPtr node_obj = GetSceneObject()->GetScene()->LoadObjectFromTemplate("RoadWP",GetSceneObject());

				node_obj->RegisterForMessage(REG_TMESS(RNRoadEdgeComponent::OnTransformation,TransformationChangedEvent,0));
				GASSAssert(node_obj,"Failed to create scene object in void RNRoadEdgeComponent::SetWaypoints");
				node_obj->SendImmediateRequest(WorldPositionRequestPtr(new WorldPositionRequest(wps[i])));
			}
		}
	}

	void RNRoadEdgeComponent::OnTransformation(TransformationChangedEventPtr message)
	{
		//update graph
		GraphComponentPtr graph = GetSceneObject()->GetParentSceneObject()->GetFirstComponentByClass<IGraphComponent>();
		GASSAssert(graph,"Failed to find IGraphComponent in RNRoadEdgeComponent::OnTransformation");
		//graph->RebuildGraph();
	}
}
