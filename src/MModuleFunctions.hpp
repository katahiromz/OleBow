#pragma once

#include "MNode.hpp"

class MModuleFunctions : public MNode
{
public:
    MModuleFunctions(MModule* module)
    {
        m_module = module;
    }
    String Name() override
    {
        return L"Functions";
    }
    String ShortName() override
    {
        return Name();
    }
    String Class() override
    {
        return L"MModuleFunctions";
    }
    void GetAttrs(StringList& attrs) override
    {
    }
    Ptr<MNodeList> GenChildren() override
    {
        return m_module->GenFuncChildren();
    }
    void Dump(MSmartWriter& writer) override
    {
        auto children = Children();
        for (auto& child : *children)
        {
            child->Dump(writer);
        }
    }
    bool DisplayAtTLBLevel(const Set<String>& ifaces) override
    {
        return true;
    }
    Ptr<StringSet> GenDepending() override
    {
        auto ret = MakePtr<StringSet>();
        auto children = Children();
        for (auto& child : *children)
        {
            auto ret2 = child->Depending();
            ret->insert(ret2->begin(), ret2->end());
        }
        return ret;
    }
    Ptr<StringSet> GenProviding() override
    {
        return MakePtr<StringSet>();
    }
protected:
    MModule* m_module;
};
