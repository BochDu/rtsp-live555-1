# RTSP服务器
C++与live555库实现

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

## live555库知识

[live555流程参考](https://blog.csdn.net/hey5178/article/details/142629215)

