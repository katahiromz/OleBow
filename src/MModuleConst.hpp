#pragma once
#include "MNode.hpp"

class MModuleConst : public MNode
{
public:
    MModuleConst(MNode* parent, MComPtr<ITypeInfo> ti, Ptr<MVarDesc> vd, int idx)
    {
        m_parent = parent;
        m_idx = idx;
        auto str = vd->elemDescVar().TypeDesc().ComTypeNameAsString(ti);
        m_name = str + L" " + MTypeInfoExtra::GetDocumentationById(ti, (*vd)->memid);
        m_val = (bstr_t)vd->varValue();
    }
    String Name() override
    {
        return L"const " + m_name;
    }
    String ShortName() override
    {
        return m_name;
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
        return MakePtr<MNodeList>();
    }
    void Dump(MSmartWriter& writer) override
    {
        writer.write_line(L"const " + m_name + L" = " + m_val + L";");
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
        auto ret = MakePtr<StringSet>();
        ret->insert(m_name);
        return ret;
    }
protected:
    int m_idx;
    String m_name;
    String m_val;
};
