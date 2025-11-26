# CLAUDE.md - AI Assistant Guide for LNLibEx

## Project Overview

**LNLibEx** is a C++ extension library for the NURBS Algorithms Library [LNLib](https://github.com/BIMCoderLiang/LNLib). It provides practical implementations for CAD file format exchange (STEP/IGES) and mesh import functionality (STL/OBJ) using OpenCascade Technology (OCCT) 7.9.1.

**Owner**: Yuqing Liang (BIMCoder Liang)
**Contact**: bim.frankliang@foxmail.com
**License**: LGPL 2.1 (except LNInternal module which is not open-source)

---

## Repository Structure

```
LNLibEx/
├── src/
│   ├── LNMesh/           # Mesh import module (STL, OBJ)
│   │   ├── public/       # Public API headers
│   │   │   ├── LNMesh.h
│   │   │   └── LNMeshDefinitions.h
│   │   └── private/      # Implementation files
│   │       └── LNMesh.cpp
│   ├── LNData/           # Data exchange module (STEP, IGES export)
│   │   ├── public/       # Public API headers
│   │   │   ├── LNData.h
│   │   │   └── LNDataDefinitions.h
│   │   └── private/      # Implementation files
│   │       ├── LNData.cpp
│   │       ├── LNConverter.h/cpp
│   │       ├── LNSTEPGenerator.h/cpp
│   │       └── LNIGESGenerator.h/cpp
│   └── LNInternal/       # Closed-source module (NOT in repository)
├── tests/                # GoogleTest unit tests
│   ├── T_Mesh.cpp        # Mesh import tests
│   ├── T_Data.cpp        # Data export tests
│   ├── T_Main.cpp        # Test entry point
│   ├── T_Utils.h/cpp     # Test utilities
│   └── TFiles/           # Test data files (cube.stl, cube.obj)
├── thirdparty/           # Third-party dependencies
│   ├── LNLib/            # Core NURBS library
│   └── OpenCascade/      # OCCT for STEP/IGES
├── assets/               # Documentation images
├── CMakeLists.txt        # Root CMake configuration
├── build.bat             # Windows build script
├── .gitignore
├── LICENSE               # LGPL 2.1
└── README.md
```

### Module Responsibilities

- **LNMesh**: Import mesh data from STL (ASCII/Binary) and OBJ files into `LN_Mesh` structures
- **LNData**: Export NURBS surfaces (`LN_NurbsSurface`) to STEP and IGES CAD formats
- **LNInternal**: Proprietary module (excluded from open-source release)

---

## Build System

### Requirements

- **CMake**: 3.16 or higher
- **C++ Standard**: C++17
- **Compiler**: MSVC (Windows-focused, uses Windows DLL export/import)
- **Build Configurations**: Debug, Release, RelWithDebInfo

### Dependencies

1. **LNLib** (thirdparty/LNLib/)
   - Core NURBS geometry library
   - Provides: `LN_Mesh`, `LN_NurbsSurface`, `XYZ`, `UV`, `XYZW` types
   - Linked as precompiled DLL

2. **OpenCascade 7.9.1** (thirdparty/OpenCascade/)
   - CAD kernel for STEP/IGES export
   - Multiple DLLs with delay-loading enabled
   - Key modules: TKBRep, TKDEIGES, TKDESTEP, TKG3d, TKMath

3. **Eigen** (auto-fetched via CMake FetchContent)
   - Used by LNMesh module
   - Header-only library from GitLab

4. **GoogleTest** (auto-fetched via CMake FetchContent)
   - Unit testing framework
   - Fetched from GitHub release archive

### Build Commands

```bash
# Windows (using build.bat)
build.bat

# Or manually with CMake
cmake -S . -B build
cmake --build build --config Release
```

### CMake Configuration

- Output directories: `build/Debug`, `build/Release`, `build/RelWithDebInfo`
- Unit tests: Enabled by default (`ENABLE_UNIT_TESTS=ON`)
- Startup project (Visual Studio): Tests
- Custom post-build steps copy DLLs and test files to output directory

---

## Code Conventions

### File Organization

- **Public headers**: `src/<Module>/public/` - API exposed to users
- **Private headers/implementations**: `src/<Module>/private/` - Internal implementation
- **Test prefix**: All test files use `T_` prefix (e.g., `T_Mesh.cpp`, `T_Data.cpp`)

### Header Structure

Every source file must include this header comment:

```cpp
/*
 * Owner:
 * 2025/MM/DD - Yuqing Liang (BIMCoder Liang)
 * bim.frankliang@foxmail.com
 *
 * Use of this source code is governed by a LGPL-2.1 license that can be found in
 * the LICENSE file.
 */
```

### DLL Export Macros

Each module defines its own export macros:

```cpp
// In LNMeshDefinitions.h or LNDataDefinitions.h
#define DLL_EXPORT __declspec(dllexport)
#define DLL_IMPORT __declspec(dllimport)

#if defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(__CYGWIN__)
    #ifdef LNMesh_HOME  // or LNData_HOME
        #define LNMesh_EXPORT DLL_EXPORT
    #else
        #define LNMesh_EXPORT DLL_IMPORT
    #endif
#else
    #define LNMesh_EXPORT
#endif
```

### Namespace Convention

All code belongs to the `LNLibEx` namespace:

```cpp
namespace LNLibEx
{
    class LNMesh_EXPORT LNMesh
    {
        // ...
    };
}
```

### API Design Pattern

- **Static methods**: Primary API uses static class methods (no instance creation needed)
- **Return values**: `bool` for success/failure
- **Parameters**: Output parameters passed by reference (e.g., `LNLib::LN_Mesh& mesh`)

Example:
```cpp
// Import
static bool FromOBJFile(const std::string& filePath, LNLib::LN_Mesh& mesh);
static bool FromSTLFile(const std::string& filePath, LNLib::LN_Mesh& mesh);

// Export
static bool ToSTEPFile(const std::vector<LNLib::LN_NurbsSurface>& surfaces, const std::string& filePath);
static bool ToIGESFile(const std::vector<LNLib::LN_NurbsSurface>& surfaces, const std::string& filePath);
```

### Coding Style

- **Include guards**: Use `#pragma once`
- **Standard library**: Prefer STL containers (`std::vector`, `std::string`)
- **File I/O**: Use `std::ifstream`/`std::ofstream`
- **String parsing**: Use `std::istringstream` for parsing
- **Error handling**: Return `false` on failure (file not found, invalid data, etc.)

---

## Key Data Structures (from LNLib)

### LN_Mesh
```cpp
struct LN_Mesh
{
    std::vector<XYZ> Vertices;              // Vertex positions
    std::vector<XYZ> Normals;               // Vertex normals
    std::vector<UV> UVs;                    // Texture coordinates
    std::vector<std::vector<int>> Faces;    // Face vertex indices
    std::vector<int> NormalIndices;         // Normal indices
    std::vector<int> UVIndices;             // UV indices
};
```

### LN_NurbsSurface
```cpp
struct LN_NurbsSurface
{
    int DegreeU;                                           // U-direction degree
    int DegreeV;                                           // V-direction degree
    std::vector<double> KnotVectorU;                      // U knot vector
    std::vector<double> KnotVectorV;                      // V knot vector
    std::vector<std::vector<XYZW>> ControlPoints;        // Control points (homogeneous coords)
};
```

### XYZ, UV, XYZW
```cpp
class XYZ { double X(), Y(), Z(); };
class UV  { double U(), V(); };
class XYZW { double X(), Y(), Z(), W(); };  // Homogeneous coordinates
```

---

## Development Workflows

### Adding a New Feature

1. **Determine module**: Choose LNMesh (import) or LNData (export)
2. **Add public API**: Extend `src/<Module>/public/LN<Module>.h` with static methods
3. **Implement logic**: Add implementation in `src/<Module>/private/`
4. **Create tests**: Add test case in `tests/T_<Module>.cpp`
5. **Test data**: Place test files in `tests/TFiles/`
6. **Update README**: Document new features in README.md

### Adding a New File Format

**For Import (Mesh)**:
```cpp
// In LNMesh.h
static bool From<FORMAT>File(const std::string& filePath, LNLib::LN_Mesh& mesh);

// In LNMesh.cpp
bool LNLibEx::LNMesh::From<FORMAT>File(const std::string& filePath, LNLib::LN_Mesh& mesh)
{
    // 1. Open file with std::ifstream
    // 2. Parse format-specific data
    // 3. Populate mesh.Vertices, mesh.Faces, etc.
    // 4. Return true on success, false on error
}
```

**For Export (Data)**:
```cpp
// In LNData.h
static bool To<FORMAT>File(const std::vector<LNLib::LN_NurbsSurface>& surfaces,
                           const std::string& filePath);

// In LNData.cpp - use generator pattern
// Create LN<FORMAT>Generator.h/cpp files
// Implement converter from LN_NurbsSurface to target format
```

### Writing Tests

Use GoogleTest framework:

```cpp
#include "gtest/gtest.h"
#include "T_Utils.h"
#include "LNMesh.h"  // or LNData.h

TEST(Test_LNMesh, ImportNewFormat)
{
    std::string testFile = LNTest::GetTestDir() + "test.ext";
    LNLib::LN_Mesh mesh;
    EXPECT_TRUE(LNLibEx::LNMesh::FromNewFormatFile(testFile, mesh));
    EXPECT_TRUE(mesh.Faces.size() > 0);
    EXPECT_TRUE(mesh.Vertices.size() > 0);
}
```

### Testing Commands

```bash
# Build and run tests (from build directory)
cmake --build build --config Debug
cd build/Debug
./Tests.exe
```

---

## Dependencies and Third-Party Libraries

### LNLib Integration

- **Location**: `thirdparty/LNLib/`
- **Headers**: `thirdparty/LNLib/include/`
- **Libraries**: `thirdparty/LNLib/lib/Debug|Release/LNLib.lib`
- **Runtime**: `thirdparty/LNLib/bin/Debug|Release/LNLib.dll`
- **CMake variable**: `${LNLib_DIR}`

Key headers to include:
```cpp
#include "LNObject.h"        // Base types and definitions
#include "XYZ.h"             // 3D point
#include "UV.h"              // 2D point
#include "XYZW.h"            // Homogeneous coordinates
#include "NurbsSurface.h"    // NURBS surface algorithms
```

### OpenCascade Integration

- **Location**: `thirdparty/OpenCascade/`
- **Headers**: `thirdparty/OpenCascade/include/`
- **Libraries**: `thirdparty/OpenCascade/lib/*.lib`
- **Runtime**: `thirdparty/OpenCascade/bin/*.dll`
- **CMake variable**: `${OCC_DIR}`
- **Delay-loaded**: All OCCT DLLs use `/DELAYLOAD` to reduce startup time

Key headers for STEP/IGES:
```cpp
#include "Standard_Handle.hxx"
#include "Geom_BSplineSurface.hxx"
#include "TopoDS_Face.hxx"
#include "BRepBuilderAPI_MakeFace.hxx"
// And many more OCCT headers...
```

---

## Git Workflow

### Branches

- **Main branch**: Not explicitly set (check remote)
- **Feature branches**: Use format `claude/claude-md-<session-id>`
- **Protected**: Never force-push to main/master

### Commit Guidelines

- Write clear, descriptive commit messages
- Reference file owner convention (Yuqing Liang)
- Keep commits atomic and focused
- Test before committing

### .gitignore Rules

Excluded from version control:
- `.vs/` - Visual Studio user files
- `.vscode/` - VS Code settings
- `build/` - CMake build output
- `src/LNInternal/` - Proprietary module

---

## Common Tasks for AI Assistants

### 1. Adding Support for a New Mesh Format

**Steps**:
1. Research the file format specification (ASCII/Binary)
2. Add static method to `LNMesh` class in `src/LNMesh/public/LNMesh.h`
3. Implement parser in `src/LNMesh/private/LNMesh.cpp`
4. Parse vertices → populate `mesh.Vertices` (vector of `XYZ`)
5. Parse faces → populate `mesh.Faces` (vector of vector of int)
6. Parse normals/UVs if available
7. Add test in `tests/T_Mesh.cpp`
8. Add sample file to `tests/TFiles/`
9. Update README.md with new format

**Example pattern** (see `FromOBJFile`, `FromSTLFile`):
```cpp
bool LNLibEx::LNMesh::FromXXXFile(const std::string& filePath, LNLib::LN_Mesh& mesh)
{
    std::ifstream file(filePath);
    if (!file.is_open()) return false;

    // Parse format-specific content
    // Populate mesh.Vertices, mesh.Faces, etc.

    return true;
}
```

### 2. Adding Support for a New CAD Export Format

**Steps**:
1. Create `LN<FORMAT>Generator.h` and `.cpp` in `src/LNData/private/`
2. Implement converter from `LN_NurbsSurface` to target format
3. Add static method to `LNData` class in `src/LNData/public/LNData.h`
4. Implement export logic in `src/LNData/private/LNData.cpp`
5. Link required third-party libraries in `src/LNData/CMakeLists.txt`
6. Add test in `tests/T_Data.cpp`
7. Update README.md

**Generator pattern** (see `LNSTEPGenerator`, `LNIGESGenerator`):
```cpp
class LNXXXGenerator
{
private:
    std::vector<LNLib::LN_NurbsSurface> _surfaces;
    std::string _filePath;

public:
    LNXXXGenerator(const std::vector<LNLib::LN_NurbsSurface>& surfaces,
                   const std::string& filePath);
    bool Process();
};
```

### 3. Fixing Build Issues

**Common problems**:

- **Missing DLL**: Check that `LNLib.dll` is copied to output directory
  - See `tests/CMakeLists.txt` custom commands

- **Linker errors**: Ensure libraries are linked in module's `CMakeLists.txt`
  ```cmake
  target_link_libraries(${TARGET_NAME} ${LNLib_DIR}/lib/$<CONFIG>/LNLib.lib)
  ```

- **Include errors**: Verify include directories
  ```cmake
  target_include_directories(${TARGET_NAME} PUBLIC "${SOURCE_DIR}/public")
  ```

- **C++ standard**: Must be C++17
  ```cmake
  set(CMAKE_CXX_STANDARD 17)
  ```

### 4. Understanding Existing Code

**Key files to read**:

- **API surface**: `src/LNMesh/public/LNMesh.h` and `src/LNData/public/LNData.h`
- **Examples**: `tests/T_Mesh.cpp` and `tests/T_Data.cpp`
- **Data structures**: Headers in `thirdparty/LNLib/include/`
- **Build configuration**: `CMakeLists.txt` files

**Code navigation**:
- Public API → Private implementation (follow static method calls)
- Generator classes handle complex exports (STEP/IGES)
- Converter classes bridge LNLib types to OCCT types

### 5. Running and Adding Tests

**Run existing tests**:
```bash
cmake --build build --config Debug
cd build/Debug
./Tests.exe
```

**Add new test**:
```cpp
TEST(Test_Category, TestName)
{
    // Arrange
    std::string inputFile = LNTest::GetTestDir() + "file.ext";
    LNLib::LN_Mesh mesh;

    // Act
    bool result = LNLibEx::LNMesh::FromFormatFile(inputFile, mesh);

    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(mesh.Vertices.size(), expectedCount);
}
```

---

## Important Constraints

### Platform Limitations

- **Windows-only**: Uses `__declspec(dllexport/dllimport)` for DLL creation
- **MSVC-specific**: Delay-loading, debugger environment settings
- **Visual Studio**: Project uses VS-specific CMake properties

### Licensing Constraints

- **LGPL 2.1**: All modifications must preserve license headers
- **LNInternal exclusion**: Do not create or modify `src/LNInternal/` (proprietary)
- **Third-party licenses**: Respect Eigen (MPL2), OCCT, LNLib licenses

### API Stability

- **Public headers only**: Only modify `public/` headers for API changes
- **Backward compatibility**: Avoid breaking existing function signatures
- **Static methods**: Maintain pattern of static class methods (no instances)

---

## Troubleshooting

### Build Errors

**Problem**: CMake can't find third-party libraries
**Solution**: Check `${LNLib_DIR}` and `${OCC_DIR}` paths in root `CMakeLists.txt`

**Problem**: Linker errors with OCCT
**Solution**: Verify delay-load settings in `src/LNData/CMakeLists.txt`

**Problem**: Eigen not downloading
**Solution**: Check internet connection; FetchContent requires GitLab access

### Runtime Errors

**Problem**: DLL not found errors
**Solution**: Check post-build commands copy DLLs to output directory
**Solution**: For debugging in VS, check `VS_DEBUGGER_ENVIRONMENT` PATH setting

**Problem**: Test files not found
**Solution**: Verify `TFILES` directory is created and populated by post-build

### Test Failures

**Problem**: Mesh import returns false
**Solution**: Check file path (tests use `LNTest::GetTestDir()`)
**Solution**: Verify test file format is correct

**Problem**: Export creates empty files
**Solution**: Check NURBS surface data validity (knot vectors, control points)
**Solution**: Enable OCCT debugging to see detailed errors

---

## Quick Reference

### File Naming Conventions

- Public headers: `LN<Module>.h`, `LN<Module>Definitions.h`
- Private implementation: `LN<Module>.cpp`, `LN<Something>.h/cpp`
- Tests: `T_<Module>.cpp`, `T_Utils.h/cpp`
- Test data: `<name>.<ext>` in `tests/TFiles/`

### Common Commands

```bash
# Generate Visual Studio solution
cmake -S . -B build

# Build specific configuration
cmake --build build --config Release

# Open in Visual Studio (Windows)
cmake --open build

# Run tests
cd build/Debug && ./Tests.exe
```

### Key Types and Namespaces

- `LNLibEx::LNMesh` - Mesh import API
- `LNLibEx::LNData` - Data export API
- `LNLib::LN_Mesh` - Mesh data structure
- `LNLib::LN_NurbsSurface` - NURBS surface data structure
- `LNLib::XYZ`, `LNLib::UV`, `LNLib::XYZW` - Geometric primitives

### Include Patterns

```cpp
// For public API
#include "LNMesh.h"          // or "LNData.h"
#include "LNMeshDefinitions.h"

// For LNLib types
#include "LNObject.h"
#include "XYZ.h"
#include "UV.h"

// For standard library
#include <string>
#include <vector>
#include <fstream>
```

---

## Contact and Contributions

**Maintainer**: Yuqing Liang (BIMCoder Liang)
**Email**: bim.frankliang@foxmail.com
**WeChat Public Account**: BIMCoder

When contributing:
1. Preserve all copyright headers with owner information
2. Follow existing code style and conventions
3. Add tests for new functionality
4. Update README.md for user-facing changes
5. Respect LGPL 2.1 license terms
6. Do not modify or create files in `src/LNInternal/`

---

*This guide is designed to help AI assistants understand and work effectively with the LNLibEx codebase. For human developers, also refer to README.md and inline code documentation.*
