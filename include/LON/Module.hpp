#pragma once

#include <KIT/EngineModule.hpp>

#include <LON/Export.hpp>

namespace kit
{
  class Engine;
  class Configuration;
}

namespace lon
{
  class SinglePlayerMode;

  class LONAPI Module : public kit::EngineModule
  {
  public:
    Module(kit::Engine *engine);
    virtual ~Module();

    virtual void initialize(kit::Configuration *configuration) override;
    virtual void shutdown() override;

    virtual void update(double seconds) override;

    virtual std::string const & name()override;

  protected:

    lon::SinglePlayerMode *m_singlePlayerMode = nullptr;
  };



}