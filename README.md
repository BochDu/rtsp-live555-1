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