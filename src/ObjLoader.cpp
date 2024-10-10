#include "../include/ObjLoader.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>

ObjLoader::ObjLoader(const std::string &filePath) { _parseObjFile(filePath); }

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
            if (!_currentObject.indices.empty()) {
                _startNewObject();
            }

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
        }
    }

    if (!_currentObject.vertices.empty() || !_currentObject.indices.empty()) {
        _objects.push_back(_currentObject);
    }

    file.close();
}

void ObjLoader::_startNewObject() {
    if (!_currentObject.vertices.empty() || !_currentObject.indices.empty()) {
        _objects.push_back(_currentObject);
    }
    _currentObject = ObjObject();
    _vertexCache.clear();
}

void ObjLoader::_processFaceData(const std::vector<std::string> &data) {
    std::vector<unsigned int> vertexIndices;

    for (const auto &vertexData : data) {
        std::string key = vertexData;

        if (_vertexCache.find(key) != _vertexCache.end()) {
            vertexIndices.push_back(_vertexCache[key]);
        } else {
            std::istringstream vertexStream(vertexData);
            std::string        posIndexStr, texIndexStr, normIndexStr;

            // Parse vertex indices (v, v/vt, v//vn, v/vt/vn)
            std::getline(vertexStream, posIndexStr, '/');
            std::getline(vertexStream, texIndexStr, '/');
            std::getline(vertexStream, normIndexStr, '/');

            unsigned int posIndex =
                static_cast<unsigned int>(_parseIndex(posIndexStr, _positions.size()));
            unsigned int texIndex =
                texIndexStr.empty()
                    ? static_cast<unsigned int>(-1)
                    : static_cast<unsigned int>(_parseIndex(texIndexStr, _texCoords.size()));
            unsigned int normIndex =
                normIndexStr.empty()
                    ? static_cast<unsigned int>(-1)
                    : static_cast<unsigned int>(_parseIndex(normIndexStr, _normals.size()));

            glm::vec3 pos = _positions.at(posIndex);
            glm::vec2 texCoords = texIndex != static_cast<unsigned int>(-1)
                                      ? _texCoords.at(texIndex)
                                      : glm::vec2(0.0f);
            glm::vec3 normal = normIndex != static_cast<unsigned int>(-1) ? _normals.at(normIndex)
                                                                          : glm::vec3(0.0f);

            unsigned int newIndex = _addVertex(key, pos, normal, texCoords);
            _vertexCache[key] = newIndex;
            vertexIndices.push_back(newIndex);
        }
    }

    // Triangulate the face if it has more than 3 vertices
    if (vertexIndices.size() == 3) {
        _currentObject.indices.push_back(vertexIndices[0]);
        _currentObject.indices.push_back(vertexIndices[1]);
        _currentObject.indices.push_back(vertexIndices[2]);
    } else if (vertexIndices.size() > 3) {
        for (size_t i = 1; i < vertexIndices.size() - 1; ++i) {
            _currentObject.indices.push_back(vertexIndices[0]);
            _currentObject.indices.push_back(vertexIndices[i]);
            _currentObject.indices.push_back(vertexIndices[i + 1]);
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

const std::vector<ObjObject> &ObjLoader::getObjects() const { return _objects; }
