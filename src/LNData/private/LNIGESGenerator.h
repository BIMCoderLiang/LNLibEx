/*
 * Owner:
 * 2025/07/19 - Yuqing Liang (BIMCoder Liang)
 * bim.frankliang@foxmail.com
 *
 * Use of this source code is governed by a LGPL-2.1 license that can be found in
 * the LICENSE file.
 */

#include "LNObject.h"
#include <string>
#include <fstream>
#include <vector>

#include "Standard_Handle.hxx"
#include "Geom_BSplineSurface.hxx"
#pragma once

namespace LNLibEx
{
	class LNIGESGenerator
	{
	private:

		std::vector<LNLib::LN_NurbsSurface> _surfaces;
		std::string _filePath;

	public:

		LNIGESGenerator(const std::vector<LNLib::LN_NurbsSurface>& surfaces, const std::string& filePath);
		bool Process();
	};
}


