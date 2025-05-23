#include <stdio.h>
#include "OOP/Movement.h"
#include "Oopecs_private.h"

using namespace Niflect;

int main(int argc, char** argv)
{
	auto reg = GetModuleRegistry();
	reg->InitRegisteredModules();
	{
		using namespace OOP;
		Niflect::TArray<CSharedNode> vecNode;
		for (uint32 idx = 0; idx < 1; ++idx)
		{
			auto node = Niflect::MakeShared<CNode>();
			node->InitComponent<CTransformComponent>();
			auto rigidBody = node->InitComponent<CRigidBodyComponent>();
			rigidBody->m_velocity = 1.0f;
			vecNode.push_back(node);
		}
		const uint32 simTimes = 1000;
		const float deltaTime = 1 / 60.0f;
		for (uint32 idx = 0; idx < simTimes; ++idx)
			SimulateMovement(vecNode, deltaTime);
		auto& result = vecNode[0]->m_vecComponent[0].Cast<CTransformComponent>()->m_position.m_x;
		auto expected = simTimes * deltaTime;
		ASSERT(std::abs(result - expected) < 1e-4f);
		printf("");
	}
	return 0;
}