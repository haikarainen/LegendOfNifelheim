
#pragma once

#include <KIT/Export.hpp>

#include <KIT/Renderer/MaterialClass.hpp>
#include <KIT/Renderer/MaterialParameters.hpp>

#include <KIT/Assets/Texture.hpp>

#include <LON/Export.hpp>

namespace lon
{
  class SkyMaterial;

  class LONAPI SkyMaterialParameters : public kit::MaterialParameters
  {
  public:
    SkyMaterialParameters(lon::SkyMaterial *materialClass);
    virtual ~SkyMaterialParameters();

    virtual bool deserialize(wir::Stream &fromData) override;

    virtual void updateEntity(kit::SceneParameters const &pScene, kit::EntityParameters const &pEntity) override;

    virtual bool isReady() override;

  protected:
    bool m_texturesSet = false;
  };

  class LONAPI SkyMaterial : public kit::MaterialClass
  {
    WIR_CLASS_DECLARATION()
  public:
    SkyMaterial(wir::DynamicArguments const &args);
    SkyMaterial(kit::Renderer *renderer);
    virtual ~SkyMaterial();

    virtual kit::MaterialParameters *createParameters() override;
    virtual void destroyParameters(kit::MaterialParameters *parameters) override;
    virtual odin::Program *regularProgram() const override;
    //virtual odin::Program *vrProgram() const override;

    virtual int64_t order() const override;

    kit::TexturePtr defaultIrradiance() const
    {
      return m_defaultIrradiance;
    }

    virtual bool isReady() override;

    virtual void updateTarget() override;

  protected:
    void initialize();

    bool m_texturesSet = false;

    kit::TexturePtr m_defaultIrradiance;
    /*
    struct
    {
      odin::Shader *vertexShader = nullptr;
      odin::Shader *fragmentShader = nullptr;
      odin::Program *program = nullptr;
    } m_vr;
    */
    struct
    {
      odin::Shader *vertexShader = nullptr;
      odin::Shader *fragmentShader = nullptr;
      odin::Program *program = nullptr;
    } m_regular;
  };

} // namespace lon