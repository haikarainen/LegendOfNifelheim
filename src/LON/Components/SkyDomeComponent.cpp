
#include "LON/Components/SkyDomeComponent.hpp"

#include "LON/RenderEntities/SkyDomeEntity.hpp"

#include <KIT/Managers/AssetManager.hpp>
#include <KIT/Managers/RenderManager.hpp>
#include <KIT/Renderer/Renderer.hpp>

lon::SkyDomeComponent::SkyDomeComponent(wir::DynamicArguments const &args)
    : kit::Component(args)
{
  
}

lon::SkyDomeComponent::SkyDomeComponent()
{
}

lon::SkyDomeComponent::~SkyDomeComponent()
{
}

void lon::SkyDomeComponent::onInitialize()
{
  auto material = assetManager()->loadSync<kit::Material>("Content/Nature/Sky/Materials/SkyMaterial.asset");
  m_instance = new lon::SkyDomeEntity(engine(), material);
  renderManager()->renderer()->registerEntity(m_instance);
}

void lon::SkyDomeComponent::onSpawned()
{
}

void lon::SkyDomeComponent::onDestroyed()
{
  renderManager()->renderer()->unregisterEntity(m_instance);
  delete m_instance;
}

void lon::SkyDomeComponent::onTick(double seconds)
{
}

bool lon::SkyDomeComponent::serialize(wir::Stream &toStream) const
{
  return kit::Component::serialize(toStream);
}

bool lon::SkyDomeComponent::deserialize(wir::Stream &fromStream)
{
  return kit::Component::deserialize(fromStream);
}
