#pragma once
#include "Niflect/Base/Array.h"
#include "ECS/Component.h"

namespace ECS
{
	class CNode
	{
	public:
		template <typename T>
		T* InitComponent()
		{
			auto component = Niflect::MakeShared<T>();
			component->Init(this, Niflect::StaticGetType<T>());
			m_vecComponent.push_back(component);
			return component.Get();
		}
		template <typename T>
		T* FindComponentOfType() const
		{
			for (auto& it : m_vecComponent)
			{
				if (it->GetType() == Niflect::StaticGetType<T>())
					return static_cast<T*>(it.Get());
			}
			ASSERT(false);
			return NULL;
		}
		Niflect::TArray<CSharedComponent> m_vecComponent;
	};
	using CSharedNode = Niflect::TSharedPtr<CNode>;
}