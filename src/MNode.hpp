#pragma once

#include "OleBow.hpp"
#include "MTypeInfoExtra.hpp"

class MNode;
typedef List<Ptr<MNode> > MNodeList;

class MNode
{
public:
    size_t m_index;
    MNode* m_parent;
    Ptr<MNodeList> m_children;

    MNode() : m_index(0), m_parent(NULL)
    {
    }
    virtual ~MNode()
    {
    }

    virtual String Name() = 0;
    virtual String ShortName() = 0;
    virtual String ObjectName() = 0;
    virtual void GetAttrs(StringList& attrs) = 0;
    virtual Ptr<MNodeList> GenChildren() = 0;
    virtual void Dump(MSmartWriter& writer) = 0;
    virtual bool DisplayAtTLBLevel(const Set<String>& ifaces) = 0;

    Ptr<MNodeList> Children();
    void CommonBuildTlibNode(MComPtr<ITypeInfo> ti,
                             bool topLevel, bool swapfordispatch,
                             MNodeList& res);
    void AddHelpStringAndContext(StringList& attrs, String help, DWORD context);
    void AddUUID(StringList& attrs, const GUID& guid);
};
