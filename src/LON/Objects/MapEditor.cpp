#include "LON/Objects/MapEditor.hpp"

#include <KIT/Engine.hpp>
#include <KIT/Managers/AssetManager.hpp>
#include <KIT/Renderer/RenderEntities/MeshInstance.hpp>

lon::MapEditor::MapEditor(glm::uvec2 const &size)
{
  m_size = size;
}

lon::MapEditor::MapEditor(wir::DynamicArguments const & args)
{
  if (args.size() != 1)
  {
    LogError("Wrong argument count for MapEditor, it takes a uvec2 size");
    return;
  }

}

lon::MapEditor::~MapEditor()
{ 

}

void lon::MapEditor::onInitialize() 
{

}

void lon::MapEditor::onSpawned()
{
  loadTheme(0, "Content/Terrain/Default");

  m_tiles.resize(m_size.x * m_size.y);

  m_indicator = spawnComponent<kit::StaticMeshComponent>("Indicator");
  m_indicator->mesh(m_themes[0].Top);
}

void lon::MapEditor::onDestroyed()
{

}

void lon::MapEditor::onTick(double seconds)
{
  static bool wasInitialized = false;
  if(!wasInitialized && ((m_themes[0].Top->status() == kit::AS_Ready) &&
    (m_themes[0].Side->status() == kit::AS_Ready) &&
    (m_themes[0].InnerCorner->status() == kit::AS_Ready) &&
    (m_themes[0].OuterCorner->status() == kit::AS_Ready) &&
    (m_themes[0].Ramp->status() == kit::AS_Ready)))
  {
    generatePreview();
    wasInitialized = true;
  }

  m_indicator->localPosition(glm::vec3(float(m_cursor.x) * 2.0f + 0.5f, tileAt(m_cursor).height*-1.5 +  -0.1f, float(-m_cursor.y) * 2.0f - 0.5f));

}

bool lon::MapEditor::serialize(wir::Stream & toStream) const
{
  kit::Object::serialize(toStream);
  return true;
}

bool lon::MapEditor::deserialize(wir::Stream & fromStream)
{
  kit::Object::deserialize(fromStream);
  return true;
}

void lon::MapEditor::loadTheme(uint8_t id, std::string const& path)
{
  auto am = engine()->assetManager();

  m_themes[id].Top = am->load<kit::Mesh>(path + "/Top.asset");
  m_themes[id].Side = am->load<kit::Mesh>(path + "/Side.asset");
  m_themes[id].InnerCorner = am->load<kit::Mesh>(path + "/InnerCorner.asset");
  m_themes[id].OuterCorner = am->load<kit::Mesh>(path + "/OuterCorner.asset");
  m_themes[id].Ramp = am->load<kit::Mesh>(path + "/Ramp.asset");

}

lon::MapEditor::Tile& lon::MapEditor::tileAt(glm::uvec2 const& idx)
{
  uint32_t n = m_size.y * glm::min(idx.y, m_size.y - 1) + glm::min(idx.x, m_size.x - 1);

  return m_tiles[n];
}

lon::MapEditor::Tile& lon::MapEditor::tileAt(glm::vec3 const& position)
{
  const float tileSize = 1.0f;

  auto p = glm::inverse(worldTransformMatrix()) * glm::vec4(position, 1.0f); 

  // @todo multiply copy of position with inverse transform matrix
  glm::uvec2 index;
  index.x = uint32_t(glm::max(0.0f, p.x / float(tileSize)));
  index.y = uint32_t(glm::max(0.0f, p.y / float(tileSize)));

  return tileAt(index);
}

void lon::MapEditor::generatePreview()
{
  clearMeshComponents();

  float stride = 2.0f;
  float strideHeight = -1.5f;

  for(uint8_t h = 0; h < 1; h++)
  {
  for (uint32_t x = 0; x < m_size.x; x++)
  {
    for (uint32_t y = 0; y < m_size.y; y++)
    {

      auto t_c  = tileAt(glm::uvec2(x, y));
      auto t_n  = tileAt(glm::uvec2(x, y - 1));
      auto t_ne = tileAt(glm::uvec2(x + 1, y - 1));
      auto t_e  = tileAt(glm::uvec2(x + 1, y));
      auto t_se = tileAt(glm::uvec2(x + 1, y + 1));
      auto t_s  = tileAt(glm::uvec2(x, y + 1));
      auto t_sw = tileAt(glm::uvec2(x - 1, y + 1));
      auto t_w  = tileAt(glm::uvec2(x - 1, y));
      auto t_nw = tileAt(glm::uvec2(x - 1, y - 1));
      
      auto c    = t_c.height  > h;
      auto n    = t_n.height  > h;
      auto ne   = t_ne.height > h;
      auto e    = t_e.height  > h;
      auto se   = t_se.height > h;
      auto s    = t_s.height  > h;
      auto sw   = t_sw.height > h;
      auto w    = t_w.height  > h;
      auto nw   = t_nw.height > h;
      
      auto& t = m_themes[t_c.themeId];
      
      kit::MeshPtr sub_nw = t.Top;
      kit::MeshPtr sub_ne = t.Top;
      kit::MeshPtr sub_se = t.Top;
      kit::MeshPtr sub_sw = t.Top;

      float sub_nwY = 0.0f;
      float sub_neY = 0.0f;
      float sub_seY = 0.0f;
      float sub_swY = 0.0f;

      uint8_t sub_nwH = 0;
      uint8_t sub_neH = 0;
      uint8_t sub_seH = 0;
      uint8_t sub_swH = 0;

      glm::vec2 sub_nwO;
      glm::vec2 sub_neO;
      glm::vec2 sub_seO;
      glm::vec2 sub_swO;

      if (c)
      {
        // sub_nw
        {
          if(n && w && nw)
          {
            sub_nw = t.Top;
            sub_nwY = 0.0f;
            sub_nwH = 1;
          }
          else if (n && w && !nw)
          {
            sub_nw = t.InnerCorner;
            sub_nwY = 180.0f;
            sub_nwO.x = 1.0f;
            sub_nwO.y = -1.0f;

          }
          else if(!n && !w)
          {
            sub_nw = t.OuterCorner;
            sub_nwY = 180.0f;
            sub_nwO.x = 1.0f;
            sub_nwO.y = -1.0f;
          }
          else if (!n && w)
          {
            sub_nw = t.Side;
            sub_nwY = 180.0f;
            sub_nwO.x = 1.0;
            sub_nwO.y = -1.0;
          }
          else if(n && !w)
          {
            sub_nw = t.Side;
            sub_nwY = 90.0f;
            sub_nwO.x = 1.0f;
          }
        }
      
        //sub_ne
        {
          if(n && e && ne)
          {
            sub_ne = t.Top;
            sub_neY = 0.0f;
            sub_neH = 1;
          }
          else if(n && e && !ne)
          {
            sub_ne = t.InnerCorner;
            sub_neY = -90.0f;
            sub_neO.y = -1.0f;
          }
          else if(!n && !e)
          {
            sub_ne = t.OuterCorner;
            sub_neY = -90.0f;
            sub_neO.y = -1.0f;
          }
          else if(!n && e)
          {
            sub_ne = t.Side;
            sub_neY = 180.0f;
            sub_neO.x = 1.0;
            sub_neO.y = -1.0;
          }
          else if(n && !e)
          {
            sub_ne = t.Side;
            sub_neY = -90.0f;
            sub_neO.y = -1.0f;
          }
        }
      
        //sub_sw
        {
          if(s && w && sw)
          {
            sub_sw = t.Top;
            sub_swY = 0.0f;
            sub_swH = 1;
          }
          else if(s && w && !sw)
          {
            sub_sw = t.InnerCorner;
            sub_swY = 90.0f;
            sub_swO.x = 1.0f;
          }
          else if(!s && !w)
          {
            sub_sw = t.OuterCorner;
            sub_swY = 90.0f;
            sub_swO.x = 1.0f;
          }
          else if(!s && w)
          {
            sub_sw = t.Side;
            sub_swY = 0.0f;
          }
          else if(s && !w)
          {
            sub_sw = t.Side;
            sub_swY = 90.0f;
            sub_swO.x = 1.0f;
          }
        }

        //sub_se
        {
          if(s && e && se)
          {
            sub_se = t.Top;
            sub_seY = 0.0f;
            sub_seH = 1;
          }
          else if(s && e && !se)
          {
            sub_se = t.InnerCorner;
            sub_seY = 0.0f;
          }
          else if(!s && !e)
          {
            sub_se = t.OuterCorner;
            sub_seY = 0.0f;

          }
          else if(!s && e)
          {
            sub_se = t.Side;
            sub_seY = 0.0f;
          }
          else if (s && !e)
          {
            sub_se = t.Side;
            sub_seY = -90.0f;
            sub_seO.y = -1.0f;
          }
            
        }
      
      }

      {
        
        kit::Transformable nwT;
        nwT.localPosition(glm::vec3(stride * float(x) + sub_nwO.x, strideHeight * float(h + sub_nwH), stride * -float(y) + sub_nwO.y));
        nwT.rotateY(sub_nwY);
        getMeshComponent(sub_nw)->meshInstance()->addInstance(nwT.worldTransformMatrix());
        

        /*
        auto newComp = spawnComponent<kit::StaticMeshComponent>("MeshNW");
        newComp->mesh(sub_nw);
        newComp->localPosition(glm::vec3(stride * float(x) + sub_nwO.x, strideHeight * float(h + sub_nwH), stride * -float(y) + sub_nwO.y));
        newComp->rotateY(sub_nwY);
        m_comps.push_back(newComp);
        */
      }
      {
        
        kit::Transformable neT;
        neT.localPosition(glm::vec3(stride * float(x) + sub_neO.x + 1.0f, strideHeight * float(h + sub_neH), stride * -float(y) + sub_neO.y));
        neT.rotateY(sub_neY);
        getMeshComponent(sub_ne)->meshInstance()->addInstance(neT.worldTransformMatrix());
        

        /*
        auto newComp = spawnComponent<kit::StaticMeshComponent>("MeshNE");
        newComp->mesh(sub_ne);
        newComp->localPosition(glm::vec3(stride * float(x) + sub_neO.x + 1.0f, strideHeight * float(h + sub_neH), stride * -float(y) + sub_neO.y));
        newComp->rotateY(sub_neY);
        m_comps.push_back(newComp);
        */
      }

      {
        
        kit::Transformable seT;
        seT.localPosition(glm::vec3(stride * float(x) + sub_seO.x + 1.0f, strideHeight * float(h + sub_seH), stride * -float(y) + sub_seO.y - 1.0f));
        seT.rotateY(sub_seY);
        getMeshComponent(sub_se)->meshInstance()->addInstance(seT.worldTransformMatrix());
        
        /*
        auto newComp = spawnComponent<kit::StaticMeshComponent>("MeshSE");
        newComp->mesh(sub_se);
        newComp->localPosition(glm::vec3(stride * float(x) + sub_seO.x + 1.0f, strideHeight * float(h + sub_seH), stride * -float(y) + sub_seO.y - 1.0f));
        newComp->rotateY(sub_seY);
        m_comps.push_back(newComp);
        */
      }

      {
        
        kit::Transformable swT;
        swT.localPosition(glm::vec3(stride * float(x) + sub_swO.x, strideHeight * float(h + sub_swH), stride * -float(y) + sub_swO.y - 1.0f));
        swT.rotateY(sub_swY);
        getMeshComponent(sub_sw)->meshInstance()->addInstance(swT.worldTransformMatrix());
        
        /*
        auto newComp = spawnComponent<kit::StaticMeshComponent>("MeshSW");
        newComp->mesh(sub_sw);
        newComp->localPosition(glm::vec3(stride * float(x) + sub_swO.x, strideHeight * float(h + sub_swH), stride * -float(y) + sub_swO.y - 1.0f));
        newComp->rotateY(sub_swY);
        m_comps.push_back(newComp);
        */
      }
    }
  }
  }
}

void lon::MapEditor::moveCursor(glm::ivec2 offset)
{
  m_cursor += offset;
}

void lon::MapEditor::applyCursor()
{
  tileAt(m_cursor).height = 1;
  generatePreview();
}

void lon::MapEditor::deleteCursor()
{
  tileAt(m_cursor).height = 0;
  generatePreview();
}

void lon::MapEditor::clearMeshComponents()
{
  for (auto p : m_meshCache)
  {
    p.second->destroy();
  }

  for (auto m : m_comps)
  {
    m->destroy();
  }
  m_comps.clear();

  m_meshCache.clear();
}

kit::StaticMeshComponent* lon::MapEditor::getMeshComponent(kit::MeshPtr mesh)
{
  auto finder = m_meshCache.find(mesh);
  if (finder == m_meshCache.end())
  {
    kit::StaticMeshComponent* newComponent = spawnComponent<kit::StaticMeshComponent>("Mesh");
    newComponent->mesh(mesh);
    m_meshCache[mesh] = newComponent;
    return newComponent;
  }

  return finder->second;
}
