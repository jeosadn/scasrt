#pragma once

#ifndef SCA_PROP_H
#define SCA_PROP_H

#include <functional>
#include <memory>
#include <vector>

#include "Tlm.h"

#define L_PROP               \
  [](const Scasrt::Tlm& tlm, \
     std::vector<std::shared_ptr<const Scasrt::Tlm> >& history)

namespace Scasrt {
using PropFunc = PropResult(
    const Tlm& Tlm, std::vector<std::shared_ptr<const Scasrt::Tlm> >& history);
using PropFuncRef = std::function<PropFunc>;

class Prop {
 public:
  const std::string probe_;
  PropFuncRef eval_;
  explicit Prop(const std::string& probe, PropFuncRef eval);
  virtual PropResult Main(const Tlm& tlm,
                          std::vector<std::shared_ptr<const Tlm> >& history);
  virtual ~Prop() = default;
};

}  // namespace Scasrt

#endif
