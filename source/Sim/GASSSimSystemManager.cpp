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

#include "Sim/GASSSystemFactory.h"
#include "Core/MessageSystem/GASSMessageManager.h"
#include "Core/Utils/GASSLogManager.h"
#include "Core/Utils/GASSException.h"
#include "Core/Serialize/GASSIXMLSerialize.h"
#include "Core/MessageSystem/GASSIMessage.h"
#include "Core/RTC/GASSTBBManager.h"
#include "Sim/GASSSimSystemManager.h"
#include "Sim/Utils/GASSSimpleProfile.h"
#include "tinyxml2.h"

namespace GASS
{
	SimSystemManager::SimSystemManager() 
	{
		m_SystemMessageManager = MessageManagerPtr(new MessageManager());
		//m_SimStats = new SimpleProfileDataMap;
	}

	SimSystemManager::~SimSystemManager()
	{
		//delete m_SimStats;
	}

	void SimSystemManager::Init()
	{
		LogManager::getSingleton().stream() << "SimSystemManager Initialization Started";
		for(size_t i = 0 ; i < m_Systems.size(); i++)
		{
			m_Systems[i]->Init();
			//auto register for updates
			m_Systems[i]->RegisterForUpdate();
		}
		LogManager::getSingleton().stream() << "SimSystemManager Initialization Completed";
	}	



	void SimSystemManager::SyncMessages(double delta_time)
	{
		m_SystemMessageManager->Update(delta_time);
	}

	size_t SimSystemManager::GetQueuedMessages() const
	{
		return m_SystemMessageManager->GetQueuedMessages();
	}

	int SimSystemManager::RegisterForMessage(const MessageType &type, MessageFuncPtr callback, int priority)
	{
		return m_SystemMessageManager->RegisterForMessage(type, callback, priority); 
	}

	void SimSystemManager::UnregisterForMessage(const MessageType &type,  MessageFuncPtr callback)
	{
		m_SystemMessageManager->UnregisterForMessage(type,  callback);
	}

	void SimSystemManager::PostMessage( SystemMessagePtr message )
	{
		m_SystemMessageManager->PostMessage(message);
	}

	void SimSystemManager::SendImmediate( SystemMessagePtr message )
	{
		m_SystemMessageManager->SendImmediate(message);
	}

	void SimSystemManager::ClearMessages()
	{
		m_SystemMessageManager->Clear();
	}

	SimSystemPtr SimSystemManager::GetSystemByName(const std::string &system_name) const
	{
		for(size_t i = 0 ; i < m_Systems.size(); i++)
		{
			if(system_name ==  m_Systems[i]->GetSystemName())
			{
				return GASS_DYNAMIC_PTR_CAST<SimSystem>(m_Systems[i]);
			}
		}
		return SimSystemPtr();
	}


	void SimSystemManager::Load(const std::string &filename)
	{
		if(filename =="")
			GASS_EXCEPT(Exception::ERR_INVALIDPARAMS,"No File name provided", "SimSystemManager::Load");
		
		tinyxml2::XMLDocument *xmlDoc = new tinyxml2::XMLDocument();
		if (xmlDoc->LoadFile(filename.c_str()) != tinyxml2::XML_NO_ERROR)
		{
			delete xmlDoc;
			GASS_EXCEPT(Exception::ERR_CANNOT_READ_FILE, "Failed to load:" + filename,"SimSystemManager::Load");
		}
		
		tinyxml2::XMLElement *systems = xmlDoc->FirstChildElement("GASS");
		systems = systems->FirstChildElement("Systems");

		if(systems)
		{
			systems= systems->FirstChildElement();
			//Load all systems tags
			while(systems)
			{
				SimSystemPtr system = LoadSystem(systems);
				if(system)
				{
					system->OnCreate(shared_from_this());
					LogManager::getSingleton().stream() << system->GetSystemName() << " created";
					
					m_Systems.push_back(system);
					
				}
				systems  = systems->NextSiblingElement();
			}
		}
		xmlDoc->Clear();
		// Delete our allocated document and return success
		delete xmlDoc;
	}

	void SimSystemManager::AddSystem(SimSystemPtr system)
	{
		m_Systems.push_back(system);
	}

	SimSystemPtr SimSystemManager::LoadSystem(tinyxml2::XMLElement *system_elem)
	{
		const std::string system_type = system_elem->Value();
		SimSystemPtr system = SystemFactory::Get().Create(system_type);
		if(system)
		{
			XMLSerializePtr  serialize = GASS_DYNAMIC_PTR_CAST<IXMLSerialize> (system);
			if(serialize)
				serialize->LoadXML(system_elem);
		}
		return system;
	}
}
