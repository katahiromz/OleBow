#include "MNode.hpp"
#include "MModule.hpp"
#include "MDispInterface.hpp"
#include "MEnum.hpp"
#include "MRecord.hpp"
#include "MUnion.hpp"
#include "MCoClass.hpp"
#include "MInterface.hpp"
#include "MTypeDef.hpp"

Ptr<MNodeList> MNode::Children()
{
    if (!m_children)
    {
        m_children = GenChildren();
        for (size_t i = 0; i < m_children->size(); ++i)
        {
            (*m_children)[i]->m_index = i;
        }
    }
    return m_children;
}

Ptr<StringSet> MNode::Depending()
{
    if (!m_depending)
    {
        m_depending = GenDepending();
    }
    return m_depending;
}

Ptr<StringSet> MNode::Providing()
{
    if (!m_providing)
    {
        m_providing = GenProviding();
    }
    return m_providing;
}

void
MNode::CommonBuildTlibNode(MComPtr<ITypeInfo> ti,
                           bool topLevel, bool swapfordispatch,
                           MNodeList& res)
{
    auto ta = MakePtr<MTypeAttr>(ti);
    switch ((*ta)->typekind)
    {
    case TKIND_ENUM:
        res.push_back(MakePtr<MEnum>(this, ti, ta));
        break;
    case TKIND_RECORD:
        res.push_back(MakePtr<MRecord>(this, ti, ta));
        break;
    case TKIND_MODULE:
        res.push_back(MakePtr<MModule>(this, ti, ta));
        break;
    case TKIND_INTERFACE:
        res.push_back(MakePtr<MInterface>(this, ti, ta, topLevel));
        break;
    case TKIND_DISPATCH:
        res.push_back(MakePtr<MDispInterface>(this, ti, ta, topLevel));
        if (swapfordispatch && MTypeInfoExtra::SwapForInterface(ti, ta))
        {
            res.push_back(MakePtr<MInterface>(this, ti, ta, topLevel));
        }
        break;
    case TKIND_COCLASS:
        res.push_back(MakePtr<MCoClass>(this, ti, ta));
        break;
    case TKIND_ALIAS:
        res.push_back(MakePtr<MTypeDef>(this, ti, ta));
        break;
    case TKIND_UNION:
        res.push_back(MakePtr<MUnion>(this, ti, ta));
        break;
    default:
        break;
    }
}

void MNode::AddHelpStringAndContext(StringList& attrs, String help, DWORD context)
{
    if (help.size())
        attrs.push_back(L"helpstring(\"" + help + L"\")");
    if (context)
        attrs.push_back(L"helpcontext(" + MTypeInfoExtra::PaddedHex(context) + L")");
}

void MNode::AddUUID(StringList& attrs, const GUID& guid)
{
    if (std::memcmp(&guid, &GUID_NULL, sizeof(GUID_NULL)) != 0)
    {
        attrs.push_back(L"uuid(" + GetString(guid) + L")");
    }
}

void MNode::AddVersion(StringList& attrs, MTypeAttr& ta)
{
    if (ta->wMajorVerNum || ta->wMinorVerNum)
    {
        String str = L"version(";
        str += std::to_wstring(ta->wMajorVerNum);
        str += L".";
        str += std::to_wstring(ta->wMinorVerNum);
        str += L")";
        attrs.push_back(str);
    }
}
