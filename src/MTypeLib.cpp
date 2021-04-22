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
#include <algorithm>

MTypeLib::MTypeLib()
    : m_pAttr(NULL)
{
}

MTypeLib::MTypeLib(const wchar_t *path)
    : m_pAttr(NULL)
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
    String helpfile;
    m_name += L" (" + MTypeInfoExtra::GetHelpDocumentation(m_tlib, helpfile, ctx) + L")";
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

void MTypeLib::GetAttrs(StringList& attrs)
{
    if (m_pAttr)
    {
        AddUUID(attrs, m_pAttr->guid);
        if (m_pAttr->wMajorVerNum || m_pAttr->wMinorVerNum)
        {
            String str = L"version(";
            str += std::to_wstring(m_pAttr->wMajorVerNum);
            str += L".";
            str += std::to_wstring(m_pAttr->wMinorVerNum);
            str += L")";
            attrs.push_back(str.c_str());
        }
    }

    DWORD cnt = 0;
    String helpfile;
    auto help = MTypeInfoExtra::GetHelpDocumentation(m_tlib, helpfile, cnt);
    if (help.size())
    {
        String str = L"helpstring(\"";
        str += help;
        str += L"\")";
        attrs.push_back(str.c_str());
    }
    if (helpfile.size())
    {
        String str = L"helpfile(\"";
        str += helpfile;
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

void MTypeLib::Sort()
{
    auto children = Children();

    // create a name-to-node mapping
    Dictionary<String, Ptr<MNode> > name_to_node;
    for (auto& child : *children)
    {
        name_to_node[child->ShortName()] = child;
        //fprintf(stderr, "%ls: %ls\n", child->Class().c_str(), child->ShortName().c_str());
    }

    // create a depending mapping (the node name to the depending nodes)
    Dictionary<String, StringSet> depending_map;
    for (auto& child : *children)
    {
        auto name = child->ShortName();
        Ptr<StringSet> d = child->Depending();
        depending_map[name];
        for (auto& item : *d)
        {
            if (name_to_node.count(item) > 0 && name != item)
            {
                depending_map[name].insert(item);
            }
        }
    }

    StringList names;
    StringSet name_set;
depending_retry:
    for (auto& pair1 : depending_map)
    {
        bool flag = false;
        for (auto& pair2 : depending_map)
        {
            if (pair1.first != pair2.first)
            {
                if (pair2.second.count(pair1.first) > 0)
                {
                    flag = true;
                    break;
                }
            }
        }
        if (!flag)
        {
            auto& item = pair1.first;
            if (name_set.count(item) == 0)
            {
                names.push_back(item);
                name_set.insert(item);
            }
            depending_map.erase(pair1.first);
            goto depending_retry;
        }
    }

#if 0
    for (auto& pair : depending_map)
    {
        for (auto& item : pair.second)
        {
            printf("%ls: %ls\n", pair.first.c_str(), item.c_str());
        }
    }
#endif

    // sort
retry:
    while (depending_map.size())
    {
        auto added = MakePtr<StringSet>();
        for (auto& pair : name_to_node)
        {
            if (depending_map.find(pair.first) == depending_map.end() ||
                depending_map[pair.first].empty())
            {
                added->insert(pair.first);
                depending_map.erase(pair.first);
            }
        }
        if (added->empty())
            goto hell;
        bool flag = false;
        for (auto& item : *added)
        {
            if (name_set.count(item) == 0)
            {
                names.push_back(item);
                name_set.insert(item);
            }
            for (auto& pair : depending_map)
            {
                auto size1 = pair.second.size();
                pair.second.erase(item);
                auto size2 = pair.second.size();
                if (size1 != size2)
                    flag = true;
            }
            auto size1 = depending_map.size();
            depending_map.erase(item);
            auto size2 = depending_map.size();
            if (size1 != size2)
                flag = true;
        }
        if (!flag)
        {
            if (depending_map.size())
            {
#if 1
                printf("---------\n");
                for (auto& pair : depending_map)
                {
                    for (auto& item : pair.second)
                    {
                        printf(":: %ls: %ls\n", pair.first.c_str(), item.c_str());
                    }
                }
#endif
                // TODO:
                for (auto& pair : depending_map)
                {
                    names.push_back(pair.first);
                    name_set.insert(pair.first);
                }
                depending_map.clear();
            }
            break;
        }
    }
hell:
    assert(names.size() == m_children->size());
    if (names.size() != m_children->size())
    {
        fprintf(stderr, "%d, %d\n", (int)names.size(), (int)m_children->size());
        for (auto& name : names)
        {
            fprintf(stderr, "name: %ls\n", name.c_str());
        }
        for (auto& node : *m_children)
        {
            fprintf(stderr, "ShortName: %ls\n", node->ShortName().c_str());
        }
    }
    auto ret = MakePtr<MNodeList>();
    for (auto& name : names)
    {
        ret->push_back(name_to_node[name]);
    }
    m_children = ret;
}

void MTypeLib::DumpDependency(MWriter& writer)
{
    auto children = Children();
    for (auto& child : *children)
    {
        auto depending = child->Depending();
        auto d = mstr_join(*depending, L", ");

        writer.Write(child->Class().c_str());
        writer.Write(L" ");
        writer.Write(child->ShortName().c_str());
        writer.Write(L" : {");
        writer.Write(d.c_str());
        writer.Write(L"}.\n");
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

        auto children = Children();

        Set<String> ifaces;
        for (auto& child : *children)
        {
            if (std::dynamic_pointer_cast<MInterface>(child))
            {
                ifaces.insert(child->ShortName());
            }
        }

        writer.write_line(L"// Forward declare all types defined in this typelib");

        auto fwdDeclarations = Dictionary<String, String>();
        for (auto& child : *children)
        {
            if (std::dynamic_pointer_cast<MCoClass>(child))
            {
                writer.write_line(child->Name() + L";");
            }
            if (std::dynamic_pointer_cast<MInterface>(child))
            {
                fwdDeclarations.insert(std::make_pair(child->ShortName(), child->Name()));
            }
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
