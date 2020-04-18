#pragma once

#include <KIT/Game/Object.hpp>
#include <KIT/Game/Components/VRCameraComponent.hpp>
#include <KIT/Game/Components/ListenerComponent.hpp>

namespace lon
{
  class DebugCamera : public kit::Object
  {
    WIR_CLASS_DECLARATION()

  public:
    DebugCamera(wir::DynamicArguments const & args);
    DebugCamera();

    virtual ~DebugCamera();

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

    float yaw() const;
    void yaw(float newYaw);

    void moveForward(float delta);
    void moveRight(float delta);
    void moveUp(float delta);

    
    kit::VRCameraComponent *cameraComponent() const;

  protected:


    // rotation around up axis, in degrees
    float m_yaw = 0.0f;
    
    glm::vec3 m_offset;

    // The internal camera component
    kit::VRCameraComponent *m_cameraComponent = nullptr;
    kit::ListenerComponent *m_listenerComponent = nullptr;
  };
}
