#pragma once

#include "MArrayDesc.hpp"

struct MTypeDesc : TYPEDESC
{
    MArrayDesc& ArrayDesc() const
    {
        return reinterpret_cast<MArrayDesc&>(*lpadesc);
    }
    MTypeDesc& TypeDesc() const
    {
        return reinterpret_cast<MTypeDesc&>(*lptdesc);
    }
    String ComTypeNameAsString(MComPtr<ITypeInfo> ti);
};
