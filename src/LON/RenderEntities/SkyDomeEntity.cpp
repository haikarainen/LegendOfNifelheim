
#include "LON/RenderEntities/SkyDomeEntity.hpp"

#include <KIT/Renderer/RenderGeometry.hpp>

#include <WIR/Data.hpp>


lon::SkyDomeEntity::SkyDomeEntity(kit::Engine *engine, kit::MaterialPtr material)
  : kit::RenderEntity(engine, material)
{
  static const glm::vec3 origins[5] =
  {
          glm::vec3(-1.0, -1.0, -1.0),
          glm::vec3(1.0, -1.0, -1.0),
          glm::vec3(1.0, -1.0, 1.0),
          glm::vec3(-1.0, -1.0, 1.0),
          //glm::vec3(-1.0, 1.0, -1.0),
          glm::vec3(-1.0, -1.0, 1.0)
  };

  static const glm::vec3 rights[5] =
  {
          glm::vec3(2.0, 0.0, 0.0),
          glm::vec3(0.0, 0.0, 2.0),
          glm::vec3(-2.0, 0.0, 0.0),
          glm::vec3(0.0, 0.0, -2.0),
          //glm::vec3(2.0, 0.0, 0.0),
          glm::vec3(2.0, 0.0, 0.0)
  };

  static const glm::vec3 ups[5] =
  {
          glm::vec3(0.0, 2.0, 0.0),
          glm::vec3(0.0, 2.0, 0.0),
          glm::vec3(0.0, 2.0, 0.0),
          glm::vec3(0.0, 2.0, 0.0),
          //glm::vec3(0.0, 0.0, 2.0),
          glm::vec3(0.0, 0.0, -2.0)
  };

  const float size = 1000.0f;
  const uint32_t subdivisions = 1;

  std::vector<uint32_t> indices;
  std::vector<lon::SkyDomeVertex> vertices;

  const double step = 1.0 / double(subdivisions);
  const glm::vec3 step3(step, step, step);

  for (uint32_t face = 0; face < 5; ++face)
  {
    auto origin = origins[face];
    auto right = rights[face];
    auto up = ups[face];
    for (uint32_t j = 0; j < subdivisions + 1; ++j)
    {
      const glm::vec3 j3(j, j, j);
      for (uint32_t i = 0; i < subdivisions + 1; ++i)
      {
        glm::vec3 i3(i, i, i);
        glm::vec3 p = origin + step3 * (i3 * right + j3 * up);
        glm::vec3 p2 = p * p;
        glm::vec3 n(p.x * std::sqrt(1.0 - 0.5 * (p2.y + p2.z) + p2.y * p2.z / 3.0),
                    p.y * std::sqrt(1.0 - 0.5 * (p2.z + p2.x) + p2.z * p2.x / 3.0),
                    p.z * std::sqrt(1.0 - 0.5 * (p2.x + p2.y) + p2.x * p2.y / 3.0));

        vertices.push_back({n * size});
      }
    }
  }

  auto addQuad = [&](uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    indices.push_back(a);
    indices.push_back(b);
    indices.push_back(c);
    indices.push_back(a);
    indices.push_back(c);
    indices.push_back(d);
  };

  auto addQuadAlt = [&](uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    indices.push_back(a);
    indices.push_back(b);
    indices.push_back(d);
    indices.push_back(b);
    indices.push_back(c);
    indices.push_back(d);
  };

  uint32_t k = subdivisions + 1;
  for (uint32_t face = 0; face < 5; ++face)
  {
    for (uint32_t j = 0; j < subdivisions; ++j)
    {
      bool bottom = j < (subdivisions / 2);
      for (uint32_t i = 0; i < subdivisions; ++i)
      {
        bool left = i < (subdivisions / 2);
        uint32_t a = (face * k + j) * k + i;
        uint32_t b = (face * k + j) * k + i + 1;
        uint32_t c = (face * k + j + 1) * k + i;
        uint32_t d = (face * k + j + 1) * k + i + 1;
        if (bottom ^ left)
        {
          addQuadAlt(a, c, d, b);
        }
        else
        {
          addQuad(a, c, d, b);
        }
      }
    }
  }


  m_geometry = new kit::RenderGeometry(m_engine, false);
  m_geometry->updateIndices(wir::data(indices.data()), indices.size() * sizeof(uint32_t), indices.size());
  m_geometry->updateVertices(wir::data(vertices.data()), vertices.size() * sizeof(SkyDomeVertex), vertices.size());


}

lon::SkyDomeEntity::~SkyDomeEntity()
{
  delete m_geometry;
}

int64_t lon::SkyDomeEntity::order() const
{
  return 999999;
}

kit::RenderGeometry *lon::SkyDomeEntity::geometry() const
{
  return m_geometry;
}

kit::RenderInstances *lon::SkyDomeEntity::instances() const
{
  return nullptr;
}
