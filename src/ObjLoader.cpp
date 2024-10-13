#include "../include/ObjLoader.h"
#include "../include/Mesh.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>

ObjLoader::ObjLoader(const std::string &filePath) { _parseObjFile(filePath); }

static std::string getParentPath(const std::string &path) {
    size_t pos = path.find_last_of("/\\");
    if (pos == std::string::npos) {
        return "";
    }
    return path.substr(0, pos);
}

static std::string combinePaths(const std::string &path1, const std::string &path2) {
    if (path1.empty()) {
        return path2;
    }
    char lastChar = path1[path1.size() - 1];
    if (lastChar != '/' && lastChar != '\\') {
        return path1 + "/" + path2;
    } else {
        return path1 + path2;
    }
}

const std::vector<ObjObject> &ObjLoader::getObjects() const { return _objects; }

void ObjLoader::_parseObjFile(const std::string &filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Erreur: impossible d'ouvrir le fichier OBJ: " << filePath << std::endl;
        throw std::runtime_error("Impossible d'ouvrir le fichier OBJ.");
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream lineStream(line);
        std::string        prefix;
        lineStream >> prefix;

        if (prefix == "v") {
            glm::vec3 position;
            lineStream >> position.x >> position.y >> position.z;
            _positions.push_back(position);
        } else if (prefix == "vn") {
            glm::vec3 normal;
            lineStream >> normal.x >> normal.y >> normal.z;
            _normals.push_back(normal);
        } else if (prefix == "vt") {
            glm::vec2 texCoord;
            lineStream >> texCoord.x >> texCoord.y;
            _texCoords.push_back(texCoord);
        } else if (prefix == "f") {
            std::vector<std::string> faceData{std::istream_iterator<std::string>{lineStream},
                                              std::istream_iterator<std::string>{}};
            _processFaceData(faceData);
        } else if (prefix == "mtllib") {
            std::string mtllibFilename;
            lineStream >> mtllibFilename;
            _loadMaterialFile(filePath, mtllibFilename);
        } else if (prefix == "usemtl") {
            if (!_currentSubMesh.indices.empty()) {
                _currentObject.subMeshes.push_back(_currentSubMesh);
                _currentSubMesh = SubMesh();
            }
            lineStream >> _currentMaterialName;
            _currentSubMesh.materialName = _currentMaterialName;
        }
    }

    if (!_currentSubMesh.indices.empty()) {
        _currentObject.subMeshes.push_back(_currentSubMesh);
    }
    if (!_currentObject.subMeshes.empty()) {
        _objects.push_back(_currentObject);
    }

    file.close();
}

void ObjLoader::_startNewObject() {
    if (!_currentSubMesh.indices.empty()) {
        _currentObject.subMeshes.push_back(_currentSubMesh);
        _currentSubMesh = SubMesh();
    }
    if (!_currentObject.subMeshes.empty()) {
        _objects.push_back(_currentObject);
    }
    _currentObject = ObjObject();
    _vertexCache.clear();
}

void ObjLoader::_processFaceData(const std::vector<std::string> &data) {
    std::vector<unsigned int> vertexIndices;

    for (const auto &vertexData : data) {
        std::string key = vertexData;

        unsigned int newIndex;
        if (_vertexCache.find(key) != _vertexCache.end()) {
            newIndex = _vertexCache[key];
        } else {
            std::istringstream vertexStream(vertexData);
            std::string        posIndexStr, texIndexStr, normIndexStr;

            std::getline(vertexStream, posIndexStr, '/');
            std::getline(vertexStream, texIndexStr, '/');
            std::getline(vertexStream, normIndexStr, '/');

            unsigned int posIndex = _parseIndex(posIndexStr, _positions.size());
            unsigned int texIndex = texIndexStr.empty()
                                        ? static_cast<unsigned int>(-1)
                                        : _parseIndex(texIndexStr, _texCoords.size());
            unsigned int normIndex = normIndexStr.empty()
                                         ? static_cast<unsigned int>(-1)
                                         : _parseIndex(normIndexStr, _normals.size());

            glm::vec3 pos = _positions.at(posIndex);
            glm::vec2 texCoords = texIndex != static_cast<unsigned int>(-1)
                                      ? _texCoords.at(texIndex)
                                      : glm::vec2(0.0f);
            glm::vec3 normal = normIndex != static_cast<unsigned int>(-1) ? _normals.at(normIndex)
                                                                          : glm::vec3(0.0f);

            newIndex = _addVertex(key, pos, normal, texCoords);
            _vertexCache[key] = newIndex;
        }
        vertexIndices.push_back(newIndex);
    }

    // Triangulate the face if it has more than 3 vertices
    if (vertexIndices.size() >= 3) {
        for (size_t i = 1; i < vertexIndices.size() - 1; ++i) {
            _currentSubMesh.indices.push_back(vertexIndices[0]);
            _currentSubMesh.indices.push_back(vertexIndices[i]);
            _currentSubMesh.indices.push_back(vertexIndices[i + 1]);
        }
    }
}

unsigned int ObjLoader::_parseIndex(const std::string &index, size_t size) const {
    int idx = std::stoi(index);
    if (idx < 0)
        idx += static_cast<int>(size);
    else
        idx -= 1;
    return static_cast<unsigned int>(idx);
}

unsigned int ObjLoader::_addVertex(const std::string &key, const glm::vec3 &pos,
                                   const glm::vec3 &normal, const glm::vec2 &texCoords) {
    Vertex vertex = {pos, texCoords, normal};
    _currentObject.vertices.push_back(vertex);
    unsigned int index = static_cast<unsigned int>(_currentObject.vertices.size() - 1);
    _vertexCache[key] = index;
    return index;
}

void ObjLoader::_loadMaterialFile(const std::string &objFilePath,
                                  const std::string &mtllibFilename) {
    // Construire le chemin complet vers le fichier .mtl
    std::string objParentPath = getParentPath(objFilePath);
    std::string mtlFilePath = combinePaths(objParentPath, mtllibFilename);

    std::ifstream mtlFile(mtlFilePath.c_str());
    if (!mtlFile.is_open()) {
        std::cerr << "Erreur: impossible d'ouvrir le fichier de matériau: " << mtlFilePath
                  << std::endl;
        return;
    }

    std::string line;
    std::string currentMaterialName;
    Material    currentMaterial;

    while (std::getline(mtlFile, line)) {
        std::istringstream lineStream(line);
        std::string        prefix;
        lineStream >> prefix;

        if (prefix == "newmtl") {
            if (!currentMaterialName.empty()) {
                _materials[currentMaterialName] = currentMaterial;
            }
            lineStream >> currentMaterialName;
            currentMaterial = Material();
            currentMaterial.name = currentMaterialName;
        } else if (prefix == "Ka") {
            lineStream >> currentMaterial.ambient.r >> currentMaterial.ambient.g >>
                currentMaterial.ambient.b;
        } else if (prefix == "Kd") {
            lineStream >> currentMaterial.diffuse.r >> currentMaterial.diffuse.g >>
                currentMaterial.diffuse.b;
        } else if (prefix == "Ks") {
            lineStream >> currentMaterial.specular.r >> currentMaterial.specular.g >>
                currentMaterial.specular.b;
        } else if (prefix == "Ns") {
            lineStream >> currentMaterial.shininess;
        } else if (prefix == "map_Kd") {
            lineStream >> currentMaterial.diffuseMapPath;
            std::string mtlParentPath = getParentPath(mtlFilePath);
            std::string texturePath = combinePaths(mtlParentPath, currentMaterial.diffuseMapPath);
            currentMaterial.diffuseMapPath = texturePath;
        }
    }
    if (!currentMaterialName.empty()) {
        _materials[currentMaterialName] = currentMaterial;
    }

    mtlFile.close();
}

std::vector<std::shared_ptr<Mesh>> ObjLoader::getMeshes() const {
    std::vector<std::shared_ptr<Mesh>> meshes;
    for (const auto &object : _objects) {
        const auto &vertices = object.vertices;
        for (const auto &subMesh : object.subMeshes) {
            std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(vertices, subMesh.indices);

            auto it = _materials.find(subMesh.materialName);
            if (it != _materials.end()) {
                mesh->setMaterial(it->second);
            }
            meshes.push_back(mesh);
        }
    }
    return meshes;
}
