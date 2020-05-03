#pragma once

#include <KIT/Game/Object.hpp>
#include <KIT/Game/Components/VRCameraComponent.hpp>
#include <KIT/Game/Components/VRHeadComponent.hpp>
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

    float yaw() const;
    void yaw(float newYaw);

    void moveForward(float delta);
    void moveRight(float delta);
    void moveUp(float delta);

    
    kit::VRCameraComponent *cameraComponent() const;

  protected:

    // The internal camera component
    kit::VRCameraComponent *m_camera = nullptr;
    kit::ListenerComponent *m_listener = nullptr;

    kit::Component *m_offsetTranslation = nullptr;
    kit::Component *m_offsetRotation = nullptr;
    kit::VRHeadComponent *m_headRotation = nullptr;
    kit::VRHeadComponent *m_headTranslation = nullptr;

    kit::Component *m_finalTransform = nullptr;
    
  };
}
