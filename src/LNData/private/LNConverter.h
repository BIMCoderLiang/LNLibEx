/*
 * Owner:
 * 2025/07/19 - Yuqing Liang (BIMCoder Liang)
 * bim.frankliang@foxmail.com
 *
 * Use of this source code is governed by a LGPL-2.1 license that can be found in
 * the LICENSE file.
 */

#include "LNObject.h"
#include <vector>

#include <Standard_Handle.hxx>
#include <Geom_BSplineSurface.hxx>
#include <TopoDS_Shape.hxx> 
#include <BRepTools.hxx> 
#include <BRep_Builder.hxx>
#pragma once

namespace LNLibEx
{
	class LNConverter
	{
	private:

		std::vector<LNLib::LN_NurbsSurface> _surfaces;

	private:

		void ConvertToOpenCascadeSurface(const LNLib::LN_NurbsSurface& surface, Handle(Geom_BSplineSurface)& internalSurface);
		bool ConvertToOpenCascadeSurfaces(const std::vector<LNLib::LN_NurbsSurface>& surfaces, std::vector<Handle(Geom_BSplineSurface)>& internalSurfaces);
		
		TopoDS_Shape MakeTopoShape(const std::vector<Handle(Geom_BSplineSurface)>& internalSurfaces);

	public:

		LNConverter(const std::vector<LNLib::LN_NurbsSurface>& surfaces);
		TopoDS_Shape Process();
	};
}


