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
    void Sort();
    void Dump(MSmartWriter& writer) override;
    void DumpDependency(MWriter& writer);
    Ptr<StringSet> GenDepending() override
    {
        assert(0);
        return NULL;
    }
    Ptr<StringSet> GenDepending2() override
    {
        assert(0);
        return NULL;
    }
protected:
    TLIBATTR *m_pAttr;
    String m_name;
    MComPtr<ITypeLib> m_tlib;

    String Name() override;
    String ShortName() override;
    String Class() override
    {
        return L"MTypeLib";
    }
    void GetAttrs(StringList& attrs) override;
    Ptr<MNodeList> GenChildren() override;
    void DumpMetadata(MSmartWriter& writer);
};
