#include "PhysXVehicleSceneQuery.h"
#include "vehicle/PxVehicleSDK.h"
#include "PxFiltering.h"

namespace GASS
{

#define CHECK_MSG(exp, msg) (!!(exp) || (physx::shdfnd::getFoundation().error(physx::PxErrorCode::eINVALID_PARAMETER, __FILE__, __LINE__, msg), 0) )
#define SIZEALIGN16(size) (((unsigned)(size)+15)&((unsigned)(~15)));

	void VehicleSetupDrivableShapeQueryFilterData(PxFilterData* qryFilterData)
	{
//		CHECK_MSG(0==qryFilterData->word3, "word3 is reserved for filter data for vehicle raycast queries");
		qryFilterData->word3 = (PxU32)VEHICLE_DRIVABLE_SURFACE;
	}

	void VehicleSetupNonDrivableShapeQueryFilterData(PxFilterData* qryFilterData)
	{
	//	CHECK_MSG(0==qryFilterData->word3, "word3 is reserved for filter data for vehicle raycast queries");
		qryFilterData->word3 = (PxU32)VEHICLE_UNDRIVABLE_SURFACE;
	}

	void VehicleSetupVehicleShapeQueryFilterData(PxFilterData* qryFilterData)
	{
		//CHECK_MSG(0==qryFilterData->word3, "word3 is reserved for filter data for vehicle raycast queries");
		qryFilterData->word3 = (PxU32)VEHICLE_UNDRIVABLE_SURFACE;
	}

	VehicleSceneQueryData* VehicleSceneQueryData::allocate(const PxU32 maxNumWheels)
	{
		const PxU32 size0 = SIZEALIGN16(sizeof(VehicleSceneQueryData));
		const PxU32 size1 = SIZEALIGN16(sizeof(PxRaycastQueryResult)*maxNumWheels);
		const PxU32 size2 = SIZEALIGN16(sizeof(PxRaycastHit)*maxNumWheels);
		const PxU32 size = size0 + size1 + size2;
		//VehicleSceneQueryData* sqData = (VehicleSceneQueryData*)PX_ALLOC(size, PX_DEBUG_EXP("PxVehicleNWSceneQueryData"));
		VehicleSceneQueryData* sqData = (VehicleSceneQueryData*)malloc(size);
		sqData->init();
		PxU8* ptr = (PxU8*) sqData;
		ptr += size0;
		sqData->mSqResults = (PxRaycastQueryResult*)ptr;
		sqData->mNbSqResults = maxNumWheels;
		ptr += size1;
		sqData->mSqHitBuffer = (PxRaycastHit*)ptr;
		ptr += size2;
		sqData->mNumQueries = maxNumWheels;
		return sqData;
	}

	void VehicleSceneQueryData::free()
	{
		delete (this);
	}

	PxBatchQuery* VehicleSceneQueryData::setUpBatchedSceneQuery(PxScene* scene)
	{
		PxBatchQueryDesc sqDesc(mNbSqResults, 0, 0);
		sqDesc.queryMemory.userRaycastResultBuffer = mSqResults;
		sqDesc.queryMemory.userRaycastTouchBuffer = mSqHitBuffer;
		sqDesc.queryMemory.raycastTouchBufferSize = mNumQueries;
		sqDesc.preFilterShader = mPreFilterShader;
		sqDesc.spuPreFilterShader = mSpuPreFilterShader;
		sqDesc.spuPreFilterShaderSize = mSpuPreFilterShaderSize;
		return scene->createBatchQuery(sqDesc);
	}


	/*void VehicleSetupDrivableShapeQueryFilterData(PxFilterData* qryFilterData)
	{
		qryFilterData->word3 = (PxU32)VEHICLE_DRIVABLE_SURFACE;
	}

	void VehicleSetupNonDrivableShapeQueryFilterData(PxFilterData* qryFilterData)
	{
		qryFilterData->word3 = (PxU32)VEHICLE_UNDRIVABLE_SURFACE;
	}

	void VehicleSetupVehicleShapeQueryFilterData(PxFilterData* qryFilterData)
	{
		qryFilterData->word3 = (PxU32)VEHICLE_UNDRIVABLE_SURFACE;
	}

	VehicleSceneQueryData* VehicleSceneQueryData::Allocate(const PxU32 max_num_wheels)
	{
		const PxU32 size = sizeof(VehicleSceneQueryData) + sizeof(PxRaycastQueryResult)*max_num_wheels + sizeof(PxRaycastHit)*max_num_wheels;
		VehicleSceneQueryData* data = (VehicleSceneQueryData*)malloc(size);
		data->Init();
		PxU8* ptr = (PxU8*) data;
		ptr += sizeof(VehicleSceneQueryData);
		data->m_SqResults = (PxRaycastQueryResult*)ptr;
		ptr +=  sizeof(PxRaycastQueryResult)*max_num_wheels;
		data->m_SqHitBuffer = (PxRaycastHit*)ptr;
		ptr += sizeof(PxRaycastHit)*max_num_wheels;
		data->m_NumQueries = max_num_wheels;
		return data;
	}

	void VehicleSceneQueryData::Free()
	{
		delete this;
	}

	PxBatchQuery* VehicleSceneQueryData::SetUpBatchedSceneQuery(PxScene* scene)
	{
		PxBatchQueryDesc sqDesc;
		sqDesc.userRaycastResultBuffer = m_SqResults;
		sqDesc.userRaycastHitBuffer = m_SqHitBuffer;
		sqDesc.raycastHitBufferSize = m_NumQueries;
		sqDesc.preFilterShader = m_PreFilterShader;
		return scene->createBatchQuery(sqDesc);
	}*/
}




