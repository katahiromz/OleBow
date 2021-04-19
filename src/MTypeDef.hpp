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

        String prefix;
        if ((*m_ta)->tdescAlias.vt & VT_ARRAY)
        {
            auto oad = (*m_ta)->tdescAlias.lpadesc;
            m_ti->GetRefTypeInfo(oad->tdescElem.hreftype, &m_oti);
            prefix = MTypeInfoExtra::GetName(m_oti) + L" ";
        }
        else
        {
            prefix = reinterpret_cast<MTypeDesc&>((*m_ta)->tdescAlias).ComTypeNameAsString(m_ti);
            prefix += L" ";
            if ((*m_ta)->tdescAlias.vt == VT_USERDEFINED)
            {
                m_ti->GetRefTypeInfo((*m_ta)->tdescAlias.hreftype, &m_oti);
            }
        }
        m_name = prefix + MTypeInfoExtra::GetName(ti);
    }
    String Name() override
    {
        return L"typedef " + m_name;
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
            writer.write_line(L"typedef " + ShortName() + L";");
        }
        else
        {
            writer.write_line(L"typedef ");
            writer.write_attrs(attrs);
            writer.write_line(ShortName() + L";");
        }
    }
    bool DisplayAtTLBLevel(const Set<String>& ifaces) override
    {
        return true;
    }
protected:
    MComPtr<ITypeInfo> m_ti;
    MComPtr<ITypeInfo> m_oti;
    Ptr<MTypeAttr> m_ta;
    String m_name;
};
