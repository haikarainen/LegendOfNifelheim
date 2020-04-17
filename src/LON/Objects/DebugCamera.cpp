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
  m_cameraComponent = spawnComponent<kit::CameraComponent>("cameraComponent");
  m_cameraComponent->attach(this);
  m_cameraComponent->primary();
  m_cameraComponent->whitepoint(4.0f); 

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
  finalRotation = glm::rotate(finalRotation, glm::radians(m_pitch), glm::vec3(1.0f, 0.0f, 0.0f));
  localRotation(finalRotation);
}

bool lon::DebugCamera::serialize(wir::Stream & toStream) const
{
  kit::Object::serialize(toStream);
  toStream << m_pitch << m_yaw;
  return true;
}

bool lon::DebugCamera::deserialize(wir::Stream & fromStream)
{
  kit::Object::deserialize(fromStream);
  fromStream >> m_pitch >> m_yaw;
  return true;
}

void lon::DebugCamera::pitch(float pitchDelta)
{
  m_pitch += pitchDelta;

  if (m_pitch > 90.0f)
  {
    m_pitch = 90.0f;
  }
  if (m_pitch < -90.0f)
  {
    m_pitch = -90.0f;
  }
} 



void lon::DebugCamera::yaw(float yawDelta)
{
  m_yaw += yawDelta;
}


float lon::DebugCamera::yaw() const
{
  return m_yaw;
}



void lon::DebugCamera::moveForward(float delta)
{
  translate(localForward() * delta);
}

void lon::DebugCamera::moveRight(float delta)
{
  translate(localRight() * delta);
}

void lon::DebugCamera::moveUp(float delta)
{
  translate(up() * delta);
}


float lon::DebugCamera::pitch() const
{
  return m_pitch;
}


kit::CameraComponent *lon::DebugCamera::cameraComponent() const
{
  return m_cameraComponent;
}
