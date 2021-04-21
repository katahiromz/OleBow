#pragma once

#include "MNode.hpp"
#include "MTypeDesc.hpp"

class MMethod : public MNode
{
public:
    MMethod(MNode* parent, MComPtr<ITypeInfo> ti, Ptr<MFuncDesc> fd)
    {
        m_parent = parent;
        m_ti = ti;
        m_fd = fd;
        auto names = MTypeInfoExtra::GetNames(fd, m_ti);
        m_name = names[0];
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
        return L"MMethod";
    }
    void GetAttrs(StringList& attrs) override;
    Ptr<MNodeList> GenChildren() override
    {
        return MakePtr<MNodeList>();
    }
    void Dump(MSmartWriter& writer) override
    {
        Dump(writer, false);
    }
    void Dump(MSmartWriter& writer, bool bAsDispatch);
    bool DisplayAtTLBLevel(const Set<String>& ifaces) override
    {
        return false;
    }
    Ptr<StringSet> GenDepending() override
    {
        auto ret = MakePtr<StringSet>();
        auto edps = m_fd->elemdescParams();
        ELEMDESC* elast = NULL;
        if ((*m_fd)->cParams > 0)
        {
            if (edps->size() > 0)
                elast = (*edps)[edps->size() - 1];
        }
        for (UINT y = 0; y < (UINT)(*m_fd)->cParams; ++y)
        {
            auto edp = (*edps)[y];
            reinterpret_cast<MTypeDesc&>(edp->tdesc).GenDepending(m_ti, *ret);
        }
        if (elast)
            reinterpret_cast<MTypeDesc&>(elast->tdesc).GenDepending(m_ti, *ret);
        return ret;
    }
protected:
    String m_name;
    Ptr<MFuncDesc> m_fd;
    MComPtr<ITypeInfo> m_ti;

    StringList ParamFlagsDescription(const PARAMDESC& pd)
    {
        auto flg = pd.wParamFlags;
        StringList attrs;
        if (flg & PARAMFLAG_FIN) attrs.push_back(L"in");
        if (flg & PARAMFLAG_FOUT) attrs.push_back(L"out");
        if (flg & PARAMFLAG_FRETVAL) attrs.push_back(L"retval");
        if (flg & PARAMFLAG_FOPT) attrs.push_back(L"optional");
        if (flg & PARAMFLAG_FLCID) attrs.push_back(L"lcid");
        if (pd.pparamdescex)
        {
            _variant_t var(pd.pparamdescex->varDefaultValue);
            if (flg & PARAMFLAG_FHASDEFAULT)
                attrs.push_back(L"defaultvalue(" + MTypeInfoExtra::QuoteString(var) + L")");
        }
        return attrs;
    }
};
