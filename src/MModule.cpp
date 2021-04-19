#include "MModule.hpp"
#include "MModuleConstants.hpp"
#include "MModuleFunctions.hpp"
#include "MVarDesc.hpp"
#include "MModuleConst.hpp"
#include "MMethod.hpp"

Ptr<MNodeList> MModule::GenChildren()
{
    auto ret = MakePtr<MNodeList>();
    if ((*m_ta)->cVars > 0)
    {
        ret->push_back(MakePtr<MModuleConstants>(this));
    }
    if ((*m_ta)->cFuncs > 0)
    {
        ret->push_back(MakePtr<MModuleFunctions>(this));
    }
    return ret;
}
Ptr<MNodeList> MModule::GenConstChildren()
{
    auto ret = MakePtr<MNodeList>();
    for (DWORD x = 0; x < (*m_ta)->cVars; ++x)
    {
        auto vd = MakePtr<MVarDesc>(m_ti, x);
        ret->push_back(MakePtr<MModuleConst>(this, m_ti, vd, x));
    }
    return ret;
}
Ptr<MNodeList> MModule::GenFuncChildren()
{
    auto ret = MakePtr<MNodeList>();
    for (INT i = 0; i < (*m_ta)->cFuncs; ++i)
    {
        auto fd = MakePtr<MFuncDesc>(m_ti, i);
        ret->push_back(MakePtr<MMethod>(this, m_ti, fd));
    }
    return ret;
}
