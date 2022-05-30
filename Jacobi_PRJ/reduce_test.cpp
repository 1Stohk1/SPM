/* -*- Mode: C++; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/* ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  As a special exception, you may use this file as part of a free software
 *  library without restriction.  Specifically, if other files instantiate
 *  templates or use macros or inline functions from this file, or you compile
 *  this file and link it with other files to produce an executable, this
 *  file does not by itself cause the resulting executable to be covered by
 *  the GNU General Public License.  This exception does not however
 *  invalidate any other reasons why the executable file might be covered by
 *  the GNU General Public License.
 *
 ****************************************************************************
 */
/* Author: Massimo
 * Date  : September 2014
 *
 */
/* Simple example demonstrating the usege of a single ParallelForReduce pattern
 * for the reduction of multiple variables having different type.
 *
 */

#include <ff/ff.hpp>
#include <ff/parallel_for.hpp>
#include "lib/utimer.hpp"
#include "lib/linear_system.hpp"

using namespace ff;
using namespace std;

struct ReductionVars
{
    ReductionVars() : u(0L) {}

    const double &sumd() const { return u.sumd; }
    double &sumd() { return u.sumd; }

    ReductionVars &operator+=(const double &v)
    {
        u.sumd += v;
        return *this;
    }

    ReductionVars &operator=(const double &v)
    {
        u.sumd += v;
        return *this;
    }

    union u
    {
        u(long x) : sumd(x) {}
        double sumd;
    } u;
};

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        cout << "Usage is: " << argv[0] << " Dimension_of_the_system(int) Number_of_iterations(int) Number_Workers(int) Check_Results[0/1](bool)" << endl;
        return (0);
    }
    int n_dim = atoi(argv[1]);
    int iterations = atoi(argv[2]);
    int nw = atoi(argv[3]);
    bool check = (atoi(argv[4]) == 0 ? false : true);
    srand(123);

    Linear_System ls(n_dim, check);

    ReductionVars R, Rzero;
    ParallelForReduce<ReductionVars> pfr(nw);

    auto reduceF = [](ReductionVars &R, const ReductionVars &r)
    {
        R += r.sumd();
    };
    {
        timer_raii tim("parallelo");
        for (int row = 0; row < n_dim; row++)
        {
            pfr.parallel_reduce(
                R, Rzero, 0, n_dim, 1, 0, [&](const long i, ReductionVars &R)
                {
                if (row != i)
                {
                    R += ls.A[row][i] * ls.x_old[i];
                } },
                reduceF, nw);
            ls.x_curr[row] = (ls.b[row] - R.sumd()) / ls.A[row][row];
            R = 0;
            ls.update();
        }
    }
    ls.print_results();
    return 0;
}