#include "LON/Module.hpp"

#include "LON/SinglePlayerMode.hpp"

#include "LON/Objects/Camera.hpp"
#include "LON/Objects/DebugCamera.hpp"
#include "LON/Objects/PlayerCharacter.hpp"
#include "LON/Objects/PlayerController.hpp"
#include "LON/Objects/MapEditor.hpp"

#include <KIT/Engine.hpp>
#include <KIT/Managers/GameManager.hpp>
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
  LogNotice("/*");
  LogNotice(" * Legend of Nifelheim - Initialize");
  LogNotice(" */");

  lon::Camera::initializeClass();
  lon::PlayerCharacter::initializeClass();
  lon::PlayerController::initializeClass();
  lon::MapEditor::initializeClass();
  lon::DebugCamera::initializeClass();

  m_singlePlayerMode = new lon::SinglePlayerMode(engine()->gameManager());

  // Always start in singleplayermode
  engine()->gameManager()->mode(m_singlePlayerMode);
}

void lon::Module::shutdown()
{
  LogNotice("/*");
  LogNotice(" * Legend of Nifelheim - Shutdown");
  LogNotice(" */");

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

