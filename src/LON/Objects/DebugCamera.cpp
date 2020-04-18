#include "LON/Objects/DebugCamera.hpp"

#include <KIT/Engine.hpp>
#include <KIT/Managers/PhysicsManager.hpp>

lon::DebugCamera::DebugCamera()
{

}

lon::DebugCamera::DebugCamera(wir::DynamicArguments const & args)
{

}

lon::DebugCamera::~DebugCamera()
{ 

}

void lon::DebugCamera::onInitialize() 
{

}

void lon::DebugCamera::onSpawned()
{
  m_cameraComponent = spawnComponent<kit::VRCameraComponent>("cameraComponent");
  m_cameraComponent->attach(this);
  m_cameraComponent->primary();

  m_listenerComponent = spawnComponent<kit::ListenerComponent>("listenerComponent");
  m_listenerComponent->attach(this);
  m_listenerComponent->active(true);
}

void lon::DebugCamera::onDestroyed()
{

}

void lon::DebugCamera::onTick(double seconds)
{
  glm::quat finalRotation;
  finalRotation = glm::rotate(finalRotation, glm::radians(m_yaw), glm::vec3(0.0f, 1.0f, 0.0f));
  localRotation(finalRotation);
}

bool lon::DebugCamera::serialize(wir::Stream & toStream) const
{
  kit::Object::serialize(toStream);
  toStream << m_yaw;
  return true;
}

bool lon::DebugCamera::deserialize(wir::Stream & fromStream)
{
  kit::Object::deserialize(fromStream);
  fromStream >>  m_yaw;
  return true;
}

void lon::DebugCamera::yaw(float yawDelta)
{

  if (!m_cameraComponent)
    return;
  
  m_yaw += yawDelta;

  m_cameraComponent->yRotation(m_yaw);
}


float lon::DebugCamera::yaw() const
{
  return m_yaw;
}

void lon::DebugCamera::moveForward(float delta)
{
  if (!m_cameraComponent)
    return;

  m_offset += kit::Transformable::forward() * delta;
  m_cameraComponent->offset(m_offset);
}

void lon::DebugCamera::moveRight(float delta)
{
  if (!m_cameraComponent)
    return;

  m_offset += kit::Transformable::right() * delta;
  m_cameraComponent->offset(m_offset);
}

void lon::DebugCamera::moveUp(float delta)
{
  if (!m_cameraComponent)
    return;

  m_offset += kit::Transformable::up() * delta;
  m_cameraComponent->offset(m_offset);
}

kit::VRCameraComponent *lon::DebugCamera::cameraComponent() const
{
  return m_cameraComponent;
}
