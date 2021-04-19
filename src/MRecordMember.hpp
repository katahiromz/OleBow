#include "MNode.hpp"
#include "MTypeDesc.hpp"

class MRecordMember : public MNode
{
public:
    MRecordMember(MNode* parent, MComPtr<ITypeInfo> ti, Ptr<MVarDesc> vd)
    {
        m_parent = parent;
        m_name = MTypeInfoExtra::GetDocumentationById(ti, (*vd)->memid);
        m_type = reinterpret_cast<MTypeDesc&>((*vd)->elemdescVar.tdesc).ComTypeNameAsString(ti);
    }
    String Name() override
    {
        return m_type + L" " + m_name;
    }
    String ShortName() override
    {
        return m_name;
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
        return MakePtr<MNodeList>();
    }
    void Dump(MSmartWriter& writer) override
    {
        writer.write_line(Name() + L";");
    }
    bool DisplayAtTLBLevel(const Set<String>& ifaces) override
    {
        return false;
    }
protected:
    String m_type;
    String m_name;
};
