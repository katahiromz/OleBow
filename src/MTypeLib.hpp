#pragma once

#include "MNode.hpp"

class MTypeLib : public MNode
{
public:
    MTypeLib();
    MTypeLib(const wchar_t *path);
    ~MTypeLib();

    void Load(const wchar_t *path);
    void Unload();
    void Dump(MSmartWriter& writer) override;

protected:
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
