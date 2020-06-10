#pragma once

#include <KIT/Game/Object.hpp>

#include <KIT/Assets/Mesh.hpp>
#include <KIT/Game/Components/StaticMeshComponent.hpp>

#include <vector>
#include <array>

#include <LON/Export.hpp>

namespace lon
{

  class LONAPI MapEditor : public kit::Object
  {
    WIR_CLASS_DECLARATION()

    struct Theme 
    {
      kit::MeshPtr Top;
      kit::MeshPtr Side;
      kit::MeshPtr InnerCorner;
      kit::MeshPtr OuterCorner;
      kit::MeshPtr Ramp;
    };

    struct Tile
    {
      uint8_t height = 0;
      uint8_t themeId = 0;
    };

  public:
    MapEditor(wir::DynamicArguments const & args);
    MapEditor(glm::uvec2 const &size);

    virtual ~MapEditor();

    /** Called on unspecified thread, when the instance is created. */
    virtual void onInitialize() override;

    /** Called on game thread, when it has entered play. */
    virtual void onSpawned() override;

    /** Called on game thread, when it's being destroyed, before destroying all of its components. */
    virtual void onDestroyed() override;

    /** Called on game thread, every tick, after ticking all components. */
    virtual void onTick(double seconds) override;

    /** Begin wir::Serializable interface */
    virtual bool serialize(wir::Stream & toStream) const override;
    virtual bool deserialize(wir::Stream & fromStream) override;
    /** End wir::Serializable interface */





    void loadTheme(uint8_t id, std::string const& path);

    Tile& tileAt(glm::uvec2 const& idx);

    Tile& tileAt(glm::vec3 const& position);

    void generatePreview();

    void moveCursor(glm::ivec2 offset);

    void applyCursor();
    void deleteCursor();

  protected:
    void clearMeshComponents();

    kit::StaticMeshComponent* getMeshComponent(kit::MeshPtr mesh);

    glm::uvec2 m_size;
    std::vector<Tile> m_tiles;
    std::array<Theme, 8> m_themes;
    std::map<kit::MeshPtr, kit::StaticMeshComponent*> m_meshCache;
    std::vector<kit::StaticMeshComponent*> m_comps;

    kit::StaticMeshComponent *m_indicator = nullptr;
    glm::ivec2 m_cursor;
  };
}
