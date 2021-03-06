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


#ifndef GASS_I_PROPERTY_H
#define GASS_I_PROPERTY_H

#include "Core/Common.h"
#include "Core/Reflection/GASSPropertyMetaData.h"

namespace GASS
{
	class ISerializer;
	class BaseReflectionObject;

	/** \addtogroup GASSCore
	*  @{
	*/
	/** \addtogroup Reflection
	*  @{
	*/

	/**
        Interface class for all properties
	*/
	class IProperty
	{
	public:
        /**
            Constructor
            @param name Name of this property
        */
        IProperty( const std::string  &name ) :
		m_Name(name)
        {

        }

        std::string GetName() const
        {
            return m_Name;
        }
		virtual const std::type_info* GetTypeID() const = 0;
		virtual std::string GetTypeName() const = 0;

		/**
            Set value of this property by string
            @param object The object that is owner of this property
            @param value The value provided as string
        */
		virtual void SetValueByString(BaseReflectionObject* object,const std::string &s) = 0;

		/**
            Get the value of this property
            @param object The object that is owner of this property
        */
    	virtual std::string GetValueAsString( const BaseReflectionObject* pObject) const = 0;

		/**
            Serialize this property
            @param object The object that is owner of this property
            @param serializer The serializer, can be Loader,Saver or Sizer
        */
		virtual void Serialize(BaseReflectionObject* object, ISerializer* serializer) = 0;
		/**
            Transfer this property from one object to another
            @param dest The object that should be modified
            @param src The object that the property value should be fetch from
        */
		virtual void SetValue(BaseReflectionObject* dest, const BaseReflectionObject* src) = 0;

		/**
            Set this property value by providing the value by the boost any class
            @param object The object that is owner of this property
            @param value Value wrapped in the GASS_ANY class
        */
		virtual void SetValue(BaseReflectionObject* object, GASS_ANY &value) = 0;

		/**
            Get the value of this property, retured by the GASS_ANY class
            @param object The object that is owner of this property
            @param value The value returned, wrapped in the GASS_ANY class
        */
		virtual void GetValue(const BaseReflectionObject* object, GASS_ANY &value) const = 0;

		/**
			Check if this property has meta data
		*/
		virtual bool HasMetaData() const = 0;
		
		/**
			Get meta data for this object, if not present an exception is thrown (use HasMetaData to be sure)
		*/
		virtual PropertyMetaDataPtr GetMetaData() const = 0;

	protected :
		std::string	m_Name;
	};
}

#endif
