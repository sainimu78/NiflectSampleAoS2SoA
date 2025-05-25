#pragma once
#include "ECS/ArchecomponentHandle.h"
#include "OOP/Vector3.h"
#include "ECS/Vector3_gen.h"

namespace ECS
{
	NIF_T()
	class CVector3 : public TArchecomponentHandle<OOP::CVector3>
	{
	public:
		void Init(const OOP::CVector3& vector)
		{
			this->GetMutable() = vector;
		}
	};
}