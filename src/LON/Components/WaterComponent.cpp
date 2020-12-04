
#include "LON/Components/WaterComponent.hpp"

#include "LON/RenderEntities/GridMeshEntity.hpp"

#include <KIT/Managers/AssetManager.hpp>
#include <KIT/Managers/RenderManager.hpp>
#include <KIT/Renderer/Renderer.hpp>

lon::WaterComponent::WaterComponent(wir::DynamicArguments const &args)
    : kit::Component(args)
{
}

lon::WaterComponent::WaterComponent()
{
}

lon::WaterComponent::~WaterComponent()
{
}

void lon::WaterComponent::onInitialize()
{
  auto material = assetManager()->loadSync<kit::Material>("Content/Nature/Water/Materials/WaterMaterial.asset");
  m_instance = new lon::GridMeshEntity(engine(), material, glm::uvec2(64, 64), glm::vec2(50.f, 50.f));
  m_instance->attach(this);
  renderManager()->renderer()->registerEntity(m_instance);
}

void lon::WaterComponent::onSpawned()
{
}

void lon::WaterComponent::onDestroyed()
{
  renderManager()->renderer()->unregisterEntity(m_instance);
  delete m_instance;
}

void lon::WaterComponent::onTick(double seconds)
{
}

bool lon::WaterComponent::serialize(wir::Stream &toStream) const
{
  return kit::Component::serialize(toStream);
}

bool lon::WaterComponent::deserialize(wir::Stream &fromStream)
{
  return kit::Component::deserialize(fromStream);
}
