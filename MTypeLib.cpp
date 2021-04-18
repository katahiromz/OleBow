#include "MTypeLib.hpp"
#include "MTypeLibMetadata.hpp"
#include "MCustData.hpp"
#include "MTypeLibAttr.hpp"
#include "MInterface.hpp"
#include "MDispInterface.hpp"
#include "MRecord.hpp"
#include "MCoClass.hpp"
#include "MEnum.hpp"
#include <shlwapi.h>

MTypeLib::MTypeLib() : m_pAttr(NULL)
{
}

MTypeLib::MTypeLib(const wchar_t *path) : m_pAttr(NULL)
{
    Load(path);
}

MTypeLib::~MTypeLib()
{
    Unload();
}

void MTypeLib::Load(const wchar_t *path)
{
    HRESULT hr = ::LoadTypeLib(path, &m_tlib);
    if (FAILED(hr))
        throw std::runtime_error("Cannot load");
    m_tlib->GetLibAttr(&m_pAttr);

    m_name = MTypeInfoExtra::GetName(m_tlib);
    DWORD ctx = 0;
    m_name += L" (" + MTypeInfoExtra::GetHelpDocumentation(m_tlib, ctx) + L")";
}

void MTypeLib::Unload()
{
    if (m_tlib && m_pAttr)
        m_tlib->ReleaseTLibAttr(m_pAttr);
    m_tlib.Release();
}

String MTypeLib::Name()
{
    return m_name;
}

String MTypeLib::ShortName()
{
    return MTypeInfoExtra::GetName(m_tlib);
}

String MTypeLib::ObjectName()
{
    return ShortName();
}

void MTypeLib::GetAttrs(StringList& attrs)
{
    if (m_pAttr)
    {
        String str;

        str = L"uuid(";
        str += GetString(m_pAttr->guid);
        str += L")";
        attrs.push_back(str.c_str());

        str = L"version(";
        str += std::to_wstring(m_pAttr->wMajorVerNum);
        str += L".";
        str += std::to_wstring(m_pAttr->wMinorVerNum);
        str += L")";
        attrs.push_back(str.c_str());
    }

    MComPtr<ITypeLib2> tlib2;
    m_tlib->QueryInterface(IID_ITypeLib2, reinterpret_cast<LPVOID *>(&tlib2));
    if (tlib2)
    {
        MCustomDatas custdata(tlib2);
        auto items = *custdata.Items();
        for (auto& item : items)
        {
            String str = L"custom(";
            str += GetString(item->m_g);
            str += L", ";
            str += MTypeInfoExtra::QuoteString(item->m_v);
            str += L")";
            attrs.push_back(str);
        }
    }

    DWORD cnt = 0;
    auto help = MTypeInfoExtra::GetHelpDocumentation(m_tlib, cnt);
    if (help.size())
    {
        String str = L"helpstring(\"";
        str += help;
        str += L"\")";
        attrs.push_back(str.c_str());
    }
    if (cnt)
    {
        String str = L"helpcontext(";
        str += MTypeInfoExtra::PaddedHex(cnt);
        str += L")";
        attrs.push_back(str.c_str());
    }
}

Ptr<MNodeList> MTypeLib::GenChildren()
{
    Ptr<MNodeList> ret = MakePtr<MNodeList>();
    UINT count = m_tlib->GetTypeInfoCount();
    for (UINT iInfo = 0; iInfo < count; ++iInfo)
    {
        MComPtr<ITypeInfo> ti;
        m_tlib->GetTypeInfo(iInfo, &ti);
        CommonBuildTlibNode(ti, true, true, *ret);
    }
    return ret;
}

void MTypeLib::DumpMetadata(MSmartWriter& writer)
{
    MTypeLibMetadata metadata;
    auto list = metadata.GetDependentLibraries(m_tlib);
    for (auto& dl : list)
    {
        String name, path;
        GUID guid;
        GetFileNameOfTypeLib(dl, guid, name, path);

        if (name.size())
            writer.write_line(L"// Tlib : " + name + L" : {" + GetString(guid) + L"}");

        auto filename = PathFindFileNameW(path.c_str());
        if (filename && *filename)
            writer.write_line(String(L"importlib(\"") + filename + L"\");");
    }
}

void MTypeLib::Dump(MSmartWriter& writer)
{
    writer.write_line(L"// Generated .IDL file (by OleBow)");

    GUID guid;
    String name, path;
    if (GetFileNameOfTypeLib(m_tlib, guid, name, path))
    {
        String str = L"// typelib filename: ";
        str += PathFindFileNameW(path.c_str());
        writer.write_line(str);
    }

    StringList attrs;
    GetAttrs(attrs);
    writer.write_attrs(attrs);

    writer.write_line(L"library " + ShortName() + L" {");
    writer.indent();
    {
        DumpMetadata(writer);
        writer.write_empty_line();

        Set<String> ifaces;
        auto children = Children();
        for (auto& child : *children)
        {
            if (std::dynamic_pointer_cast<MInterface>(child))
            {
                ifaces.insert(child->ShortName());
            }
        }

        writer.write_line(L"// Forward declare all types defined in this typelib");
        for (auto& child : *children)
        {
            if (std::dynamic_pointer_cast<MCoClass>(child))
            {
                writer.write_line(child->Name() + L";");
            }
        }
        auto fwdDeclarations = Dictionary<String, String>();
        for (auto& child : *children)
        {
            if (std::dynamic_pointer_cast<MInterface>(child))
            {
                fwdDeclarations.insert(std::make_pair(child->ShortName(), child->Name()));
            }
        }
        for (auto& child : *children)
        {
            if (std::dynamic_pointer_cast<MDispInterface>(child))
            {
                fwdDeclarations.insert(std::make_pair(child->ShortName(), child->Name()));
            }
        }
        for (auto& decl : fwdDeclarations)
        {
            writer.write_line(decl.second + L";");
        }

        for (auto& child : *children)
        {
            if (std::dynamic_pointer_cast<MEnum>(child))
            {
                if (!writer.block_first_line())
                    writer.write_empty_line();
                child->Dump(writer);
            }
        }
        for (auto& child : *children)
        {
            if (std::dynamic_pointer_cast<MRecord>(child))
            {
                if (!writer.block_first_line())
                    writer.write_empty_line();
                child->Dump(writer);
            }
        }

retry:
        for (size_t i = 0; i < children->size() - 1; ++i)
        {
            for (size_t j = i + 1; j < children->size(); ++j)
            {
                auto& item1 = (*children)[i];
                auto& item2 = (*children)[j];
                auto i1 = std::dynamic_pointer_cast<MInterface>(item1);
                auto i2 = std::dynamic_pointer_cast<MInterface>(item2);
                if (!i1 || !i2)
                    continue;
                if (i1->BaseName() == i2->ShortName())
                {
                    std::swap(item1, item2);
                    goto retry;
                }
            }
        }

        for (auto& child : *children)
        {
            if (std::dynamic_pointer_cast<MEnum>(child))
                continue;
            if (std::dynamic_pointer_cast<MRecord>(child))
                continue;
            if (child->DisplayAtTLBLevel(ifaces))
            {
                if (!writer.block_first_line())
                    writer.write_empty_line();
                child->Dump(writer);
            }
        }
    }
    writer.unindent();
    writer.write_line(L"};");
}
