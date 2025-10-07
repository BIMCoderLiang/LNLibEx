#include "T_Utils.h"
#include <windows.h>
#include <string>

std::string LNTest::GetProgramDir()
{
    char exeFullPath[MAX_PATH];
    std::string strPath = "";
    GetModuleFileName(NULL, exeFullPath, MAX_PATH);
    strPath = (std::string)exeFullPath;
    int pos = strPath.find_last_of('\\', strPath.length());
    std::string folder = strPath.substr(0, pos);
    return folder;
}

std::string LNTest::GetTestDir()
{
    return LNTest::GetProgramDir() + "\\TFILES\\";
}


