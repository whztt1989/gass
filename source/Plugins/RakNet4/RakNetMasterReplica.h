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

#ifndef RAKNET_MASTER_REPLICA_H
#define RAKNET_MASTER_REPLICA_H
#include "PacketPriority.h"
#include "ReplicaManager3.h"
//R4 #include "AutoRPC.h"
#include "StringTable.h"
#include "BitStream.h"
#include "GetTime.h"
//#include "Network/INetworkObject.h"
#include "Core/Utils/GASSLogManager.h"
#include "Core/Math/GASSVector.h"
#include "Core/Math/GASSQuaternion.h"
//#include "RakNetNetworkComponent.h"
//#include "RakNetReplicaMember.h"
#include "RakNetBaseReplica.h"


class ReplicaManager;
	
namespace GASS
{
	class SceneObject;
	class IProperty;
	typedef SPTR<SceneObject> SceneObjectPtr;

	class RakNetMasterReplica  : public RakNetBaseReplica
	{
	public:
		RakNetMasterReplica(ReplicaManager* manager);
		virtual ~RakNetMasterReplica();
		void RemoteInit(RakNet::BitStream *inBitStream, RakNet::Time timestamp, RakNet::NetworkID networkID, RakNet::SystemAddress senderId);
		void LocalInit(SceneObjectPtr object);
		
		//Replica member functions
		virtual ReplicaReturnResult SendConstruction( RakNet::Time currentTime, RakNet::SystemAddress systemAddress, unsigned int &flags, RakNet::BitStream *outBitStream, bool *includeTimestamp );
		virtual ReplicaReturnResult SendDestruction(RakNet::BitStream *outBitStream, RakNet::SystemAddress systemAddress, bool *includeTimestamp);
		virtual ReplicaReturnResult ReceiveDestruction(RakNet::BitStream *inBitStream, RakNet::SystemAddress systemAddress, RakNet::Time timestamp);
		virtual ReplicaReturnResult SendScopeChange(bool inScope, RakNet::BitStream *outBitStream, RakNet::Time currentTime, RakNet::SystemAddress systemAddress, bool *includeTimestamp);
		virtual ReplicaReturnResult ReceiveScopeChange(RakNet::BitStream *inBitStream, RakNet::SystemAddress systemAddress, RakNet::Time timestamp);
		virtual ReplicaReturnResult Serialize(bool *sendTimestamp, RakNet::BitStream *outBitStream, RakNet::Time lastSendTime, PacketPriority *priority, PacketReliability *reliability, RakNet::Time currentTime, RakNet::SystemAddress systemAddress, unsigned int &flags);
		virtual ReplicaReturnResult Deserialize(RakNet::BitStream *inBitStream, RakNet::Time timestamp, RakNet::Time lastDeserializeTime, RakNet::SystemAddress systemAddress );
		virtual int GetSortPriority(void) const 
		{
			return 0;
		}
		virtual void ReceiveConstruction(RakNet::BitStream *inBitStream);
		void SendConstruction(RakNet::BitStream *outBitStream);
		RakNet::SystemAddress  GetOwnerSystemAddress() {return m_OwnerSystemAddress;}
		void SetOwnerSystemAddress(RakNet::SystemAddress sa) {m_OwnerSystemAddress = sa;}
		bool AllowRemoteOwner(){return m_AllowRemoteOwner;}
		std::string GetTemplateName() {return m_TemplateName;}
		
		void SerializeProperties(RakNet::BitStream *bit_stream);
		//int AUTO_RPC_CALLSPEC EnterObject(const char *str, RakNet::AutoRPC* networkCaller);
	protected:
		IProperty* GetProperty(const std::string &prop_name);
		std::string m_TemplateName;
		bool m_AllowRemoteOwner;
	private:
		RakNet::SystemAddress m_OwnerSystemAddress;
		ReplicaManager* m_Manager;
	protected:
	};
}

#endif
