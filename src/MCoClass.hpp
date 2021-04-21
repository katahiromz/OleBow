#pragma once

#include "MNode.hpp"
#include "MCoClassInterface.hpp"
#include "MCustData.hpp"

class MCoClass : public MNode
{
public:
    MCoClass(MNode* parent, MComPtr<ITypeInfo> ti, Ptr<MTypeAttr> ta)
    {
        m_parent = parent;
        m_name = MTypeInfoExtra::GetName(ti);
        m_ta = ta;
        m_ti = ti;
    }
    String Name() override
    {
        return L"coclass " + m_name;
    }
    String ShortName() override
    {
        return m_name;
    }
    String Class() override
    {
        return L"MCoClass";
    }
    void GetAttrs(StringList& attrs) override
    {
        AddUUID(attrs, (*m_ta)->guid);

        MTypeAttr ta(m_ti);
        AddVersion(attrs, ta);

        MCustData::GetCustData(m_ti, attrs);

        DWORD context = 0;
        auto help = MTypeInfoExtra::GetHelpDocumentationById(m_ti, -1, context);
        AddHelpStringAndContext(attrs, help, context);

        if (!((*m_ta)->wTypeFlags & TYPEFLAG_FCANCREATE))
            attrs.push_back(L"noncreatable");
    }
    Ptr<MNodeList> GenChildren() override
    {
        Ptr<MNodeList> ret = MakePtr<MNodeList>();
        for (UINT iImplType = 0; iImplType < (*m_ta)->cImplTypes; ++iImplType)
        {
            HREFTYPE href;
            m_ti->GetRefTypeOfImplType(iImplType, &href);
            MComPtr<ITypeInfo> ti2;
            m_ti->GetRefTypeInfo(href, &ti2);
            INT flags;
            m_ti->GetImplTypeFlags(iImplType, &flags);
            auto iface = MakePtr<MCoClassInterface>(this, ti2, flags);
            ret->push_back(iface);
        }
        return ret;
    }
    void Dump(MSmartWriter& writer) override
    {
        StringList attrs;
        GetAttrs(attrs);
        writer.write_attrs(attrs);
        writer.write_line(Name() + L" {");
        writer.indent();
        auto children = Children();
        for (auto& child : *children)
        {
            child->Dump(writer);
        }
        writer.unindent();
        writer.write_line(L"};");
    }
    bool DisplayAtTLBLevel(const Set<String>& ifaces) override
    {
        return true;
    }
    Ptr<StringSet> GenDepending() override
    {
        auto ret = MakePtr<StringSet>();
        auto children = Children();
        for (auto& child : *children)
        {
            ret->insert(child->ShortName());
        }
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
    Ptr<MTypeAttr> m_ta;
    MComPtr<ITypeInfo> m_ti;
};
