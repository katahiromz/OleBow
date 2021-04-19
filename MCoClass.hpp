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
    String ObjectName() override
    {
        return m_name + L"#c";
    }
    void GetAttrs(StringList& attrs) override
    {
        String str = L"uuid(";
        str += GetString((*m_ta)->guid);
        str += L")";
        attrs.push_back(str);

        auto ta = MakePtr<MTypeAttr>(m_ti);
        if ((*ta)->wMajorVerNum != 0 || (*ta)->wMinorVerNum != 0)
        {
            str = L"version(";
            str += std::to_wstring((*ta)->wMajorVerNum);
            str += L".";
            str += std::to_wstring((*ta)->wMinorVerNum);
            str += L")";
            attrs.push_back(str);
        }
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
            CommonBuildTlibNode(ti2, false, false, *ret.get());
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
        for (UINT i = 0; i < (*m_ta)->cImplTypes; ++i)
        {
            HREFTYPE href;
            m_ti->GetRefTypeOfImplType(i, &href);
            MComPtr<ITypeInfo> ti2;
            m_ti->GetRefTypeInfo(href, &ti2);
            INT flags;
            m_ti->GetImplTypeFlags(i, &flags);
            auto iface = MakePtr<MCoClassInterface>(this, ti2, flags);
            iface->Dump(writer);
        }
        writer.unindent();
        writer.write_line(L"};");
    }
    bool DisplayAtTLBLevel(const Set<String>& ifaces) override
    {
        return true;
    }
protected:
    String m_name;
    Ptr<MTypeAttr> m_ta;
    MComPtr<ITypeInfo> m_ti;
};
