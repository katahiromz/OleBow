#pragma once

#include "MDispInterface.hpp"

class MIDispatchMethods : public MNode
{
public:
    MIDispatchMethods(MDispInterface* di)
    {
        m_di = di;
    }
    String Name() override
    {
        return L"Methods";
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
        return m_di->MethodChildren();
    }
    void Dump(MSmartWriter& writer) override
    {
        writer.write_line(L"methods:");
        auto children = Children();
        writer.indent();
        for (auto& child : *children)
        {
            std::static_pointer_cast<MMethod>(child)->Dump(writer, true);
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
