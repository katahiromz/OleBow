#pragma once

#include "MNode.hpp"
#include "MTypeAttr.hpp"
#include "MTypeInfoExtra.hpp"

class MTypeDef : public MNode
{
public:
    MTypeDef(MNode* parent, MComPtr<ITypeInfo> ti, Ptr<MTypeAttr> ta)
    {
        m_parent = parent;
        m_ta = ta;
        m_ti = ti;
        m_name = MTypeInfoExtra::GetName(m_ti);
        m_typed_name = reinterpret_cast<MTypeDesc&>(
            (*m_ta)->tdescAlias).GetTypedName(m_ti, m_name);
    }
    String Name() override
    {
        return L"typedef " + m_typed_name;
    }
    String ShortName() override
    {
        return m_name;
    }
    String ObjectName() override
    {
        return m_name + L"#i";
    }
    String Class() override
    {
        return L"MTypeDef";
    }
    void GetAttrs(StringList& attrs) override
    {
        attrs.push_back(L"public");
    }
    Ptr<MNodeList> GenChildren() override
    {
        auto ret = MakePtr<MNodeList>();
        if (m_oti)
        {
            CommonBuildTlibNode(m_oti, false, false, *ret);
        }
        return ret;
    }
    void Dump(MSmartWriter& writer) override
    {
        StringList attrs;
        GetAttrs(attrs);

        if (attrs.empty())
        {
            writer.write_line(L"typedef " + m_typed_name + L";");
        }
        else
        {
            writer.write_line(L"typedef ");
            writer.write_attrs(attrs);
            writer.write_line(L" " + m_typed_name + L";");
        }
    }
    bool DisplayAtTLBLevel(const Set<String>& ifaces) override
    {
        return true;
    }
    Ptr<StringSet> GenDepending() override
    {
        auto ret = MakePtr<StringSet>();
        reinterpret_cast<MTypeDesc&>((*m_ta)->tdescAlias).GenDepending(m_ti, *ret);
        return ret;
    }
    Ptr<StringSet> GenProviding() override
    {
        auto ret = MakePtr<StringSet>();
        ret->insert(m_name);
        return ret;
    }
protected:
    MComPtr<ITypeInfo> m_ti;
    MComPtr<ITypeInfo> m_oti;
    Ptr<MTypeAttr> m_ta;
    String m_name;
    String m_typed_name;
};
