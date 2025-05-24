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
		CTransformComponent()
			//: m_rigidBody(NULL)
		{
		}

	public:
		//virtual void StartFrame(float deltaTime) override
		//{
		//	m_rigidBody = m_owner->FindComponentOfType<CRigidBodyComponent>();
		//}
		//virtual void Tick(float deltaTime) override
		//{
		//	m_position += m_rigidBody->m_velocity * deltaTime;
		//}

	public:
		NIF_F()
		CVector3 m_position;

	//private:
		//CRigidBodyComponent* m_rigidBody;
	};
}