#pragma once
#include "Niflect/Base/Array.h"
#include "OOP/Component.h"

namespace OOP
{
	class CNode;
	using CSharedNode = Niflect::TSharedPtr<CNode>;

	class CNode
	{
	public:
		Niflect::TArray<CSharedComponent> m_vecComponent;
		Niflect::TArray<CSharedNode> m_vecChild;
	};
}