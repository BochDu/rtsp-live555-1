# RTSP服务器
C++与live555库实现

[Github](https://github.com/BochDu/rtsp-live555-1)

## 项目目录

- `build`: 编译目录
- `live`: live555源码
- `resources`: 测试视频资源
- `rtsp`: RTSP程序

## 代码运行

```
cd build
cmake ..
make
./rtsp_server
```

## 软件下载
[VLC播放器下载](https://www.videolan.org/vlc/index.zh_CN.html)

## OpenSSL库安装
```
sudo apt-get install libssl-dev
```

## live555库移植

- [live555源码下载](www.live555.com/liveMedia/)
- [live555编译教程](https://juejin.cn/post/7458648750766915603)
- 编译指令参考
```
tar vxf live555-latest.tar.gz
cd live
./genMakefiles linux-64bit
make clean
make
```
- 测试live555库
```
cd ./live/testProgs
./testH264VideoStreamer
```
- 在`/live/testProgs/`目录放入视频文件`test.264`，仓库已经`resources`复制该文件
- 可执行文件会读`test.264`启动视频流，在`VLC播放器`选择`媒体`,`打开网络串流`播放器输入IP播放

## RTSP协议

RTSP全称实时流协议（Real Time Streaming Protocol）它是一个网络控制协议，设计用于娱乐、会议系统中控制流媒体服务器。RTSP用于在希望通讯的两端建立并控制媒体会话（session），客户端通过发出VCR-style命令如play、record、pause等来实时控制媒体流。在线实时观看情景下，传统的HTTP下载显然不能够匹配该需求，因此在寻求streaming的道路上，RTSP脱颖而出。

- RTSP协议：负责服务器与客户端之间的请求与响应
- RTP协议：负责传输媒体数据
- RTCP协议：在RTP传输过程中提供传输信息

## live555库

[live555流程参考](https://blog.csdn.net/hey5178/article/details/142629215)

### 任务调度器（TaskScheduler）

负责管理和调度所有的事件，如网络读写、定时器事件等。事件驱动的模型，确保高效处理多个并发任务。

```C
void BasicTaskScheduler0::doEventLoop(EventLoopWatchVariable* watchVariable) {
  // Repeatedly loop, handling readble sockets and timed events:
  while (1) {
    if (watchVariable != NULL && *watchVariable != 0) break;
    SingleStep();
  }
}
```

SingleStep()功能：
- 监听socket事件完成套接字读和写通过select函数，遍历handler列表
- fEventTriggersAreBeingUsed标志位的事件触发器，上层函数createEventTrigger来触发事件

```C
class HandlerDescriptor {
  HandlerDescriptor(HandlerDescriptor* nextHandler);
  virtual ~HandlerDescriptor();

public:
  int socketNum;
  int conditionSet;
  TaskScheduler::BackgroundHandlerProc* handlerProc;
  void* clientData;

private:
  // Descriptors are linked together in a doubly-linked list:
  friend class HandlerSet;
  friend class HandlerIterator;
  HandlerDescriptor* fNextHandler;
  HandlerDescriptor* fPrevHandler;
};
```
- handler列表的实现是基于一个双向链表，调度任务通过链表进行增删改查，维护记录socket信息
- 由setBackgroundHandling函数中assignHandler创建

### RTSP协议实现（RTSPServer）

```C
unsigned RTSPServer::registerStream(ServerMediaSession* serverMediaSession,
				    char const* remoteClientNameOrAddress, portNumBits remoteClientPortNum,
				    responseHandlerForREGISTER* responseHandler,
				    char const* username, char const* password,
				    Boolean receiveOurStreamViaTCP, char const* proxyURLSuffix) {
  // Create a new "RegisterRequestRecord" that will send the "REGISTER" command.
  // (This object will automatically get deleted after we get a response to the "REGISTER" command, or if we're deleted.)
  Authenticator* authenticator = NULL;
  if (username != NULL) {
    if (password == NULL) password = "";
    authenticator = new Authenticator(username, password);
  }
  unsigned requestId = ++fRegisterOrDeregisterRequestCounter;
  char const* url = rtspURL(serverMediaSession);
  new RegisterRequestRecord(*this, requestId,
			    remoteClientNameOrAddress, remoteClientPortNum, url,
			    responseHandler, authenticator,
			    receiveOurStreamViaTCP, proxyURLSuffix);
  
  delete[] (char*)url; // we can do this here because it was copied to the "RegisterRequestRecord" 
  delete authenticator; // ditto
  return requestId;
}
```
- registerStream函数注册流，fRegisterOrDeregisterRequestCounter确保唯一性，rtspURL分配url
- setUpTunnelingOverHTTP用于在HTTP上设置隧道传输建立socket连接
- TCP流管理，RTSPServer::RTSPClientConnection客户端连接处理，RTSPServer::RTSPClientSession客户端会话
