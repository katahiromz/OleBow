#pragma once

#include "OleBow.hpp"

struct MParamDesc : PARAMDESC
{
    _variant_t varDefaultValue()
    {
        return pparamdescex->varDefaultValue;
    }
};
