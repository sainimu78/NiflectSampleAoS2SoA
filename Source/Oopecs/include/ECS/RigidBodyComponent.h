#pragma once
#include "Niflect/Default/DefaultMacroTag.h"
#include "ECS/Component.h"
#include "ECS/Vector3.h"

namespace ECS
{
	NIF_T()
	class CRigidBodyComponent : public CComponent
	{
	public:
		//virtual void Tick(float deltaTime) override
		//{
		//}

	public:
		NIF_F()
		CVector3 m_velocity;
	};
}