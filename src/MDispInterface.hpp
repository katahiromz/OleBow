#pragma once

#include "MNode.hpp"
#include "MMethod.hpp"
#include "MVarDesc.hpp"
#include "MDispProperty.hpp"

class MIDispatchMethods;
class MIDispatchProperties;

class MDispInterface : public MNode
{
public:
    MDispInterface(MNode* parent, MComPtr<ITypeInfo> ti, Ptr<MTypeAttr> ta, bool topLevel);
    String Name() override
    {
        return (m_topLevel ? L"dispinterface " : L"") + m_name;
    }
    String ShortName() override
    {
        return m_name;
    }
    String ObjectName() override
    {
        return m_name + L"#di";
    }
    void GetAttrs(StringList& attrs) override;
    Ptr<MNodeList> GenChildren() override;
    Ptr<MNodeList> MethodChildren()
    {
        auto ret = MakePtr<MNodeList>();
        auto nfuncs = (*m_ta)->cFuncs;
        for (UINT idx = 0; idx < nfuncs; ++idx)
        {
            auto fd = MakePtr<MFuncDesc>(m_ti, idx);
            ret->push_back(MakePtr<MMethod>(this, m_ti, fd));
        }
        return ret;
    }
    Ptr<MNodeList> PropertyChildren()
    {
        auto ret = MakePtr<MNodeList>();
        auto nvars = (*m_ta)->cVars;
        for (UINT x = 0; x < nvars; ++x)
        {
            auto vd = MakePtr<MVarDesc>(m_ti, x);
            ret->push_back(MakePtr<MDispProperty>(this, m_ti, vd));
        }
        return ret;
    }
    void Dump(MSmartWriter& writer) override;
    bool DisplayAtTLBLevel(const Set<String>& ifaces) override
    {
        return ifaces.count(ShortName()) == 0;
    }
protected:
    String m_name;
    Ptr<MTypeAttr> m_ta;
    MComPtr<ITypeInfo> m_ti;
    bool m_topLevel;
    Ptr<MNode> m_methodChildren;
    Ptr<MNode> m_propChildren;
};