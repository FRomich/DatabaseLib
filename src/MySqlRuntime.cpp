#include "MySqlRuntime.h"

#include <mutex>
#include <QtGlobal>

#ifdef Q_OS_WIN

#include <Windows.h>
#include <filesystem>
#include <iterator>
#include <QByteArray>
#include <QString>

namespace
{
    std::once_flag initFlag;
    DLL_DIRECTORY_COOKIE dllDirectoryCookie = nullptr;
    bool initialized = false;

    std::filesystem::path getLibraryDirectory()
    {
        HMODULE module = nullptr;

        if (!GetModuleHandleExW(
            GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
            GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            reinterpret_cast<LPCWSTR>(
                reinterpret_cast<ULONG_PTR>(
                    &ensureMySqlRuntimeInitialized)),
            &module))
        {
            return {};
        }

        wchar_t path[MAX_PATH]{};

        const DWORD length = GetModuleFileNameW(
            module, path, static_cast<DWORD>(std::size(path)));

        if (length == 0 || length >= std::size(path))
            return {};

        return std::filesystem::path(path).parent_path();
    }
}

bool ensureMySqlRuntimeInitialized()
{
    std::call_once(initFlag, [] {
        const auto libraryDirectory = getLibraryDirectory();

        if (libraryDirectory.empty())
            return;

        const auto dllDirectory =
            libraryDirectory / L"thirdparty" / L"mysql";

        if (!std::filesystem::exists(dllDirectory / L"libmysql.dll"))
            return;

        if (!SetDefaultDllDirectories(LOAD_LIBRARY_SEARCH_DEFAULT_DIRS))
            return;

        dllDirectoryCookie = AddDllDirectory(dllDirectory.c_str());

        if (!dllDirectoryCookie)
            return;

        const auto pluginDir = dllDirectory;

        qputenv(
            "LIBMYSQL_PLUGIN_DIR",
            QString::fromStdWString(pluginDir.wstring()).toLocal8Bit()
        );

        initialized = true;
        });

    return initialized;
}

#else

bool ensureMySqlRuntimeInitialized()
{
    return true;
}

#endif