#pragma once
#include "Niflect/Component/DefaultMacroTag.h"
#include "OOP/Component.h"
#include "OOP/Vector3.h"
#include "OOP/RigidBodyComponent.h"

namespace OOP
{
	NIF_T()
	class CTransformComponent : public CComponent
	{
	public:
		CTransformComponent()
			: m_rigidBody(NULL)
		{
		}

	public:
		virtual void StartFrame(float deltaTime) override
		{
			m_rigidBody = m_owner->FindComponentOfType<CRigidBodyComponent>();
		}
		virtual void Tick(float deltaTime) override
		{
			m_position += m_rigidBody->m_velocity * deltaTime;
		}

	public:
		NIF_F()
		CVector3 m_position;

	private:
		CRigidBodyComponent* m_rigidBody;
	};
}