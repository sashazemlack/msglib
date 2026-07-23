#include "msglib.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace {

void PrintUsage(const char* argv0)
{
    std::fprintf(stderr,
        "Usage: %s <icon> <message> <title>\n"
        "\n"
        "Arguments match MsgBox(icon, message, title):\n"
        "  icon     0=none, 1=info, 2=warning, 3=error\n"
        "  message  UTF-8 message text\n"
        "  title    UTF-8 window title\n"
        "\n"
        "Example:\n"
        "  %s 2 \"Disk space is low\" \"Warning\"\n",
        argv0,
        argv0);
}

bool ParseIcon(const char* text, int* icon)
{
    if (text == nullptr || text[0] == '\0') {
        return false;
    }

    char* end = nullptr;
    const long value = std::strtol(text, &end, 10);
    if (end == text || *end != '\0' || value < 0 || value > 3) {
        return false;
    }

    *icon = static_cast<int>(value);
    return true;
}

} // namespace

int main(int argc, char* argv[])
{
    if (argc == 2 && (std::strcmp(argv[1], "-h") == 0 || std::strcmp(argv[1], "--help") == 0)) {
        PrintUsage(argv[0]);
        return 0;
    }

    if (argc != 4) {
        std::fprintf(stderr, "Error: expected 3 arguments (icon, message, title)\n");
        PrintUsage(argv[0]);
        return 2;
    }

    int icon = 0;
    if (!ParseIcon(argv[1], &icon)) {
        std::fprintf(stderr, "Error: icon must be an integer 0-3\n");
        PrintUsage(argv[0]);
        return 2;
    }

    const char* message = argv[2];
    const char* title = argv[3];

    const int result = MsgBox(icon, message, title);
    std::printf("MsgBox returned %d\n", result);
    return result == 1 ? 0 : 1;
}
