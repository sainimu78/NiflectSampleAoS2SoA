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
		{
			CSoaEntitiesBuffer entitiesBuffer;
			Niflect::TArray<CSharedNode> vecNode;
			for (uint32 idx = 0; idx < 2; ++idx)
			{
				auto node = Niflect::MakeShared<CNode>();
				node->InitComponent<CTransformComponent>();
				node->InitComponent<CRigidBodyComponent>();
				vecNode.push_back(node);
			}
			{
				CAosEntitiesSoaArchecomponentsBinder binder;
				binder.Bind(vecNode, CArchecomponentAndFieldBindings().Add(&CTransformComponent::m_position));
				entitiesBuffer.InitAllocBind(binder);
			}
			auto srcNode = vecNode[0].Get();
			auto dstNode = vecNode[1].Get();
			auto type = StaticGetType<CVector3>();
			auto& src = srcNode->FindComponentOfType<CTransformComponent>()->m_position;
			src.Init({ 1, 2, 3 });
			CRwNode rw;
			type->SaveInstanceToRwNode(&src, &rw);
			ASSERT(rw.IsValue());
			auto& dst = dstNode->FindComponentOfType<CTransformComponent>()->m_position;
			type->LoadInstanceFromRwNode(&dst, &rw);
			auto& srcRef = src.Get();
			auto& dstRef = dst.Get();
			ASSERT(srcRef.m_x == dstRef.m_x);
			ASSERT(srcRef.m_y == dstRef.m_y);
			ASSERT(srcRef.m_z == dstRef.m_z);
		}

		Niflect::TArray<CSharedNode> vecNode;
		for (uint32 idx = 0; idx < nodesCount; ++idx)
		{
			auto node = Niflect::MakeShared<CNode>();
			node->InitComponent<CTransformComponent>();
			node->InitComponent<CRigidBodyComponent>();
			vecNode.push_back(node);
		}
		CSoaEntitiesBuffer entitiesBuffer;
		{
			CAosEntitiesSoaArchecomponentsBinder binder;
			binder.Bind(vecNode, CArchecomponentAndFieldBindings()
				.Add(&CTransformComponent::m_position)
				.Add(&CRigidBodyComponent::m_velocity));
			entitiesBuffer.InitAllocBind(binder);
		}
		for (auto& it : vecNode)
			it->FindComponentOfType<CRigidBodyComponent>()->m_velocity.Init({ 1 });
		for (uint32 idx = 0; idx < simTimes; ++idx)
			SimulateMovement(entitiesBuffer, deltaTime);
		for (auto& it : vecNode)
			vecResultPosition.push_back(it->FindComponentOfType<CTransformComponent>()->m_position.Get());
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