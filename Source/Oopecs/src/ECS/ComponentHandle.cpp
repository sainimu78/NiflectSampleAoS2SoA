#include "ECS/ComponentHandle.h"
#include "ECS/System.h"

namespace ECS
{
	void* CComponentHandle::GetAddr() const
	{
		return reinterpret_cast<void*>(&m_buffer->m_vecSoaComponentBuffer[m_componentIdx].m_bytes[m_entityOffset]);
	}
}