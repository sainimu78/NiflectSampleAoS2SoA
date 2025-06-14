#include <stdio.h>
#include "OOP/Movement.h"
#include "ECS/Movement.h"
#include <chrono>
#include <iostream>
#include "AoS2SoA_private.h"

using namespace Niflect;

int main(int argc, char** argv)
{
	CNiflectModuleRegistry reg;
	reg.InitLoadTimeModules();
	const uint32 simTimes = 1000;
	const float deltaTime = 1 / 60.0f;
	const uint32 nodesCount = 5;
	Niflect::TArray<OOP::CVector3> vecResultPosition;
	if (false)//if (true)//
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
		auto start = std::chrono::high_resolution_clock::now();
		for (uint32 idx = 0; idx < simTimes; ++idx)
			SimulateMovement(vecNode, deltaTime);
		auto end = std::chrono::high_resolution_clock::now();
		std::cout << "OOP Time: " << (end - start).count() << " ns" << std::endl;
		for (uint32 idx = 0; idx < vecNode.size(); ++idx)
			vecResultPosition.push_back(vecNode[idx]->m_vecComponent[0].Cast<CTransformComponent>()->m_position);
	}
	else
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
				binder.Bind(vecNode, CArchecomponentAndFieldBindings().AddByFieldAddr(&CTransformComponent::m_position));
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
		if (true)//if (false)
		{
			CAosEntitiesSoaArchecomponentsBinder binder;
			binder.Bind(vecNode, CArchecomponentAndFieldBindings()
				.AddByFieldAddr(&CTransformComponent::m_position)
				.AddByFieldAddr(&CRigidBodyComponent::m_velocity));
			entitiesBuffer.InitAllocBind(binder);
		}
		if (false)//if (true)//
		{
			CAosEntitiesSoaArchecomponentsBinder binder;
			Niflect::TArray<Niflect::CNiflectType*> vecSystemTypeBindFor;
			vecSystemTypeBindFor.push_back(Niflect::StaticGetType<CMovementSystem>());
			binder.BindDefault(vecNode, vecSystemTypeBindFor);
			entitiesBuffer.InitAllocBind(binder);
		}
		for (auto& it : vecNode)
			it->FindComponentOfType<CRigidBodyComponent>()->m_velocity.Init({ 1 });
		auto start = std::chrono::high_resolution_clock::now();
		for (uint32 idx = 0; idx < simTimes; ++idx)
			CMovementSystem::Simulate(entitiesBuffer, deltaTime);
		auto end = std::chrono::high_resolution_clock::now();
		std::cout << "ECS Time: " << (end - start).count() << " ns" << std::endl;
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