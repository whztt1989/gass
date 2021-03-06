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
//NOTE THAT THIS FILE IS BASED ON CODE FROM: Ogre3d?

#ifndef GASS_QUARTERNION_H
#define GASS_QUARTERNION_H

#include "Core/Math/GASSMatrix.h"
#include "Core/Math/GASSVector.h"

namespace GASS
{

	/** \addtogroup GASSCore
	*  @{
	*/
	/** \addtogroup Math
	*  @{
	*/

	/**
	* Class holding quaternion information used for 
	   rotation representation.
	*/

	class GASSCoreExport Quaternion
	{
	public:
		Quaternion (
			Float fW = 1.0,
			Float fX = 0.0, Float fY = 0.0, Float fZ = 0.0);
		Quaternion (const Quaternion& rkQ);
		Quaternion(const Vec3 &euler_rot);

		void FromEulerAngles (const Vec3 &rot);
		void FromRotationMatrix (const Mat4& kRot);
		Mat4 GetRotationMatrix() const
		{
			Mat4 rot_mat;
			rot_mat.Identity();
			ToRotationMatrix(rot_mat);
			return rot_mat;
		}
		void ToRotationMatrix (Mat4& kRot) const;

		void FromAngleAxis (const Float& rfAngle, const Vec3& rkAxis);
		void ToAngleAxis (Float& rfAngle, Vec3& rkAxis) const;
		void FromAxes (const Vec3* akAxis);
		
		/** Constructs the quaternion using 3 axes, the axes are assumed to be orthonormal
			@see ToAxes
		*/
		void FromAxes(const Vec3& xAxis, const Vec3& yAxis, const Vec3& zAxis);

		/** Gets the 3 orthonormal axes defining the quaternion. @see FromAxes */
		void ToAxes(Vec3* akAxis) const;
		void ToAxes(Vec3& xAxis, Vec3& yAxis, Vec3& zAxis) const;

		/** Returns the X orthonormal axis defining the quaternion. Same as doing
			xAxis = Vector3::UNIT_X * this. Also called the local X-axis
		*/
		Vec3 GetXAxis(void) const;

		/** Returns the Y orthonormal axis defining the quaternion. Same as doing
			yAxis = Vector3::UNIT_Y * this. Also called the local Y-axis
		*/
		Vec3 GetYAxis(void) const;

		/** Returns the Z orthonormal axis defining the quaternion. Same as doing
			zAxis = Vector3::UNIT_Z * this. Also called the local Z-axis
		*/
		Vec3 GetZAxis(void) const;

		Quaternion& operator= (const Quaternion& rkQ);
		Quaternion operator+ (const Quaternion& rkQ) const;
		Quaternion operator- (const Quaternion& rkQ) const;
		Quaternion operator* (const Quaternion& rkQ) const;
		Quaternion operator* (Float fScalar) const;

		friend Quaternion operator* (Float fScalar,
			const Quaternion& rkQ);
		Quaternion operator- () const;
		bool operator== (const Quaternion& rhs) const;
		bool operator!= (const Quaternion &q) const;

		// functions of a quaternion
		Float Dot (const Quaternion& rkQ) const;  // dot product
		Float Norm () const;  // squared-length
		Quaternion Inverse () const;  // apply to non-zero quaternion
		Quaternion UnitInverse () const;  // apply to unit-length quaternion
		Quaternion Exp () const;
		Quaternion Log () const;

		// rotation of a vector by a quaternion
		Vec3 operator* (const Vec3& rkVector) const;

		// spherical linear interpolation
		static Quaternion Slerp (Float fT, const Quaternion& rkP,
			const Quaternion& rkQ);

		static Quaternion Slerp2(Float t, const Quaternion &q1, const Quaternion &q2);

		static Quaternion SlerpExtraSpins (Float fT,
			const Quaternion& rkP, const Quaternion& rkQ,
			int iExtraSpins);

		// setup for spherical quadratic interpolation
		static void Intermediate (const Quaternion& rkQ0,
			const Quaternion& rkQ1, const Quaternion& rkQ2,
			Quaternion& rka, Quaternion& rkB);

		// spherical quadratic interpolation
		static Quaternion Squad (Float fT, const Quaternion& rkP,
			const Quaternion& rkA, const Quaternion& rkB,
			const Quaternion& rkQ);

		friend std::ostream& operator << (std::ostream& os, const Quaternion& q)
		{
			os << q.x << " " << q.y << " " << q.z << " " << q.w;
			return os;
		}

		friend std::istream& operator >> (std::istream& is, Quaternion& q)
		{
			if(!(is >> q.x >>  q.y >> q.z >> q.w))
			{
				GASS_EXCEPT(Exception::ERR_INVALIDPARAMS, "Failed to parse Quaternion","Quaternion::>>");
			}
			return is;
		}

		// cutoff for sine near zero
		static const Float ms_fEpsilon;

		// special values
		static const Quaternion ZERO;
		static const Quaternion IDENTITY;

		Float w, x, y, z;

	};
}
#endif // #ifndef QUARTERNION_HH
