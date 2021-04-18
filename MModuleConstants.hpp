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
protected:
    MModule* m_module;
};
