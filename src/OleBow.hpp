#pragma once

#ifndef _INC_WINDOWS
    #include <windows.h>
#endif
#include <oaidl.h>
#include <comdef.h>
#include <comip.h> // for _com_ptr_t
#include <comutil.h> // for _variant_t
#include <vector> // for std::vector
#include <map> // for std::map
#include <string> // for std::wstring, ...
#include <cstring> // for std::memcmp, ...
#include <memory> // for std::shared_ptr, std::make_shared, ...
#include <stdexcept> // for std::runtime_error, ...
#include <set> // for std::set

#define Ptr std::shared_ptr
#define MakePtr std::make_shared
#define List std::vector
#define Dictionary std::map
#define Set std::set
typedef std::wstring String;
typedef List<String> StringList;
typedef Set<String> StringSet;

#include "MComPtr.hpp"
#include "MSmartWriter.hpp"
#include "MTypeInfoExtra.hpp"
#include "MNode.hpp"

#ifdef OLEBOW_DLL
    #ifdef OLEBOW_BUILD
        #define OLEBOWAPI __declspec(dllexport)
    #else
        #define OLEBOWAPI __declspec(dllimport)
    #endif
#else
    #define OLEBOWAPI /*empty*/
#endif

EXTERN_C OLEBOWAPI void APIENTRY
DumpTypeLib(MWriter& writer, const wchar_t *path, bool sort, bool dependency);

String GetString(const GUID& guid);
String GetCallConvString(CALLCONV callconv);
bool GetFileNameOfTypeLib(MComPtr<ITypeLib> dl, GUID& guid, String& name, String& path);
