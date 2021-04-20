#include "MMethod.hpp"
#include "MCustData.hpp"
#include "MElemDesc.hpp"

void MMethod::GetAttrs(StringList& attrs)
{
    String str = L"id(";
    str += MTypeInfoExtra::PaddedHex((*m_fd)->memid);
    str += L")";
    attrs.push_back(str);

    switch ((*m_fd)->invkind)
    {
    case INVOKE_PROPERTYGET: attrs.push_back(L"propget"); break;
    case INVOKE_PROPERTYPUT: attrs.push_back(L"propput"); break;
    case INVOKE_PROPERTYPUTREF: attrs.push_back(L"propputref"); break;
    }

    MCustData::GetAllFuncCustData((*m_fd)->memid, (*m_fd)->invkind, m_ti, attrs);

    if ((*m_fd)->wFuncFlags & FUNCFLAG_FRESTRICTED) attrs.push_back(L"restricted");
    if ((*m_fd)->wFuncFlags & FUNCFLAG_FHIDDEN) attrs.push_back(L"hidden");

    DWORD context = 0;
    auto help = MTypeInfoExtra::GetHelpDocumentationById(m_ti, (*m_fd)->memid, context);
    AddHelpStringAndContext(attrs, help, context);
}
void MMethod::Dump(MSmartWriter& writer, bool bAsDispatch)
{
    if (!writer.block_first_line())
        writer.write_empty_line();

    StringList attrs;
    GetAttrs(attrs);
    writer.write_attrs(attrs);

    auto ed = (*m_fd)->elemdescFunc;
    ELEMDESC* elast = NULL;
    bool bRetvalPresent = false;
    auto names = MTypeInfoExtra::GetNames(m_fd, m_ti);
    auto edps = m_fd->elemdescParams();
    if ((*m_fd)->cParams > 0)
    {
        if (edps->size() > 0)
            elast = (*edps)[edps->size() - 1];
        if (bAsDispatch && elast && (elast->paramdesc.wParamFlags & PARAMFLAG_FRETVAL))
            bRetvalPresent = true;
    }

    StringList params;
    for (UINT y = 0; y < (UINT)(*m_fd)->cParams; ++y)
    {
        String paramname = L"rhs";
        if (y + 1 < names.size() && names[y + 1].size())
            paramname = names[y + 1];
        auto edp = (*edps)[y];
        auto attrs = ParamFlagsDescription(edp->paramdesc);
        String str;
        if (attrs.size())
        {
            str += L"[";
            str += mstr_join(attrs, L", ");
            str += L"] ";
        }
        params.push_back(str +
            reinterpret_cast<MTypeDesc&>(edp->tdesc)
                .GetTypedName(m_ti, String(paramname)));
    }

    String head;
    if (bRetvalPresent)
    {
        head += reinterpret_cast<MTypeDesc&>(elast->tdesc).ComTypeNameAsString(m_ti);
    }
    else
    {
        head += reinterpret_cast<MTypeDesc&>(ed.tdesc).ComTypeNameAsString(m_ti);
    }
    {
        auto str = GetCallConvString((*m_fd)->callconv);
        if (str.size())
            head += L" " + str;
    }
    head += L" " + Name() + L"(";

    writer.write_function(head, params, L");");
}
