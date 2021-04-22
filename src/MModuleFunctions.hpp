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
    Ptr<StringSet> GenDepending2() override
    {
        auto ret = MakePtr<StringSet>();
        auto children = Children();
        for (auto& child : *children)
        {
            auto ret2 = child->Depending2();
            ret->insert(ret2->begin(), ret2->end());
        }
        return ret;
    }
protected:
    MModule* m_module;
};
