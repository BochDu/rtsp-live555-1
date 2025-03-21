#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"

#include <iostream>
#include <cassert>

// 封装创建任务调度器的函数
TaskScheduler *createTaskScheduler()
{
    TaskScheduler *scheduler = BasicTaskScheduler::createNew();
    if (scheduler == nullptr)
    {
        std::cerr << "Failed to create task scheduler." << std::endl;
    }
    return scheduler;
}

// 封装创建使用环境的函数
UsageEnvironment *createUsageEnvironment(TaskScheduler *scheduler)
{
    if (scheduler == nullptr)
        return nullptr;
    UsageEnvironment *env = BasicUsageEnvironment::createNew(*scheduler);
    if (env == nullptr)
    {
        std::cerr << "Failed to create usage environment." << std::endl;
        delete scheduler;
    }
    return env;
}

bool check_live555(void)
{
    // 创建任务调度器
    TaskScheduler *scheduler = createTaskScheduler();
    if (scheduler == nullptr)
    {
        return false;
    }

    // 创建使用环境
    UsageEnvironment *env = createUsageEnvironment(scheduler);
    if (env == nullptr)
    {
        return false;
    }

    // 输出成功信息
    *env << "Live555 library is working correctly.\n";

    // 清理资源
    env->reclaim();
    delete scheduler;

    return true;
}

int main(int argc, char **argv)
{
    if (!check_live555())
        assert(true);
}