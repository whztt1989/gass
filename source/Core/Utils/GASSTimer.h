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

#ifndef TIMER_HH
#define TIMER_HH

#include "Core/Common.h"

#ifndef WIN32
#include <inttypes.h>
#endif

namespace GASS
{

	/** \addtogroup GASSCore
	*  @{
	*/
	/** \addtogroup Utility
	*  @{
	*/

	/**
		Class used for timing 
	*/

	class GASSCoreExport Timer
	{
	public:
		Timer();
		virtual ~Timer();
		/**
			Get time in seconds
		*/
		double GetTime() const;
		/**
			Reset time to 0
		*/
		void Reset();
	private:
#ifdef WIN32
	    __int64			m_PerfTimerStart;	// Performance Timer Start Value
		__int64			m_PerfTimerElapsed;	// Performance Timer Elapsed Time
		bool			m_PerfTimer;		// Using The Performance Timer?
		__int64			m_Frequency;		// Timer Frequency
#else
        int64_t         m_Frequency;		// Timer Frequency
#endif
		float			m_Resolution;		// Timer Resolution
		unsigned long	m_MMTimerStart;		// Multimedia Timer Start Value
		unsigned long	m_MMTimerElapsed;	// Multimedia Timer Elapsed Time
		double m_StartTime;


	};
}

#endif // #ifndef TIMER_HH
