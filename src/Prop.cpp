#include "Prop.h"

namespace Scasrt {
Prop::Prop(const std::string& probe, PropFuncRef eval)
    : probe_(probe), eval_(eval) {}

PropResult Prop::Main(const Tlm& tlm,
                      std::vector<std::shared_ptr<const Tlm> >& history) {
  if (probe_.compare(tlm.observation_point_)) {
    return PROP_SKIP;
  } else {
    return eval_(tlm, history);
  }
}

}  // namespace Scasrt
