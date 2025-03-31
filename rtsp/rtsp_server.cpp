#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"
#include "H264VideoFileServerMediaSubsession.hh"

#include <iostream>
#include <cassert>

#define CODE_MODE 1
#define CODE_MODE_CHECK(x) (x == CODE_MODE)

#define CONFIG_TEST_LIVE555 0
#define CONFIG_RTSP_PROCESS 1

#if CODE_MODE_CHECK(CONFIG_TEST_LIVE555)

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

    return 0;
}

#elif CODE_MODE_CHECK(CONFIG_RTSP_PROCESS)

// 主函数
int main(int argc, char **argv)
{
    // 创建一个任务调度器
    TaskScheduler *scheduler = BasicTaskScheduler::createNew();

    // 创建一个使用环境
    UsageEnvironment *env = BasicUsageEnvironment::createNew(*scheduler);

    // 打印一条消息，表明任务调度器已创建
    *env << "Live555 任务调度器已创建！\n";

    // 创建一个媒体服务器
    RTSPServer *rtspServer = RTSPServer::createNew(*env, 8554);
    if (rtspServer == NULL)
    {
        *env << "无法创建 RTSP 服务器: " << env->getResultMsg() << "\n";
        return 1;
    }

    // 创建一个媒体会话
    ServerMediaSession *sms = ServerMediaSession::createNew(*env, "testStream", "Test H.264 Stream");

    // 添加 H.264 视频子会话
    sms->addSubsession(H264VideoFileServerMediaSubsession::createNew(*env, "../resources/test.264", false));

    // 向 RTSP 服务器添加媒体会话
    rtspServer->addServerMediaSession(sms);

    // 打印 RTSP URL
    *env << "可以通过以下 URL 访问该流: " << rtspServer->rtspURL(sms) << "\n";

    // 进入事件循环
    env->taskScheduler().doEventLoop();

    return 0;
}

#else
#endif