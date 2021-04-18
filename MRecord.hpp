#pragma once

#include "MNode.hpp"
#include "MTypeAttr.hpp"
#include "MVarDesc.hpp"
#include "MRecordMember.hpp"

class MRecord : public MNode
{
public:
    MRecord(MNode* parent, MComPtr<ITypeInfo> ti, Ptr<MTypeAttr> ta)
    {
        m_parent = parent;
        m_ti = ti;
        m_ta = ta;
        m_name = MTypeInfoExtra::GetName(m_ti);
    }
    String Name() override
    {
        return L"typedef struct tag" + m_name;
    }
    String ShortName() override
    {
        return m_name;
    }
    String ObjectName() override
    {
        return m_name + L"#s";
    }
    void GetAttrs(StringList& attrs) override
    {
    }
    Ptr<MNodeList> GenChildren() override
    {
        auto ret = MakePtr<MNodeList>();
        for (UINT iVar = 0; iVar < (*m_ta)->cVars; ++iVar)
        {
            auto vd = MakePtr<MVarDesc>(m_ti, iVar);
            ret->push_back(MakePtr<MRecordMember>(this, m_ti, vd));
        }
        return ret;
    }
    void Dump(MSmartWriter& writer) override
    {
        writer.write_line(Name() + L" {");
        writer.indent();
        auto children = Children();
        for (auto& child : *children)
        {
            child->Dump(writer);
        }
        writer.unindent();
        writer.write_line(L"} " + ShortName() + L";");
    }
    bool DisplayAtTLBLevel(const Set<String>& ifaces) override
    {
        return true;
    }
protected:
    String m_name;
    MComPtr<ITypeInfo> m_ti;
    Ptr<MTypeAttr> m_ta;
};
