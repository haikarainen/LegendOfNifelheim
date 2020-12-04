
#pragma once

#include <LON/Export.hpp>

#include <KIT/Assets/Mesh.hpp>
#include <KIT/Renderer/RenderEntity.hpp>

namespace lon
{
  struct LONAPI GridMeshVertex
  {
    glm::vec3 position;
    glm::vec2 uv;
  };

  class LONAPI GridMeshEntity : public kit::RenderEntity
  {
  public:
    GridMeshEntity(kit::Engine *engine, kit::MaterialPtr mesh, glm::uvec2 resolution, glm::vec2 size);
    virtual ~GridMeshEntity();

    virtual int64_t order() const override;

    virtual kit::RenderGeometry *geometry() const override;
    virtual kit::RenderInstances *instances() const override;

  protected:
    kit::RenderGeometry *m_geometry = nullptr;
  };

} // namespace lon