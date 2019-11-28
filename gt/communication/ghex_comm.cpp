#include "./ghex_comm.hpp"

#include <ghex/glue/gridtools/field.hpp>

namespace communication {

namespace ghex_comm {

std::function<void(storage_t &)>
comm_halo_exchanger(grid &g, storage_t::storage_info_t const &sinfo) {
  auto co_ptr = &g.co();
  auto patterns_ptr = &g.patterns();
  const auto domain_id = g.domain_id();
  return [co_ptr, patterns_ptr, domain_id](const storage_t &storage) mutable {
    auto &co = *co_ptr;
    auto &patterns = *patterns_ptr;

    auto field = ::gridtools::ghex::wrap_gt_field(domain_id, storage);

    co.exchange(patterns(field)).wait();
  };
}

} // namespace ghex_comm

} // namespace communication
