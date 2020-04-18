
#pragma once

#include <KIT/Game/GameMode.hpp>
#include <KIT/Game/PlayerState.hpp>

#include "LON/Objects/DebugCamera.hpp"


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

  protected:

    kit::PlayerState *m_playerState = nullptr;
    lon::DebugCamera *m_debugCamera = nullptr;
  };
}