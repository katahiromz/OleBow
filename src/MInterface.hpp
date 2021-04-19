#pragma once

#include "MNode.hpp"
#include "MFuncDesc.hpp"
#include "MMethod.hpp"
#include "MCustData.hpp"

class MInterface : public MNode
{
public:
    MInterface(MNode* parent, MComPtr<ITypeInfo> ti, Ptr<MTypeAttr> ta, bool topLevel)
    {
        m_parent = parent;
        m_name = MTypeInfoExtra::GetName(ti);
        m_ta = ta;
        m_ti = ti;
        m_topLevel = topLevel;

        if ((*m_ta)->cImplTypes > 0)
        {
            HREFTYPE href;
            m_ti->GetRefTypeOfImplType(0, &href);
            MComPtr<ITypeInfo> ti2;
            m_ti->GetRefTypeInfo(href, &ti2);
            m_base = MTypeInfoExtra::GetName(ti2);
        }
    }
    String BaseName()
    {
        return m_base;
    }
    String Name() override
    {
        return (m_topLevel ? L"interface " : L"") + m_name;
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
        AddUUID(attrs, (*m_ta)->guid);

        MTypeAttr ta(m_ti);
        AddVersion(attrs, ta);

        MCustData::GetCustData(m_ti, attrs);

        DWORD context = 0;
        auto help = MTypeInfoExtra::GetHelpDocumentationById(m_ti, -1, context);
        AddHelpStringAndContext(attrs, help, context);

        if ((*m_ta)->wTypeFlags & TYPEFLAG_FAGGREGATABLE) attrs.push_back(L"aggregatable");
        if ((*m_ta)->wTypeFlags & TYPEFLAG_FAPPOBJECT) attrs.push_back(L"appobject");
        // TYPEFLAG_FCANCREATE is not applicable to interfaces/dispinterfaces
        if ((*m_ta)->wTypeFlags & TYPEFLAG_FCONTROL) attrs.push_back(L"control");
        // No IDL syntax for TYPEFLAG_FDISPATCHABLE -- it is computed
        if ((*m_ta)->wTypeFlags & TYPEFLAG_FHIDDEN) attrs.push_back(L"hidden");
        if ((*m_ta)->wTypeFlags & TYPEFLAG_FDUAL) attrs.push_back(L"dual");
        if ((*m_ta)->wTypeFlags & TYPEFLAG_FLICENSED) attrs.push_back(L"licensed");
        if ((*m_ta)->wTypeFlags & TYPEFLAG_FNONEXTENSIBLE) attrs.push_back(L"nonextensible");
        if ((*m_ta)->wTypeFlags & TYPEFLAG_FOLEAUTOMATION) attrs.push_back(L"oleautomation");
        // Can't find IDL for TYPEFLAG_FPREDECLID?!?
        if ((*m_ta)->wTypeFlags & TYPEFLAG_FPROXY) attrs.push_back(L"proxy");
        // Can't find IDL for TYPEFLAG_FREPLACEABLE?!?
        if ((*m_ta)->wTypeFlags & TYPEFLAG_FRESTRICTED) attrs.push_back(L"restricted");
        // Can't find IDL for TYPEFLAG_FREVERSEBIND?!?
    }
    Ptr<MNodeList> GenChildren() override
    {
        auto ret = MakePtr<MNodeList>();
        DWORD cFuncs = (*m_ta)->cFuncs;
        for (DWORD idx = 0; idx < cFuncs; ++idx)
        {
            auto fd = MakePtr<MFuncDesc>(m_ti, idx);
            ret->push_back(MakePtr<MMethod>(this, m_ti, fd));
        }
        return ret;
    }
    void Dump(MSmartWriter& writer) override;
    bool DisplayAtTLBLevel(const Set<String>& ifaces) override
    {
        return true;
    }
protected:
    String m_name;
    String m_base;
    Ptr<MTypeAttr> m_ta;
    MComPtr<ITypeInfo> m_ti;
    bool m_topLevel;
};
