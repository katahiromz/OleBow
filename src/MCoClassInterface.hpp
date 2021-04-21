#pragma once
#include "MNode.hpp"

class MCoClassInterface : public MNode
{
public:
    MCoClassInterface(MNode* parent, MComPtr<ITypeInfo> ti, UINT impltypeflags)
    {
        m_parent = parent;
        m_ti = ti;
        m_flags = impltypeflags;
        m_name = MTypeInfoExtra::GetName(m_ti);
    }
    String Name() override
    {
        return m_name;
    }
    String ShortName() override
    {
        return m_name;
    }
    String Class() override
    {
        return L"MCoClassInterface";
    }
    void GetAttrs(StringList& attrs) override
    {
        if (m_flags & IMPLTYPEFLAG_FDEFAULT)
            attrs.push_back(L"default");
        if (m_flags & IMPLTYPEFLAG_FSOURCE)
            attrs.push_back(L"source");
        if (m_flags & IMPLTYPEFLAG_FRESTRICTED)
            attrs.push_back(L"restricted");
    }
    Ptr<MNodeList> GenChildren() override
    {
        return MakePtr<MNodeList>();
    }
    void Dump(MSmartWriter& writer) override
    {
        StringList attrs;
        GetAttrs(attrs);

        String strAttrs;
        if (attrs.size())
        {
            strAttrs += L"[";
            strAttrs += mstr_join(attrs, L", ");
            strAttrs += L"] ";
        }

        MTypeAttr ta(m_ti);
        if (!(ta->wTypeFlags & TYPEFLAG_FDUAL) && (ta->typekind & TKIND_DISPATCH))
        {
            writer.write_line(strAttrs + L"dispinterface " + ShortName() + L";");
        }
        else
        {
            writer.write_line(strAttrs + L"interface " + ShortName() + L";");
        }
    }
    bool DisplayAtTLBLevel(const Set<String>& ifaces) override
    {
        return false;
    }
    Ptr<StringSet> GenDepending() override
    {
        return MakePtr<StringSet>();
    }
    Ptr<StringSet> GenProviding() override
    {
        return MakePtr<StringSet>();
    }
protected:
    String m_name;
    MComPtr<ITypeInfo> m_ti;
    UINT m_flags;
};
