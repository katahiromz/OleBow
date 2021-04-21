#pragma once

#include "MNode.hpp"

class MDispProperty : public MNode
{
public:
    MDispProperty(MNode* parent, MComPtr<ITypeInfo> ti, Ptr<MVarDesc> vd)
    {
        m_parent = parent;
        m_name = MTypeInfoExtra::GetDocumentationById(ti, (*vd)->memid);
        m_vd = vd;
        m_ti = ti;
    }
    String Name() override
    {
        return m_name;
    }
    String ShortName() override
    {
        return m_name;
    }
    String Class() override
    {
        return L"MDispProperty";
    }
    void GetAttrs(StringList& attrs) override
    {
        {
            String str = L"id(";
            str += MTypeInfoExtra::PaddedHex((*m_vd)->memid);
            str += L")";
            attrs.push_back(str);
        }
        DWORD ctx = 0;
        auto help = MTypeInfoExtra::GetHelpDocumentationById(m_ti, (*m_vd)->memid, ctx);
        AddHelpStringAndContext(attrs, help, ctx);
    }
    Ptr<MNodeList> GenChildren() override
    {
        return MakePtr<MNodeList>();
    }
    void Dump(MSmartWriter& writer) override
    {
        StringList attrs;
        GetAttrs(attrs);
        writer.write_attrs(attrs);

        auto& ed = m_vd->elemDescVar();
        writer.write_line(ed.TypeDesc().GetTypedName(m_ti, m_name) + L";");
    }
    bool DisplayAtTLBLevel(const Set<String>& ifaces) override
    {
        return false;
    }
    Ptr<StringSet> GenDepending() override
    {
        auto ret = MakePtr<StringSet>();
        auto& ed = m_vd->elemDescVar();
        ed.TypeDesc().GenDepending(m_ti, *ret);
        return ret;
    }
    Ptr<StringSet> GenProviding() override
    {
        auto ret = MakePtr<StringSet>();
        ret->insert(m_name);
        return ret;
    }
protected:
    String m_name;
    Ptr<MVarDesc> m_vd;
    MComPtr<ITypeInfo> m_ti;
};
