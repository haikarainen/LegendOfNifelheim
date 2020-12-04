
#pragma once

#include <LON/Export.hpp>

#include <KIT/Game/Component.hpp>
#include <WIR/Class.hpp>

namespace lon
{
  class GridMeshEntity;

  class LONAPI WaterComponent : public kit::Component
  {
    WIR_CLASS_DECLARATION()

  public:
    /** Reflected constructor */
    WaterComponent(wir::DynamicArguments const &args);

    /** Constructor */
    WaterComponent();

    /** Destructor */
    virtual ~WaterComponent();

    /** Called on unspecified thread, when instance is created */
    virtual void onInitialize() override;

    /** Called on game thread, when it has entered play */
    virtual void onSpawned() override;

    /** Called on game thread, when it's been destroyed */
    virtual void onDestroyed() override;

    /** Called on game thread, every tick */
    virtual void onTick(double seconds) override;

    /** Begin wir::Serializable interface */
    virtual bool serialize(wir::Stream &toStream) const override;
    virtual bool deserialize(wir::Stream &fromStream) override;
    /** End wir::Serializable interface */

  protected:
    lon::GridMeshEntity *m_instance = nullptr;
  };
} // namespace lon