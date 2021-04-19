#pragma once

#include "MNode.hpp"

class MInheritedInterfaces : public MNode
{
public:
    MInheritedInterfaces(MNode* parent, MComPtr<ITypeInfo> ti, Ptr<MTypeAttr> ta)
    {
        m_parent = parent;
        m_ta = ta;
        m_ti = ti;
    }
    String Name() override
    {
        return L"Inherited Interfaces";
    }
    String ShortName() override
    {
        return L"";
    }
    String ObjectName() override
    {
        return L"";
    }
    void GetAttrs(StringList& attrs) override
    {
    }
    Ptr<MNodeList> GenChildren() override
    {
        auto ret = MakePtr<MNodeList>();
        if ((*m_ta)->cImplTypes > 0)
        {
            assert((*m_ta)->cImplTypes == 1);
            HREFTYPE href;
            m_ti->GetRefTypeOfImplType(0, &href);
            MComPtr<ITypeInfo> ti2;
            m_ti->GetRefTypeInfo(href, &ti2);
            CommonBuildTlibNode(ti2, false, true, *ret);
        }
        return ret;
    }
    void Dump(MSmartWriter& writer) override
    {
        writer.write_empty_line();
    }
    bool DisplayAtTLBLevel(const Set<String>& ifaces) override
    {
        return false;
    }
    Ptr<StringSet> GenDepending() override
    {
        return MakePtr<StringSet>();
    }
    Ptr<StringSet> GenProviding() override
    {
        return MakePtr<StringSet>();
    }
protected:
    Ptr<MTypeAttr> m_ta;
    MComPtr<ITypeInfo> m_ti;
};
