#pragma once
#include "Niflect/Base/Array.h"
#include "OOP/Component.h"

namespace OOP
{
	class CNode
	{
	public:
		template <typename T>
		T* InitComponent()
		{
			auto idx = T::GetAtIndex();
			if (m_vecComponent.size() <= idx)
				m_vecComponent.resize(idx + 1);
			auto component = Niflect::MakeShared<T>();
			m_vecComponent[idx] = component;
			return component.Get();
		}
		template <typename T>
		T* GetComponent() const
		{
			ASSERT(dynamic_cast<T*>(m_vecComponent[T::GetAtIndex()].Get()) != NULL);
			return static_cast<T*>(m_vecComponent[T::GetAtIndex()].Get());
		}
		Niflect::TArray<CSharedComponent> m_vecComponent;
	};
	using CSharedNode = Niflect::TSharedPtr<CNode>;
}