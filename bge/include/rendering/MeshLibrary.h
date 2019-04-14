#pragma once

#include "Mesh.h"

#include "core/Common.h"

#include <unordered_map>

namespace bge
{

class MeshLibrary
{
public:
  Mesh GetMesh(const std::string& filepath);

  void ClearLibrary();

private:
  std::unordered_map<std::string, Mesh> m_MeshMap;
};

} // namespace bge