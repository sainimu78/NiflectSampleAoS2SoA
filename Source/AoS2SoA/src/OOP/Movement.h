#pragma once
#include "OOP/Node.h"
#include "OOP/TransformComponent.h"
#include "OOP/RigidBodyComponent.h"

namespace OOP
{
	static void SimulateMovement(const Niflect::TArray<CSharedNode>& vecNode, float deltaTime)
	{
		for (auto& it0 : vecNode)
		{
			for (auto& it1 : it0->m_vecComponent)
				it1->StartFrame(deltaTime);
		}
		for (auto& it0 : vecNode)
		{
			for (auto& it1 : it0->m_vecComponent)
				it1->Tick(deltaTime);
		}
	}
}