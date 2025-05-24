#include <stdio.h>
#include "OOP/Movement.h"
#include "ECS/RigidBodyComponent.h"
#include "ECS/TransformComponent.h"
#include "ECS/Node.h"
#include "ECS/Movement.h"
#include "Oopecs_private.h"

using namespace Niflect;

int main(int argc, char** argv)
{
	auto reg = GetModuleRegistry();
	reg->InitRegisteredModules();
	const uint32 simTimes = 1000;
	const float deltaTime = 1 / 60.0f;
	const uint32 nodesCount = 5;
	Niflect::TArray<OOP::CVector3> vecResultPosition;
	if (false)//if (true)
	{
		using namespace OOP;
		Niflect::TArray<CSharedNode> vecNode;
		for (uint32 idx = 0; idx < nodesCount; ++idx)
		{
			auto node = Niflect::MakeShared<CNode>();
			node->InitComponent<CTransformComponent>();
			auto rigidBody = node->InitComponent<CRigidBodyComponent>();
			rigidBody->m_velocity = 1.0f;
			vecNode.push_back(node);
		}
		for (uint32 idx = 0; idx < simTimes; ++idx)
			SimulateMovement(vecNode, deltaTime);
		for (uint32 idx = 0; idx < vecNode.size(); ++idx)
			vecResultPosition.push_back(vecNode[idx]->m_vecComponent[0].Cast<CTransformComponent>()->m_position);
	}
	if (true)//if (false)//
	{
		using namespace ECS;
		Niflect::TArray<CSharedNode> vecNode;
		for (uint32 idx = 0; idx < nodesCount; ++idx)
		{
			auto node = Niflect::MakeShared<CNode>();
			node->InitComponent<CTransformComponent>();
			node->InitComponent<CRigidBodyComponent>();
			vecNode.push_back(node);
		}
		CAosSoaBinder binder;
		for (auto& it : vecNode)
		{
			FindAndBindFieldInComponent(it.Get(), &CTransformComponent::m_position, binder);
			FindAndBindFieldInComponent(it.Get(), &CRigidBodyComponent::m_velocity, binder);
		}
		CSystem sys;
		sys.InitEntityComponent(vecNode, 0, binder);
		auto velocities = sys.m_componentBuffer.GetMutableComponentBase<ECS::CVector3::RefType>(ToIndex(EMovementComponentIndex::Velocity));
		for (uint32 idx = 0; idx < sys.m_entitiesCount; ++idx)
			velocities[idx] = 1;
		for (uint32 idx = 0; idx < simTimes; ++idx)
			SimulateMovement(sys, deltaTime);
		auto positions = sys.m_componentBuffer.GetComponentBase<ECS::CVector3::RefType>(ToIndex(EMovementComponentIndex::Position));
		for (uint32 idx = 0; idx < sys.m_entitiesCount; ++idx)
			vecResultPosition.push_back(positions[idx]);
	}
	auto expected = simTimes * deltaTime;
	for (auto& it : vecResultPosition)
	{
		ASSERT(std::abs(it.m_x - expected) < 1e-4f);
		ASSERT(std::abs(it.m_y - expected) < 1e-4f);
		ASSERT(std::abs(it.m_z - expected) < 1e-4f);
	}
	return 0;
}