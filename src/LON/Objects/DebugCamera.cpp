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
  m_offsetTranslation = spawnComponent<kit::Component>("offsetTranslation");
  m_headTranslation = spawnComponent<kit::Component>("headTranslation");

  m_offsetRotation = spawnComponent<kit::Component>("offsetRotation");

  m_headRotation = spawnComponent<kit::Component>("headRotation");
  m_finalTransform = spawnComponent<kit::Component>("finalTransform");
  m_camera = spawnComponent<kit::CameraComponent>("cameraComponent");
  m_listener = spawnComponent<kit::ListenerComponent>("listenerComponent");


  m_headTranslation->attach(m_offsetTranslation);
  m_offsetRotation->attach(m_headTranslation);
  m_headRotation->attach(m_offsetRotation);
  m_finalTransform->attach(m_headRotation);
  attach(m_finalTransform);
  m_camera->attach(this);
  m_listener->attach(this);

}

void lon::DebugCamera::onSpawned()
{
  m_camera->primary();
  m_listener->active(true);
}

void lon::DebugCamera::onDestroyed()
{

}

void lon::DebugCamera::onTick(double seconds)
{
  if (!m_offsetRotation)
    return;

  auto newRotation = glm::rotate(glm::quat(), glm::radians(m_yaw), glm::vec3(0.0f, 1.0f, 0.0f));
  newRotation = glm::rotate(newRotation, glm::radians(m_pitch), glm::vec3(1.0f, 0.0f, 0.0f));

  m_offsetRotation->localRotation(newRotation);
}


void lon::DebugCamera::yaw(float yawDelta)
{
  m_yaw += yawDelta;
}

float lon::DebugCamera::pitch() const
{
  return m_pitch;
}

void lon::DebugCamera::pitch(float newPitch)
{
  m_pitch += newPitch;
  m_pitch = glm::clamp(m_pitch, -90.0f, 90.0f);
}


void lon::DebugCamera::moveForward(float delta)
{
  if (!m_offsetTranslation)
    return;

  m_offsetTranslation->translate(worldForward() * delta);
}

void lon::DebugCamera::moveRight(float delta)
{
  if (!m_offsetTranslation)
    return;

  m_offsetTranslation->translate(worldRight() * delta);
}

void lon::DebugCamera::moveUp(float delta)
{
  if (!m_offsetTranslation)
    return;

  m_offsetTranslation->translate(kit::Transformable::up() * delta);
}

kit::CameraComponent *lon::DebugCamera::cameraComponent() const
{
  return m_camera;
}
