#pragma once

#include "MNode.hpp"

class MTypeLib : public MNode
{
public:
    MTypeLib(bool sort = false);
    MTypeLib(const wchar_t *path, bool sort = false);
    ~MTypeLib();
    void Load(const wchar_t *path);
    void Unload();
    void Dump(MSmartWriter& writer) override;
    Ptr<StringSet> GenDepending() override
    {
        assert(0);
        return NULL;
    }
    Ptr<StringSet> GenProviding() override
    {
        assert(0);
        return NULL;
    }
protected:
    bool m_sort;
    TLIBATTR *m_pAttr;
    String m_name;
    MComPtr<ITypeLib> m_tlib;

    String Name() override;
    String ShortName() override;
    String ObjectName() override;
    void GetAttrs(StringList& attrs) override;
    Ptr<MNodeList> GenChildren() override;
    bool DisplayAtTLBLevel(const Set<String>& ifaces) override
    {
        assert(0);
        return true;
    }
    void DumpMetadata(MSmartWriter& writer);
};
