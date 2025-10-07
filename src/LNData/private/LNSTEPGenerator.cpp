/*
 * Owner:
 * 2025/07/18 - Yuqing Liang (BIMCoder Liang)
 * bim.frankliang@foxmail.com
 *
 * Use of this source code is governed by a LGPL-2.1 license that can be found in
 * the LICENSE file.
 *
 */

#include "LNSTEPGenerator.h"
#include "LNObject.h"
#include "XYZ.h"
#include "XYZW.h"
#include "NurbsSurface.h"
#include "LNConverter.h"

#include <STEPControl_Writer.hxx>

LNLibEx::LNSTEPGenerator::LNSTEPGenerator(const std::vector<LNLib::LN_NurbsSurface>& surfaces, 
                                            const std::string& filePath):
                                                _surfaces(surfaces),_filePath(filePath){}

bool LNLibEx::LNSTEPGenerator::Process()
{
    if (_surfaces.empty()) return false;

    LNConverter converter(_surfaces);
    TopoDS_Shape source = std::move(converter.Process());

    STEPControl_Writer writer;
    IFSelect_ReturnStatus transferStatus = writer.Transfer(source, STEPControl_ShellBasedSurfaceModel);
    if (transferStatus != IFSelect_ReturnStatus::IFSelect_RetDone) return false;
    IFSelect_ReturnStatus writeStatus = writer.Write(_filePath.c_str());
    if (writeStatus != IFSelect_ReturnStatus::IFSelect_RetDone) return false;
    return true;
}
