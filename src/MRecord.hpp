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
        return L"struct tag" + m_name;
    }
    String ShortName() override
    {
        return m_name;
    }
    String Class() override
    {
        return L"MRecord";
    }
    INT SortOfType() const override { return 1; }
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
        writer.write_line(L"typedef " + Name() + L" {");
        writer.indent();
        auto children = Children();
        for (auto& child : *children)
        {
            child->Dump(writer);
        }
        writer.unindent();
        writer.write_line(L"} " + ShortName() + L";");
    }
    Ptr<StringSet> GenDepending() override
    {
        auto ret = MakePtr<StringSet>();
        auto children = Children();
        for (auto& child : *children)
        {
            auto ret2 = child->Depending();
            ret->insert(ret2->begin(), ret2->end());
        }
        return ret;
    }
    Ptr<StringSet> GenDepending2() override
    {
        auto ret = MakePtr<StringSet>();
        auto children = Children();
        for (auto& child : *children)
        {
            auto ret2 = child->Depending2();
            ret->insert(ret2->begin(), ret2->end());
        }
        return ret;
    }
protected:
    String m_name;
    MComPtr<ITypeInfo> m_ti;
    Ptr<MTypeAttr> m_ta;
};
