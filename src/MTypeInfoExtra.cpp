#include "MTypeInfoExtra.hpp"
#include <shlwapi.h>

/*static*/ String MTypeInfoExtra::PaddedHex(DWORD value)
{
    wchar_t sz[16];
    std::swprintf(sz, _countof(sz), L"0x%08X", value);
    return sz;
}

/*static*/ String MTypeInfoExtra::GetName(MComPtr<ITypeLib> tl)
{
    BSTR bstr = NULL;
    DWORD context = 0;
    tl->GetDocumentation(-1, &bstr, NULL, &context, NULL);
    String ret;
    if (bstr)
    {
        ret = bstr;
        ::SysFreeString(bstr);
    }
    return ret;
}

/*static*/ String MTypeInfoExtra::GetName(MComPtr<ITypeInfo> ti)
{
	return MTypeInfoExtra::GetDocumentationById(ti, -1);
}

/*static*/ String MTypeInfoExtra::GetDocumentationById(MComPtr<ITypeInfo> ti, MEMBERID memid)
{
    BSTR bstr = NULL;
    DWORD context;
    ti->GetDocumentation(memid, &bstr, NULL, &context, NULL);
    String ret;
    if (bstr)
    {
        ret = bstr;
        ::SysFreeString(bstr);
    }
    return ret;
}

/*static*/ String
MTypeInfoExtra::GetHelpDocumentationById(MComPtr<ITypeInfo> ti, MEMBERID memid, DWORD& context)
{
    BSTR bstr = NULL;
    ti->GetDocumentation(memid, NULL, &bstr, &context, NULL);
    String ret;
    if (bstr)
    {
        ret = bstr;
        ::SysFreeString(bstr);
    }
    return ret;
}

/*static*/ String MTypeInfoExtra::GetHelpDocumentation(MComPtr<ITypeLib> ti, String& helpfile, DWORD& context)
{
    String ret;
    BSTR bstr = NULL, bstrHelpFile = NULL;
    ti->GetDocumentation(-1, NULL, &bstr, &context, &bstrHelpFile);
    if (bstr)
    {
        ret = bstr;
        ::SysFreeString(bstr);
    }
    if (bstrHelpFile)
    {
        helpfile = PathFindFileNameW(bstrHelpFile);
        ::SysFreeString(bstrHelpFile);
    }
    return ret;
}

/*static*/ bool MTypeInfoExtra::SwapForInterface(MComPtr<ITypeInfo>& ti, Ptr<MTypeAttr>& ta)
{
    if ((*ta)->typekind == TKIND_DISPATCH && ((*ta)->wTypeFlags & TYPEFLAG_FDUAL))
    {
        HREFTYPE href;
        ti->GetRefTypeOfImplType(-1, &href);
        MComPtr<ITypeInfo> ti2;
        ti->GetRefTypeInfo(href, &ti2);
        ti = ti2;
        ta = MakePtr<MTypeAttr>(ti);
        return true;
    }
    return false;
}

/*static*/ String MTypeInfoExtra::ReEscape(const String& s)
{
    String ret = L"\"";
    for (auto ch : s)
    {
        switch (ch)
        {
        case L'\0': ret += L"\\0"; break;
        case L'\n': ret += L"\\n"; break;
        case L'\r': ret += L"\\r"; break;
        case L'\b': ret += L"\\b"; break;
        case L'\a': ret += L"\\a"; break;
        case L'\f': ret += L"\\f"; break;
        case L'\t': ret += L"\\t"; break;
        case L'\v': ret += L"\\v"; break;
        default:
            if (ch < 0x20)
            {
                wchar_t sz[8];
                std::swprintf(sz, _countof(sz), L"\\%03o", ch);
                ret += sz;
            }
            else
            {
                ret += ch;
            }
        }
    }
    ret += L'\"';
    return ret;
}

/*static*/ String MTypeInfoExtra::QuoteString(const _variant_t& var)
{
    if (var.vt == VT_NULL)
        return L"NULL";
    if (var.vt == VT_DISPATCH && !var.pdispVal)
        return L"NULL";
    if (var.vt == VT_BOOL)
        return (var.boolVal ? L"-1" : L"0");
    if (var.vt == VT_BSTR)
        return MTypeInfoExtra::ReEscape(String(var.operator _bstr_t()));
    return String(var.operator _bstr_t());
}

/*static*/ List<String>
MTypeInfoExtra::GetNames(Ptr<MFuncDesc> fd, MComPtr<ITypeInfo> ti)
{
    BSTR names[64] = { NULL };
    UINT cGot = 0;
    HRESULT hr = ti->GetNames((*fd)->memid, names, (*fd)->cParams + 1, &cGot);
    List<String> ret;
    for (UINT i = 0; i < cGot; ++i)
    {
        if (!names[i])
            break;
        ret.push_back(names[i]);
        ::SysFreeString(names[i]);
    }
    return ret;
}

/*static*/ String
MTypeInfoExtra::GetDllEntry(MComPtr<ITypeInfo> ti, INVOKEKIND invkind, int memid)
{
    BSTR dllentry = NULL;
    ti->GetDllEntry(memid, invkind, &dllentry, 0, 0);
    String ret;
    if (dllentry)
    {
        ret = dllentry;
        ::SysFreeString(dllentry);
    }
    return ret;
}
