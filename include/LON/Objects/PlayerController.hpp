#pragma once

#include <KIT/Game/PlayerController.hpp>

#include <LON/Export.hpp>

namespace lon
{
  class LONAPI PlayerController : public kit::PlayerController
  {
    WIR_CLASS_DECLARATION()

  public:
    PlayerController(wir::DynamicArguments const & args);
    PlayerController();

    virtual ~PlayerController();

    /** Called on unspecified thread, when the instance is created. */
    virtual void onInitialize() override;

    /** Called on game thread, when it has entered play. */
    virtual void onSpawned() override;

    /** Called on game thread, when it's being destroyed, before destroying all of its components. */
    virtual void onDestroyed() override;

    /** Called on game thread, every tick, after ticking all components. */
    virtual void onTick(double seconds) override;

    /** Begin wir::Serializable interface */
    virtual bool serialize(wir::Stream & toStream) const override;
    virtual bool deserialize(wir::Stream & fromStream) override;
    /** End wir::Serializable interface */

    //-- Begin kit::Controller itnerface --//
    virtual void onCharacterControlled(kit::Character *controlledCharacter) override;
    virtual void onCharacterReleased(kit::Character *releasedCharacter) override;
    //-- End kit::Controller itnerface --//

    //-- Begin kit::PlayerController itnerface --//
    virtual void onStateAssigned(kit::PlayerState *assignedState) override;
    virtual void onStateUnassigned(kit::PlayerState *unassignedState) override;
    //-- End kit::PlayerController itnerface --//


  protected:
    void onMoveHorizontal(float delta);
    void onMoveVertical(float delta);
  };
}
