#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <windows.h>
#include <Wininet.h>
#include <inttypes.h>

#pragma comment(lib,"wininet.lib")

std::vector<uintptr_t> GetOffsets(const std::string& pastebinUrl)
{
    HINTERNET hInternet = InternetOpenA("WinInet", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet == NULL)
    {
        std::cerr << "[!] Failed to initialize WinInet library" << std::endl;
        exit(EXIT_FAILURE);
    }

    HINTERNET hUrl = InternetOpenUrlA(hInternet, pastebinUrl.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (hUrl == NULL)
    {
        std::cerr << "[!] Failed to open handle to pastebin URL" << std::endl;
        InternetCloseHandle(hInternet);
        exit(EXIT_FAILURE);
    }

    std::stringstream output;
    char buffer[1024];
    DWORD bytesRead;
    while (InternetReadFile(hUrl, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0)
    {
        buffer[bytesRead] = '\0';
        output << buffer;
    }

    InternetCloseHandle(hUrl);
    InternetCloseHandle(hInternet);

    std::string pastebinContent = output.str();

    std::vector<uintptr_t> lines;
    std::string line;
    std::istringstream stream(pastebinContent);
    while (std::getline(stream, line))
    {
        uintptr_t value = strtoul(line.c_str(), NULL, 16);
        lines.push_back(value);
    }

    return lines;
}

int main()
{
    std::vector<uintptr_t> lines = GetOffsets("https://pastebin.com/raw/BVrKkCWb");
    size_t numLines = lines.size();

    std::vector<std::string> offsets;
    offsets.reserve(numLines);

    for (size_t i = 0; i < numLines; i++)
    {
        char hex_string[16];
        sprintf_s(hex_string, sizeof(hex_string), "%" PRIxPTR, lines[i]);
        offsets.push_back(hex_string);
    }

    char* endptr;

    DWORD64 EntityListOffset;
    sscanf_s(offsets[0].c_str(), "%llX", &EntityListOffset);
    DWORD64 LocalEntOffset;
    sscanf_s(offsets[1].c_str(), "%llX", &LocalEntOffset);
    DWORD64 ViewAnglesOffset;
    sscanf_s(offsets[2].c_str(), "%llX", &ViewAnglesOffset);
    DWORD64 AimPunchOffset;
    sscanf_s(offsets[3].c_str(), "%llX", &AimPunchOffset);
    DWORD64 NameOffset;
    sscanf_s(offsets[4].c_str(), "%llX", &NameOffset);
    DWORD64 GlowTypeOffset;
    sscanf_s(offsets[5].c_str(), "%llX", &GlowTypeOffset);
    printf("These are not real offsets\n");
    printf("EntityListOffset: 0x%llX\n", EntityListOffset);
    printf("LocalEntOffset: 0x%llX\n", LocalEntOffset);
    printf("ViewAnglesOffset: 0x%llX\n", ViewAnglesOffset);
    printf("AimPunchOffset: 0x%llX\n", AimPunchOffset);
    printf("NameOffset: 0x%llX\n", NameOffset);
    printf("GlowTypeOffset: 0x%llX\n", GlowTypeOffset);
    getchar();
}
