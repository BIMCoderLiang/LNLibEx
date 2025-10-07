/*
 * Owner:
 * 2025/07/19 - Yuqing Liang (BIMCoder Liang)
 * bim.frankliang@foxmail.com
 * 
 * Use of this source code is governed by a LGPL-2.1 license that can be found in
 * the LICENSE file.
 *
 */

#include "LNMesh.h"
#include "LNObject.h"
#include "XYZ.h"
#include "UV.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

#pragma region OBJ
bool LNLibEx::LNMesh::FromOBJFile(const std::string& filePath, LNLib::LN_Mesh& mesh)
{
    std::vector<LNLib::XYZ> temp_vertices;
    std::vector<LNLib::UV> temp_uvs;
    std::vector<LNLib::XYZ> temp_normals;

    std::ifstream file(filePath);
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") { 
            LNLib::XYZ vertex;
            iss >> vertex.X() >> vertex.Y() >> vertex.Z();
            temp_vertices.push_back(vertex);
        }
        else if (prefix == "vt") {
            LNLib::UV uv;
            iss >> uv.U() >> uv.V();
            temp_uvs.push_back(uv);
        }
        else if (prefix == "vn") {
            LNLib::XYZ normal;
            iss >> normal.X() >> normal.Y() >> normal.Z();
            temp_normals.push_back(normal);
        }
        else if (prefix == "f") {
            std::vector<int> faceVertices;
            std::vector<int> faceUVs;
            std::vector<int> faceNormals;

            std::string faceData;
            while (iss >> faceData) {
                std::replace(faceData.begin(), faceData.end(), '/', ' ');
                std::istringstream faceDataStream(faceData);

                int vIdx = -1, uvIdx = -1, nIdx = -1;
                faceDataStream >> vIdx;
                if (faceDataStream.peek() != ' ' && !faceDataStream.eof()) {
                    faceDataStream >> uvIdx;
                }
                if (faceDataStream.peek() != ' ' && !faceDataStream.eof()) {
                    faceDataStream >> nIdx;
                }
                if (vIdx > 0) {
                    faceVertices.push_back(vIdx - 1);
                }
                if (uvIdx > 0 && !temp_uvs.empty()) {
                    faceUVs.push_back(uvIdx - 1);
                }
                if (nIdx > 0 && !temp_normals.empty()) {
                    faceNormals.push_back(nIdx - 1);
                }
            }

            if (!faceVertices.empty()) {
                mesh.Faces.push_back(faceVertices);
            }
            if (!faceUVs.empty()) {
                for (int idx : faceUVs) {
                    mesh.UVIndices.push_back(idx);
                }
            }
            if (!faceNormals.empty()) {
                for (int idx : faceNormals) {
                    mesh.NormalIndices.push_back(idx);
                }
            }
        }
    }
    mesh.Vertices = std::move(temp_vertices);
    mesh.UVs = std::move(temp_uvs);
    mesh.Normals = std::move(temp_normals);
}
#pragma endregion

#pragma region STL
namespace
{
    std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t");
        if (std::string::npos == first) return str;
        size_t last = str.find_last_not_of(" \t");
        return str.substr(first, (last - first + 1));
    }

    bool startsWith(const std::string& str, const std::string& prefix) {
        return str.size() >= prefix.size() &&
            std::equal(prefix.begin(), prefix.end(), str.begin(), [](char a, char b) {
            return std::tolower(a) == std::tolower(b);});
    }

    bool isBinarySTL(std::ifstream& file) {
        file.seekg(80, std::ios::beg);
        uint32_t facetCount;
        file.read(reinterpret_cast<char*>(&facetCount), 4);
        file.seekg(0, std::ios::beg);
        file.seekg(0, std::ios::end);
        size_t fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        return fileSize == (84 + facetCount * 50);
    }

    bool readBinarySTL(std::ifstream& file, LNLib::LN_Mesh& mesh) {

        file.seekg(80, std::ios::beg);
        uint32_t facetCount;
        file.read(reinterpret_cast<char*>(&facetCount), 4);

        for (uint32_t i = 0; i < facetCount; ++i) {
            LNLib::XYZ normal;
            file.read(reinterpret_cast<char*>(&normal.X()), 4);
            file.read(reinterpret_cast<char*>(&normal.Y()), 4);
            file.read(reinterpret_cast<char*>(&normal.Z()), 4);
            mesh.Normals.push_back(normal);

            std::vector<int> faceIndices;
            for (int j = 0; j < 3; ++j) {
                LNLib::XYZ vertex;
                file.read(reinterpret_cast<char*>(&vertex.X()), 4);
                file.read(reinterpret_cast<char*>(&vertex.Y()), 4);
                file.read(reinterpret_cast<char*>(&vertex.Z()), 4);

                mesh.Vertices.push_back(vertex);
                faceIndices.push_back(mesh.Vertices.size() - 1);
                mesh.NormalIndices.push_back(mesh.Normals.size() - 1);
            }
            mesh.Faces.push_back(faceIndices);
            file.seekg(2, std::ios::cur);
        }
        return true;
    }

    bool readASCIISTL(std::ifstream& file, LNLib::LN_Mesh& mesh) {
        std::string line;
        std::vector<int> currentFaceIndices;
        LNLib::XYZ currentNormal;
        bool hasNormal = false;
        int vertexIndex = 0;

        while (std::getline(file, line)) {
            line = trim(line);
            if (line.empty()) continue;

            std::istringstream iss(line);
            std::string keyword;
            iss >> keyword;

            if (startsWith(keyword, "facet") && iss >> keyword && startsWith(keyword, "normal")) {
                iss >> currentNormal.X() >> currentNormal.Y() >> currentNormal.Z();
                hasNormal = true;
                currentFaceIndices.clear();
            }
            else if (startsWith(keyword, "vertex")) {
                LNLib::XYZ vertex;
                iss >> vertex.X() >> vertex.Y() >> vertex.Z();
                mesh.Vertices.push_back(vertex);
                currentFaceIndices.push_back(vertexIndex++);
            }
            else if (startsWith(keyword, "endfacet") || 
                        (startsWith(keyword, "end") && iss >> keyword && startsWith(keyword, "facet"))){
                if (currentFaceIndices.size() >= 3) {
                    mesh.Faces.push_back(currentFaceIndices);

                    if (hasNormal) {
                        mesh.Normals.push_back(currentNormal);
                        for (size_t i = 0; i < currentFaceIndices.size(); ++i) {
                            mesh.NormalIndices.push_back(mesh.Normals.size() - 1);
                        }
                    }
                }
                hasNormal = false;
            }
        }
        return true;
    }
}
bool LNLibEx::LNMesh::FromSTLFile(const std::string& filePath, LNLib::LN_Mesh& mesh)
{
    std::ifstream file(filePath, std::ios::binary);

    if (!file.is_open()) {
        return false;
    }
    bool binary = isBinarySTL(file);

    if (binary) {
        return readBinarySTL(file, mesh);
    }
    else {
        file.seekg(0, std::ios::beg);
        file.close();
        file.open(filePath);
        return readASCIISTL(file, mesh);
    }
	return false;
}
#pragma endregion

