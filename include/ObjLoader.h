#pragma once

#include "struct.h"

class ObjLoader {
  public:
    explicit ObjLoader(const std::string &filePath);

    // Obtenir les objets chargés
    const std::vector<ObjObject> &getObjects() const;

  private:
    std::vector<glm::vec3>                        _positions;
    std::vector<glm::vec3>                        _normals;
    std::vector<glm::vec2>                        _texCoords;
    std::vector<ObjObject>                        _objects;
    std::unordered_map<std::string, unsigned int> _vertexCache;

    void         _parseObjFile(const std::string &filePath);
    void         _startNewObject();
    void         _processFaceData(const std::vector<std::string> &data);
    unsigned int _parseIndex(const std::string &index, size_t size) const;
    unsigned int _addVertex(const std::string &key, const glm::vec3 &pos, const glm::vec3 &normal,
                            const glm::vec2 &texCoords);

    ObjObject _currentObject;
};
