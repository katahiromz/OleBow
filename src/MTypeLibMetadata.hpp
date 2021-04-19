#pragma once

struct MTypeLibMetadata
{
    Set<MComPtr<ITypeLib> > GetDependentLibraries(MComPtr<ITypeLib> pTypeLib)
    {
        Set<MComPtr<ITypeLib> > output;
        Set<MComPtr<ITypeInfo> > history;
        GetDependenciesHelper(pTypeLib, &history, &output);
        return output;
    }

    void GetDependenciesHelper(MComPtr<ITypeLib> pTypeLib, Set<MComPtr<ITypeInfo> >* pHistory, Set<MComPtr<ITypeLib> >* pOutput)
    {
        auto typeInfoCount = pTypeLib->GetTypeInfoCount();
        for (UINT index = 0; index < typeInfoCount; ++index)
        {
            MComPtr<ITypeInfo> pTypeInfo;
            pTypeLib->GetTypeInfo(index, &pTypeInfo);

            GetDependenciesHelper(pTypeLib, pTypeInfo, pHistory, pOutput);
        }
    }

    void GetDependenciesHelper(MComPtr<ITypeLib> pTypeLib, MComPtr<ITypeInfo> pTypeInfo,
        Set<MComPtr<ITypeInfo> >* pHistory, Set<MComPtr<ITypeLib> >* pOutput)
    {
        if (!pHistory->insert(pTypeInfo).second)
            return;

        TYPEATTR* typeAttributes;
        pTypeInfo->GetTypeAttr(&typeAttributes);
        try
        {
            if (typeAttributes->typekind == TKIND_ALIAS)
            {
                GetDependenciesHelper(pTypeLib, pTypeInfo, typeAttributes->tdescAlias, pHistory, pOutput);
            }
            else
            {
                auto implementedTypeCount = typeAttributes->cImplTypes;
                for (WORD implementedTypeIndex = 0; implementedTypeIndex < implementedTypeCount; ++implementedTypeIndex)
                {
                    HREFTYPE hRefType;
                    pTypeInfo->GetRefTypeOfImplType(implementedTypeIndex, &hRefType);

                    GetDependenciesHelper(pTypeLib, pTypeInfo, hRefType, pHistory, pOutput);
                }

                auto functionCount = typeAttributes->cFuncs;
                for (WORD functionIndex = 0; functionIndex < functionCount; ++functionIndex)
                {
                    FUNCDESC* functionDescription;
                    pTypeInfo->GetFuncDesc(functionIndex, &functionDescription);
                    try
                    {
                        GetDependenciesHelper(pTypeLib, pTypeInfo, *functionDescription, pHistory, pOutput);
                    }
                    catch (...)
                    {
                        pTypeInfo->ReleaseFuncDesc(functionDescription);
                        throw;
                    }

                    pTypeInfo->ReleaseFuncDesc(functionDescription);
                }

                auto variableCount = typeAttributes->cVars;
                for (WORD variableIndex = 0; variableIndex < variableCount; ++variableIndex)
                {
                    VARDESC* variableDescription;
                    pTypeInfo->GetVarDesc(variableIndex, &variableDescription);
                    try
                    {
                        GetDependenciesHelper(pTypeLib, pTypeInfo, *variableDescription, pHistory, pOutput);
                    }
                    catch (...)
                    {
                        pTypeInfo->ReleaseVarDesc(variableDescription);
                        throw;
                    }
                    pTypeInfo->ReleaseVarDesc(variableDescription);
                }
            }
        }
        catch (...)
        {
            pTypeInfo->ReleaseTypeAttr(typeAttributes);
            throw;
        }

        pTypeInfo->ReleaseTypeAttr(typeAttributes);
    }

    void GetDependenciesHelper(MComPtr<ITypeLib> pTypeLib, MComPtr<ITypeInfo> pTypeInfo,
        HREFTYPE hRefType, Set<MComPtr<ITypeInfo> >* pHistory, Set<MComPtr<ITypeLib> >* pOutput)
    {
        MComPtr<ITypeInfo> referencedTypeInfo;
        pTypeInfo->GetRefTypeInfo(hRefType, &referencedTypeInfo);

        MComPtr<ITypeLib> referencedTypeLibrary;
        UINT referencedTypeInfoIndex;
        referencedTypeInfo->GetContainingTypeLib(&referencedTypeLibrary, &referencedTypeInfoIndex);

        if (referencedTypeLibrary != pTypeLib)
            pOutput->insert(referencedTypeLibrary);
    }

    void GetDependenciesHelper(MComPtr<ITypeLib> pTypeLib, MComPtr<ITypeInfo> pTypeInfo, TYPEDESC& referencedTypeDescription,
        Set<MComPtr<ITypeInfo> >* pHistory, Set<MComPtr<ITypeLib> >* pOutput)
    {
        switch (referencedTypeDescription.vt)
        {
        case VT_PTR:
            GetDependenciesHelper(pTypeLib, pTypeInfo, *referencedTypeDescription.lptdesc, pHistory, pOutput);
            break;
        case VT_CARRAY:
            GetDependenciesHelper(pTypeLib, pTypeInfo, *referencedTypeDescription.lpadesc, pHistory, pOutput);
            break;
        case VT_USERDEFINED:
            GetDependenciesHelper(pTypeLib, pTypeInfo, referencedTypeDescription.hreftype, pHistory, pOutput);
            break;
        }
    }

    void GetDependenciesHelper(MComPtr<ITypeLib> pTypeLib, MComPtr<ITypeInfo> pTypeInfo, FUNCDESC& functionDescription,
        Set<MComPtr<ITypeInfo> >* pHistory, Set<MComPtr<ITypeLib> >* pOutput)
    {
        GetDependenciesHelper(pTypeLib, pTypeInfo, functionDescription.elemdescFunc, pHistory, pOutput);

        auto parameterCount = functionDescription.cParams;
        for (SHORT parameterIndex = 0; parameterIndex < parameterCount; ++parameterIndex)
        {
            auto& parameterDescription = functionDescription.lprgelemdescParam[parameterIndex];
            GetDependenciesHelper(pTypeLib, pTypeInfo, parameterDescription, pHistory, pOutput);
        }
    }

    void GetDependenciesHelper(MComPtr<ITypeLib> pTypeLib, MComPtr<ITypeInfo> pTypeInfo, VARDESC& variableDescription,
        Set<MComPtr<ITypeInfo> >* pHistory, Set<MComPtr<ITypeLib> >* pOutput)
    {
        GetDependenciesHelper(pTypeLib, pTypeInfo, variableDescription.elemdescVar, pHistory, pOutput);
    }

    void GetDependenciesHelper(MComPtr<ITypeLib> pTypeLib, MComPtr<ITypeInfo> pTypeInfo, ARRAYDESC& arrayDescription,
        Set<MComPtr<ITypeInfo> >* pHistory, Set<MComPtr<ITypeLib> >* pOutput)
    {
        GetDependenciesHelper(pTypeLib, pTypeInfo, arrayDescription.tdescElem, pHistory, pOutput);
    }

    void GetDependenciesHelper(MComPtr<ITypeLib> pTypeLib, MComPtr<ITypeInfo> pTypeInfo, ELEMDESC& elementDescription,
        Set<MComPtr<ITypeInfo> >* pHistory, Set<MComPtr<ITypeLib> >* pOutput)
    {
        GetDependenciesHelper(pTypeLib, pTypeInfo, elementDescription.tdesc, pHistory, pOutput);
    }
};
