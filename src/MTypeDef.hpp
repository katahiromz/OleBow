#pragma once

#include "MNode.hpp"
#include "MTypeAttr.hpp"
#include "MTypeInfoExtra.hpp"
#include "MTypeDesc.hpp"

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
    String Class() override
    {
        return L"MTypeDef";
    }
    INT SortOfType() const override { return 1; }
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
        writer.write_line(L"typedef [public] " + m_typed_name + L";");
    }
    Ptr<StringSet> GenDepending() override
    {
        auto ret = MakePtr<StringSet>();
        reinterpret_cast<MTypeDesc&>((*m_ta)->tdescAlias).GenDepending(m_ti, *ret);
        return ret;
    }
    Ptr<StringSet> GenDepending2() override
    {
        auto ret = MakePtr<StringSet>();
        reinterpret_cast<MTypeDesc&>((*m_ta)->tdescAlias).GenDepending2(m_ti, *ret);
        return ret;
    }
protected:
    MComPtr<ITypeInfo> m_ti;
    MComPtr<ITypeInfo> m_oti;
    Ptr<MTypeAttr> m_ta;
    String m_name;
    String m_typed_name;
};
