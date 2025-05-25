#include "ECS/ArchecomponentHandle.h"
#include "ECS/System.h"

namespace ECS
{
	//static Niflect::TSharedPtr<Niflect::TMap<Niflect::CNiflectType*, Niflect::CNiflectType*> > g_map;
	//Niflect::TMap<Niflect::CNiflectType*, Niflect::CNiflectType*>& GetMutableArchecomponentHandleTypeToArchetypeMapping()
	//{
	//	if (g_map == NULL)
	//		g_map = Niflect::MakeShared<Niflect::TMap<Niflect::CNiflectType*, Niflect::CNiflectType*> >();
	//	return *g_map;
	//}
	//const Niflect::TMap<Niflect::CNiflectType*, Niflect::CNiflectType*>& GetArchecomponentHandleTypeToArchetypeMapping()
	//{
	//	ASSERT(g_map != NULL);
	//	return *g_map;
	//}

	void* CArchecomponentHandle::GetAddr() const
	{
		return reinterpret_cast<void*>(&m_buffer->m_vecArchecomponentBuffer[m_archecompIdx].m_bytes[m_entityOffset]);
	}
}