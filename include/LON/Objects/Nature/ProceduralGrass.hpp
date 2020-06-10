#pragma once

#include <KIT/Game/Components/CameraComponent.hpp>
#include <KIT/Game/Components/ListenerComponent.hpp>
#include <KIT/Renderer/RenderEntities/MeshInstance.hpp>
#include <KIT/Assets/Mesh.hpp>
#include <KIT/Game/Object.hpp>

#include <LON/Export.hpp>

namespace lon
{

  class LONAPI ProceduralGrass : public kit::Object
  {
    WIR_CLASS_DECLARATION()

  public:
    ProceduralGrass(wir::DynamicArguments const &args);

    ProceduralGrass();

    virtual ~ProceduralGrass();

    /** Called on unspecified thread, when the instance is created. */
    virtual void onInitialize() override;

    /** Called on game thread, when it has entered play. */
    virtual void onSpawned() override;

    /** Called on game thread, when it's being destroyed, before destroying all of its components. */
    virtual void onDestroyed() override;

    /** Called on game thread, every tick, after ticking all components. */
    virtual void onTick(double seconds) override;

    /** Begin wir::Serializable interface */
    virtual bool serialize(wir::Stream &toStream) const override;
    virtual bool deserialize(wir::Stream &fromStream) override;
    /** End wir::Serializable interface */

    kit::MeshPtr generatePatch(glm::vec2 leafScale,
                           float leafPositionScatter,
                           glm::vec2 chunkSize,
                           float leafDensity);


  protected:
    std::vector<kit::MeshPtr> m_patches;
    std::map<kit::MeshPtr, kit::MeshInstance*> m_chunks;
    kit::MaterialPtr m_material;
  };
} // namespace lon
