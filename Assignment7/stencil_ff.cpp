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
 * Date  : July 2014
 *         
 */
/*
 * This test tests the ff_Map pattern (aka ParallelForReudce) in a pipeline computation.
 * The structure is:
 *
 *    pipeline( farm(mapWorker,2) , mapStage );
 *
 */

#include <vector>
#include <ff/config.hpp>
#include <iostream>
#if !defined(HAS_CXX11_VARIADIC_TEMPLATES)
#define HAS_CXX11_VARIADIC_TEMPLATES 1
#endif
#include <ff/parallel_for.hpp>
#include <ff/ff.hpp>
#include <ff/map.hpp>


using namespace ff;
const long MYSIZE = 100;

typedef  std::vector<float> ff_task_t;

#ifdef min
#undef min //MD workaround to avoid clashing with min macro in minwindef.h
#endif

struct mapWorker : ff_node_t<ff_task_t> {
    ff_task_t *svc(ff_task_t *) {
        ff_task_t *A = new ff_task_t(MYSIZE);
        
        // this is the parallel_for provided by the ff_Map class
        parallel_for(0,A->size(),[&A](const long i) { 
                A->operator[](i)=0;
		}, std::min(3, (int)ff_realNumCores()));
        A->operator[](MYSIZE/2)=25;
        A->operator[](MYSIZE-1)=100;
        ff_send_out(A);
        return EOS;
    }
};


struct stencilStage: ff_Map<ff_task_t> {
    ff_task_t *svc(ff_task_t *inA) {
        ff_task_t &A = *inA;
        ff_task_t B = A;
        
        // this is the parallel_for provided by the ff_Map class
        for (size_t i = 0; i < 10; i++)
        {
        parallel_for(1,A.size()-1,[&A, &B] (long i) { 
                B[i] = (A[i-1] + A[i] + A[i+1])/3;
            },2);
        A = B;
        }
            
        printf("Stencil computed:\n");
        for(size_t i=0;i<A.size();++i)
            std::cout<<A[i]<<" ";
        printf("\n");
        delete inA;
        return GO_ON;
    }    
};


int main() {
    mapWorker emitter;
    stencilStage stage;
    ff_Pipe<> pipe(emitter,stage);
    if (pipe.run_and_wait_end()<0)
        error("running pipe");   
    return 0;	
}