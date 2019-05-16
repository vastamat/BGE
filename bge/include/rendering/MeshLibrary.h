#pragma once

#include "Mesh.h"

#include "core/Common.h"

#include <unordered_map>

namespace bge
{

/**
 * Manages the loading and unloading of meshes
 */
class MeshLibrary
{
public:
  /**
   * Loads a mesh from file or returns an existing instance if already loaded
   * once
   * @param filepath the filepath to the mesh
   */
  Mesh GetMesh(const std::string& filepath);

  /**
   * Clears the resource map
   */
  void ClearLibrary();

private:
  // The resource map which maps filepaths to meshes
  std::unordered_map<std::string, Mesh> m_MeshMap;
};

} // namespace bge