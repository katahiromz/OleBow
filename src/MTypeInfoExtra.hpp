#pragma once

#include "OleBow.hpp"
#include "MFuncDesc.hpp"
#include "MTypeAttr.hpp"

struct MTypeInfoExtra
{
    static String PaddedHex(DWORD value);
    static String GetName(MComPtr<ITypeLib> tl);
    static String GetName(MComPtr<ITypeInfo> ti);
    static String GetDocumentationById(MComPtr<ITypeInfo> ti, MEMBERID memid);
    static String GetHelpDocumentationById(MComPtr<ITypeInfo> ti, MEMBERID memid, DWORD& context);
    static String GetHelpDocumentation(MComPtr<ITypeLib> ti, String& helpfile, DWORD& context);
    static bool SwapForInterface(MComPtr<ITypeInfo>& ti, Ptr<MTypeAttr>& ta);
    static String ReEscape(const String& s);
    static String QuoteString(const _variant_t& var);
    static String VarTypeToString(VARTYPE vt);
    static List<String> GetNames(Ptr<MFuncDesc> fd, MComPtr<ITypeInfo> ti);
    static String GetDllEntry(MComPtr<ITypeInfo> ti, INVOKEKIND invkind, int memid);
    static bool GetVersion(MTypeAttr& ta, String& str);
};
