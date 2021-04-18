#pragma once

#include "MInterface.hpp"
#include "MInheritedInterfaces.hpp"

class MDispInterfaceInheritedInterfaces : public MInheritedInterfaces
{
public:
    MDispInterfaceInheritedInterfaces(MNode *parent, MComPtr<ITypeInfo> ti, Ptr<MTypeAttr> ta) 
        : MInheritedInterfaces(parent, ti, ta)
    {
    }
    Ptr<MNodeList> GenChildren() override
    {
        auto ti = m_ti;
        auto ta = m_ta;
        MTypeInfoExtra::SwapForInterface(ti, ta);
        auto ret = MakePtr<MNodeList>();
        ret->push_back(MakePtr<MInterface>(this, ti, ta, false));
        return ret;
    }
};
