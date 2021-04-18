#pragma once

#include "MWriter.hpp"
#include <cstdio>

struct MFileWriter : MWriter
{
    FILE *m_fp;

    MFileWriter(const wchar_t *file)
    {
        m_fp = _wfopen(file, L"w");
    }
    MFileWriter(FILE *fp)
    {
        m_fp = fp;
    }
    ~MFileWriter()
    {
        if (m_fp != stdout && m_fp != stderr)
            std::fclose(m_fp);
    }
    void Write(const wchar_t *psz) override
    {
        std::fputws(psz, m_fp);
    }
};
