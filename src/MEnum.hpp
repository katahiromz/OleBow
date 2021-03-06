#pragma once

#include "MEnumValue.hpp"

class MEnum : public MNode
{
public:
    MEnum(MNode* parent, MComPtr<ITypeInfo> ti, Ptr<MTypeAttr> ta)
    {
        m_parent = parent;
        m_name = MTypeInfoExtra::GetName(ti);
        m_ta = ta;
        m_ti = ti;
    }
    String Name() override
    {
        return L"enum tag" + m_name;
    }
    String ShortName() override
    {
        return m_name;
    }
    String Class() override
    {
        return L"MEnum";
    }
    INT SortOfType() const override { return 0; }
    void GetAttrs(StringList& attrs) override
    {
        String str;

        AddUUID(attrs, (*m_ta)->guid);

        AddVersion(attrs, *m_ta);
    }
    Ptr<MNodeList> GenChildren() override
    {
        auto ret = MakePtr<MNodeList>();
        for (UINT iVar = 0; iVar < (*m_ta)->cVars; ++iVar)
        {
            auto vd = MakePtr<MVarDesc>(m_ti, iVar);
            ret->push_back(MakePtr<MEnumValue>(this, m_ti, vd));
        }
        return ret;
    }
    void Dump(MSmartWriter& writer) override
    {
        StringList attrs;
        GetAttrs(attrs);
        writer.write_attrs(attrs);

        writer.write_line(L"typedef [public] " + Name() + L" {");
        writer.indent();
        {
            auto children = Children();
            size_t i = 0;
            for (auto& child : *children)
            {
                std::static_pointer_cast<MEnumValue>(child)->Dump(writer, true, ++i == children->size());
            }
        }
        writer.unindent();
        writer.write_line(L"} " + ShortName() + L";");
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
    Ptr<MTypeAttr> m_ta;
    MComPtr<ITypeInfo> m_ti;
};
