#include "indexer/covered_object.hpp"

#include "indexer/feature_decl.hpp"

#include "coding/byte_stream.hpp"
#include "coding/geometry_coding.hpp"

namespace indexer
{
void CoveredObject::Deserialize(char const * data)
{
  ArrayByteSource src(data);
  serial::GeometryCodingParams cp = {};
  ReadPrimitiveFromSource(src, m_id);
  feature::GeomType type;
  ReadPrimitiveFromSource(src, type);

  if (type == feature::GeomType::Point)
  {
    m_points.push_back(serial::LoadPoint(src, cp));
    return;
  }

  if (type == feature::GeomType::Line)
  {
    uint32_t ptsCount{};
    ReadPrimitiveFromSource(src, ptsCount);
    CHECK_GREATER(ptsCount, 1, ());
    char const * start = src.PtrC();
    serial::LoadInnerPath(start, ptsCount, cp, m_points);
    return;
  }

  ASSERT_EQUAL(type, feature::GeomType::Area, ());
  uint32_t trgCount{};
  ReadPrimitiveFromSource(src, trgCount);
  CHECK_GREATER(trgCount, 0, ());
  trgCount += 2;
  char const * start = static_cast<char const *>(src.PtrC());
  serial::LoadInnerTriangles(start, trgCount, cp, m_triangles);
}
}  // namespace indexer
