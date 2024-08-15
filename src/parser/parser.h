#ifndef THREE_D_VIEWER_PARSER_H_
#define THREE_D_VIEWER_PARSER_H_

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace viewer {
struct ModelObj {
  ModelObj() : vertices{}, faces{}, scaler{} {}
  ModelObj(ModelObj&& other) noexcept
      : vertices(std::move(other.vertices)),
        faces(std::move(other.faces)),
        scaler(other.scaler) {}

  ModelObj(const ModelObj& other) = delete;

  ModelObj& operator=(ModelObj&& other) noexcept {
    if (this != &other) {
      vertices = std::move(other.vertices);
      faces = std::move(other.faces);
      scaler = other.scaler;
    }
    return *this;
  }

  ModelObj& operator=(const ModelObj& other) = delete;

  std::vector<float> vertices;
  std::vector<unsigned int> faces;
  double scaler;
};

class ObjParser {
 public:
  ModelObj Parse(const std::string& filename);

 private:
  ModelObj ParseData(const std::string& filename);
  ModelObj CalculateScaler(ModelObj& model);
};
};  // namespace viewer

#endif  // THREE_D_VIEWER_PARSER_H_
