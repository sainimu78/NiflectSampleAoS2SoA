#include "OOP/Accessor/Vector3Accessor.h"
#include "Niflect/Util/StringUtil.h"

namespace OOP
{
	void SaveCVector3ToRwNode(const CVector3& vec, CRwNode* rw)
	{
		auto str = NiflectUtil::FormatString("%f %f %f", vec.m_x, vec.m_y, vec.m_z);
		SetRwValueAs<Niflect::CString>(rw->ToValue(), str);
	}
	void LoadCVector3FromRwNode(const CRwNode* rw, CVector3& vec)
	{
		auto str = GetRwValueAs<Niflect::CString>(rw->GetValue());
		auto vecEntryStr = NiflectUtil::Split(str, ' ');
		vec.m_x = std::stof(vecEntryStr[0].c_str());
		vec.m_y = std::stof(vecEntryStr[1].c_str());
		vec.m_z = std::stof(vecEntryStr[2].c_str());
	}

	bool CVector3Accessor::SaveImpl(const InstanceType* base, CRwNode* rw) const
	{
		auto& instance = *static_cast<const CVector3*>(base);
		SaveCVector3ToRwNode(instance, rw);//表明可任意编解码
		return true;
	}
	bool CVector3Accessor::LoadImpl(InstanceType* base, const CRwNode* rw) const
	{
		auto& instance = *static_cast<CVector3*>(base);
		ASSERT(rw->IsValue());
		LoadCVector3FromRwNode(rw, instance);
		return true;
	}
}