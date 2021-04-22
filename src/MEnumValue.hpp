#pragma once

#include "MNode.hpp"
#include "MVarDesc.hpp"

class MEnumValue : public MNode
{
public:
    MEnumValue(MNode* parent, MComPtr<ITypeInfo> ti, Ptr<MVarDesc> vd)
    {
        m_parent = parent;
        m_name = MTypeInfoExtra::GetDocumentationById(ti, (*vd)->memid);
        m_val = (int)vd->varValue();
        m_vd = vd;
        m_ti = ti;
    }
    String Name() override
    {
        return m_name + L" = " + std::to_wstring(m_val);
    }
    String ShortName() override
    {
        return m_name;
    }
    String Class() override
    {
        return L"MEnumValue";
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
        Dump(writer, false, false);
    }
    void Dump(MSmartWriter& writer, bool embedded, bool islast)
    {
        writer.write_line(Name() + (embedded ? (islast ? L"" : L",") : L";"));
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
    String m_name;
    Ptr<MVarDesc> m_vd;
    MComPtr<ITypeInfo> m_ti;
    int m_val;
};
