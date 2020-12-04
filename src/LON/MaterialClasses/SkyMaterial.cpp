
#include "LON/MaterialClasses/SkyMaterial.hpp"

#include "LON/RenderEntities/SkyDomeEntity.hpp"

#include "KIT/Assets/Material.hpp"
#include "KIT/KXF/KXFMesh.hpp"
#include "KIT/Managers/AssetManager.hpp"
#include "KIT/Managers/RenderManager.hpp"
#include "KIT/Managers/VRManager.hpp"
#include "KIT/Renderer/Renderer.hpp"


lon::SkyMaterialParameters::SkyMaterialParameters(lon::SkyMaterial *materialClass)
    : kit::MaterialParameters(materialClass)
{
}

lon::SkyMaterialParameters::~SkyMaterialParameters()
{
}

bool lon::SkyMaterialParameters::deserialize(wir::Stream &fromData)
{
  auto assMan = materialClass()->renderer()->assetManager();
  //auto vrMan = materialClass()->renderer()->vrManager();

  auto rp = m_regular.parameters->uniformBuffer("parameters");
  rp->set("projectionMatrix", glm::mat4(1.0f));
  rp->set("invViewMatrix", glm::mat4(1.0f));

  /*if (vrMan->supported())
  {
    auto vp = m_vr.parameters->uniformBuffer("parameters");
    vp->set("projectionMatrixLeft", glm::mat4(1.0f));
    vp->set("invViewMatrixLeft", glm::mat4(1.0f));
    vp->set("projectionMatrixRight", glm::mat4(1.0f));
    vp->set("mvMatrixRight", glm::mat4(1.0f));
    vp->set("invViewMatrixRight", glm::mat4(1.0f));
  }*/

  return true;
}

void lon::SkyMaterialParameters::updateEntity(kit::SceneParameters const &pScene, kit::EntityParameters const &pEntity)
{
  auto renderer = materialClass()->renderer();
  auto camera = renderer->camera();
 // auto vrMan = renderer->vrManager();

  /*
  if (vrMan->supported())
  {
    auto vp = m_vr.parameters->uniformBuffer("parameters");
    vp->set("projectionMatrixLeft", camera->projectionMatrix(odin::VE_Left));
    vp->set("invViewMatrixLeft", glm::inverse(camera->viewEyeMatrix(odin::VE_Left)));
    vp->set("projectionMatrixRight", camera->projectionMatrix(odin::VE_Right));
    vp->set("invViewMatrixRight", glm::inverse(camera->viewEyeMatrix(odin::VE_Right)));
  }
  */
  {
    auto rp = m_regular.parameters->uniformBuffer("parameters");
    rp->set("projectionMatrix", camera->projectionMatrix(/*odin::VE_Left*/));
    rp->set("invViewMatrix", glm::inverse(camera->viewMatrix()));
  }
}

bool lon::SkyMaterialParameters::isReady()
{
  if (!m_texturesSet)
  {
    auto mc = materialClass()->cast<lon::SkyMaterial>();

    m_regular.parameters->set("irradiance", mc->defaultIrradiance()->texture(), mc->defaultIrradiance()->sampler());

    /*
    if (mc->renderer()->vrManager()->supported())
    {
      m_vr.parameters->set("irradiance", mc->defaultIrradiance()->texture(), mc->defaultIrradiance()->sampler());
    }
    */

    m_texturesSet = true;
  }
  return true;
}

lon::SkyMaterial::SkyMaterial(wir::DynamicArguments const &args)
    : kit::MaterialClass(args)
{
  initialize();
}

lon::SkyMaterial::SkyMaterial(kit::Renderer *renderer)
    : kit::MaterialClass(renderer)
{
  initialize();
}

lon::SkyMaterial::~SkyMaterial()
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

kit::MaterialParameters *lon::SkyMaterial::createParameters()
{
  return new lon::SkyMaterialParameters(this);
}

void lon::SkyMaterial::destroyParameters(kit::MaterialParameters *parameters)
{
  delete parameters;
}

odin::Program *lon::SkyMaterial::regularProgram() const
{
  return m_regular.program;
}

/*
odin::Program *lon::SkyMaterial::vrProgram() const
{
  return m_vr.program;
}
*/

int64_t lon::SkyMaterial::order() const
{
  return 10000;
}

bool lon::SkyMaterial::isReady()
{
  return (m_defaultIrradiance && m_defaultIrradiance->ready());
}

void lon::SkyMaterial::updateTarget()
{
}

void lon::SkyMaterial::initialize()
{
  auto context = m_renderer->renderManager()->context();
  odin::PipelineSettings settings;
  settings.samples = 8;
  settings.depthCompareOp = odin::CO_Greater;
  //settings.cullingMode = odin::CM_None;

  /*
  if (m_renderer->vrManager()->supported())
  {
    m_vr.fragmentShader = new odin::Shader(context, odin::SS_Fragment, "Content/Nature/Sky/Shaders/Sky.fragment.glsl", {{"VR", "1"}});
    m_vr.vertexShader = new odin::Shader(context, odin::SS_Vertex, "Content/Nature/Sky/Shaders/Sky.vertex.glsl", {{"VR", "1"}});
    m_vr.program = new odin::Program(context, {m_vr.vertexShader, m_vr.fragmentShader}, settings, 1);
    m_vr.program->vertexStride(sizeof(lon::SkyDomeVertex));
    m_vr.program->link();
  }
  */

  m_regular.fragmentShader = new odin::Shader(context, odin::SS_Fragment, "Content/Nature/Sky/Shaders/Sky.fragment.glsl");
  m_regular.vertexShader = new odin::Shader(context, odin::SS_Vertex, "Content/Nature/Sky/Shaders/Sky.vertex.glsl");
  m_regular.program = new odin::Program(context, {m_regular.vertexShader, m_regular.fragmentShader}, settings, 1);
  m_regular.program->vertexStride(sizeof(lon::SkyDomeVertex));
  m_regular.program->link();

  m_defaultIrradiance = m_renderer->assetManager()->load<kit::Texture>("Content/Nature/HDRi/Irradiance.asset");
}
