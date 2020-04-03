

#include <KIT/EngineModule.hpp>

#include "LON/Module.hpp"

extern "C"
{
  LONAPI kit::EngineModule *allocateModuleInstance(kit::Engine *engine)
  {
    return new lon::Module(engine);
  }

  LONAPI void freeModuleInstance(kit::EngineModule *module)
  {
    delete module;
  }
}