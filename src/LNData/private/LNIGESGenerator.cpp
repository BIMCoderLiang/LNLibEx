/*
 * Owner:
 * 2025/07/19 - Yuqing Liang (BIMCoder Liang)
 * bim.frankliang@foxmail.com
 *
 * Use of this source code is governed by a LGPL-2.1 license that can be found in
 * the LICENSE file.
 *
 */

#include "LNIGESGenerator.h"
#include "LNObject.h"
#include "XYZ.h"
#include "XYZW.h"
#include "NurbsSurface.h"
#include "LNConverter.h"

#include <Interface_Static.hxx>
#include <IGESControl_Controller.hxx> 
#include <IGESControl_Writer.hxx> 

LNLibEx::LNIGESGenerator::LNIGESGenerator(const std::vector<LNLib::LN_NurbsSurface>& surfaces,
                                            const std::string& filePath):
                                                _surfaces(surfaces),_filePath(filePath){}

bool LNLibEx::LNIGESGenerator::Process()
{
    if (_surfaces.empty()) return false;

    LNConverter converter(_surfaces);
    TopoDS_Shape source = std::move(converter.Process());

    Interface_Static::SetCVal("write.iges.unit", "MM");
    Interface_Static::SetRVal("write.iges.unit.scale", 1.0);

    IGESControl_Controller::Init();
    IGESControl_Writer ICW(Interface_Static::CVal("write.iges.unit"), 1);
    ICW.AddShape(source);
    ICW.ComputeModel();
    return ICW.Write(_filePath.c_str());
}
