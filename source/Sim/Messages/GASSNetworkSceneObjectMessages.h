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
#include "Core/MessageSystem/GASSBaseMessage.h"
#include "Core/MessageSystem/GASSIMessage.h"
#include "Core/Math/GASSVector.h"
#include "Core/Math/GASSQuaternion.h"

namespace GASS
{
	class SimSceneManager;
	class ISceneManager;
	class IGeometryComponent;
	typedef GASS_SHARED_PTR<ISceneManager> SceneManagerPtr;
	typedef GASS_SHARED_PTR<IGeometryComponent> GeometryComponentPtr;



	//*********************************************************
	// ALL MESSAGES IN THIS SECTION CAN BE POSTED BY USER
	//*********************************************************

	//*********************************************************
	// ALL MESSAGES BELOW SHOULD ONLY BE POSTED GASS INTERNALS
	//*********************************************************

	/*class LoadNetworkComponentsMessage : public SceneObjectRequestMessage
	{
	public:
		LoadNetworkComponentsMessage(SceneManagerPtr network_scene_manager, SenderID sender_id = -1, double delay= 0) :
		  SceneObjectRequestMessage( sender_id , delay), m_NetworkSceneManager(network_scene_manager){}
		  SceneManagerPtr GetNetworkSceneManager() const {return m_NetworkSceneManager;}
	private:
		SceneManagerPtr m_NetworkSceneManager;
	};
	typedef GASS_SHARED_PTR<LoadNetworkComponentsMessage> LoadNetworkComponentsMessagePtr;
	*/
	
	

	//typedef GASS_SHARED_PTR<char> NetworkDataPtr;
	class NetworkPackage
	{
	public:
		NetworkPackage()
		{}
		NetworkPackage(int id) : Id(id)
		{}
		virtual ~NetworkPackage(){}
		virtual int GetSize() = 0;
		virtual void Assign(char* data) = 0;
		int Id;
		//NetworkDataPtr Data;
	};
	typedef GASS_SHARED_PTR<NetworkPackage> NetworkPackagePtr;

	class NetworkAddress
	{
	public:
		NetworkAddress(unsigned int address = 0, unsigned int port = 0) : m_Address(address),m_Port(port) {}
		unsigned int m_Address;
		unsigned int m_Port;
	};

	class NetworkSerializeRequest : public SceneObjectRequestMessage
	{

	public:
		NetworkSerializeRequest(const NetworkAddress &address, unsigned int time_stamp, NetworkPackagePtr package, SenderID sender_id = -1, double delay= 0) :
		  SceneObjectRequestMessage( sender_id , delay),
			  m_Package(package),
			  m_TimeStamp(time_stamp),
			  m_Address(address)
		  {
		  }
		  NetworkPackagePtr GetPackage() const {return m_Package;}
		  unsigned int GetTimeStamp() const {return m_TimeStamp;}
		  NetworkAddress GetAddress() const {return m_Address;}
	private:
		NetworkPackagePtr m_Package;
		unsigned int m_TimeStamp;
		NetworkAddress m_Address;

	};
	typedef GASS_SHARED_PTR<NetworkSerializeRequest> NetworkSerializeRequestPtr;



	class NetworkDeserializeRequest : public SceneObjectRequestMessage
	{
	public:
		NetworkDeserializeRequest(const NetworkAddress &address, unsigned int time_stamp, NetworkPackagePtr package, SenderID sender_id = -1, double delay= 0) :
		  SceneObjectRequestMessage( sender_id , delay),
			  m_Package(package),
			  m_TimeStamp(time_stamp),
			  m_Address(address)
		  {
		  }
		  NetworkPackagePtr GetPackage() const {return m_Package;}
		  unsigned int GetTimeStamp() const {return m_TimeStamp;}
		  NetworkAddress GetAddress() const {return m_Address;}
	private:
		NetworkPackagePtr m_Package;
		unsigned int m_TimeStamp;
		NetworkAddress m_Address;

	};
	typedef GASS_SHARED_PTR<NetworkDeserializeRequest> NetworkDeserializeRequestPtr;

	class ClientRemoteMessage : public SceneObjectRequestMessage
	{
	public:
		ClientRemoteMessage(const std::string &client, const std::string message, const std::string data, SenderID sender_id = -1, double delay= 0) :
		  SceneObjectRequestMessage(sender_id , delay) , m_Client(client),m_Message(message){}
		  std::string GetClient() const {return m_Client;}
		  std::string GetMessage() const {return m_Message;}
		  std::string GetData() const {return m_Data;}
	private:
		std::string m_Client;
		std::string m_Message;
		std::string m_Data;
	};
	typedef GASS_SHARED_PTR<ClientRemoteMessage> ClientRemoteMessagePtr;

	
}
