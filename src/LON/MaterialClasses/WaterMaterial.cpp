
#include "LON/MaterialClasses/WaterMaterial.hpp"
#include "LON/RenderEntities/GridMeshEntity.hpp"

#include "KIT/Assets/Material.hpp"
#include "KIT/KXF/KXFMesh.hpp"
#include "KIT/Managers/AssetManager.hpp"
#include "KIT/Managers/RenderManager.hpp"
#include "KIT/Managers/VRManager.hpp"
#include "KIT/Renderer/Renderer.hpp"

lon::WaterMaterialParameters::WaterMaterialParameters(lon::WaterMaterial *materialClass)
    : kit::MaterialParameters(materialClass)
{
}

lon::WaterMaterialParameters::~WaterMaterialParameters()
{
}

bool lon::WaterMaterialParameters::deserialize(wir::Stream &fromData)
{
  auto rp = m_regular.parameters->uniformBuffer("parameters");
  rp->set("mvpMatrix", glm::mat4(1.0f));
  rp->set("mvMatrix", glm::mat4(1.0f));

  /*
  if (vrManager()->supported())
  {
    auto vp = m_vr.parameters->uniformBuffer("parameters");
    vp->set("mvpMatrixLeft", glm::mat4(1.0f));
    vp->set("mvMatrixLeft", glm::mat4(1.0f));
    vp->set("mvpMatrixRight", glm::mat4(1.0f));
    vp->set("mvMatrixRight", glm::mat4(1.0f));
  }
  */
  return true;
}

void lon::WaterMaterialParameters::updateEntity(kit::SceneParameters const &pScene, kit::EntityParameters const &pEntity)
{
  auto renderer = renderManager()->renderer();
  auto camera = renderer->camera();

  /*
  if (vrManager()->supported())
  {
    auto mvMatrixLeft = camera->viewEyeMatrix(odin::VE_Left) * pEntity.modelMatrix;
    auto mvpMatrixLeft = camera->projectionMatrix(odin::VE_Left) * camera->viewEyeMatrix(odin::VE_Left) * pEntity.modelMatrix;
    auto mvMatrixRight = camera->viewEyeMatrix(odin::VE_Right) * pEntity.modelMatrix;
    auto mvpMatrixRight = camera->projectionMatrix(odin::VE_Right) * camera->viewEyeMatrix(odin::VE_Right) * pEntity.modelMatrix;

    auto vp = m_vr.parameters->uniformBuffer("parameters");
    vp->set("mvpMatrixLeft", mvpMatrixLeft);
    vp->set("mvMatrixLeft", mvMatrixLeft);
    vp->set("mvpMatrixRight", mvpMatrixRight);
    vp->set("mvMatrixRight", mvMatrixRight);
    vp->set("seconds", pScene.seconds);
    vp->set("cameraForward", camera->worldForward());
    vp->set("cameraPosition", camera->worldPosition());
  }
  */

  {
    auto mvMatrix = camera->viewMatrix() * pEntity.modelMatrix;
    auto mvpMatrix = camera->projectionMatrix(/*odin::VE_Left*/) * camera->viewMatrix() * pEntity.modelMatrix;

    auto rp = m_regular.parameters->uniformBuffer("parameters");
    rp->set("mvpMatrix", mvpMatrix);
    rp->set("mvMatrix", mvMatrix);
    rp->set("seconds", pScene.seconds);
    rp->set("cameraForward", camera->worldForward());
    rp->set("cameraPosition", camera->worldPosition());
  }
}

bool lon::WaterMaterialParameters::isReady()
{
  if (!m_texturesSet)
  {
    auto mc = materialClass()->cast<lon::WaterMaterial>();

    m_regular.parameters->set("terrainHeight", mc->terrainHeight()->texture(), mc->terrainHeight()->sampler());

    /*
    if (vrManager()->supported())
    {
      m_vr.parameters->set("terrainHeight", mc->terrainHeight()->texture(), mc->terrainHeight()->sampler());
    }
    */

    m_texturesSet = true;
  }
  return true;
}

lon::WaterMaterial::WaterMaterial(wir::DynamicArguments const &args)
    : kit::MaterialClass(args)
{
  initialize();
}

lon::WaterMaterial::WaterMaterial(kit::Renderer *renderer)
    : kit::MaterialClass(renderer)
{
  initialize();
}

lon::WaterMaterial::~WaterMaterial()
{
  delete m_regular.program;
  delete m_regular.fragmentShader;
  delete m_regular.vertexShader;

  /*
  if (renderer()->vrManager()->supported())
  {
    delete m_vr.program;
    delete m_vr.fragmentShader;
    delete m_vr.vertexShader;
  }
  */
}

kit::MaterialParameters *lon::WaterMaterial::createParameters()
{
  return new lon::WaterMaterialParameters(this);
}

void lon::WaterMaterial::destroyParameters(kit::MaterialParameters *parameters)
{
  delete parameters;
}

odin::Program *lon::WaterMaterial::regularProgram() const
{
  return m_regular.program;
}

/*
odin::Program *lon::WaterMaterial::vrProgram() const
{
  return m_vr.program;
}
*/

int64_t lon::WaterMaterial::order() const
{
  return 999999000;
}

bool lon::WaterMaterial::isReady()
{
  return (m_terrainHeight && m_terrainHeight->ready());
}

void lon::WaterMaterial::updateTarget()
{
}

void lon::WaterMaterial::initialize()
{
  auto context = renderManager()->context();
  odin::PipelineSettings settings;
  settings.samples = 8;
  settings.depthCompareOp = odin::CO_Greater;
  settings.attachmentStates = { odin::AlphaBlend };
  //settings.cullingMode = odin::CM_None;

  /*
  if (vrManager()->supported())
  {
    m_vr.fragmentShader = new odin::Shader(context, odin::SS_Fragment, "Content/Nature/Water/Shaders/Water.fragment.glsl", {{"VR", "1"}});
    m_vr.vertexShader = new odin::Shader(context, odin::SS_Vertex, "Content/Nature/Water/Shaders/Water.vertex.glsl", {{"VR", "1"}});
    m_vr.program = new odin::Program(context, {m_vr.vertexShader, m_vr.fragmentShader}, settings, 4096);
    m_vr.program->vertexStride(sizeof(lon::GridMeshVertex));
    m_vr.program->link();
  }
  */

  m_regular.fragmentShader = new odin::Shader(context, odin::SS_Fragment, "Content/Nature/Water/Shaders/Water.fragment.glsl");
  m_regular.vertexShader = new odin::Shader(context, odin::SS_Vertex, "Content/Nature/Water/Shaders/Water.vertex.glsl");
  m_regular.program = new odin::Program(context, {m_regular.vertexShader, m_regular.fragmentShader}, settings, 4096);
  m_regular.program->vertexStride(sizeof(lon::GridMeshVertex));
  m_regular.program->link();

  m_terrainHeight = assetManager()->load<kit::Texture>("Content/Nature/ProceduralGrass/Textures/TerrainHeight.asset");
}
