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
    String ObjectName() override
    {
        return L"";
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
        writer.write_line(ed.TypeDesc().ComTypeNameAsString(m_ti) + L" " + Name() + L";");
    }
    bool DisplayAtTLBLevel(const Set<String>& ifaces) override
    {
        return false;
    }
protected:
    String m_name;
    Ptr<MVarDesc> m_vd;
    MComPtr<ITypeInfo> m_ti;
};
