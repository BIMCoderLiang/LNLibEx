/*
 * Owner:
 * 2025/07/19 - Yuqing Liang (BIMCoder Liang)
 * bim.frankliang@foxmail.com
 *
 * Use of this source code is governed by a LGPL-2.1 license that can be found in
 * the LICENSE file.
 *
 */

#include "LNConverter.h"
#include "LNObject.h"
#include "XYZ.h"
#include "XYZW.h"
#include "NurbsSurface.h"
#include "KnotVectorUtils.h"

#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Compound.hxx>
#include <BRep_Builder.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <Geom_BSplineSurface.hxx>

#include <map>

LNLibEx::LNConverter::LNConverter(const std::vector<LNLib::LN_NurbsSurface>& surfaces):_surfaces(surfaces){}

bool LNLibEx::LNConverter::ConvertToOpenCascadeSurfaces(const std::vector<LNLib::LN_NurbsSurface>& surfaces, std::vector<Handle(Geom_BSplineSurface)>& internalSurfaces)
{
    if (surfaces.empty()) {
        return false;
    }

    int size = surfaces.size();
    internalSurfaces.clear();
    internalSurfaces.reserve(size);

    for (int i = 0; i < surfaces.size(); i++)
    {
        LNLib::LN_NurbsSurface surface = surfaces[i];
        Handle(Geom_BSplineSurface) internalSurface;
        ConvertToOpenCascadeSurface(surface, internalSurface);

        internalSurfaces.emplace_back(internalSurface);
    }
    return true;
}

void LNLibEx::LNConverter::ConvertToOpenCascadeSurface(const LNLib::LN_NurbsSurface& surface, Handle(Geom_BSplineSurface)& internalSurface)
{
    LNLib::NurbsSurface::Check(surface);

    std::vector<double> knotU = surface.KnotVectorU;
    std::vector<double> knotV = surface.KnotVectorV;

    const int numPolesU = static_cast<int>(surface.ControlPoints.size());
    const int numPolesV = static_cast<int>(surface.ControlPoints[0].size());

    TColgp_Array2OfPnt poles(1, numPolesU, 1, numPolesV);
    TColStd_Array2OfReal weights(1, numPolesU, 1, numPolesV);

    for (int i = 0; i < numPolesU; i++) {
        for (int j = 0; j < numPolesV; j++) {
            const LNLib::XYZW& wcp = surface.ControlPoints[i][j];
            const LNLib::XYZ& cp = wcp.ToXYZ(true);
            poles.SetValue(i+1, j+1, gp_Pnt(cp.GetX(), cp.GetY(), cp.GetZ()));
            weights.SetValue(i+1, j+1, wcp.GetW());
        }
    }

    std::map<double, int> mapU = LNLib::KnotVectorUtils::GetKnotMultiplicityMap(knotU);
    TColStd_Array1OfReal knotsU(1, static_cast<int>(mapU.size()));
    TColStd_Array1OfInteger multsU(1, static_cast<int>(mapU.size()));
    
    std::vector<double> Ukeys;
    Ukeys.reserve(mapU.size());
    std::vector<int> UMults;
    UMults.reserve(mapU.size());
    for (auto it = mapU.begin(); it != mapU.end(); ++it) {
        Ukeys.emplace_back(it->first);
        UMults.emplace_back(it->second);
    }
    for (int i = 0; i < Ukeys.size(); i++) {
        knotsU.SetValue(i+1, Ukeys[i]);
    }
    for (int i = 0; i < UMults.size(); i++) {
        multsU.SetValue(i+1, UMults[i]);
    }

    std::map<double, int> mapV = LNLib::KnotVectorUtils::GetKnotMultiplicityMap(knotV);
    TColStd_Array1OfReal knotsV(1, static_cast<int>(mapV.size()));
    TColStd_Array1OfInteger multsV(1, static_cast<int>(mapV.size()));

    std::vector<double> Vkeys;
    Vkeys.reserve(mapV.size());
    std::vector<int> VMults;
    VMults.reserve(mapV.size());
    for (auto it = mapV.begin(); it != mapV.end(); ++it) {
        Vkeys.emplace_back(it->first);
        VMults.emplace_back(it->second);
    }
    for (int i = 0; i < Vkeys.size(); i++) {
        knotsV.SetValue(i+1, Vkeys[i]);
    }
    for (int i = 0; i < VMults.size(); i++) {
        multsV.SetValue(i+1, VMults[i]);
    }

    internalSurface = new Geom_BSplineSurface(
        poles, weights, knotsU, knotsV,
        multsU, multsV,
        surface.DegreeU, surface.DegreeV);
}

TopoDS_Shape LNLibEx::LNConverter::MakeTopoShape(const std::vector<Handle(Geom_BSplineSurface)>& internalSurfaces)
{
    BRep_Builder builder;
    TopoDS_Compound result;
    builder.MakeCompound(result);

    for (const auto& surface : internalSurfaces)
    {
        if (surface.IsNull())continue; 
        TopoDS_Face face = BRepBuilderAPI_MakeFace(surface, Precision::Confusion());
        if (!face.IsNull()) {
            builder.Add(result, face);
        }
    }
    return result;
}

TopoDS_Shape LNLibEx::LNConverter::Process()
{
    std::vector<Handle(Geom_BSplineSurface)> internalSurfaces;
    ConvertToOpenCascadeSurfaces(_surfaces, internalSurfaces);
    return MakeTopoShape(internalSurfaces);
}
