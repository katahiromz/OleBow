#pragma once
#include "MNode.hpp"

class MModuleConst : public MNode
{
public:
    MModuleConst(MNode* parent, MComPtr<ITypeInfo> ti, Ptr<MVarDesc> vd, int idx)
    {
        m_parent = parent;
        m_idx = idx;
        m_name = MTypeInfoExtra::GetDocumentationById(ti, (*vd)->memid);
        m_typed_name = vd->elemDescVar().TypeDesc().GetTypedName(ti, m_name);
        m_val = MTypeInfoExtra::QuoteString(vd->varValue());
    }
    String Name() override
    {
        return L"const " + m_typed_name;
    }
    String ShortName() override
    {
        return m_name;
    }
    String Class() override
    {
        return L"MModuleConst";
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
        writer.write_line(L"const " + m_typed_name + L" = " + m_val + L";");
    }
    Ptr<StringSet> GenDepending() override
    {
        return MakePtr<StringSet>();
    }
    Ptr<StringSet> GenDepending2() override
    {
        return MakePtr<StringSet>();
    }
protected:
    int m_idx;
    String m_name;
    String m_typed_name;
    String m_val;
};
