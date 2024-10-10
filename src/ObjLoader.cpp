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
            // Si on trouve une nouvelle série de sommets après des faces, on démarre un nouvel
            // objet.
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

    // Ajouter le dernier objet si non vide
    if (!_currentObject.vertices.empty() || !_currentObject.indices.empty()) {
        _objects.push_back(_currentObject);
    }

    file.close();
}

void ObjLoader::_startNewObject() {
    if (!_currentObject.vertices.empty() || !_currentObject.indices.empty()) {
        _objects.push_back(_currentObject);
    }
    _currentObject = ObjObject(); // Réinitialiser l'objet courant
    _vertexCache.clear();         // Vider le cache pour les nouveaux sommets
}

void ObjLoader::_processFaceData(const std::vector<std::string> &data) {
    // Store all vertex indices for the current face
    std::vector<unsigned int> vertexIndices;

    // Parse each vertex data and retrieve its index
    for (const auto &vertexData : data) {
        std::string key = vertexData;

        // Check if the vertex is already in the cache
        if (_vertexCache.find(key) != _vertexCache.end()) {
            vertexIndices.push_back(_vertexCache[key]);
        } else {
            std::istringstream vertexStream(vertexData);
            std::string        posIndexStr, texIndexStr, normIndexStr;

            // Parse vertex indices (v, v/vt, v//vn, v/vt/vn)
            std::getline(vertexStream, posIndexStr, '/');
            std::getline(vertexStream, texIndexStr, '/');
            std::getline(vertexStream, normIndexStr, '/');

            int posIndex = _parseIndex(posIndexStr, _positions.size());
            int texIndex = texIndexStr.empty() ? -1 : _parseIndex(texIndexStr, _texCoords.size());
            int normIndex = normIndexStr.empty() ? -1 : _parseIndex(normIndexStr, _normals.size());

            glm::vec3 pos = _positions[posIndex];
            glm::vec2 texCoords = texIndex != -1 ? _texCoords[texIndex] : glm::vec2(0.0f);
            glm::vec3 normal = normIndex != -1 ? _normals[normIndex] : glm::vec3(0.0f);

            unsigned int newIndex = _addVertex(key, pos, normal, texCoords);
            _vertexCache[key] = newIndex;
            vertexIndices.push_back(newIndex);
        }
    }

    // Triangulate the face if it has more than 3 vertices
    if (vertexIndices.size() == 3) {
        // Directly add the triangle
        _currentObject.indices.push_back(vertexIndices[0]);
        _currentObject.indices.push_back(vertexIndices[1]);
        _currentObject.indices.push_back(vertexIndices[2]);
    } else if (vertexIndices.size() > 3) {
        // Use a triangle fan method to break the polygon into triangles
        for (size_t i = 1; i < vertexIndices.size() - 1; ++i) {
            _currentObject.indices.push_back(vertexIndices[0]);
            _currentObject.indices.push_back(vertexIndices[i]);
            _currentObject.indices.push_back(vertexIndices[i + 1]);
        }
    }
}

unsigned int ObjLoader::_parseIndex(const std::string &index, int size) const {
    int idx = std::stoi(index);
    if (idx < 0)
        idx += size;
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
