#pragma once

struct MCustData
{
    static void GetCustData(MComPtr<ITypeInfo> ti, StringList& attrs)
    {
        MComPtr<ITypeInfo2> t2;
        ti->QueryInterface(IID_ITypeInfo2, (LPVOID *)&t2);
        if (!t2)
        {
            return;
        }

        CUSTDATA custdata;
        t2->GetAllCustData(&custdata);
        for (DWORD x = 0; x < custdata.cCustData; x++)
        {
            auto item = &custdata.prgCustData[x];
            String str = L"custom(";
            str += GetString(item->guid);
            str += L", ";
            str += MTypeInfoExtra::QuoteString(item->varValue);
            str += L")";
            attrs.push_back(str);
        }
        ::ClearCustData(&custdata);
    }

    static void GetAllFuncCustData(MEMBERID memberid, INVOKEKIND invokekind,
                                   MComPtr<ITypeInfo> ti, StringList& attrs)
    {
        MComPtr<ITypeInfo2> t2;
        ti->QueryInterface(IID_ITypeInfo2, (LPVOID *)&t2);
        if (!t2)
        {
            return;
        }

        CUSTDATA custdata;
        UINT index;
        t2->GetFuncIndexOfMemId(memberid, invokekind, &index);
        t2->GetAllFuncCustData(index, &custdata);
        for (DWORD x = 0; x < custdata.cCustData; x++)
        {
            auto item = &custdata.prgCustData[x];
            String str = L"custom(";
            str += GetString(item->guid);
            str += L", ";
            str += MTypeInfoExtra::QuoteString(item->varValue);
            str += L")";
            attrs.push_back(str);
        }
        ::ClearCustData(&custdata);
    }
};

struct MCustomData
{
    GUID m_g;
    _variant_t m_v;

    MCustomData(GUID g, const VARIANT& v)
    {
        m_g = g;
        m_v = v;
    }
};

class MCustomDatas
{
public:
    MCustomDatas(MComPtr<ITypeLib2> tl2)
    {
        tl2->GetAllCustData(&m_custdata);
    }
    ~MCustomDatas()
    {
    }
    Ptr<List<Ptr<MCustomData> > > Items()
    {
        auto ret = MakePtr<List<Ptr<MCustomData> > >();
        for (size_t x = 0; x < m_custdata.cCustData; ++x)
        {
            CUSTDATAITEM& cdi = m_custdata.prgCustData[x];
            Ptr<MCustomData> cd = MakePtr<MCustomData>(cdi.guid, cdi.varValue);
            ret->push_back(cd);
        }
        return ret;
    }
protected:
    CUSTDATA m_custdata;
};
