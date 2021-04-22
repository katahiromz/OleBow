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
    String Class() override
    {
        return L"MIDispatchMethods";
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
        writer.indent();
        auto children = Children();
        for (auto& child : *children)
        {
            std::static_pointer_cast<MMethod>(child)->Dump(writer, true);
        }
        writer.unindent();
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
protected:
    MDispInterface* m_di;
};
