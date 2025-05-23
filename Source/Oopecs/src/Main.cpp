#include <stdio.h>
#include "OOP/TransformComponent.h"
#include "Oopecs_private.h"

using namespace Niflect;

int main(int argc, char** argv)
{
	auto reg = GetModuleRegistry();
	reg->InitRegisteredModules();
	using namespace OOP;
	auto type = StaticGetType<CTransformComponent>();
	CTransformComponent src;
	src.m_position.m_x = 1;
	src.m_position.m_y = 2;
	src.m_position.m_z = 3;
	CRwNode rw;
	type->SaveInstanceToRwNode(&src, &rw);
	CTransformComponent dst;
	type->LoadInstanceFromRwNode(&dst, &rw);
	ASSERT(src.m_position.m_x == dst.m_position.m_x);
	ASSERT(src.m_position.m_y == dst.m_position.m_y);
	ASSERT(src.m_position.m_z == dst.m_position.m_z);
	return 0;
}