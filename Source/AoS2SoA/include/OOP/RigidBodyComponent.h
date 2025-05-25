#pragma once
#include "Niflect/Default/DefaultMacroTag.h"
#include "OOP/Component.h"
#include "OOP/Vector3.h"

namespace OOP
{
	NIF_T()
	class CRigidBodyComponent : public CComponent
	{
	public:
		virtual void Tick(float deltaTime) override
		{
		}

	public:
		NIF_F()
		CVector3 m_velocity;
	};
}