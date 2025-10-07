#include "gtest/gtest.h"
#include "T_Utils.h"
#include "LNData.h"
#include "LNObject.h"
#include <string>

TEST(Test_LNData, ExportSTEP)
{
    std::vector<LNLib::LN_NurbsSurface> surfaces;

    LNLib::LN_NurbsSurface surface1;
    surface1.DegreeU = 2;
    surface1.DegreeV = 2;
    surface1.KnotVectorU = { 0, 0, 0, 1, 1, 1 };
    surface1.KnotVectorV = { 0, 0, 0, 1, 1, 1 };
    surface1.ControlPoints = {
        {{0, 0, 0, 1}, {1, 0, 0, 1}, {2, 0, 0, 1}},
        {{0, 1, 0, 1}, {1, 1, 0, 1}, {2, 1, 0, 1}},
        {{0, 2, 0, 1}, {1, 2, 0, 1}, {2, 2, 0, 1}}
    };
    surfaces.push_back(surface1);

    LNLib::LN_NurbsSurface surface2;
    surface2.DegreeU = 3;
    surface2.DegreeV = 3;
    surface2.KnotVectorU = { 0, 0, 0, 0, 1, 1, 1, 1 };
    surface2.KnotVectorV = { 0, 0, 0, 0, 1, 1, 1, 1 };
    surface2.ControlPoints = {
        {{3, 0, 0, 1}, {4, 0, 0, 1}, {5, 0, 0, 1}, {6, 0, 0, 1}},
        {{3, 1, 0, 1}, {4, 1, 0, 2}, {5, 1, 0, 1}, {6, 1, 0, 1}},
        {{3, 2, 0, 1}, {4, 2, 0, 1}, {5, 2, 0, 2}, {6, 2, 0, 1}},
        {{3, 3, 0, 1}, {4, 3, 0, 1}, {5, 3, 0, 1}, {6, 3, 0, 1}}
    };
    surfaces.push_back(surface2);
    std::string exportPath = LNTest::GetProgramDir() + "\\STEPTest.stp";
    EXPECT_TRUE(LNLibEx::LNData::ToSTEPFile(surfaces, exportPath));
}

TEST(Test_LNData, ExportIGES)
{
    std::vector<LNLib::LN_NurbsSurface> surfaces;

    LNLib::LN_NurbsSurface surface1;
    surface1.DegreeU = 2;
    surface1.DegreeV = 2;
    surface1.KnotVectorU = { 0, 0, 0, 1, 1, 1 };
    surface1.KnotVectorV = { 0, 0, 0, 1, 1, 1 };
    surface1.ControlPoints = {
        {{0, 0, 0, 1}, {1, 0, 0, 1}, {2, 0, 0, 1}},
        {{0, 1, 0, 1}, {1, 1, 0, 1}, {2, 1, 0, 1}},
        {{0, 2, 0, 1}, {1, 2, 0, 1}, {2, 2, 0, 1}}
    };
    surfaces.push_back(surface1);

    LNLib::LN_NurbsSurface surface2;
    surface2.DegreeU = 3;
    surface2.DegreeV = 3;
    surface2.KnotVectorU = { 0, 0, 0, 0, 1, 1, 1, 1 };
    surface2.KnotVectorV = { 0, 0, 0, 0, 1, 1, 1, 1 };
    surface2.ControlPoints = {
        {{3, 0, 0, 1}, {4, 0, 0, 1}, {5, 0, 0, 1}, {6, 0, 0, 1}},
        {{3, 1, 0, 1}, {4, 1, 0, 2}, {5, 1, 0, 1}, {6, 1, 0, 1}},
        {{3, 2, 0, 1}, {4, 2, 0, 1}, {5, 2, 0, 2}, {6, 2, 0, 1}},
        {{3, 3, 0, 1}, {4, 3, 0, 1}, {5, 3, 0, 1}, {6, 3, 0, 1}}
    };
    surfaces.push_back(surface2);
    std::string exportPath = LNTest::GetProgramDir() + "\\IGESTest.igs";
    EXPECT_TRUE(LNLibEx::LNData::ToIGESFile(surfaces, exportPath));
}
