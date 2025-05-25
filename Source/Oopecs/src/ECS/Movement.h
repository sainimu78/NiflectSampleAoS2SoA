#pragma once
#include "ECS/System.h"
#include "OOP/Vector3.h"

namespace ECS
{
	enum class EMovementComponentIndex
	{
		Position,
		Velocity,
	};
	uint32 ToIndex(EMovementComponentIndex compType) { return static_cast<uint32>(compType); }

	static void SimulateMovement(CSystem& sys, float deltaTime)
	{
		auto positions = sys.m_entityBuffer.GetMutableComponentBase<ECS::CVector3::TArchetype>(ToIndex(EMovementComponentIndex::Position));
		auto velocities = sys.m_entityBuffer.GetComponentBase<ECS::CVector3::TArchetype>(ToIndex(EMovementComponentIndex::Velocity));
		for (uint32 idx = 0; idx < sys.m_entitiesCount; ++idx)
			positions[idx] += velocities[idx] * deltaTime;
	}
}