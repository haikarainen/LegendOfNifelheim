#include "LON/Module.hpp"

#include "LON/SinglePlayerMode.hpp"

#include "LON/Objects/Camera.hpp"
#include "LON/Objects/DebugCamera.hpp"
#include "LON/Objects/PlayerCharacter.hpp"
#include "LON/Objects/PlayerController.hpp"
#include "LON/Objects/MapEditor.hpp"

#include "LON/Components/SkyDomeComponent.hpp"
#include "LON/Components/WaterComponent.hpp"

#include "LON/MaterialClasses/GrassMaterial.hpp"
#include "LON/MaterialClasses/SkyMaterial.hpp"
#include "LON/MaterialClasses/WaterMaterial.hpp"

#include <KIT/Engine.hpp>
#include <KIT/Managers/GameManager.hpp>
#include <KIT/Managers/InputManager.hpp>
#include <KIT/Configuration.hpp>

#include <WIR/Error.hpp>

lon::Module::Module(kit::Engine *engine)
  : kit::EngineModule(engine)
{
    
}

lon::Module::~Module()
{

}

void lon::Module::initialize(kit::Configuration *configuration)
{
  lon::Camera::initializeClass();
  lon::PlayerCharacter::initializeClass();
  lon::PlayerController::initializeClass();
  lon::MapEditor::initializeClass();
  lon::DebugCamera::initializeClass();
  lon::ProceduralGrass::initializeClass();
  
  lon::WaterComponent::initializeClass();
  lon::SkyDomeComponent::initializeClass();

  lon::GrassMaterial::initializeClass();
  lon::SkyMaterial::initializeClass();
  lon::WaterMaterial::initializeClass();



  auto game = gameManager();
  auto input = inputManager();
  auto playerOne = game->playerState(0);


  // Always start in singleplayermode
  m_singlePlayerMode = new lon::SinglePlayerMode(game);
  game->mode(m_singlePlayerMode);
}

void lon::Module::shutdown()
{
  engine()->gameManager()->mode(nullptr);

  delete m_singlePlayerMode;
}

void lon::Module::update(double seconds)
{

}

std::string const & lon::Module::name()
{
#if defined (LON_DEBUG)
  static const std::string moduleName = "Legend Of Nifelheim (DEBUG)";
#else 
  static const std::string moduleName = "Legend Of Nifelheim";
#endif
  return moduleName;
}

