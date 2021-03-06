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

#include "Core/Serialize/GASSSerialize.h"

namespace GASS
{
	template <>
	void SerialSaver::IO<std::string>(std::string &value)
	{
		if(buffer)
		{
			unsigned long l = static_cast<unsigned long>(value.length());
			IO<unsigned long>(l);
			if(bHasOverflowed)return;
			if(bytesUsed+l>length){bHasOverflowed=true; return; }
			memcpy(buffer,value.c_str(),l);
			buffer+=l; bytesUsed+=l;
		}
		else
		{
			int type_size = sizeof(unsigned long);
			length +=type_size;
			length += static_cast<int>( value.length());
		}
	}

	template <>
	void SerialLoader::IO<std::string>(std::string &value)
	{
		unsigned long l;
		IO<unsigned long>(l);
		if(bHasOverflowed)return;
		if(bytesUsed + l > length){bHasOverflowed=true; return; }
		char *szBuf=new char[l+1];
		szBuf[l]=0;
		memcpy(szBuf,buffer,l);
		value=szBuf;
		delete[] szBuf;
		buffer+=l; bytesUsed+=l;
	}

	template <>
	void SerialSaver::IO<FilePath>(FilePath &path)
	{
	    std::string value = path.GetRawPath();
		IO(value);
	}


	template <>
	void SerialLoader::IO<FilePath>(FilePath &path)
	{
		std::string value;
		IO(value);
		path = FilePath(value);
	}


}
