
#ifndef SIGNATURE_COMPONENT
#define SIGNATURE_COMPONENT

#include "Sim/GASSCommon.h"
#include "Core/Utils/GASSFilePath.h"
#include "Sim/GASS.h"
#include "Plugins/Game/GameMessages.h"
#include "Plugins/Game/PlatformType.h"
#include "Plugins/Game/PlatformTeam.h"

namespace GASS
{
	/*
		Component that should be attached if the scene object should be discoverd by the SensorComponent
	*/
	class SignatureComponent :  public Reflection<SignatureComponent,BaseSceneComponent>
	{
	public:
		SignatureComponent(void);
		~SignatureComponent(void);
		static void RegisterReflection();
		void OnInitialize();
		
		Vec3 GetPosition() const {return m_Pos;}
		Quaternion GetRotation() const {return m_Rotation;}
		Vec3 GetVelocity() const {return m_Velocity;}
		ADD_PROPERTY(float,RadarCrossSection)
		ADD_PROPERTY(PlatformTypeReflection,PlatformType)
		ADD_PROPERTY(PlatformTeamReflection,PlatformTeam)
	protected:
		void SetPlatformType(PlatformType pt) {m_PlatformType = pt;}
		void OnLoad(MessagePtr message);
		void OnUnload(MessagePtr message);
		void OnTransChanged(TransformationChangedEventPtr);
		void OnVelocity(PhysicsVelocityEventPtr message);
	private:
		void Update();
		bool m_Initialized;
		Vec3 m_Pos;
		Quaternion m_Rotation;
		Vec3 m_Velocity;
	};

	typedef SPTR<SignatureComponent> SignatureComponentPtr;
	typedef WPTR<SignatureComponent> SignatureComponentWeakPtr;
}
#endif