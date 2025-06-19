#pragma once
#include "ECS/Component.h"
#include "ECS/Vector3.h"

namespace ECS
{
	NIF_T()
	class CRigidBodyComponent : public CComponent
	{
	public:
		NIF_F(CComponentNata().ConfirmCompatible<CMovementSystem>())
		CVector3 m_velocity;
	};
}