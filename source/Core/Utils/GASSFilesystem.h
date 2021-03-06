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

#ifdef GASS_USE_BOOST_FILESYSTEM
#include <boost/filesystem.hpp>
#define GASS_FILESYSTEM boost::filesystem
#define GASS_TO_GENERIC_STRING(a) a.generic_string()
#define GASS_CURRENT_PATH GASS_FILESYSTEM::current_path
#define GASS_IS_DIRECTORY GASS_FILESYSTEM::is_directory
#else //use experimental filesystem, TODO:test gcc
#include <filesystem>
#define GASS_FILESYSTEM std::tr2::sys
//hacks to support msvc2013, in msvc2015 
#if (_MSC_VER == 1800)
#define GASS_TO_GENERIC_STRING(a) a
#define GASS_CURRENT_PATH GASS_FILESYSTEM::current_path<GASS_FILESYSTEM::path>
#define GASS_IS_DIRECTORY GASS_FILESYSTEM::is_directory<GASS_FILESYSTEM::path>
#else //same as boost
#define GASS_TO_GENERIC_STRING(a) a.generic_string()
#define GASS_CURRENT_PATH GASS_FILESYSTEM::current_path
#define GASS_IS_DIRECTORY GASS_FILESYSTEM::is_directory
#endif
#endif