#pragma once

#include "OleBow.hpp"

struct MWriter
{
    virtual void Write(const wchar_t *psz) = 0;

    void Write(const String& str)
    {
        Write(str.c_str());
    }
};
