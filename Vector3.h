#ifndef VECTOR3_H
#define VECTOR3_H

#include <iostream>

namespace xsf_data_structures {

struct Vector3 {
  float x = 0.0f, y = 0.0f, z = 0.0f;
  int *memory_block = nullptr;

  Vector3() { memory_block = new int[3]; }

  Vector3(float scalar) : x(scalar), y(scalar), z(scalar) {
    memory_block = new int[3];
  }

  Vector3(float a, float b, float c) : x(a), y(b), z(c) {
    memory_block = new int[3];
  }

  Vector3(const Vector3 &other) : x(other.x), y(other.y), z(other.z) {
    std::cout << "Copy\n";
    memory_block = new int[3];
    memcpy(memory_block, other.memory_block, 3);
  }

  Vector3 &operator=(const Vector3 &other) {
    std::cout << "Copy\n";
    x = other.x;
    y = other.y;
    z = other.z;
    memory_block = new int[3];
    memcpy(memory_block, other.memory_block, 3);
    return *this;
  }

  Vector3(Vector3 &&other) : x(other.x), y(other.y), z(other.z) {
    std::cout << "Move\n";
    memory_block = other.memory_block;
    other.memory_block = nullptr;
  }

  Vector3 &operator=(Vector3 &&other) {
    std::cout << "Move\n";
    x = other.x;
    y = other.y;
    z = other.z;
    memory_block = other.memory_block;
    other.memory_block = nullptr;
    return *this;
  }

  ~Vector3() {
    // std::cout << "Destroy\n";
    delete[] memory_block;
  }

  bool operator==(const Vector3 &other) const {
    return x == other.x && y == other.y && z == other.z;
  }

  bool operator!=(const Vector3 &other) const { return !(*this == other); }
};

std::ostream &operator<<(std::ostream &stream, const Vector3 &vector3) {
  stream << vector3.x << ", " << vector3.y << ", " << vector3.z;
  return stream;
}

class Vector3Hash {
 public:
  size_t operator()(const Vector3 &v) const {
    size_t hash = 17;
    hash = hash * 31 + std::hash<int>()(v.x);
    hash = hash * 31 + std::hash<int>()(v.y);
    hash = hash * 31 + std::hash<int>()(v.z);
    return hash;
  }
  size_t operator()(Vector3 &&v) const {
    size_t hash = 17;
    hash = hash * 31 + std::hash<int>()(v.x);
    hash = hash * 31 + std::hash<int>()(v.y);
    hash = hash * 31 + std::hash<int>()(v.z);
    return hash;
  }
};

class Vector3Compare {
 public:
  bool operator()(const Vector3 &lhs, const Vector3 &rhs) const {
    if (lhs.x != rhs.x) {
      return lhs.x < rhs.x;
    }
    if (lhs.y != rhs.y) {
      return lhs.y < rhs.y;
    }
    return lhs.z < rhs.z;
  }

  bool operator()(Vector3 &&lhs, const Vector3 &rhs) const {
    if (lhs.x != rhs.x) {
      return lhs.x < rhs.x;
    }
    if (lhs.y != rhs.y) {
      return lhs.y < rhs.y;
    }
    return lhs.z < rhs.z;
  }

  bool operator()(const Vector3 &lhs, Vector3 &&rhs) const {
    if (lhs.x != rhs.x) {
      return lhs.x < rhs.x;
    }
    if (lhs.y != rhs.y) {
      return lhs.y < rhs.y;
    }
    return lhs.z < rhs.z;
  }

  bool operator()(Vector3 &&lhs, Vector3 &&rhs) const {
    if (lhs.x != rhs.x) {
      return lhs.x < rhs.x;
    }
    if (lhs.y != rhs.y) {
      return lhs.y < rhs.y;
    }
    return lhs.z < rhs.z;
  }
};

}  // namespace xsf_data_structures

#endif  // VECTOR3_H