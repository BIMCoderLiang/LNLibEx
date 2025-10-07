/*
 * Owner:
 * 2025/07/18 - Yuqing Liang (BIMCoder Liang)
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
	class LNSTEPGenerator
	{
	private:

		std::vector<LNLib::LN_NurbsSurface> _surfaces;
		std::string _filePath;

	public:

		LNSTEPGenerator(const std::vector<LNLib::LN_NurbsSurface>& surfaces, const std::string& filePath);
		bool Process();
	};
}


