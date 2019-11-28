#pragma once

#include <mpi.h>

#include "./communication.hpp"

namespace communication {

namespace simple_mpi {

struct world {
  world(int &argc, char **&argv);
  world(world const &) = delete;
  world(world &&);
  world &operator=(world const &) = delete;
  world &operator=(world &&);
  ~world();

  double global_max(double t) const {
    double max;
    MPI_Allreduce(&t, &max, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    return max;
  }

  bool active = true;
};

struct grid {
  grid(vec<std::size_t, 3> const &global_resolution);
  grid(grid const &) = delete;
  grid(grid &&);
  grid &operator=(grid const &) = delete;
  grid &operator=(grid &&);
  ~grid();

  vec<std::size_t, 3> resolution;
  vec<std::size_t, 2> global_resolution, offset;
  MPI_Comm comm_cart = MPI_COMM_NULL;
};

inline grid comm_grid(world &, vec<std::size_t, 3> const &global_resolution) {
  return {global_resolution};
}

std::function<void(storage_t &)>
comm_halo_exchanger(grid const &grid, storage_t::storage_info_t const &sinfo);

} // namespace simple_mpi

} // namespace communication
