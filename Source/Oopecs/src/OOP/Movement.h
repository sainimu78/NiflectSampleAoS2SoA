#pragma once
#include "OOP/Node.h"
#include "OOP/TransformComponent.h"
#include "OOP/RigidBodyComponent.h"

namespace OOP
{
	static void SimulateMovement(const Niflect::TArray<CSharedNode>& vecNode, float deltaTime)
	{
		for (auto& it : vecNode)
		{
			auto transform = it->GetComponent<CTransformComponent>();
			auto rigidBody = it->GetComponent<CRigidBodyComponent>();
			transform->m_position += rigidBody->m_velocity * deltaTime;
		}
	}
}