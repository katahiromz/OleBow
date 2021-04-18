#include "MInterface.hpp"

void MInterface::Dump(MSmartWriter& writer)
{
    StringList attrs;
    GetAttrs(attrs);
    writer.write_attrs(attrs);

    if (m_base.size())
        writer.write_line(Name() + L" : " + m_base + L" {");
    else
        writer.write_line(L"interface " + Name() + L" {");

    writer.indent();
    for (auto& child : *Children())
    {
        child->Dump(writer);
    }
    writer.unindent();

    writer.write_line(L"};");
}
