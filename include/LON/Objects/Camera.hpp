#pragma once

#include <KIT/Game/Object.hpp>
#include <KIT/Game/Components/CameraComponent.hpp>
#include <KIT/Game/Components/ListenerComponent.hpp>

#include <LON/Export.hpp>

namespace lon
{
  class LONAPI Camera : public kit::Object
  {
    WIR_CLASS_DECLARATION()

  public:
    Camera(wir::DynamicArguments const & args);
    Camera();

    virtual ~Camera();

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

    float distance() const;

    void distance(float newDistance);

    float yaw() const;

    void yaw(float newYaw);

    glm::vec3 focusPoint() const;

    void focusPoint(glm::vec3 newFocusPoint);

    kit::CameraComponent *cameraComponent() const;

  protected:

    // The point that the camera focuses on, in world coordinates
    glm::vec3 m_focusPoint;

    // distance from focus point, in meters
    float m_distance = 7.0f;

    // rotation around up axis, in degrees
    float m_yaw = 0.0f;

    // The internal camera component
    kit::CameraComponent *m_cameraComponent = nullptr;
    kit::ListenerComponent *m_listenerComponent = nullptr;
  };
}
