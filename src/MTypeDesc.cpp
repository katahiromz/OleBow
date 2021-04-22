#include "MTypeDesc.hpp"

static String stringifyCustomType(HREFTYPE refType, MComPtr<ITypeInfo> pti)
{
    MComPtr<ITypeInfo> pTypeInfo(pti);
    MComPtr<ITypeInfo> pCustTypeInfo;
    HRESULT hr = pTypeInfo->GetRefTypeInfo(refType, &pCustTypeInfo);
    if (hr)
    {
        return L"UnknownCustomType";
    }
    BSTR bstrType;
    hr = pCustTypeInfo->GetDocumentation(-1, &bstrType, NULL, NULL, NULL);
    if (hr)
    {
        return L"UnknownCustomType";
    }
    String str;
    if (bstrType)
    {
        str = bstrType;
        ::SysFreeString(bstrType);
    }
    return str;
}

static String
stringifyTypeDesc(TYPEDESC* typeDesc, MComPtr<ITypeInfo> pTypeInfo)
{
    String ret;
    switch (typeDesc->vt)
    {
    case VT_PTR:
        ret = stringifyTypeDesc(typeDesc->lptdesc, pTypeInfo);
        ret += L"*";
        return ret;
    case VT_SAFEARRAY:
        ret += L"SAFEARRAY(";
        ret += stringifyTypeDesc(typeDesc->lptdesc, pTypeInfo);
        ret += L")";
        return ret;
    case VT_CARRAY:
        ret += stringifyTypeDesc(&typeDesc->lpadesc->tdescElem, pTypeInfo);
        for (INT dim = 0; dim < typeDesc->lpadesc->cDims; ++dim)
        {
            ret += L"[";
            if (typeDesc->lpadesc->rgbounds[dim].lLbound == 0)
            {
                ret += std::to_wstring(typeDesc->lpadesc->rgbounds[dim].cElements);
            }
            else
            {
                ret += std::to_wstring(typeDesc->lpadesc->rgbounds[dim].lLbound);
                ret += L"...";
                ret += std::to_wstring(typeDesc->lpadesc->rgbounds[dim].cElements +
                                       typeDesc->lpadesc->rgbounds[dim].lLbound - 1);
            }
            ret += L"]";
        }
        return ret;
    case VT_USERDEFINED:
        return stringifyCustomType(typeDesc->hreftype, pTypeInfo);
    case VT_I2: return L"short";
    case VT_I4: return L"long";
    case VT_R4: return L"float";
    case VT_R8: return L"double";
    case VT_CY: return L"CURRENCY";
    case VT_DATE: return L"DATE";
    case VT_BSTR: return L"BSTR";
    case VT_DISPATCH: return L"IDispatch*";
    case VT_ERROR: return L"SCODE";
    case VT_BOOL: return L"VARIANT_BOOL";
    case VT_VARIANT: return L"VARIANT";
    case VT_UNKNOWN: return L"IUnknown*";
    case VT_UI1: return L"unsigned char";
    case VT_DECIMAL: return L"DECIMAL";
    case VT_I1: return L"char";
    case VT_UI2: return L"unsigned short";
    case VT_UI4: return L"unsigned long";
    case VT_I8: return L"int64";
    case VT_UI8: return L"uint64";
    case VT_INT: return L"int";
    case VT_UINT: return L"unsigned int";
    case VT_HRESULT: return L"HRESULT";
    case VT_VOID: return L"void";
    case VT_LPSTR: return L"LPSTR";
    case VT_LPWSTR: return L"LPWSTR";
    default:
        return L"<unknown type: " + std::to_wstring(typeDesc->vt) + L">";
    }
}

String MTypeDesc::ComTypeNameAsString(MComPtr<ITypeInfo> ti)
{
    return stringifyTypeDesc(this, ti);
}

void MTypeDesc::GenDepending(MComPtr<ITypeInfo> ti, StringSet& depending)
{
    GenDepending(this, ti, depending);
}

void MTypeDesc::GenDepending2(MComPtr<ITypeInfo> ti, StringSet& depending)
{
    GenDepending2(this, ti, depending);
}

void MTypeDesc::GenDepending(TYPEDESC* typeDesc, MComPtr<ITypeInfo> ti, StringSet& depending)
{
    String ret;
    switch (typeDesc->vt)
    {
    case VT_PTR:
    case VT_SAFEARRAY:
        GenDepending(typeDesc->lptdesc, ti, depending);
        break;
    case VT_CARRAY:
        GenDepending(&typeDesc->lpadesc->tdescElem, ti, depending);
        break;
    case VT_USERDEFINED:
        GenDepending(ti, typeDesc->hreftype, depending);
    default:
        break;
    }
}

void MTypeDesc::GenDepending2(TYPEDESC* typeDesc, MComPtr<ITypeInfo> ti, StringSet& depending2)
{
    String ret;
    switch (typeDesc->vt)
    {
    case VT_PTR:
        // ignore
        break;
    case VT_SAFEARRAY:
        GenDepending2(typeDesc->lptdesc, ti, depending2);
        break;
    case VT_CARRAY:
        GenDepending2(&typeDesc->lpadesc->tdescElem, ti, depending2);
        break;
    case VT_USERDEFINED:
        GenDepending(ti, typeDesc->hreftype, depending2);
    default:
        break;
    }
}

/*static*/ void MTypeDesc::GenDepending(MComPtr<ITypeInfo> pti, HREFTYPE refType, StringSet& depending)
{
    MComPtr<ITypeInfo> pTypeInfo(pti);
    MComPtr<ITypeInfo> pCustTypeInfo;
    HRESULT hr = pTypeInfo->GetRefTypeInfo(refType, &pCustTypeInfo);
    if (hr)
        return;

    BSTR bstrType;
    hr = pCustTypeInfo->GetDocumentation(-1, &bstrType, NULL, NULL, NULL);
    if (hr)
        return;

    String str = bstrType;
    ::SysFreeString(bstrType);
    depending.insert(str);
}

String MTypeDesc::GetTypedName(MComPtr<ITypeInfo> ti, String name)
{
    return MTypeDesc::GetTypedName(this, ti, name);
}

/*static*/ String
MTypeDesc::GetTypedName(TYPEDESC* typeDesc, MComPtr<ITypeInfo> pTypeInfo, String name)
{
    String ret;
    switch (typeDesc->vt)
    {
    case VT_PTR:
        ret = stringifyTypeDesc(typeDesc->lptdesc, pTypeInfo);
        ret += L"* ";
        ret += name;
        return ret;
    case VT_SAFEARRAY:
        ret += L"SAFEARRAY(";
        ret += stringifyTypeDesc(typeDesc->lptdesc, pTypeInfo);
        ret += L") ";
        ret += name;
        return ret;
    case VT_CARRAY:
        ret += name;
        for (INT dim = 0; dim < typeDesc->lpadesc->cDims; ++dim)
        {
            ret += L"[";
            if (typeDesc->lpadesc->rgbounds[dim].lLbound == 0)
            {
                ret += std::to_wstring(typeDesc->lpadesc->rgbounds[dim].cElements);
            }
            else
            {
                ret += std::to_wstring(typeDesc->lpadesc->rgbounds[dim].lLbound);
                ret += L"...";
                ret += std::to_wstring(typeDesc->lpadesc->rgbounds[dim].cElements +
                                       typeDesc->lpadesc->rgbounds[dim].lLbound - 1);
            }
            ret += L"]";
        }
        ret = GetTypedName(&typeDesc->lpadesc->tdescElem, pTypeInfo, ret);
        return ret;
    case VT_USERDEFINED:
        ret += stringifyCustomType(typeDesc->hreftype, pTypeInfo);
        ret += L" ";
        ret += name;
        return ret;
    case VT_I2: return L"short " + name;
    case VT_I4: return L"long " + name;
    case VT_R4: return L"float " + name;
    case VT_R8: return L"double " + name;
    case VT_CY: return L"CURRENCY " + name;
    case VT_DATE: return L"DATE " + name;
    case VT_BSTR: return L"BSTR " + name;
    case VT_DISPATCH: return L"IDispatch* " + name;
    case VT_ERROR: return L"SCODE " + name;
    case VT_BOOL: return L"VARIANT_BOOL " + name;
    case VT_VARIANT: return L"VARIANT " + name;
    case VT_UNKNOWN: return L"IUnknown* " + name;
    case VT_UI1: return L"unsigned char " + name;
    case VT_DECIMAL: return L"DECIMAL " + name;
    case VT_I1: return L"char " + name;
    case VT_UI2: return L"unsigned short " + name;
    case VT_UI4: return L"unsigned long " + name;
    case VT_I8: return L"int64 " + name;
    case VT_UI8: return L"uint64 " + name;
    case VT_INT: return L"int " + name;
    case VT_UINT: return L"unsigned int " + name;
    case VT_HRESULT: return L"HRESULT " + name;
    case VT_VOID: return L"void " + name;
    case VT_LPSTR: return L"LPSTR " + name;
    case VT_LPWSTR: return L"LPWSTR " + name;
    default:
        return L"<unknown type: " + std::to_wstring(typeDesc->vt) + L"> " + name;
    }
}
