#pragma once

#include "MNode.hpp"
#include "MModule.hpp"

class MModuleConstants : public MNode
{
public:
    MModuleConstants(MModule* module)
    {
        m_module = module;
    }
    String Name() override
    {
        return L"Constants";
    }
    String ShortName() override
    {
        return Name();
    }
    String ObjectName() override
    {
        return L"";
    }
    String Class() override
    {
        return L"MModuleConstants";
    }
    void GetAttrs(StringList& attrs) override
    {
    }
    Ptr<MNodeList> GenChildren() override
    {
        return m_module->GenConstChildren();
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
