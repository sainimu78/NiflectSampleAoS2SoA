#pragma once
#include "Niflect/Default/DefaultMacroTag.h"
#include "OOP/Component.h"
#include "Math/Vector3.h"

namespace OOP
{
	NIF_T()
	class CTransformComponent : public CComponent
	{
	public:
		static uint32 GetAtIndex() { return 0; }

	public:
		NIF_F()
		CVector3 m_position;
	};
}