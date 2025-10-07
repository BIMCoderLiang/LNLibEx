/*
 * Owner:
 * 2025/07/14 - Yuqing Liang (BIMCoder Liang)
 * bim.frankliang@foxmail.com
 *
 * Use of this source code is governed by a LGPL-2.1 license that can be found in
 * the LICENSE file.
 * 
 */

#include "LNData.h"
#include "LNObject.h"
#include "XYZ.h"
#include "UV.h"

#include "LNSTEPGenerator.h"
#include "LNIGESGenerator.h"

#include <windows.h>
#include <filesystem>
#include <iostream>
#include <vector>
#include <string>

namespace
{
    static LNLibEx::LNData sData;

    std::string GetCurrentDllDirectory() {
        HMODULE hModule = NULL;
        GetModuleHandleEx(
            GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
            (LPCTSTR)GetCurrentDllDirectory,
            &hModule);

        char path[MAX_PATH];
        GetModuleFileName(hModule, path, MAX_PATH);
        return std::filesystem::path(path).parent_path().string();
    }

    std::vector<std::string> GetDllNames()
    {
        return {

            "avutil-55.dll",
            "swscale-4.dll",
            "avcodec-57.dll",
            "avformat-57.dll",
            "freetype.dll",
            "FreeImage.dll",
            "openvr_api.dll",
            "jemalloc.dll",
            "TKernel.dll",
            "TKMath.dll",
            "TKG2d.dll",
            "TKG3d.dll",
            "TKGeomBase.dll",
            "TKBRep.dll",
            "TKGeomAlgo.dll",
            "TKTopAlgo.dll",
            "TKPrim.dll",
            "TKShHealing.dll",
            "TKBO.dll",
            "TKService.dll",
            "TKMesh.dll",
            "TKHLR.dll",
            "TKV3d.dll",
            "TKCDF.dll",
            "TKBool.dll",
            "TKLCAF.dll",
            "TKCAF.dll",
            "TKVCAF.dll",
            "TKXCAF.dll",
            "TKDE.dll",
            "TKXSBase.dll",
            "TKDESTEP.dll",
            "TKDEIGES.dll",
        };
    }
}

LNLibEx::LNData::LNData()
{
    std::string occPath = GetCurrentDllDirectory() + "\\OCCT";
    std::vector<std::string> dllNames = GetDllNames();
    for (int i = 0; i < dllNames.size(); i++)
    {
        std::string dllPath = occPath + "\\" + dllNames[i];
        HMODULE hModule = LoadLibraryA(dllPath.c_str());
        if (!hModule) {
            DWORD error = GetLastError();
            LPVOID errMsg;
            FormatMessage(
                FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                NULL,
                error,
                0,
                (LPTSTR)&errMsg,
                0,
                NULL
            );
        }
    }
}

bool LNLibEx::LNData::ToSTEPFile(const std::vector<LNLib::LN_NurbsSurface>& surfaces, const std::string& filePath)
{
    LNSTEPGenerator generator(surfaces, filePath);
    return generator.Process();
}

bool LNLibEx::LNData::ToIGESFile(const std::vector<LNLib::LN_NurbsSurface>& surfaces, const std::string& filePath)
{
    LNIGESGenerator generator(surfaces, filePath);
    return generator.Process();
}



