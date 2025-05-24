#pragma once
#include "ECS/ComponentBufferElementRef.h"
#include "OOP/Vector3.h"
#include "ECS/Vector3_gen.h"

namespace ECS
{
	NIF_T()
	class CVector3 : public TComponentBufferElementRef<OOP::CVector3>
	{
	};
}