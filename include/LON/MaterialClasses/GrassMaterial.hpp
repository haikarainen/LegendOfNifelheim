
#pragma once

#include <KIT/Export.hpp>

#include <KIT/Renderer/MaterialClass.hpp>
#include <KIT/Renderer/MaterialParameters.hpp>

#include <KIT/Assets/Texture.hpp>

#include <LON/Export.hpp>

namespace lon
{
  class GrassMaterial;

  struct LONAPI ProceduralGrassVertex
  {
    glm::vec3 root;
    glm::vec3 offset;
    glm::vec3 normal;
  };



  class LONAPI GrassMaterialParameters : public kit::MaterialParameters
  {
  public:
    GrassMaterialParameters(lon::GrassMaterial *materialClass);
    virtual ~GrassMaterialParameters();

    virtual bool deserialize(wir::Stream &fromData) override;

    virtual void updateEntity(kit::SceneParameters const &pScene, kit::EntityParameters const &pEntity) override;

    virtual bool isReady() override;

  protected:
    bool m_texturesSet = false;
  };

  class LONAPI GrassMaterial : public kit::MaterialClass
  {
    WIR_CLASS_DECLARATION()
  public:
    GrassMaterial(wir::DynamicArguments const &args);
    GrassMaterial(kit::Renderer *renderer);
    virtual ~GrassMaterial();

    virtual kit::MaterialParameters *createParameters() override;
    virtual void destroyParameters(kit::MaterialParameters *parameters) override;
    virtual odin::Program *regularProgram() const override;
    virtual odin::Program *vrProgram() const override;

    virtual int64_t order() const override;

    kit::TexturePtr defaultIrradiance() const
    {
      return m_defaultIrradiance;
    }

    kit::TexturePtr testMap() const
    {
      return m_testMap;
    }

    virtual bool isReady() override;

    virtual void updateTarget() override;

  protected:
    void initialize();

    bool m_texturesSet = false;

    kit::TexturePtr m_defaultIrradiance;
    kit::TexturePtr m_testMap;

    struct
    {
      odin::Shader *vertexShader = nullptr;
      odin::Shader *fragmentShader = nullptr;
      odin::Program *program = nullptr;
    } m_vr;

    struct
    {
      odin::Shader *vertexShader = nullptr;
      odin::Shader *fragmentShader = nullptr;
      odin::Program *program = nullptr;
    } m_regular;

  };

} // namespace lon