#include "MNode.hpp"
#include "MTypeDesc.hpp"

class MRecordMember : public MNode
{
public:
    MRecordMember(MNode* parent, MComPtr<ITypeInfo> ti, Ptr<MVarDesc> vd)
    {
        m_parent = parent;
        m_name = MTypeInfoExtra::GetDocumentationById(ti, (*vd)->memid);
        m_vd = vd;
        m_ti = ti;
        m_typed_name = reinterpret_cast<MTypeDesc&>(
            (*m_vd)->elemdescVar.tdesc).GetTypedName(m_ti, m_name, parent->ShortName());
    }
    String Name() override
    {
        return m_typed_name;
    }
    String ShortName() override
    {
        return m_name;
    }
    String Class() override
    {
        return L"MRecordMember";
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
    Ptr<StringSet> GenDepending() override
    {
        auto ret = MakePtr<StringSet>();
        reinterpret_cast<MTypeDesc&>((*m_vd)->elemdescVar.tdesc).GenDepending(m_ti, *ret);
        return ret;
    }
    Ptr<StringSet> GenDepending2() override
    {
        auto ret = MakePtr<StringSet>();
        reinterpret_cast<MTypeDesc&>((*m_vd)->elemdescVar.tdesc).GenDepending2(m_ti, *ret);
        return ret;
    }
protected:
    String m_name;
    String m_typed_name;
    MComPtr<ITypeInfo> m_ti;
    Ptr<MVarDesc> m_vd;
};
