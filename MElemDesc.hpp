#pragma once

#include "OleBow.hpp"
#include "MTypeDesc.hpp"
#include "MParamDesc.hpp"

struct MElemDesc : ELEMDESC
{
    MTypeDesc& TypeDesc()
    {
        return reinterpret_cast<MTypeDesc&>(tdesc);
    }
    MParamDesc& ParamDesc()
    {
        return reinterpret_cast<MParamDesc&>(paramdesc);
    }
};
