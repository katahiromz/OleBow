#pragma once

#include "MWriter.hpp"

template <typename T_STR_CONTAINER>
static inline typename T_STR_CONTAINER::value_type
mstr_join(const T_STR_CONTAINER& container,
          const typename T_STR_CONTAINER::value_type& sep)
{
    typename T_STR_CONTAINER::value_type result;
    typename T_STR_CONTAINER::const_iterator it, end;
    it = container.begin();
    end = container.end();
    if (it != end)
    {
        result = *it;
        for (++it; it != end; ++it)
        {
            result += sep;
            result += *it;
        }
    }
    return result;
}

class MSmartWriter
{
public:
    typedef std::wstring string_t;
    typedef std::vector<string_t> string_list_t;

    MSmartWriter(MWriter& writer, size_t width = 78, size_t depth = 0)
        : m_writer(writer)
        , m_width(width)
        , m_depth(depth)
        , m_block_first_line(false)
    {
    }

    // depth of indentation
    size_t depth() const
    {
        return m_depth;
    }
    size_t depth(size_t depth)
    {
        size_t old_depth = m_depth;
        m_depth = depth;
        return old_depth;
    }

    // max line width
    size_t width() const
    {
        return m_width;
    }
    size_t width(size_t width)
    {
        size_t old_width = m_width;
        m_width = width;
        return old_width;
    }

    void write_line(const string_t& text)
    {
        m_writer.Write(string_t(m_depth, L' '));
        m_writer.Write(text);
        m_writer.Write(L"\n");
        m_block_first_line = false;
    }

    void write_lines(const string_list_t& lines, bool add_comma = false)
    {
        for (size_t i = 0; i < lines.size(); ++i)
        {
            auto& line = lines[i];
            if (i + 1 == lines.size())
                write_line(line);
            else
                write_line(add_comma ? line + L"," : line);
        }
    }

    void indent(size_t width = 4)
    {
        m_depth += width;
        m_block_first_line = true;
    }

    void unindent(size_t width = 4)
    {
        if (m_depth > width)
            m_depth -= width;
        else
            m_depth = 0;
        m_block_first_line = false;
    }

    void write_empty_line()
    {
        m_writer.Write(L"\n");
        m_block_first_line = false;
    }

    bool block_first_line() const
    {
        return m_block_first_line;
    }

    void write_attrs(const string_list_t& attrs)
    {
        if (attrs.empty())
            return;

        std::wstring line = L"[";
        line += mstr_join(attrs, L", ") + L"]";

        if (depth() + line.size() > m_width)
        {
            write_line(L"[");
            indent(2);
            write_lines(attrs, true);
            unindent(2);
            write_line(L"]");
        }
        else
        {
            write_line(line);
        }
    }

    void write_function(const string_t& head, const string_list_t& params, const string_t& tail)
    {
        size_t hanging_depth;
        string_t first_param = params.size() ? params[0] : L"";
        if (m_depth + head.size() + first_param.size() >= m_width)
        {
            write_line(head);
            hanging_depth = 4;
            m_writer.Write(string_t(m_depth + hanging_depth, L' '));
        }
        else
        {
            m_writer.Write(string_t(m_depth, L' '));
            m_writer.Write(head);
            hanging_depth = head.size();
        }
        write_params(params, hanging_depth);
        m_writer.Write(tail + L"\n");
    }

protected:
    MWriter& m_writer;
    size_t m_width;
    size_t m_depth;
    bool m_block_first_line;

    void write_params(const string_list_t& params, size_t hanging_depth)
    {
        size_t ix = hanging_depth, iparam = 0;
        string_t comma = L", ";
        for (auto& param : params)
        {
            bool is_last = (iparam == params.size() - 1);
            if (is_last)
                comma.clear();

            if (ix != hanging_depth &&
                m_depth + ix + param.size() + comma.size() >= m_width)
            {
                m_writer.Write(L"\n");
                ix = hanging_depth;
                m_writer.Write(string_t(m_depth + ix, L' '));
            }
            m_writer.Write(param + comma);
            ix += param.size() + comma.size();

            ++iparam;
        }
        m_block_first_line = false;
    }
};
