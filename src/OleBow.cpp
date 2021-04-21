#include "OleBow.hpp"
#include "MTypeLib.hpp"
#include "MTypeLibAttr.hpp"
#include "MFileWriter.hpp"
#include <shlwapi.h>

EXTERN_C OLEBOWAPI void APIENTRY
DumpTypeLib(MWriter& writer, const wchar_t *path, bool sort, bool dependency)
{
    auto tlib = MakePtr<MTypeLib>();
    tlib->Load(path);

    if (sort)
    {
        tlib->Sort();
    }

    if (dependency)
    {
        MFileWriter writer(stdout);
        tlib->DumpDependency(writer);
    }

    MSmartWriter smart_writer(writer);
    tlib->Dump(smart_writer);
}

String GetString(const GUID& guid)
{
    WCHAR sz[40] = L"";
    if (::StringFromGUID2(guid, sz, _countof(sz)))
    {
        sz[37] = 0;
        return &sz[1];
    }
    return L"(internal error)";
}

String GetCallConvString(CALLCONV callconv)
{
    switch (callconv)
    {
    case CC_FASTCALL: return L"_fastcall";
    case CC_CDECL: return L"_cdecl";
    case CC_PASCAL: return L"_pascal";
    case CC_MACPASCAL: return L"_pascal";
    case CC_STDCALL: return L"_stdcall";
    case CC_FPFASTCALL: return L"_fastcall";
    case CC_MPWCDECL: return L"_cdecl";
    case CC_MPWPASCAL: return L"_pascal";
    default:
        return L"";
    }
}

bool GetFileNameOfTypeLib(MComPtr<ITypeLib> dl, GUID& guid, String& name, String& path)
{
    MTypeLibAttr attr(dl);
    guid = attr->guid;

    WCHAR sz[MAX_PATH];
    std::swprintf(sz, _countof(sz), L"TypeLib\\{%ls}\\%u.%u",
        GetString(attr->guid).c_str(), attr->wMajorVerNum, attr->wMinorVerNum);

    HKEY hKey = NULL;
    ::RegOpenKeyExW(HKEY_CLASSES_ROOT, sz, 0, KEY_READ, &hKey);
    if (!hKey)
        return false;

    DWORD cb = sizeof(sz);
    sz[0] = 0;
    ::RegQueryValueExW(hKey, NULL, NULL, NULL, (LPBYTE)sz, &cb);
    name = sz;

    HKEY hSubKey = NULL;
    std::swprintf(sz, _countof(sz), L"%u\\win32", attr->lcid);
    ::RegOpenKeyExW(hKey, sz, 0, KEY_READ, &hSubKey);
    if (!hSubKey)
    {
        std::swprintf(sz, _countof(sz), L"%u\\win64", attr->lcid);
        ::RegOpenKeyExW(hKey, sz, 0, KEY_READ, &hSubKey);
    }
    if (hSubKey)
    {
        cb = sizeof(sz);
        sz[0] = 0;
        ::RegQueryValueExW(hSubKey, NULL, NULL, NULL, (LPBYTE)sz, &cb);
        path = sz;
        ::RegCloseKey(hSubKey);
    }
    ::RegCloseKey(hKey);

    auto pch = PathFindFileNameW(path.c_str());
    while (L'0' <= *pch && *pch <= L'9')
        ++pch;
    if (*pch == 0)
    {
        auto ich = path.find_last_of(L'\\');
        if (ich != path.npos)
        {
            path = path.substr(0, ich);
        }
    }
    return true;
}
