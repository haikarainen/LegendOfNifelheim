
#include "LON/SinglePlayerMode.hpp"

#include <KIT/Engine.hpp>

#include <KIT/Assets/Texture.hpp>

#include <KIT/Managers/AssetManager.hpp>
#include <KIT/Managers/GameManager.hpp>
#include <KIT/Managers/InputManager.hpp>
#include <KIT/Managers/VRManager.hpp>
#include <KIT/Managers/RenderManager.hpp>
#include <KIT/Renderer/Renderer.hpp>

#include <KIT/Game/Object.hpp>
#include <KIT/Game/World.hpp>

#include <KIT/Game/PlayerState.hpp>

#include <KIT/Assets/Mesh.hpp>
#include <KIT/Assets/PhysicsMesh.hpp>

#include <KIT/Game/Components/BoxComponent.hpp>
#include <KIT/Game/Components/IBLComponent.hpp>
#include <KIT/Game/Components/PhysicsMeshComponent.hpp>
#include <KIT/Game/Components/StaticMeshComponent.hpp>

#include "LON/Objects/Camera.hpp"
#include "LON/Objects/DebugCamera.hpp"
#include "LON/Objects/MapEditor.hpp"
#include "LON/Objects/PlayerCharacter.hpp"
#include "LON/Objects/PlayerController.hpp"
#include "LON/Components/SkyDomeComponent.hpp"
#include "LON/Components/WaterComponent.hpp"

#include <KIT/Rendering/SpriteRenderer.hpp>

#include <WIR/String.hpp>

lon::SinglePlayerMode::SinglePlayerMode(wir::DynamicArguments const &args)
    : kit::GameMode(args)
{
}

lon::SinglePlayerMode::SinglePlayerMode(kit::GameManager *manager)
    : kit::GameMode(manager)
{
}

lon::SinglePlayerMode::~SinglePlayerMode()
{
}

void lon::SinglePlayerMode::onModeActivated()
{

  m_playerState = engine()->gameManager()->playerState(0);

  inputManager()->onDiscoveredDevice() += [=](auto dev) {
    dev->assign(m_playerState);
  };
  /*
  inputManager()->keyboard()->assign(m_playerState);
  inputManager()->mouse()->assign(m_playerState);
  */
  //m_playerState->bindAxis("MoveRight", "leftvr_horizontal", wir::AT_Normal);
  //m_playerState->bindAxis("MoveForward", "leftvr_vertical", wir::AT_Normal);
  //m_playerState->bindAxis("MoveUp", "rightvr_vertical", wir::AT_Normal);
  //m_playerState->bindAxis("LookHorizontal", "rightvr_horizontal", wir::AT_Normal);

  
  m_playerState->bindButton("MoveRight", "d", wir::BT_AsAxisPositive);
  m_playerState->bindButton("MoveForward", "w", wir::BT_AsAxisPositive);
  m_playerState->bindButton("MoveRight", "a", wir::BT_AsAxisNegative);
  m_playerState->bindButton("MoveForward", "s", wir::BT_AsAxisNegative);
  m_playerState->bindButton("MoveUp", "space", wir::BT_AsAxisPositive);
  m_playerState->bindButton("MoveUp", "ctrl", wir::BT_AsAxisNegative);

  m_playerState->bindAxis("LookHorizontal", "ma_x", wir::AT_Normal);
  m_playerState->bindAxis("LookVertical", "ma_y", wir::AT_Normal);

  /*
  m_playerState->bindButton("ToggleVR", "v", wir::BT_Down);

  m_playerState->getButtonEvent("ToggleVR") += [&]() {
    if (vrManager()->enabled())
      vrManager()->disable();
    else 
      vrManager()->enable();

  };
  */

  m_playerState->getAxisEvent("MoveForward") += [&](float delta)
  {
    if (!m_debugCamera)
      return;

    m_debugCamera->moveForward(delta * engine()->lastDelta() * 2.0f);
  };

  m_playerState->getAxisEvent("MoveUp") += [&](float delta) {
    if (!m_debugCamera)
      return;

    m_debugCamera->moveUp(delta * engine()->lastDelta() * 2.0f);
  };

  m_playerState->getAxisEvent("MoveRight") += [&](float delta)
  {
    if (!m_debugCamera)
      return;

    m_debugCamera->moveRight(delta * engine()->lastDelta() * 2.0f);
  };

  m_playerState->getAxisEvent("LookHorizontal") += [&](float delta)
  {
    if (!m_debugCamera)
      return;

    m_debugCamera->yaw(delta * engine()->lastDelta() * 10.0f);
  };

  m_playerState->getAxisEvent("LookVertical") += [&](float delta) {
    if (!m_debugCamera)
      return;

    m_debugCamera->pitch(-delta * engine()->lastDelta() * 10.0f);
  };

  auto font = assetManager()->loadSync<kit::Font>("Core/Fonts/System.asset");

  m_debugText = new kit::Text(font, 20.f, U"Procedural Grass Test");
  m_debugText->position(glm::vec2(8.0f, 8.0f));
  m_debugText->alignment(kit::TA_TopLeft);

  world()->start();
}

void lon::SinglePlayerMode::onModeDeactivated()
{
  delete m_debugText;
  world()->destroy();
}

void lon::SinglePlayerMode::onWorldLoading()
{
}

void lon::SinglePlayerMode::onWorldStart()
{
  m_debugCamera = world()->spawnObject<lon::DebugCamera>("Camera");
  
  m_grass = world()->spawnObject<lon::ProceduralGrass>("Grass");
  
  auto groundPlane = world()->spawnObject<kit::Object>("GroundPlane");
  auto groundMesh = groundPlane->spawnComponent<kit::StaticMeshComponent>("GroundMesh");
  groundMesh->mesh(assetManager()->load<kit::Mesh>("Content/Nature/Meshes/GroundPlane.asset"));
  groundMesh->attach(groundPlane);
  

  auto sky = world()->spawnObject<kit::Object>("Sky");
  auto skyDome = sky->spawnComponent<lon::SkyDomeComponent>("SkyDome");
  skyDome->attach(sky);


  /*
  auto water = world()->spawnObject<kit::Object>("Water");
  auto waterComp = water->spawnComponent<lon::WaterComponent>("Watercomp");
  waterComp->attach(water);
  

  auto water2 = world()->spawnObject<kit::Object>("Water2");
  auto waterComp2 = water2->spawnComponent<lon::WaterComponent>("Watercomp2");
  waterComp2->attach(water2);
  water2->translate(glm::vec3(0.0f, -2.0f, 0.0f));
  */

  /*
  std::vector<kit::MeshPtr> meshes = {
      assetManager()->load<kit::Mesh>("Content/Glock17/Glock17.asset"),
      assetManager()->load<kit::Mesh>("Content/Pipes/PipeLong.asset"),
      assetManager()->load<kit::Mesh>("Content/Pipes/PipeMedium.asset"),
      assetManager()->load<kit::Mesh>("Content/Pipes/PipeShort.asset"),
      assetManager()->load<kit::Mesh>("Content/Pipes/PipeTurn.asset"),
      assetManager()->load<kit::Mesh>("Content/Pipes/Pump.asset"),
      assetManager()->load<kit::Mesh>("Content/Pipes/SmallPipeLong.asset"),
      assetManager()->load<kit::Mesh>("Content/Pipes/SmallPipeMedium.asset"),
      assetManager()->load<kit::Mesh>("Content/Pipes/SmallPipeShort.asset"),
      assetManager()->load<kit::Mesh>("Content/Pipes/SmallPipeSmallTurn.asset"),
      assetManager()->load<kit::Mesh>("Content/Pipes/SmallPipeSmallUturn.asset"),
      assetManager()->load<kit::Mesh>("Content/Pipes/SmallPipeTurn.asset"),
  };

  glm::vec3 offset(0.0f, 0.0f, 7.0f);
  glm::vec3 randomBounds(10.0f, 0.0f, 10.0f);
  glm::vec3 randomStart(-randomBounds.x / 2.0f, 0.0f, -randomBounds.z / 2.0f);
  glm::vec3 randomEnd(randomBounds.x / 2.0f, 0.0f, randomBounds.z / 2.0f);

  uint32_t count = 100;
  for (uint32_t x = 0; x < count; x++)
  {
    glm::vec3 location(wir::randomFloat(randomStart.x, randomEnd.x),
                       wir::randomFloat(randomStart.y, randomEnd.y),
                       wir::randomFloat(randomStart.z, randomEnd.z));

    kit::MeshPtr mesh = meshes.at(wir::randomInt(0, meshes.size()-1));

    auto obj = world()->spawnObject<kit::Object>("RandomObject");
    auto comp = obj->spawnComponent<kit::StaticMeshComponent>("RandomComponent");
    comp->mesh(mesh);
    comp->attach(obj);

    obj->localPosition(offset + location);
  }*/
  

  auto gizmoObj = world()->spawnObject<kit::Object>("Gizmo");
  gizmoObj->localPosition(glm::vec3(0.0f, 0.0f, 0.0f));

  auto gizmoComp = gizmoObj->spawnComponent<kit::StaticMeshComponent>("GizmoMesh");
  auto gizmoMesh = assetManager()->load<kit::Mesh>("Core/Models/Gizmo.asset");
  gizmoComp->mesh(gizmoMesh);
  gizmoComp->attach(gizmoObj);

  /*
  auto dust2 = world()->spawnObject("Dust2");
  dust2->localScale(glm::vec3(0.1f, 0.1f, 0.1f));
  for (uint32_t i = 0; i < 30; i++)
  {
    auto newAsset = assetManager()->load<kit::Mesh>(wir::format("Content/Dust2/Dust2Mesh_%u.asset", i));
    auto newComp = dust2->spawnComponent<kit::StaticMeshComponent>("DustyMesh");
    newComp->mesh(newAsset);
    newComp->attach(dust2);
  }*/
  
}

void lon::SinglePlayerMode::onWorldTick(double seconds)
{
  auto camera = renderManager()->renderer()->camera();

  if (camera)
  {
    auto fwd = camera->worldForward();
    auto pos = camera->worldPosition();
    m_debugText->text(wir::utf8to32(wir::format("Position: %.2f %.2f %.2f\nForward: %.2f %.2f %.2f",
                                                pos.x, pos.y, pos.z,
                                                fwd.x, fwd.y, fwd.z)));
  }
  else 
  {
    m_debugText->text(wir::utf8to32("No signal"));
  }



  auto glockTex = assetManager()->load<kit::Texture>("Content/Glock17/Glock17Albedo.asset");

  struct QUAD
  {
    glm::vec2 position;
    glm::vec2 size;
    glm::vec4 color;
  };
  const static int quadNum = 1+(64*4)+(32)+(4*128);
  static std::array<QUAD, quadNum> quads;

  static bool init = false;
  if (!init)
  {
    for (uint32_t x = 0; x < quadNum; x++)
    {
      glm::vec2 pos(wir::randomFloat(0.0f, 1300.0f), wir::randomFloat(0.0f, 800.0f));
      glm::vec2 size(wir::randomFloat(4.0f, 256.0f));
      glm::vec4 hsl(wir::randomFloat(0.0f, 1.0f), wir::randomFloat(0.3f, 1.0f), wir::randomFloat(0.2f, 0.8f), wir::randomFloat(0.4f, 1.0f));

      quads[x] = {pos, size, wir::hslaToRgba(hsl)};
    }
    init = true;
  }

  if (glockTex->ready())
  {
    auto spriteRenderer = renderManager()->spriteRenderer();


    for (uint32_t x = 0; x < quadNum; x++)
    {
      spriteRenderer->renderSprite(quads[x].position, quads[x].size, quads[x].color,{0.0f, 0.0f, 1.0f, 1.0f}, glockTex->texture(), glockTex->sampler());
    }
    



    spriteRenderer->renderSprite({512.0f, 8.0f}, {128.0f, 256.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}, glockTex->texture(), glockTex->sampler());

      
  }
    
  m_debugText->render();
}

void lon::SinglePlayerMode::onWorldDestroyed()
{
}
