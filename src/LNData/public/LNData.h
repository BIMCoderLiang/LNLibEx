/*
 * Owner:
 * 2025/07/08 - Yuqing Liang (BIMCoder Liang)
 * bim.frankliang@foxmail.com
 *
 * Use of this source code is governed by a LGPL-2.1 license that can be found in
 * the LICENSE file.
 */

#include "LNDataDefinitions.h"
#include "LNObject.h"
#include <string>
#include <vector>
#pragma once

namespace LNLibEx
{
	
	class LNData_EXPORT LNData
	{
	public:

		LNData();

		/// <summary>
		/// Export NurbsSurfaces to .stp/step file.
		/// </summary>
		static bool ToSTEPFile(const std::vector<LNLib::LN_NurbsSurface>& surfaces, const std::string& filePath);

		/// <summary>
		/// Export NurbsSurfaces to .iges file.
		/// </summary>
		static bool ToIGESFile(const std::vector<LNLib::LN_NurbsSurface>& surfaces, const std::string& filePath);
	};

}


