
#include "LON/Objects/PlayerController.hpp"
#include "LON/Objects/PlayerCharacter.hpp"

#include <KIT/Game/PlayerState.hpp>


lon::PlayerController::PlayerController()
  : kit::PlayerController()
{

}

lon::PlayerController::PlayerController(wir::DynamicArguments const & args)
  : kit::PlayerController(args)
{

}
 
lon::PlayerController::~PlayerController()
{

}

void lon::PlayerController::onInitialize()
{

}

void lon::PlayerController::onSpawned()
{

}

void lon::PlayerController::onDestroyed()
{

}

void lon::PlayerController::onTick(double seconds)
{

}

bool lon::PlayerController::serialize(wir::Stream & toStream) const
{
  return kit::PlayerController::serialize(toStream);
}

bool lon::PlayerController::deserialize(wir::Stream & fromStream)
{
  return kit::PlayerController::deserialize(fromStream);
}

void lon::PlayerController::onCharacterControlled(kit::Character *controlledCharacter)
{
}

void lon::PlayerController::onCharacterReleased(kit::Character *releasedCharacter)
{

}

void lon::PlayerController::onStateAssigned(kit::PlayerState *assignedState)
{
  assignedState->getAxisEvent("MoveHorizontal") += wir::MemberFunction(this, &lon::PlayerController::onMoveHorizontal);
  assignedState->getAxisEvent("MoveVertical") += wir::MemberFunction(this, &lon::PlayerController::onMoveVertical);
}

void lon::PlayerController::onStateUnassigned(kit::PlayerState *unassignedState)
{
  unassignedState->getAxisEvent("MoveHorizontal") -= wir::MemberFunction(this, &lon::PlayerController::onMoveHorizontal);
  unassignedState->getAxisEvent("MoveVertical") -= wir::MemberFunction(this, &lon::PlayerController::onMoveVertical);
}

void lon::PlayerController::onMoveHorizontal(float delta)
{
  auto c = character()->cast<lon::PlayerCharacter>();
  if (c)
  {
    c->addMoveInput(glm::vec2(delta, 0.0f));
  }
}

void lon::PlayerController::onMoveVertical(float delta)
{
  auto c = character()->cast<lon::PlayerCharacter>();
  if (c)
  {
    c->addMoveInput(glm::vec2(0.0f, delta));
  }
}
