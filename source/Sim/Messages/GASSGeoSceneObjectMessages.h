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
#include "Sim/Messages/GASSCoreSceneObjectMessages.h"

namespace GASS
{

	/**
		Message used to set object position by latitude and longitude
	*/
	class GeoLocationRequest : public SceneObjectRequestMessage
	{
	public:
		GeoLocationRequest(double lat, double lon, double alt = 0, bool ground_clamp = false, SenderID sender_id = -1, double delay= 0) : SceneObjectRequestMessage(sender_id , delay), 
			m_Latitude(lat),
			m_Longitude(lon),
			m_Altitude(alt),
			m_GroundClamp(ground_clamp)
		{
		}
		double GetLatitude() const {return m_Latitude;}
		double GetLongitude() const {return m_Longitude;}
		double GetAltitude() const {return m_Altitude;}
		bool GetGroundClamp() const {return m_GroundClamp;}
	private:
		double m_Latitude;
		double m_Longitude;
		bool m_GroundClamp;
		double m_Altitude;
	};
	typedef GASS_SHARED_PTR<GeoLocationRequest> GeoLocationRequestPtr;
	
}
