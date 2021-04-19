#pragma once

#include "MWriter.hpp"
#include <cstdio>

struct MFileWriter : MWriter
{
    FILE *m_fp;
    int m_codepage = 0;

    MFileWriter(const wchar_t *file, int codepage = 0)
    {
        _wfopen_s(&m_fp, file, L"w");
        m_codepage = codepage;
    }
    MFileWriter(FILE *fp, int codepage = 0)
    {
        m_fp = fp;
        m_codepage = codepage;
    }
    ~MFileWriter()
    {
        if (m_fp != stdout && m_fp != stderr)
            std::fclose(m_fp);
    }
    void Write(const wchar_t *psz) override
    {
        if (m_codepage)
        {
            char sz[1024];
            ::WideCharToMultiByte(m_codepage, 0, psz, -1, sz, _countof(sz), NULL, NULL);
            std::fputs(sz, m_fp);
        }
        else
        {
            std::fputws(psz, m_fp);
        }
    }
};
