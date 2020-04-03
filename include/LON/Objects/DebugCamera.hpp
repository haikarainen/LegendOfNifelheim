#pragma once

#include <KIT/Game/Object.hpp>
#include <KIT/Game/Components/CameraComponent.hpp>
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

    float pitch() const;
    void pitch(float newPitch);

    void moveForward(float delta);
    void moveRight(float delta);
    void moveUp(float delta);

    
    kit::CameraComponent *cameraComponent() const;

  protected:


    // rotation around up axis, in degrees
    float m_yaw = 0.0f;
    float m_pitch = 0.0f;

    // The internal camera component
    kit::CameraComponent *m_cameraComponent = nullptr;
    kit::ListenerComponent *m_listenerComponent = nullptr;
  };
}
