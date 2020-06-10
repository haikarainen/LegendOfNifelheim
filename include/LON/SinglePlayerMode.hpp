
#pragma once

#include <KIT/Game/GameMode.hpp>
#include <KIT/Game/PlayerState.hpp>
#include <KIT/Rendering/TextRenderer.hpp>

#include "LON/Objects/DebugCamera.hpp"
#include "LON/Objects/Nature/ProceduralGrass.hpp"

#include <LON/Export.hpp>

namespace lon
{
  class LONAPI SinglePlayerMode : public kit::GameMode 
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
    lon::ProceduralGrass *m_grass = nullptr;
    kit::PlayerState *m_playerState = nullptr;
    lon::DebugCamera *m_debugCamera = nullptr;
    kit::Text *m_debugText = nullptr;
  };
}
