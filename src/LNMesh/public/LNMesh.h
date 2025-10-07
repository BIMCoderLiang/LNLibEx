/*
 * Owner:
 * 2025/07/08 - Yuqing Liang (BIMCoder Liang)
 * bim.frankliang@foxmail.com
 *
 * Use of this source code is governed by a LGPL-2.1 license that can be found in
 * the LICENSE file.
 * 
 */

#include "LNMeshDefinitions.h"
#include "LNObject.h"
#include <string>
#include <vector>
#pragma once

namespace LNLibEx
{
	class LNMesh_EXPORT LNMesh
	{
	public:

		/// <summary>
		/// Load .obj file to generate Mesh.
		/// </summary>
		/// <remarks>
		/// Not Support Material Information.
		/// </remarks>
		static bool FromOBJFile(const std::string& filePath, LNLib::LN_Mesh& mesh);

		/// <summary>
		/// Load ASCII or Binary .stl file to generate Mesh.
		/// </summary>
		static bool FromSTLFile(const std::string& filePath, LNLib::LN_Mesh& mesh);
	};
}


