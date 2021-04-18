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
protected:
    MDispInterface* m_di;
};
