#include "OleBow.hpp"
#include "MFileWriter.hpp"

static void show_version(void)
{
#ifdef _WIN64
    std::puts("OleBow (64-bit) ver.0.0 by katahiromz");
#else
    std::puts("OleBow (32-bit) ver.0.0 by katahiromz");
#endif
    std::puts("See: https://github.com/katahiromz/OleBow");
}

static void show_help(void)
{
#ifdef _WIN64
    std::puts("Usage: OleBow your_file.tlb [output.idl]");
#else
    std::puts("Usage: OleBow your_file.tlb [output.idl]");
#endif
    std::puts("Options:");
    std::puts("  --help      Show this message");
    std::puts("  --version   Show version information");
}

INT JustDoIt(const wchar_t *input_file, const wchar_t *output_file)
{
    HRESULT hr = ::OleInitialize(NULL);

    bool ok = false;
    try
    {
        MFileWriter writer(output_file);
        DumpTypeLib(writer, input_file);
        ok = true;
    }
    catch(...)
    {
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
    if (argc > 3)
    {
        fprintf(stderr, "ERROR: Too many arguments\n");
        show_help();
        return EXIT_FAILURE;
    }

    String arg = wargv[1];
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
    if (arg[0] == L'-')
    {
        std::fprintf(stderr, "ERROR: Invalid argument\n");
        return EXIT_FAILURE;
    }

    String input_file = arg, output_file;
    if (argc == 3)
    {
        output_file = wargv[2];
    }
    else
    {
        output_file = input_file + L".idl";
    }

    return JustDoIt(input_file.c_str(), output_file.c_str());
}

int main(int argc, char **argv)
{
    INT my_argc;
    LPWSTR *my_wargv = ::CommandLineToArgvW(GetCommandLineW(), &my_argc);
    INT ret = wmain(my_argc, my_wargv);
    LocalFree(my_wargv);
    return ret;
}
