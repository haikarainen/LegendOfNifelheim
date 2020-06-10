#pragma once

#include <KIT/Game/RigidCharacter.hpp>
#include <KIT/Game/Components/StaticMeshComponent.hpp>

#include <LON/Export.hpp>

namespace lon
{
  class LONAPI PlayerCharacter : public kit::RigidCharacter
  {
    WIR_CLASS_DECLARATION()

  public:
    PlayerCharacter(wir::DynamicArguments const & args);
    PlayerCharacter();

    virtual ~PlayerCharacter();

    /** Called on unspecified thread, when the instance is created. */
    virtual void onInitialize() override;

    /** Called on game thread, when it has entered play. */
    virtual void onSpawned() override;

    /** Called on game thread, when it's being destroyed, before destroying all of its components. */
    virtual void onDestroyed() override;

    /** Called on game thread, every tick, after ticking all components. */
    virtual void onTick(double seconds) override;

    //-- Begin wir::Serializable interface --//
    virtual bool serialize(wir::Stream & toStream) const override;
    virtual bool deserialize(wir::Stream & fromStream) override;
    //-- End wir::Serializable interface --//

    // -- Begin kit::Character interface --//
    virtual void onReleased(kit::Controller *controller) override;
    virtual void onControlled(kit::Controller *controller) override;
    // -- End kit::Character interface --//

    void addMoveInput(glm::vec2 input);

  protected:
    glm::vec2 m_accumulatedInput;
    kit::MeshPtr m_meshAsset;
    kit::StaticMeshComponent *m_mesh = nullptr;

    float m_acceleration = 10.0f; // 20m/s/s
    float m_decceleration = 16.0f; // 28m/s/s
    float m_maxVelocity = 3.0f; // 7m/s

    glm::vec2 m_velocity;
    glm::vec3 m_moveDirection;
    glm::vec3 m_interpDirection;
  };
}
