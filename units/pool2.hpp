#ifndef EXECUTOR_POOL_HPP
#define EXECUTOR_POOL_HPP

//#include "cor/cor.hpp"

#include <mutex>
#include <condition_variable>
#include <hpx/hpx.hpp>

namespace cor
{
    class ExecutorPool
    {

    public:
        ExecutorPool(idp_t idp, std::size_t num_hpx_threads);
        ~ExecutorPool();

        //void Dispatch(hpx::function<void(void*)> fct, void *arg);
        void Dispatch(void (*taskfct)(void *), void *arg);
        void WaitForIdle();

        hpx::function<void(void *arg)> AgentFunc;
        int GetRank();
        int GetNumThreads();

        std::pair<int,int> ScheduleStatic(int Beg, int End);
        std::vector<std::pair<int,int>> ScheduleStatic(int Beg, int End, int chunk);
        std::pair<int,int> ScheduleDynamic(int Beg, int End, int chunk);
        std::pair<int,int> ScheduleGuided(int Beg, int End, int chunk);

        template < typename Func, typename ... Args >
        void DispatchTemplated(Func && func, Args && ... args);


    private:
        idp_t _idp;
        std::size_t _num_hpx_threads;
        void (*_fct)(void*);
        void *_arg;

        idp_t _group;
        std::vector<idp_t> _agents;


        std::vector<hpx::future<void>> _futures;
        std::vector<hpx::thread::id> th_ids;
        hpx::lcos::local::barrier barrier;
        int aux = 99;

        // index for sheduling
        std::atomic<int> index;
        hpx::future<void> _future;
        hpx::mutex _mtx;

    };
}

#include "cor/elements/executor_pool.tpp"

#endif