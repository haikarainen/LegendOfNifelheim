
#include "LON/SinglePlayerMode.hpp"

#include <KIT/Engine.hpp>

#include <KIT/Managers/GameManager.hpp>
#include <KIT/Managers/InputManager.hpp>
#include <KIT/Managers/AssetManager.hpp>
#include <KIT/Managers/WindowManager.hpp>

#include <KIT/Game/Object.hpp>
#include <KIT/Game/World.hpp>

#include <KIT/Game/PlayerState.hpp>

#include <KIT/Assets/Mesh.hpp>
#include <KIT/Assets/PhysicsMesh.hpp>

#include <KIT/Game/Components/StaticMeshComponent.hpp>
#include <KIT/Game/Components/BoxComponent.hpp>
#include <KIT/Game/Components/PhysicsMeshComponent.hpp>

#include <KIT/Renderer/RenderEntities/MeshInstance.hpp>
#include <KIT/Renderer/MaterialClasses/TestForward.hpp>

#include "LON/Objects/Camera.hpp"
#include "LON/Objects/DebugCamera.hpp"
#include "LON/Objects/PlayerCharacter.hpp"
#include "LON/Objects/PlayerController.hpp"
#include "LON/Objects/MapEditor.hpp"


#include <WIR/Rendering/RenderWindow.hpp>

namespace
{
  float dd = 0.0f;
  float yaw = 0.0f;
  float pitch = 0.0f;
  bool mleft = false;
  lon::Camera *camera;
  lon::MapEditor *mapEditor;
}

lon::SinglePlayerMode::SinglePlayerMode(wir::DynamicArguments const & args)
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

  m_playerState->bindButton("CursorRight", "right", wir::BT_Down);
  m_playerState->bindButton("CursorLeft", "left", wir::BT_Down);

  m_playerState->bindButton("CursorUp", "up", wir::BT_Down);
  m_playerState->bindButton("CursorDown", "down", wir::BT_Down);

  m_playerState->bindButton("CursorApply", "space", wir::BT_Down);
  m_playerState->bindButton("CursorDelete", "backspace", wir::BT_Down);

  for (auto device : engine()->inputManager()->devices())
  {
    auto keyboard = dynamic_cast<wir::RawKeyboardDevice*>(device);
    auto mouse = dynamic_cast<wir::RawMouseDevice*>(device);
    if (keyboard)
    {
      keyboard->assign(m_playerState);
    }
    else if (mouse)
    {
      mouse->assign(m_playerState);
    }
  }

  m_playerState->getAxisEvent("MoveVertical") += [&](float delta)
  {
    //camera->moveForward(delta * dd * 2.0f);
  };

  m_playerState->getAxisEvent("MoveHorizontal") += [&](float delta)
  {
    
    //camera->moveRight(delta * dd * 2.0f);
  };

  m_playerState->getAxisEvent("LookHorizontal") += [&](float delta)
  {
    yaw += delta * 0.3f;
    camera->yaw(yaw);
  };

  m_playerState->getAxisEvent("LookVertical") += [&](float delta)
  {
    pitch -= delta * 0.3f;
    //camera->pitch(pitch);
  };

  m_playerState->getAxisEvent("MouseMiddle") += [&](float delta)
  {
    mleft = delta > 0.5f;
  };

  m_playerState->getButtonEvent("CursorDown") += [&](){
    mapEditor->moveCursor(glm::ivec2(0, 1));
  }; 

  m_playerState->getButtonEvent("CursorUp") += [&](){
    mapEditor->moveCursor(glm::ivec2(0, -1));
  };

  m_playerState->getButtonEvent("CursorRight") += [&](){
    mapEditor->moveCursor(glm::ivec2(1, 0));
  };

  m_playerState->getButtonEvent("CursorLeft") += [&](){
    mapEditor->moveCursor(glm::ivec2(-1, 0));
  };

  m_playerState->getButtonEvent("CursorApply") += [&](){
    mapEditor->applyCursor();
  };

  m_playerState->getButtonEvent("CursorDelete") += [&](){
    mapEditor->deleteCursor();
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
  ::camera = world()->spawnObject<lon::Camera>("Camera");
  ::mapEditor = world()->spawnObject<lon::MapEditor>("Map Editor", glm::uvec2(16, 16));
}

void lon::SinglePlayerMode::onWorldTick(double seconds)
{
  ::dd = seconds;
}

void lon::SinglePlayerMode::onWorldDestroyed()
{
}
