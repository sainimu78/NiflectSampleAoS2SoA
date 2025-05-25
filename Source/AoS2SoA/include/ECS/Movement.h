#pragma once
#include "ECS/System.h"
#include "Niflect/Default/DefaultMacroTag.h"

namespace ECS
{
	enum class EMovementComponentIndex
	{
		Position,
		Velocity,
	};
	uint32 ToIndex(EMovementComponentIndex compType) { return static_cast<uint32>(compType); }

	NIF_T()
	class CMovementSystem
	{
	public:
		static void Simulate(CSoaEntitiesBuffer& entitiesBuffer, float deltaTime)
		{
			auto positions = entitiesBuffer.GetMutableBase<ECS::CVector3::TArchetype>(ToIndex(EMovementComponentIndex::Position));
			auto velocities = entitiesBuffer.GetBase<ECS::CVector3::TArchetype>(ToIndex(EMovementComponentIndex::Velocity));
			for (uint32 idx = 0; idx < entitiesBuffer.m_entitiesCount; ++idx)
				positions[idx] += velocities[idx] * deltaTime;
		}
	};
}