#pragma once

#include <iostream>

#include "../communication/communication.hpp"
#include "../verification/analytical.hpp"
#include "../verification/convergence.hpp"
#include "../verification/run.hpp"
#include "solver.hpp"

#include <thread>

template <class CommWorld> void run_convergence_tests(CommWorld &&comm_world, int num_threads = 1)
{
    {
        std::cout << "HORIZONTAL DIFFUSION: Spatial Convergence" << std::endl;
        analytical::horizontal_diffusion exact{0.05};
        auto error_f = [&comm_world, exact](std::size_t resolution) 
        {
            auto comm_grid = communication::grid_mt(comm_world, {resolution, resolution, resolution});
            using grid_t = communication::grid_mt::sub_grid_type;
            std::vector<std::thread> threads;
            threads.reserve(num_threads);
            std::vector<double> errors(num_threads, 0.0);

            for (int i=0; i<num_threads; ++i)
            {
                threads.push_back( std::thread{
                    [&ret,exact,errors](communication::grid_mt& grid, int thread_id)
                    {
                        errors[thread_id] = run(grid[thread_id], hdiff_stepper(exact.diffusion_coeff), 1e-4, 1e-5, exact);
                    },
                    std::ref(comm_grid),
                    i});
            }
            auto error = errors[0];
            for (unsigned int i=1; i<errors.size(); ++i)
                error = std::max(errors[i], error);
            return std::sqrt(communication::global_max(comm_grid, error));
        };
        print_order_verification_result(order_verification(error_f, 8, 128));
    }
}

