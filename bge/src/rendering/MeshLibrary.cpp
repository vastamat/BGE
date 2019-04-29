#include "rendering/MeshLibrary.h"

#include "logging/Log.h"
#include "math/Vec.h"

#include <tinyobj/tiny_obj_loader.h>

namespace bge
{

struct Vertex
{
  bool operator==(const Vertex& other) const
  {
    return m_Pos == other.m_Pos && m_TexCoords == other.m_TexCoords &&
           m_Normal == other.m_Normal;
  }

  Vec3f m_Pos;
  Vec3f m_Normal;
  Vec2f m_TexCoords;
};

Mesh MeshLibrary::GetMesh(const std::string& filepath)
{
  auto foundIt = m_MeshMap.find(filepath);
  if (foundIt != m_MeshMap.end())
  {
    return foundIt->second;
  }

  std::vector<Vertex> vertices;
  std::vector<uint32> indices;

  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string warn, err;

  bool loadSuccess = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
                                      filepath.c_str());

  BGE_CORE_ASSERT(loadSuccess, "Unable to load obj file")

  for (const auto& shape : shapes)
  {
    for (const auto& index : shape.mesh.indices)
    {
      Vertex vertex;

      vertex.m_Pos[0] = attrib.vertices[3 * index.vertex_index + 0];
      vertex.m_Pos[1] = attrib.vertices[3 * index.vertex_index + 1];
      vertex.m_Pos[2] = attrib.vertices[3 * index.vertex_index + 2];

      vertex.m_TexCoords[0] = attrib.texcoords[2 * index.texcoord_index + 0];
      vertex.m_TexCoords[1] = attrib.texcoords[2 * index.texcoord_index + 1];

      vertex.m_Normal[0] = attrib.normals[3 * index.normal_index + 0];
      vertex.m_Normal[1] = attrib.normals[3 * index.normal_index + 1];
      vertex.m_Normal[2] = attrib.normals[3 * index.normal_index + 2];

      // check if vertex doesn't exist
      auto foundIt = std::find(vertices.begin(), vertices.end(), vertex);
      if (foundIt == vertices.end())
      {
        indices.push_back(vertices.size());
        vertices.push_back(vertex);
      }
      else
      {
        // vertex already exists, add its index
        indices.push_back(std::distance(vertices.begin(), foundIt));
      }
    }
  }

  Mesh newMesh;

  newMesh.m_VertexBuffer = RenderDevice::CreateVertexBuffer(
      vertices.size(), sizeof(Vertex), vertices.data());

  VertexBufferLayout bufferLayout;
  bufferLayout.PushFloat(3, false); // first 3 floats (position)
  bufferLayout.PushFloat(3, false); // last 3 floats (normals)
  bufferLayout.PushFloat(2, false); // next 2 floats (texCoords)

  newMesh.m_VertexArray = RenderDevice::CreateVertexArray(
      &newMesh.m_VertexBuffer, &bufferLayout, 1);

  newMesh.m_IndexBuffer =
      RenderDevice::CreateIndexBuffer(indices.size(), indices.data());

  newMesh.m_IndicesCount = indices.size();

  m_MeshMap.insert(std::make_pair(filepath, newMesh));
  return newMesh;
}

void MeshLibrary::ClearLibrary()
{
  for (const auto& mesh : m_MeshMap)
  {
    RenderDevice::DestroyIndexBuffer(mesh.second.m_IndexBuffer);
    RenderDevice::DestroyVertexArray(mesh.second.m_VertexArray);
    RenderDevice::DestroyVertexBuffer(mesh.second.m_VertexBuffer);
  }

  m_MeshMap.clear();
}

} // namespace bge