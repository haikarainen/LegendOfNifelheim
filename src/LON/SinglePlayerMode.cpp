
#include "LON/SinglePlayerMode.hpp"

#include <KIT/Engine.hpp>

#include <KIT/Managers/AssetManager.hpp>
#include <KIT/Managers/GameManager.hpp>
#include <KIT/Managers/InputManager.hpp>
#include <KIT/Managers/WindowManager.hpp>

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

namespace
{
  float dd = 0.0f;
  float yaw = 0.0f;
  float pitch = 0.0f;
  bool mleft = false;
  lon::DebugCamera *camera;
  kit::Object *model;
  kit::Object *light;
} // namespace

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
  engine()->windowManager()->window()->captureMouse(true);

  m_playerState = engine()->gameManager()->playerState(0);

  m_playerState->bindButton("MoveHorizontal", "d", wir::BT_AsAxisPositive);
  m_playerState->bindButton("MoveHorizontal", "a", wir::BT_AsAxisNegative);

  m_playerState->bindButton("MoveVertical", "s", wir::BT_AsAxisPositive);
  m_playerState->bindButton("MoveVertical", "w", wir::BT_AsAxisNegative);

  m_playerState->bindAxis("LookHorizontal", "ma_x", wir::AT_Normal);
  m_playerState->bindAxis("LookVertical", "ma_y", wir::AT_Normal);
  m_playerState->bindAxis("Scroll", "ma_sy", wir::AT_Normal);
  m_playerState->bindButton("MouseLeft", "mb_left", wir::BT_AsAxisPositive);
  m_playerState->bindButton("MouseRight", "mb_right", wir::BT_AsAxisPositive);
  m_playerState->bindButton("MouseMiddle", "mb_middle", wir::BT_AsAxisPositive);
  m_playerState->bindButton("MouseExtra", "mb_extra1", wir::BT_AsAxisPositive);

  for (auto device : engine()->inputManager()->devices())
  {
    auto keyboard = dynamic_cast<wir::RawKeyboardDevice *>(device);
    auto mouse = dynamic_cast<wir::RawMouseDevice *>(device);
    if (keyboard)
    {
      keyboard->assign(m_playerState);
    }
    else if (mouse)
    {
      mouse->assign(m_playerState);
    }
  }

  m_playerState->getAxisEvent("MoveVertical") += [&](float delta) {
    camera->moveForward(delta * dd * 2.0f);
  };

  m_playerState->getAxisEvent("MoveHorizontal") += [&](float delta) {
    camera->moveRight(delta * dd * 2.0f);
  };

  m_playerState->getAxisEvent("LookHorizontal") += [&](float delta) {
    camera->yaw(delta * 0.3f);
  };

  m_playerState->getAxisEvent("LookVertical") += [&](float delta) {
    camera->pitch(delta * -0.3f);
  };

  m_playerState->getAxisEvent("MouseMiddle") += [&](float delta) {
    mleft = delta > 0.5f;
  };

  world()->start();
}

void lon::SinglePlayerMode::onModeDeactivated()
{
  world()->destroy();
  engine()->windowManager()->window()->captureMouse(false);
}

void lon::SinglePlayerMode::onWorldLoading()
{
}

void lon::SinglePlayerMode::onWorldStart()
{
  ::camera = world()->spawnObject<lon::DebugCamera>("Camera");
  ::model = world()->spawnObject<kit::Object>("Model");

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

  ::light = world()->spawnObject<kit::Object>("Light");
  auto lc = ::light->spawnComponent<kit::IBLComponent>("ibl");

  glm::vec3 randomBounds(10.0f, 0.0f, 10.0f);
  glm::vec3 randomStart(-randomBounds.x / 2.0f, -randomBounds.y / 2.0f, -randomBounds.z / 2.0f);
  glm::vec3 randomEnd(randomBounds.x / 2.0f, randomBounds.y / 2.0f, randomBounds.z / 2.0f);
  uint32_t count = 50;
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

    obj->localPosition(location);
  }
}

void lon::SinglePlayerMode::onWorldTick(double seconds)
{
  ::dd = seconds;
}

void lon::SinglePlayerMode::onWorldDestroyed()
{
}
