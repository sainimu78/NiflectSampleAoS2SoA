#include "ECS/ComponentBufferElementRef.h"
#include "ECS/System.h"

namespace ECS
{
	void* CComponentBufferElementRef::GetAddr() const
	{
		return reinterpret_cast<void*>(&m_buffer->m_vecArchetypeIdxToComponentBuffer[m_archetypeIdx].m_bytes[m_bufferOffset]);
	}
}