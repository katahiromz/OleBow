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

        if ((*m_ta)->tdescAlias.vt & VT_ARRAY)
        {
            auto oad = (*m_ta)->tdescAlias.lpadesc;
            m_ti->GetRefTypeInfo(oad->tdescElem.hreftype, &m_oti);
            m_type = MTypeInfoExtra::GetName(m_oti) + L" ";
        }
        else
        {
            m_type = reinterpret_cast<MTypeDesc&>((*m_ta)->tdescAlias).ComTypeNameAsString(m_ti);
            if ((*m_ta)->tdescAlias.vt == VT_USERDEFINED)
            {
                m_ti->GetRefTypeInfo((*m_ta)->tdescAlias.hreftype, &m_oti);
            }
        }
        m_name = MTypeInfoExtra::GetName(ti);
    }
    String Name() override
    {
        return L"typedef " + m_type + L" " + m_name;
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
            writer.write_line(L"typedef " + m_type + L" " + ShortName() + L";");
        }
        else
        {
            writer.write_line(L"typedef ");
            writer.write_attrs(attrs);
            writer.write_line(L" " + m_type + L" " + ShortName() + L";");
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
    String m_type;
};
