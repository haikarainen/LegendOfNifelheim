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
  m_headTranslation = spawnComponent<kit::VRHeadComponent>("headTranslation");
  m_headTranslation->TrackRotation = false;

  m_offsetRotation = spawnComponent<kit::Component>("offsetRotation");
  m_headTranslation->OffsetRotation = m_offsetRotation;

  m_headRotation = spawnComponent<kit::VRHeadComponent>("headRotation");
  m_headRotation->TrackPosition = false;
  m_finalTransform = spawnComponent<kit::Component>("finalTransform");
  m_camera = spawnComponent<kit::VRCameraComponent>("cameraComponent");
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

}


void lon::DebugCamera::yaw(float yawDelta)
{

  if (!m_offsetRotation)
    return;
  
  m_offsetRotation->rotateY(yawDelta);
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

kit::VRCameraComponent *lon::DebugCamera::cameraComponent() const
{
  return m_camera;
}
