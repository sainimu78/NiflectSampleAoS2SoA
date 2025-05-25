#pragma once
#include "Niflect/Default/DefaultAccessorSetting.h"
#include "OOP/Accessor/Vector3Accessor.h"
#include "ECS/Accessor/Vector3Accessor.h"

using namespace NiflectAccessorSetting;

NIFAS_A() TSetting<OOP::CVector3Accessor, OOP::CVector3>;
NIFAS_A() TSetting<ECS::CVector3Accessor, ECS::CVector3>;