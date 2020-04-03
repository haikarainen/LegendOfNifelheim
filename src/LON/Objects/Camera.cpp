#include "LON/Objects/Camera.hpp"

#include <KIT/Engine.hpp>
#include <KIT/Managers/PhysicsManager.hpp>

lon::Camera::Camera()
{

}

lon::Camera::Camera(wir::DynamicArguments const & args)
{

}
 
lon::Camera::~Camera()
{ 

}
 
void lon::Camera::onInitialize() 
{

}

void lon::Camera::onSpawned()
{
  m_cameraComponent = spawnComponent<kit::CameraComponent>("cameraComponent");
  m_cameraComponent->attach(this);
  m_cameraComponent->primary();

  m_listenerComponent = spawnComponent<kit::ListenerComponent>("listenerComponent");
  m_listenerComponent->attach(this);
  m_listenerComponent->active(true);
}

void lon::Camera::onDestroyed()
{

}

void lon::Camera::onTick(double seconds)
{
  glm::quat finalRotation;
  finalRotation = glm::rotate(finalRotation, glm::radians(m_yaw), glm::vec3(0.0f, 1.0f, 0.0f));
  finalRotation = glm::rotate(finalRotation, glm::radians(-56.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  glm::vec3 finalPosition = m_focusPoint + ((finalRotation * glm::vec3(0.0f, 0.0f, -1.0f)) * m_distance);

  /*
  auto ray = engine()->physicsManager()->rayCast(m_focusPoint, finalPosition);
  if (ray.size() > 0)
  {

    finalPosition = ray.front().position + (finalRotation * glm::vec3(0.0f, 0.0f, -1.0f) * -0.1f);
  }
  */ 

  localRotation(finalRotation);
  localPosition(finalPosition);
}

bool lon::Camera::serialize(wir::Stream & toStream) const
{
  kit::Object::serialize(toStream);
  toStream << m_distance << m_yaw << m_focusPoint;
  return true;
}

bool lon::Camera::deserialize(wir::Stream & fromStream)
{
  kit::Object::deserialize(fromStream);
  fromStream >> m_distance >> m_yaw >> m_focusPoint;
  return true;
}

float lon::Camera::distance() const
{
  return m_distance;
}

void lon::Camera::distance(float newDistance)
{ 
  m_distance = newDistance;
}

float lon::Camera::yaw() const
{
  return m_yaw;
}

glm::vec3 lon::Camera::focusPoint() const
{
  return m_focusPoint;
}

kit::CameraComponent * lon::Camera::cameraComponent() const
{
  return m_cameraComponent;
}

void lon::Camera::focusPoint(glm::vec3 newFocusPoint)
{
  m_focusPoint = newFocusPoint;
}

void lon::Camera::yaw(float newYaw)
{
  m_yaw = newYaw;
}
