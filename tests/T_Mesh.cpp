#include "gtest/gtest.h"
#include "T_Utils.h"
#include "LNMesh.h"
#include "LNObject.h"
#include <string>

//#include "LNMeshEx.h"

TEST(Test_LNMesh, ImportOBJ)
{
    std::string stlTestFile = LNTest::GetTestDir() + "cube.obj";
    LNLib::LN_Mesh mesh;
    LNLibEx::LNMesh::FromOBJFile(stlTestFile, mesh);
    EXPECT_TRUE(mesh.Faces.size() == 12);
    EXPECT_TRUE(mesh.Vertices.size() == 8);
}

TEST(Test_LNMesh, ImportSTL)
{
    std::string stlTestFile = LNTest::GetTestDir() + "cube.stl";
    LNLib::LN_Mesh mesh;
    LNLibEx::LNMesh::FromSTLFile(stlTestFile, mesh);
    EXPECT_TRUE(mesh.Faces.size() == 12);
    EXPECT_TRUE(mesh.Vertices.size() == 36);
}