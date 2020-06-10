
#include "LON/MaterialClasses/GrassMaterial.hpp"

#include "KIT/Assets/Material.hpp"
#include "KIT/KXF/KXFMesh.hpp"
#include "KIT/Managers/AssetManager.hpp"
#include "KIT/Managers/RenderManager.hpp"
#include "KIT/Managers/VRManager.hpp"
#include "KIT/Renderer/Renderer.hpp"

lon::GrassMaterialParameters::GrassMaterialParameters(lon::GrassMaterial *materialClass)
    : kit::MaterialParameters(materialClass)
{
}

lon::GrassMaterialParameters::~GrassMaterialParameters()
{
}

bool lon::GrassMaterialParameters::deserialize(wir::Stream &fromData)
{
  auto assMan = materialClass()->renderer()->assetManager();
  auto vrMan = materialClass()->renderer()->vrManager();

  auto rp = m_regular.parameters->uniformBuffer("parameters");
  rp->set("mvpMatrix", glm::mat4(1.0f));
  rp->set("mvMatrix", glm::mat4(1.0f));
  rp->set("normalMatrix", glm::mat3(1.0f));
  rp->set("invViewMatrix", glm::mat4(1.0f));

  if (vrMan->supported())
  {
    auto vp = m_vr.parameters->uniformBuffer("parameters");
    vp->set("mvpMatrixLeft", glm::mat4(1.0f));
    vp->set("mvMatrixLeft", glm::mat4(1.0f));
    vp->set("normalMatrixLeft", glm::mat3(1.0f));
    vp->set("invViewMatrixLeft", glm::mat4(1.0f));
    vp->set("mvpMatrixRight", glm::mat4(1.0f));
    vp->set("mvMatrixRight", glm::mat4(1.0f));
    vp->set("normalMatrixRight", glm::mat3(1.0f));
    vp->set("invViewMatrixRight", glm::mat4(1.0f));
  }
  return true;
}

void lon::GrassMaterialParameters::updateEntity(kit::SceneParameters const &pScene, kit::EntityParameters const &pEntity)
{
  auto renderer = materialClass()->renderer();
  auto camera = renderer->camera();
  auto vrMan = renderer->vrManager();

  if (vrMan->supported())
  {
    auto mvMatrixLeft = camera->viewEyeMatrix(odin::VE_Left) * pEntity.modelMatrix;
    auto mvpMatrixLeft = camera->projectionMatrix(odin::VE_Left) * camera->viewEyeMatrix(odin::VE_Left) * pEntity.modelMatrix;
    glm::mat3 normalMatrixLeft = glm::transpose(glm::inverse(mvMatrixLeft));

    auto mvMatrixRight = camera->viewEyeMatrix(odin::VE_Right) * pEntity.modelMatrix;
    auto mvpMatrixRight = camera->projectionMatrix(odin::VE_Right) * camera->viewEyeMatrix(odin::VE_Right) * pEntity.modelMatrix;
    glm::mat3 normalMatrixRight = glm::transpose(glm::inverse(mvMatrixRight));

    auto vp = m_vr.parameters->uniformBuffer("parameters");
    vp->set("mvpMatrixLeft", mvpMatrixLeft);
    vp->set("mvMatrixLeft", mvMatrixLeft);
    vp->set("normalMatrixLeft", normalMatrixLeft);
    vp->set("invViewMatrixLeft", glm::inverse(camera->viewEyeMatrix(odin::VE_Left)));
    vp->set("mvpMatrixRight", mvpMatrixRight);
    vp->set("mvMatrixRight", mvMatrixRight);
    vp->set("normalMatrixRight", normalMatrixRight);
    vp->set("invViewMatrixRight", glm::inverse(camera->viewEyeMatrix(odin::VE_Right)));
  }

  {
    auto mvMatrix = camera->viewMatrix() * pEntity.modelMatrix;
    auto mvpMatrix = camera->projectionMatrix(odin::VE_Left) * camera->viewMatrix() * pEntity.modelMatrix;
    glm::mat3 normalMatrix = glm::transpose(glm::inverse(mvMatrix));

    auto rp = m_regular.parameters->uniformBuffer("parameters");
    rp->set("mvpMatrix", mvpMatrix);
    rp->set("mvMatrix", mvMatrix);
    rp->set("normalMatrix", normalMatrix);
    rp->set("invViewMatrix", glm::inverse(camera->viewMatrix()));
  }
}

bool lon::GrassMaterialParameters::isReady()
{
  if (!m_texturesSet)
  {
    auto mc = materialClass()->cast<lon::GrassMaterial>();

    m_regular.parameters->set("irradiance", mc->defaultIrradiance()->texture(), mc->defaultIrradiance()->sampler());
    m_regular.parameters->set("testMap", mc->testMap()->texture(), mc->testMap()->sampler());

    if (mc->renderer()->vrManager()->supported())
    {
      m_vr.parameters->set("irradiance", mc->defaultIrradiance()->texture(), mc->defaultIrradiance()->sampler());
      m_vr.parameters->set("testMap", mc->testMap()->texture(), mc->testMap()->sampler());
    }

    m_texturesSet = true;

  }
  return true;
}

lon::GrassMaterial::GrassMaterial(wir::DynamicArguments const &args)
    : kit::MaterialClass(args)
{
  initialize();
}

lon::GrassMaterial::GrassMaterial(kit::Renderer *renderer)
    : kit::MaterialClass(renderer)
{
  initialize();
}

lon::GrassMaterial::~GrassMaterial()
{
  delete m_regular.program;
  delete m_regular.fragmentShader;
  delete m_regular.vertexShader;

  if (renderer()->vrManager()->supported())
  {
    delete m_vr.program;
    delete m_vr.fragmentShader;
    delete m_vr.vertexShader;
  }
}

kit::MaterialParameters *lon::GrassMaterial::createParameters()
{
  return new lon::GrassMaterialParameters(this);
}

void lon::GrassMaterial::destroyParameters(kit::MaterialParameters *parameters)
{
  delete parameters;
}


odin::Program *lon::GrassMaterial::regularProgram() const
{
  return m_regular.program;
}

odin::Program *lon::GrassMaterial::vrProgram() const
{
  return m_vr.program;
}

int64_t lon::GrassMaterial::order() const
{
  return 0;
}

bool lon::GrassMaterial::isReady()
{
  return (m_defaultIrradiance && m_defaultIrradiance->status() == kit::AS_Ready);
}

void lon::GrassMaterial::updateTarget()
{

}

void lon::GrassMaterial::initialize()
{
  auto context = m_renderer->renderManager()->context();
  odin::PipelineSettings settings;
  settings.samples = 8;
  settings.depthCompareOp = odin::CO_Greater;
  //settings.cullingMode = odin::CM_None;

  if (m_renderer->vrManager()->supported())
  {
    m_vr.fragmentShader = new odin::Shader(context, odin::SS_Fragment, "Content/Nature/ProceduralGrass/Shaders/ProceduralGrass.fragment.glsl", {{"VR", "1"}});
    m_vr.vertexShader = new odin::Shader(context, odin::SS_Vertex, "Content/Nature/ProceduralGrass/Shaders/ProceduralGrass.vertex.glsl", {{"VR", "1"}});
    m_vr.program = new odin::Program(context, {m_vr.vertexShader, m_vr.fragmentShader}, settings, 4096);
    m_vr.program->vertexStride(sizeof(lon::ProceduralGrassVertex));
    m_vr.program->link();
  }

  m_regular.fragmentShader = new odin::Shader(context, odin::SS_Fragment, "Content/Nature/ProceduralGrass/Shaders/ProceduralGrass.fragment.glsl");
  m_regular.vertexShader = new odin::Shader(context, odin::SS_Vertex, "Content/Nature/ProceduralGrass/Shaders/ProceduralGrass.vertex.glsl");
  m_regular.program = new odin::Program(context, {m_regular.vertexShader, m_regular.fragmentShader}, settings, 4096);
  m_regular.program->vertexStride(sizeof(lon::ProceduralGrassVertex));
  m_regular.program->link();

  m_defaultIrradiance = m_renderer->assetManager()->load<kit::Texture>("Content/Nature/ProceduralGrass/HDRi/Irradiance.asset");
  m_testMap = m_renderer->assetManager()->load<kit::Texture>("Content/Nature/ProceduralGrass/Textures/TestMap.asset");
}
