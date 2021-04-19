#pragma once

#include "MNode.hpp"

class MModule : public MNode
{
public:
    MModule(MNode* parent, MComPtr<ITypeInfo> ti, Ptr<MTypeAttr> ta)
    {
        m_parent = parent;
        m_ti = ti;
        m_ta = ta;
        m_name = MTypeInfoExtra::GetName(m_ti);
        if ((*m_ta)->cVars <= 0 && (*m_ta)->cFuncs <= 0)
            return;
        if ((*m_ta)->cFuncs > 0)
        {
            MFuncDesc fd(m_ti, 0);
            auto invkind = fd->invkind;
            auto memid = fd->memid;
            m_dllname = MTypeInfoExtra::GetDllEntry(ti, invkind, memid);
        }
    }
    String Name() override
    {
        return L"module " + m_name;
    }
    String ShortName() override
    {
        return m_name;
    }
    String ObjectName() override
    {
        return L"";
    }
    void GetAttrs(StringList& attrs) override
    {
        if (m_dllname.size())
        {
            String str = L"dllname(\"";
            str += m_dllname;
            str += L"\")";
            attrs.push_back(str);
        }
        else
        {
            attrs.push_back(L"dllname(<no entry points>)");
        }

        if (std::memcmp(&(*m_ta)->guid, &GUID_NULL, sizeof(GUID_NULL)) != 0)
        {
            String str = L"uuid(";
            str += GetString((*m_ta)->guid);
            str += L")";
            attrs.push_back(str);
        }

        DWORD cnt = 0;
        auto help = MTypeInfoExtra::GetHelpDocumentationById(m_ti, -1, cnt);
        if (help.size())
        {
            String str = L"helpstring(";
            str += help;
            str += L")";
            attrs.push_back(str);
        }
        if (cnt)
        {
            String str = L"helpcontext(";
            str += MTypeInfoExtra::PaddedHex(cnt);
            str += L")";
            attrs.push_back(str);
        }
    }
    Ptr<MNodeList> GenChildren() override;
    Ptr<MNodeList> GenConstChildren();
    Ptr<MNodeList> GenFuncChildren();
    void Dump(MSmartWriter& writer) override
    {
        if ((*m_ta)->cFuncs == 0)
        {
            writer.write_line(L"// NOTE: This module has no entry points. There is no way to");
            writer.write_line(L"//       extract the dllname of a module with no entry points!");
        }
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
protected:
    String m_name;
    MComPtr<ITypeInfo> m_ti;
    Ptr<MTypeAttr> m_ta;
    String m_dllname;
};