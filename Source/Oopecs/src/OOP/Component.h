#pragma once
#include "Niflect/Base/SharedPtr.h"

namespace OOP
{
	class CComponent
	{
	public:
		virtual ~CComponent() {}
	};
	using CSharedComponent = Niflect::TSharedPtr<CComponent>;
}