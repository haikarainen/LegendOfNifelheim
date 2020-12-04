
#include "LON/RenderEntities/GridMeshEntity.hpp"

#include <KIT/Renderer/RenderGeometry.hpp>

#include <WIR/Data.hpp>

lon::GridMeshEntity::GridMeshEntity(kit::Engine *engine, kit::MaterialPtr material, glm::uvec2 resolution, glm::vec2 size)
    : kit::RenderEntity(engine, material)
{
 
  std::vector<uint32_t> indices;
  std::vector<lon::GridMeshVertex> vertices;


  glm::vec2 cellSize = size / glm::vec2(resolution);
  for (uint32_t x = 0; x < resolution.x; x++)
  {
    for (uint32_t y = 0; y < resolution.y; y++)
    {
      glm::uvec2 index(x, y);
      glm::uvec2 endIndex(x+1, y+1);
      glm::vec2 position = cellSize * glm::vec2(index);
      glm::vec2 uv = glm::vec2(index) / glm::vec2(resolution);

      auto linearIndex = [&](glm::uvec2 input) -> uint32_t {
        return input.y * resolution.x + input.x; 
      };

      bool xEven = x % 2 == 0;
      bool yEven = y % 2 == 0;

      vertices.push_back({glm::vec3(position.x, 0.0f, position.y), glm::vec2(uv.x, uv.y)});

      if (y < resolution.y - 1 && x < resolution.x - 1)
      {
        if (xEven != yEven)
        {
          indices.push_back(linearIndex(index));
          indices.push_back(linearIndex(glm::uvec2(endIndex.x, index.y)));
          indices.push_back(linearIndex(endIndex));

          indices.push_back(linearIndex(index));
          indices.push_back(linearIndex(endIndex));
          indices.push_back(linearIndex(glm::uvec2(index.x, endIndex.y)));
        }
        else
        {
          indices.push_back(linearIndex(index));
          indices.push_back(linearIndex(glm::uvec2(endIndex.x, index.y)));
          indices.push_back(linearIndex(glm::uvec2(index.x, endIndex.y)));

          indices.push_back(linearIndex(glm::uvec2(endIndex.x, index.y)));
          indices.push_back(linearIndex(endIndex));
          indices.push_back(linearIndex(glm::uvec2(index.x, endIndex.y)));
        }
      }
    }

    // add end vertices here
    //vertices.push_back({glm::vec3(position.x, 0.0f, position.y), glm::vec2(uv.x, uv.y)});
    //vertices.push_back({glm::vec3(position.x, 0.0f, position.y), glm::vec2(uv.x, uv.y)});
  }

  m_geometry = new kit::RenderGeometry(m_engine, false);
  m_geometry->updateIndices(wir::data(indices.data()), indices.size() * sizeof(uint32_t), indices.size());
  m_geometry->updateVertices(wir::data(vertices.data()), vertices.size() * sizeof(GridMeshVertex), vertices.size());
}

lon::GridMeshEntity::~GridMeshEntity()
{
  delete m_geometry;
}

int64_t lon::GridMeshEntity::order() const
{
  return 100;
}

kit::RenderGeometry *lon::GridMeshEntity::geometry() const
{
  return m_geometry;
}

kit::RenderInstances *lon::GridMeshEntity::instances() const
{
  return nullptr;
}
