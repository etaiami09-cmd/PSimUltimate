#include "open_shell_url.hpp"

#if defined(_WIN32)
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#undef NOMINMAX
#undef WIN32_LEAN_AND_MEAN
#else
#include <cstdlib>
#include <string>
#endif

void openShellURL(const char* url) {
#if defined(_WIN32)
    ShellExecuteA(nullptr, "open", url, nullptr, nullptr, SW_SHOWNORMAL);
#elif defined(__APPLE__)
    std::string command = std::string("open ") + url;
    std::system(command.c_str());
#else
    std::string command = std::string("xdg-open ") + url;
    std::system(command.c_str());
#endif
}