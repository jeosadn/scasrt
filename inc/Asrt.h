#pragma once

#ifndef SCA_ASRT_H
#define SCA_ASRT_H

#include <memory>
#include <string>
#include <vector>

#include "Prop.h"
#include "Tlm.h"

namespace Scasrt {
class Asrt {
 public:  // protected
  std::string name_;
  int id_;
  bool id_set_;

 public:  // protected:
  std::shared_ptr<Prop> start_prop_ptr_;
  std::shared_ptr<Prop> stop_prop_ptr_;
  std::shared_ptr<Prop> eval_prop_ptr_;
  AsrtState state_;
  bool spawn_next_asrt_;
  int spawn_count_;
  bool is_leaf_;
  std::shared_ptr<Asrt> next_asrt_;
  std::vector<std::shared_ptr<const Tlm> > history_;
  std::shared_ptr<const Tlm> latest_tlm_;
  AsrtPass criteria_;

 public:
  explicit Asrt(const std::string& name, std::shared_ptr<Prop> eval_prop_ptr,
                std::shared_ptr<Prop> start_prop_ptr,
                std::shared_ptr<Prop> stop_prop_ptr, AsrtPass criteria);
  explicit Asrt(const std::string& name, std::shared_ptr<Prop> eval_prop_ptr,
                std::shared_ptr<Prop> start_prop_ptr,
                std::shared_ptr<Prop> stop_prop_ptr,
                std::shared_ptr<Asrt> next_asrt);
  explicit Asrt(const Asrt& asrt);
  ~Asrt() = default;

  void SetId(int id);

  std::string Name();

  std::shared_ptr<Asrt> GenNextAsrt();

  void Main(std::shared_ptr<const Tlm> tlm_ptr);
  AsrtStatus GetStatus();
  bool Passes(AsrtStatus status, AsrtPass criteria);
  AsrtPass GetCriteria();

  std::string ToString();
};
}  // namespace Scasrt

#endif
