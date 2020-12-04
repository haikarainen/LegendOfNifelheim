
#pragma once

#include <LON/Export.hpp>

#include <KIT/Renderer/MaterialClass.hpp>
#include <KIT/Renderer/MaterialParameters.hpp>

#include <KIT/Assets/Texture.hpp>

#include <WIR/Class.hpp>

namespace lon
{
  class WaterMaterial;

  class LONAPI WaterMaterialParameters : public kit::MaterialParameters
  {
  public:
    WaterMaterialParameters(lon::WaterMaterial *materialClass);
    virtual ~WaterMaterialParameters();

    virtual bool deserialize(wir::Stream &fromData) override;

    virtual void updateEntity(kit::SceneParameters const &pScene, kit::EntityParameters const &pEntity) override;

    virtual bool isReady() override;
  protected:
    bool m_texturesSet = false;
  };

  class LONAPI WaterMaterial : public kit::MaterialClass
  {
    WIR_CLASS_DECLARATION()
  public:
    WaterMaterial(wir::DynamicArguments const &args);
    WaterMaterial(kit::Renderer *renderer);
    virtual ~WaterMaterial();

    virtual kit::MaterialParameters *createParameters() override;
    virtual void destroyParameters(kit::MaterialParameters *parameters) override;
    virtual odin::Program *regularProgram() const override;
    //virtual odin::Program *vrProgram() const override;

    virtual int64_t order() const override;

    virtual bool isReady() override;

    virtual void updateTarget() override;

    
    kit::TexturePtr terrainHeight() const
    {
      return m_terrainHeight;
    }


  protected:
    void initialize();
    /*
    struct
    {
      odin::Shader *vertexShader = nullptr;
      odin::Shader *fragmentShader = nullptr;
      odin::Program *program = nullptr;
    } m_vr;*/

    struct
    {
      odin::Shader *vertexShader = nullptr;
      odin::Shader *fragmentShader = nullptr;
      odin::Program *program = nullptr;
    } m_regular;

    kit::TexturePtr m_terrainHeight;
  };

} // namespace lon