#include "ECS/ArchecomponentHandle.h"
#include "ECS/System.h"

namespace ECS
{
	void* CArchecomponentHandle::GetAddr() const
	{
		return reinterpret_cast<void*>(&m_buffer->m_vecArchecomponentBuffer[m_archecompIdx].m_bytes[m_entityOffset]);
	}
}