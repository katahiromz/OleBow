#include "OleBow.hpp"
#include "MFileWriter.hpp"

static void show_version(void)
{
    std::puts("OleBow ver.0.7 by katahiromz");
    std::puts("See: https://github.com/katahiromz/OleBow");
}

static void show_help(void)
{
    std::puts("Usage: olebow [options] your_file.tlb [output.idl]");
    std::puts("Options:");
    std::puts("  --help          Show this message");
    std::puts("  --version       Show version information");
    std::puts("  --codepage XXX  Set output codepage");
}

INT JustDoIt(const wchar_t *input_file, const wchar_t *output_file, int codepage)
{
    HRESULT hr = ::OleInitialize(NULL);

    bool ok = false;
    try
    {
        MFileWriter writer(output_file, codepage);
        DumpTypeLib(writer, input_file);
        ok = true;
    }
    catch(...)
    {
        std::fprintf(stderr, "ERROR: Something is wrong.\n");
    }

    if (SUCCEEDED(hr))
        ::OleUninitialize();

    return (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}

int wmain(int argc, wchar_t **wargv)
{
    if (argc <= 1)
    {
        show_help();
        return EXIT_SUCCESS;
    }

    int codepage = 0;
    String input_file, output_file;
    for (int i = 1; i < argc; ++i)
    {
        String arg = wargv[i];
        if (arg == L"--help")
        {
            show_help();
            return EXIT_SUCCESS;
        }
        if (arg == L"--version")
        {
            show_version();
            return EXIT_SUCCESS;
        }
        if (arg == L"--codepage")
        {
            ++i;
            codepage = _wtoi(wargv[i]);
            continue;
        }
        if (arg[0] == L'-')
        {
            std::fprintf(stderr, "ERROR: Invalid argument\n");
            return EXIT_FAILURE;
        }
        if (input_file.empty())
        {
            input_file = arg;
        }
        else if (output_file.empty())
        {
            output_file = arg;
        }
        else
        {
            std::fprintf(stderr, "ERROR: Too many arguments\n");
            return EXIT_FAILURE;
        }
    }

    if (output_file.empty())
    {
        output_file = input_file + L".idl";
    }

    return JustDoIt(input_file.c_str(), output_file.c_str(), codepage);
}

int main(int argc, char **argv)
{
    INT my_argc;
    LPWSTR *my_wargv = ::CommandLineToArgvW(GetCommandLineW(), &my_argc);
    INT ret = wmain(my_argc, my_wargv);
    LocalFree(my_wargv);
    return ret;
}
