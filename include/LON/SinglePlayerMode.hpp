
#pragma once

#include <KIT/Game/GameMode.hpp>

#include "LON/Objects/PlayerCharacter.hpp"
#include "LON/Objects/PlayerController.hpp"
#include "LON/Objects/Camera.hpp"


namespace lon
{
  class SinglePlayerMode : public kit::GameMode 
  {
    WIR_CLASS_DECLARATION()
  public:
    SinglePlayerMode(wir::DynamicArguments const & args);
    SinglePlayerMode(kit::GameManager *manager);
    virtual ~SinglePlayerMode();

    virtual void onModeActivated() override;
    virtual void onModeDeactivated() override;

    virtual void onWorldLoading() override;
    virtual void onWorldStart() override;
    virtual void onWorldTick(double seconds) override;
    virtual void onWorldDestroyed() override;

    lon::PlayerCharacter *playerCharacter() const
    {
      return m_playerCharacter;
    }

    lon::PlayerController *playerController() const
    {
      return m_playerController;
    }

    lon::Camera *playerCamera() const
    {
      return m_playerCamera;
    }

  protected:

    kit::PlayerState *m_playerState = nullptr;
    lon::PlayerCharacter *m_playerCharacter = nullptr;
    lon::PlayerController *m_playerController = nullptr;
    lon::Camera *m_playerCamera = nullptr;
  };
}