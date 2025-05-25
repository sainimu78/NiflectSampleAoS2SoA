#pragma once
#include "Niflect/Default/DefaultMacroTag.h"
#include "ECS/Component.h"
#include "ECS/Vector3.h"
#include "ECS/RigidBodyComponent.h"

namespace ECS
{
	NIF_T()
	class CTransformComponent : public CComponent
	{
	public:
		NIF_F(CComponentNata().ConfirmCompatible<CMovementSystem>())
		CVector3 m_position;
	};
}