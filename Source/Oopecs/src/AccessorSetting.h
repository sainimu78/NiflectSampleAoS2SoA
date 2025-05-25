#pragma once
#include "Niflect/Default/DefaultAccessorSetting.h"
#include "OOP/Accessor/Vector3Accessor.h"
#include "ECS/Accessor/ArchecomponentHandleAccessor.h"
#include "ECS/Vector3.h"

using namespace NiflectAccessorSetting;

NIFAS_A() TSetting<OOP::CVector3Accessor, OOP::CVector3>;
NIFAS_A() TSetting<ECS::TArchecomponentHandleAccessor<ECS::CVector3>, ECS::CVector3>;