
#include "LON/Objects/PlayerCharacter.hpp"

#include <KIT/Engine.hpp>
#include <KIT/Managers/AssetManager.hpp>

lon::PlayerCharacter::PlayerCharacter()
  : kit::RigidCharacter()
{

}



lon::PlayerCharacter::PlayerCharacter(wir::DynamicArguments const & args)
  : kit::RigidCharacter(args)
{

} 
  
lon::PlayerCharacter::~PlayerCharacter()
{

}

void lon::PlayerCharacter::onInitialize()
{
  kit::RigidCharacter::onInitialize();
  m_meshAsset = engine()->assetManager()->load<kit::Mesh>("Content/Player/PlayerMesh.asset");
}

void lon::PlayerCharacter::onSpawned()
{
  kit::RigidCharacter::onSpawned();

  //gravityLock(true);

  m_mesh = spawnComponent<kit::StaticMeshComponent>("playerMesh");
  m_mesh->mesh(m_meshAsset);
  m_mesh->attach(this);
}

void lon::PlayerCharacter::onDestroyed()
{
  kit::RigidCharacter::onDestroyed();
}

void lon::PlayerCharacter::onTick(double seconds)
{
  // Retrieve a clamped input magnitude as well as a normalized direction
  // And then reset the accumulated input
  glm::vec2 inputDirection = glm::normalize(m_accumulatedInput);
  float inputMagnitude = glm::clamp(glm::length(m_accumulatedInput), 0.0f, 1.0f);
  m_accumulatedInput = glm::vec2(0.0f, 0.0f);

  // If we want input, accelerate velocity
  if (inputMagnitude > 0.0f)
  {
    m_velocity += inputDirection * inputMagnitude * m_acceleration * float(seconds);
  }
  // Otherwise, deccelerate velocity
  else
  {
    float velocityMagnitude = glm::length(m_velocity);
    glm::vec2 velocityDirection = glm::normalize(m_velocity); 
    if (velocityMagnitude > 0.0f)
    {
      float timedDecceleration = m_decceleration * float(seconds); 
      velocityMagnitude -= glm::min(timedDecceleration, velocityMagnitude);
      m_velocity = velocityDirection * velocityMagnitude; 
    } 
  }

  // Clamp velocity to max
  float velocityMagnitude = glm::length(m_velocity);
  if (velocityMagnitude > m_maxVelocity)
  {
    glm::vec2 velocityDirection = glm::normalize(m_velocity);
    m_velocity = velocityDirection * m_maxVelocity;
  }

  // Integrate velocity in the rigid character 
  auto inVel = glm::vec3(m_velocity.x, 0.0f, m_velocity.y);
  inputVelocity(inVel);

  // Tick the rigid character
  kit::RigidCharacter::onTick(seconds);

  // Update the view direction
  auto moveVec = glm::vec3(inputDirection.x, 0.0f, inputDirection.y);
  if (glm::length(moveVec) > 0.0f)
  {
    m_moveDirection = moveVec;
    m_interpDirection = glm::mix(m_interpDirection, m_moveDirection, 10.0f*float(seconds));

    m_mesh->localForward(m_interpDirection);
  }

}

bool lon::PlayerCharacter::serialize(wir::Stream & toStream) const
{
  return kit::Character::serialize(toStream);
}

bool lon::PlayerCharacter::deserialize(wir::Stream & fromStream)
{
  return kit::Character::deserialize(fromStream);
}

void lon::PlayerCharacter::onReleased(kit::Controller *controller)
{

}

void lon::PlayerCharacter::onControlled(kit::Controller *controller)
{

}

void lon::PlayerCharacter::addMoveInput(glm::vec2 input)
{
  m_accumulatedInput += input;
}
