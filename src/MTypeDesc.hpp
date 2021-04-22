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

    String GetTypedName(MComPtr<ITypeInfo> ti, String name);
    static String GetTypedName(TYPEDESC* typeDesc, MComPtr<ITypeInfo> pTypeInfo, String name);

    void GenDepending(MComPtr<ITypeInfo> ti, StringSet& depending);
    void GenDepending2(MComPtr<ITypeInfo> ti, StringSet& depending2);

    static void GenDepending(TYPEDESC* typeDesc, MComPtr<ITypeInfo> ti, StringSet& depending);
    static void GenDepending2(TYPEDESC* typeDesc, MComPtr<ITypeInfo> ti, StringSet& depending2);
    static void GenDepending(MComPtr<ITypeInfo> pti, HREFTYPE refType, StringSet& depending);
    static void GenDepending2(MComPtr<ITypeInfo> pti, HREFTYPE refType, StringSet& depending2);
};
