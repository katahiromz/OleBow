#pragma once

#include "MNode.hpp"
#include "MDispInterface.hpp"

class MIDispatchProperties : public MNode
{
public:
    MIDispatchProperties(MDispInterface* di)
    {
        m_di = di;
    }
    String Name() override
    {
        return L"Properties";
    }
    String ShortName() override
    {
        return L"";
    }
    String ObjectName() override
    {
        return L"";
    }
    String Class() override
    {
        return L"MIDispatchProperties";
    }
    void GetAttrs(StringList& attrs) override
    {
    }
    Ptr<MNodeList> GenChildren() override
    {
        return m_di->PropertyChildren();
    }
    void Dump(MSmartWriter& writer) override
    {
        writer.write_line(L"properties:");
        writer.indent();
        auto children = Children();
        for (auto& child : *children)
        {
            child->Dump(writer);
        }
        writer.unindent();
    }
    bool DisplayAtTLBLevel(const Set<String>& ifaces) override
    {
        return false;
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
    MDispInterface* m_di;
};
