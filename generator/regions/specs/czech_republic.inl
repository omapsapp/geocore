#include "generator/regions/collector_region_info.hpp"
#include "generator/regions/country_specifier.hpp"
#include "generator/regions/country_specifier_builder.hpp"
#include "generator/regions/region.hpp"

#include <string>
#include <vector>

namespace generator
{
namespace regions
{
namespace specs
{
class CzechRepublicSpecifier final : public CountrySpecifier
{
public:
  static std::vector<std::string> GetCountryNames() { return {"CzechRepublic"}; }

private:
  // CountrySpecifier overrides:
  PlaceLevel GetSpecificCountryLevel(Region const & region) const override;
};

REGISTER_COUNTRY_SPECIFIER(CzechRepublicSpecifier);

PlaceLevel CzechRepublicSpecifier::GetSpecificCountryLevel(Region const & region) const
{
  AdminLevel adminLevel = region.GetAdminLevel();
  switch (adminLevel)
  {
  case AdminLevel::Six: return PlaceLevel::Region;       // Regions
  case AdminLevel::Seven: return PlaceLevel::Subregion;  // Districts
  case AdminLevel::Eight: return PlaceLevel::Locality;   // Towns / village
  default: break;
  }

  return PlaceLevel::Unknown;
}
}  // namespace specs
}  // namespace regions
}  // namespace generator
