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
        return L"enum " + m_name;
    }
    String ShortName() override
    {
        return m_name;
    }
    String ObjectName() override
    {
        return m_name + L"#i";
    }
    void GetAttrs(StringList& attrs) override
    {
        String str;

        if (std::memcmp(&(*m_ta)->guid, &GUID_NULL, sizeof(GUID)) != 0)
        {
            str = L"uuid(";
            str += GetString((*m_ta)->guid);
            str += L")";
            attrs.push_back(str.c_str());
        }

        String strVersion;
        if (MTypeInfoExtra::GetVersion(*m_ta, strVersion))
        {
            attrs.push_back(strVersion);
        }
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

        writer.write_line(L"typedef enum " + ShortName() + L" {");
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
    bool DisplayAtTLBLevel(const Set<String>& ifaces) override
    {
        return false;
    }
protected:
    String m_name;
    Ptr<MTypeAttr> m_ta;
    MComPtr<ITypeInfo> m_ti;
};
