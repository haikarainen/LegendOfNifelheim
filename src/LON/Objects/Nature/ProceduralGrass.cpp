#include "LON/Objects/Nature/ProceduralGrass.hpp"
#include "LON/MaterialClasses/GrassMaterial.hpp"

#include <KIT/Engine.hpp>
#include <KIT/Managers/PhysicsManager.hpp>
#include <KIT/Managers/AssetManager.hpp>
#include <KIT/Managers/RenderManager.hpp>

#include <KIT/Renderer/RenderGeometry.hpp>
#include <KIT/Renderer/Renderer.hpp>
#include <KIT/Renderer/RenderEntities/MeshInstance.hpp>
#include <KIT/Renderer/RenderInstances.hpp>

#include <vector>

lon::ProceduralGrass::ProceduralGrass()
{
}

lon::ProceduralGrass::ProceduralGrass(wir::DynamicArguments const &args)
{
}

lon::ProceduralGrass::~ProceduralGrass()
{
}

void lon::ProceduralGrass::onInitialize()
{
  m_material = assetManager()->loadSync<kit::Material>("Content/Nature/ProceduralGrass/Materials/GrassMaterial.asset");

  // Actual grass has approx. 1500 leafs per square meter.
  // 60-90cm high at peak (with flowers)
  // 10-20cm cut

  // 40-60cm, 500 leafs per square meter seems good enough

  // Roots  have 3-5 leafs in them

  //generatePatch(glm::vec2(0.4f, 0.6f), 0.5f, glm::vec2(5.0f, 5.0f), 500.0f);

  std::vector<ProceduralGrassVertex> patchVertices;
  std::vector<uint32_t> patchIndices;

  std::vector<ProceduralGrassVertex> leafVertices = {

      {{0.0f, 0.0f, 0.0f}, {0.0f, -1.0511f, 0.0f},      kit::Transformable::forward()},   // 0
      {{0.0f, 0.0f, 0.0f}, {-0.02407, -0.69759, 0.0f},  kit::Transformable::forward()}, // 1
      {{0.0f, 0.0f, 0.0f}, {0.02407, -0.69759, 0.0f},   kit::Transformable::forward()},  // 2
      {{0.0f, 0.0f, 0.0f}, {-0.032376, -0.34403, 0.0f}, kit::Transformable::forward()}, // 3 
      {{0.0f, 0.0f, 0.0f}, {0.032376, -0.34403, 0.0f},  kit::Transformable::forward()},  // 4
      {{0.0f, 0.0f, 0.0f}, {-0.02407, 0.009521, 0.0f},  kit::Transformable::forward()},  // 5
      {{0.0f, 0.0f, 0.0f}, {0.02407, 0.009521, 0.0f},   kit::Transformable::forward()},    // 6

      {{0.0f, 0.0f, 0.0f}, {0.0f, -1.0511f, 0.0f},      -kit::Transformable::forward()},      // 7
      {{0.0f, 0.0f, 0.0f}, {-0.02407, -0.69759, 0.0f},  -kit::Transformable::forward()},  // 8
      {{0.0f, 0.0f, 0.0f}, {0.02407, -0.69759, 0.0f},   -kit::Transformable::forward()},   // 9
      {{0.0f, 0.0f, 0.0f}, {-0.032376, -0.34403, 0.0f}, -kit::Transformable::forward()}, // 10
      {{0.0f, 0.0f, 0.0f}, {0.032376, -0.34403, 0.0f},  -kit::Transformable::forward()},  // 11
      {{0.0f, 0.0f, 0.0f}, {-0.02407, 0.009521, 0.0f},  -kit::Transformable::forward()},  // 12
      {{0.0f, 0.0f, 0.0f}, {0.02407, 0.009521, 0.0f},   -kit::Transformable::forward()}    // 13
  };

  auto leafIndices = [](uint32_t offset) -> std::vector<uint32_t> {
    return {
        // front
        offset + 0, offset + 2, offset + 1,
        offset + 1, offset + 2, offset + 4,
        offset + 1, offset + 4, offset + 3,
        offset + 3, offset + 4, offset + 6,
        offset + 3, offset + 6, offset + 5,

        // back
        offset + 1 + 7, offset + 2 + 7, offset + 0 + 7,
        offset + 4 + 7, offset + 2 + 7, offset + 1 + 7,
        offset + 3 + 7, offset + 4 + 7, offset + 1 + 7,
        offset + 6 + 7, offset + 4 + 7, offset + 3 + 7,
        offset + 5 + 7, offset + 6 + 7, offset + 3 + 7,
    };
  };

  auto addLeaf = [&](kit::Transformable transform) {
    uint32_t offset = patchVertices.size();
    auto newIndices = leafIndices(offset);
    patchIndices.insert(patchIndices.end(), newIndices.begin(), newIndices.end());

    for (auto vertex : leafVertices)
    {
      vertex.root = transform.localPosition();
      vertex.offset = glm::vec3(transform.worldTransformMatrix() * glm::vec4(vertex.offset, 1.0f)) - vertex.root;
      vertex.normal = transform.worldTransformMatrix() * glm::vec4(vertex.normal, 0.0f);
      patchVertices.push_back(vertex);
    }

  };

  float patchSize = 5.0f;

  uint32_t numRoots = 32;
  glm::uvec2 leafsInRoot(3, 7);
  glm::vec3 rotationRange(25.0f, 90.0f, 25.0f);
  glm::vec2 scaleRange(0.67f, 1.75f);
  //glm::vec3 rotationRange(0.0f, 0.f, 0.f);
  for (uint32_t x = 0; x < numRoots; x++)
  {
    for (uint32_t y = 0; y < numRoots; y++)
    {
      float step = (patchSize / float(numRoots));
      float halfStep = step / 2.0f;

      glm::vec3 rootOffset(wir::randomFloat(-halfStep, halfStep), 0.0f, wir::randomFloat(-halfStep, halfStep));
      glm::vec3 rootPosition(float(x) * step, 0.0f, float(y) * step);
      rootPosition += rootOffset;

      uint32_t numLeafs = wir::randomInt(leafsInRoot.x, leafsInRoot.y);
      for (uint32_t i = 0; i < numLeafs; i++)
      {
        kit::Transformable leafTransform;
        leafTransform.localPosition(rootPosition);
        leafTransform.localEuler(glm::vec3(wir::randomFloat(-rotationRange.x, rotationRange.x),
                                           wir::randomFloat(-rotationRange.y, rotationRange.y),
                                           wir::randomFloat(-rotationRange.z, rotationRange.z)));
        leafTransform.localScale(glm::vec3(wir::randomFloat(scaleRange.x, scaleRange.y)));
        addLeaf(leafTransform);
      }

    }
  }



  LogNotice("%i vertices, %i triangles", patchVertices.size(), patchIndices.size() / 3);
  kit::RenderGeometry *patchGeometry = new kit::RenderGeometry(engine(), false);
  patchGeometry->updateVertices(reinterpret_cast<uint8_t *>(patchVertices.data()), patchVertices.size() * sizeof(ProceduralGrassVertex), patchVertices.size());
  patchGeometry->updateIndices(reinterpret_cast<uint8_t *>(patchIndices.data()), patchIndices.size() * sizeof(uint32_t), patchIndices.size());

  auto newPatch = std::make_shared<kit::Mesh>(assetManager(), patchGeometry, m_material);
  m_patches.push_back(newPatch);
  auto newPatchInstance = new kit::MeshInstance(engine(), newPatch);


  std::vector<glm::vec4> instances;
  for (uint32_t x = 0; x < 10; x++)
  {
    for (uint32_t y = 0; y < 10; y++)
    {
      float xPos = float(x) * patchSize;
      float yPos = float(y) * patchSize;

      instances.push_back(glm::vec4(xPos, 0.0f, yPos, 1.0f));
    }
  }

  newPatchInstance->instances()->update(
    reinterpret_cast<uint8_t*>(instances.data()),
    instances.size() * sizeof(glm::vec4),
    instances.size()
  );
  


  m_chunks[newPatch] = newPatchInstance;

  auto renderer = renderManager()->renderer();
  renderer->registerEntity(newPatchInstance);
}

void lon::ProceduralGrass::onSpawned()
{
}

void lon::ProceduralGrass::onDestroyed()
{
  auto renderer = renderManager()->renderer();
  for(auto chunk : m_chunks)
  {
    renderer->unregisterEntity(chunk.second);
    delete chunk.second;
  }
}

void lon::ProceduralGrass::onTick(double seconds)
{
}

bool lon::ProceduralGrass::serialize(wir::Stream &toStream) const
{
  return kit::Object::serialize(toStream);
}

bool lon::ProceduralGrass::deserialize(wir::Stream &fromStream)
{
  return kit::Object::deserialize(fromStream);
}

kit::MeshPtr lon::ProceduralGrass::generatePatch(glm::vec2 leafScale, float leafPositionScatter, glm::vec2 chunkSize, float leafDensity)
{
  return kit::MeshPtr();
}
