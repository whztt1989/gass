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


#include "Core/Common.h"
#include "GASSFileUtils.h"
#include "GASSLogManager.h"
#include "GASSStringUtils.h"
#include "GASSFilesystem.h"


namespace GASS
{
	FileUtils::FileUtils()
	{

	}

	FileUtils::~FileUtils()
	{

	}


	std::string FileUtils::GetExtension(const std::string &file_name)
	{
		std::string ret, reversed_string;
		std::string::size_type pos  = file_name.find_last_of(".");

		if (pos == file_name.npos)
			return "";

		ret = file_name.substr(pos+1);

		return ret;
	}

	std::string FileUtils::RemoveExtension(const std::string &file_name)
	{
		std::string ret, reversed_string;
		std::string::size_type pos  = file_name.find_last_of(".");

		if (pos == file_name.npos)
			return "";

		ret = file_name.substr(0, pos);

		return ret;
	}



	std::string FileUtils::GetFilename(const std::string &path)
	{
		std::string ret = path;
		std::string::size_type pos = path.find_last_of("/\\");
		if(pos != std::string::npos)
		{
			ret = path.substr(pos+1);
		}
		return ret;
	}

	std::string FileUtils::RemoveFilename(const std::string &path)
	{
		std::string ret = StringUtils::Replace(path,"\\","/");
		std::string::size_type pos = ret.find_last_of("/");

		if(pos != std::string::npos)
		{
			ret = ret.substr(0,pos+1);
		}
		else
			return "";
		return ret;
	}

	bool FileUtils::FileExist(const std::string &file_name)
	{
		GASS_FILESYSTEM::path boost_path(file_name); 
		return GASS_FILESYSTEM::exists(boost_path);
	}


	void FileUtils::GetFilesFromPath(std::vector<std::string> &files, const std::string &path, bool recursive, bool full_path)
	{
		GASS_FILESYSTEM::path boost_path(path); 
		if( GASS_FILESYSTEM::exists(boost_path))  
		{
			GASS_FILESYSTEM::directory_iterator end ;    
			for( GASS_FILESYSTEM::directory_iterator iter(boost_path) ; iter != end ; ++iter )      
			{
				if (GASS_IS_DIRECTORY( *iter )  && recursive)      
				{   
					GetFilesFromPath(files,iter->path().string(), recursive,full_path);
				}
				else
				{
					std::string filename; 
					if(full_path)
						filename = iter->path().string();
					else
						filename = GASS_TO_GENERIC_STRING(iter->path().filename());
					files.push_back(filename);
				}
			}
		}
	}
}
