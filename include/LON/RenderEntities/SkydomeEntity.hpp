
#pragma once

#include <LON/Export.hpp>

#include <KIT/Assets/Mesh.hpp>
#include <KIT/Renderer/RenderEntity.hpp>

namespace lon
{
  struct LONAPI SkyDomeVertex
  {
    glm::vec3 position;
  };

  class LONAPI SkyDomeEntity : public kit::RenderEntity
  {
  public:
    SkyDomeEntity(kit::Engine *engine, kit::MaterialPtr mesh);
    virtual ~SkyDomeEntity();

    virtual int64_t order() const override;

    virtual kit::RenderGeometry *geometry() const override;
    virtual kit::RenderInstances *instances() const override;

  protected:
    kit::RenderGeometry *m_geometry = nullptr;
  };

} // namespace kit