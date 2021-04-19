#include "MDispInterface.hpp"
#include "MCustData.hpp"
#include "MIDispatchMethods.hpp"
#include "MIDispatchProperties.hpp"
#include "MDispInterfaceInheritedInterfaces.hpp"

MDispInterface::MDispInterface(MNode* parent, MComPtr<ITypeInfo> ti, Ptr<MTypeAttr> ta, bool topLevel)
{
    m_parent = parent;
    m_name = MTypeInfoExtra::GetName(ti);
    m_ta = ta;
    m_ti = ti;
    m_methodChildren = std::static_pointer_cast<MNode>(MakePtr<MIDispatchMethods>(this));
    m_propChildren = std::static_pointer_cast<MNode>(MakePtr<MIDispatchProperties>(this));
    m_topLevel = topLevel;
}
Ptr<MNodeList> MDispInterface::GenChildren()
{
    auto ret = MakePtr<MNodeList>();
    if ((*m_ta)->cImplTypes > 0)
    {
        ret->push_back(MakePtr<MDispInterfaceInheritedInterfaces>(this, m_ti, m_ta));
    }
    else
    {
        ret->push_back(m_propChildren);
        ret->push_back(m_methodChildren);
    }
    return ret;
}
void MDispInterface::GetAttrs(StringList& attrs)
{
    attrs.push_back(L"uuid(" + GetString((*m_ta)->guid) + L")");

    MTypeAttr ta(m_ti);
    String strVersion;
    if (MTypeInfoExtra::GetVersion(ta, strVersion))
    {
        attrs.push_back(strVersion);
    }

    MCustData::GetCustData(m_ti, attrs);

    DWORD context;
    auto help = MTypeInfoExtra::GetHelpDocumentationById(m_ti, -1, context);
    AddHelpStringAndContext(attrs, help, context);

    if ((*m_ta)->wTypeFlags & TYPEFLAG_FAGGREGATABLE) attrs.push_back(L"aggregatable");
    if ((*m_ta)->wTypeFlags & TYPEFLAG_FAPPOBJECT) attrs.push_back(L"appobject");
    // TYPEFLAG_FCANCREATE is not applicable to interfaces/dispinterfaces
    if ((*m_ta)->wTypeFlags & TYPEFLAG_FCONTROL) attrs.push_back(L"control");
    // No IDL syntax for TYPEFLAG_FDISPATCHABLE -- it is computed
    if ((*m_ta)->wTypeFlags & TYPEFLAG_FHIDDEN) attrs.push_back(L"hidden");
    if ((*m_ta)->wTypeFlags & TYPEFLAG_FDUAL) attrs.push_back(L"dual");
    if ((*m_ta)->wTypeFlags & TYPEFLAG_FLICENSED) attrs.push_back(L"licensed");
    if ((*m_ta)->wTypeFlags & TYPEFLAG_FNONEXTENSIBLE) attrs.push_back(L"nonextensible");
    if ((*m_ta)->wTypeFlags & TYPEFLAG_FOLEAUTOMATION) attrs.push_back(L"oleautomation");
    // Can't find IDL for TYPEFLAG_FPREDECLID?!?
    if ((*m_ta)->wTypeFlags & TYPEFLAG_FPROXY) attrs.push_back(L"proxy");
    // Can't find IDL for TYPEFLAG_FREPLACEABLE?!?
    if ((*m_ta)->wTypeFlags & TYPEFLAG_FRESTRICTED) attrs.push_back(L"restricted");
    // Can't find IDL for TYPEFLAG_FREVERSEBIND?!?
}
void MDispInterface::Dump(MSmartWriter& writer)
{
    StringList attrs;
    GetAttrs(attrs);
    writer.write_attrs(attrs);

    writer.write_line(Name() + L" {");
    writer.indent();
    m_propChildren->Dump(writer);
    m_methodChildren->Dump(writer);
    writer.unindent();
    writer.write_line(L"};");
}
