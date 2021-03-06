//////////////////////////////////////////////////////////////////////////////
/// Copyright (C) 2014 Gefu Tang <tanggefu@gmail.com>. All Rights Reserved.
///
/// This file is part of LSHBOX.
///
/// LSHBOX is free software: you can redistribute it and/or modify it under
/// the terms of the GNU General Public License as published by the Free
/// Software Foundation, either version 3 of the License, or(at your option)
/// any later version.
///
/// LSHBOX is distributed in the hope that it will be useful, but WITHOUT
/// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
/// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
/// more details.
///
/// You should have received a copy of the GNU General Public License along
/// with LSHBOX. If not, see <http://www.gnu.org/licenses/>.
///
/// @version 0.1
/// @author Gefu Tang & Zhifeng Xiao
/// @date 2014.6.30
//////////////////////////////////////////////////////////////////////////////

/**
 * @file scan_run.cpp
 *
 * @brief Linear scan dataset and construct benchmark.
 */
#include <lshbox.h>
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: ./scan_run data_file benchmark_file" << std::endl;
        return -1;
    }
    unsigned K = 100, Q = 1000, seed = 2;
    lshbox::timer timer;
    std::cout << "CREATE BENCHMARK FOR DATA ..." << std::endl;
    timer.restart();
    std::string file(argv[1]);
    std::string query_file(argv[2]);
    lshbox::Matrix<float> data(file);
    lshbox::Benchmark bench;
    bench.init(Q, K, data.getSize(), seed);
    lshbox::Metric<float> metric(data.getDim(), L1_DIST);
    lshbox::progress_display pd(Q);
    for (unsigned i = 0; i != Q; ++i)
    {
        unsigned q = bench.getQuery(i);
        lshbox::Topk &topk = bench.getAnswer(i);
        for (unsigned j = 0; j != data.getSize(); ++j)
        {
            if (q == j)
            {
                continue;
            }
            topk.push(j, metric.dist(data[q], data[j]));
        }
        topk.genTopk();
        ++pd;
    }
    bench.save(query_file);
    std::cout << "MEAN QUERY TIME: " << timer.elapsed() / Q << "s." << std::endl;
}