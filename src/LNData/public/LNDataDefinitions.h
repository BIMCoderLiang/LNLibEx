/*
 * Owner:
 * 2025/07/14 - Yuqing Liang (BIMCoder Liang)
 * bim.frankliang@foxmail.com
 *
 * Use of this source code is governed by a LGPL-2.1 license that can be found in
 * the LICENSE file.
 * 
 */

#pragma once

#define DLL_EXPORT __declspec(dllexport)
#define DLL_IMPORT __declspec(dllimport)

#if defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(__CYGWIN__)
    #ifdef LNData_HOME
        #define LNData_EXPORT DLL_EXPORT
    #else
        #define LNData_EXPORT DLL_IMPORT
    #endif
#else
    #define LNData_EXPORT
#endif
