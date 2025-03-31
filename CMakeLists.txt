# 设置 CMake 最低版本要求
cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(RTSP_SERVER)

# 设置 live555 库的路径
set(LIVE555_DIR ${CMAKE_CURRENT_SOURCE_DIR}/live)

# 添加 live555 库的头文件路径
include_directories(
    ${LIVE555_DIR}/liveMedia/include
    ${LIVE555_DIR}/groupsock/include
    ${LIVE555_DIR}/BasicUsageEnvironment/include
    ${LIVE555_DIR}/UsageEnvironment/include
    /usr/include # 添加 OpenSSL 头文件路径
)

# 查找 live555 库文件
find_library(LIVEMEDIA_LIBRARY liveMedia ${LIVE555_DIR}/liveMedia)
find_library(GROUPSOCK_LIBRARY groupsock ${LIVE555_DIR}/groupsock)
find_library(BASICUSAGEENVIRONMENT_LIBRARY BasicUsageEnvironment ${LIVE555_DIR}/BasicUsageEnvironment)
find_library(USAGEENVIRONMENT_LIBRARY UsageEnvironment ${LIVE555_DIR}/UsageEnvironment)

# 查找 OpenSSL 库文件
find_library(SSL_LIBRARY ssl)
find_library(CRYPTO_LIBRARY crypto)

# 输出查找结果
message(STATUS "LIVEMEDIA_LIBRARY: ${LIVEMEDIA_LIBRARY}")
message(STATUS "GROUPSOCK_LIBRARY: ${GROUPSOCK_LIBRARY}")
message(STATUS "BASICUSAGEENVIRONMENT_LIBRARY: ${BASICUSAGEENVIRONMENT_LIBRARY}")
message(STATUS "USAGEENVIRONMENT_LIBRARY: ${USAGEENVIRONMENT_LIBRARY}")
message(STATUS "SSL_LIBRARY: ${SSL_LIBRARY}")
message(STATUS "CRYPTO_LIBRARY: ${CRYPTO_LIBRARY}")

# 检查是否找到所有库
if(NOT LIVEMEDIA_LIBRARY OR NOT GROUPSOCK_LIBRARY OR NOT BASICUSAGEENVIRONMENT_LIBRARY OR NOT USAGEENVIRONMENT_LIBRARY OR NOT SSL_LIBRARY OR NOT CRYPTO_LIBRARY)
    message(FATAL_ERROR "Could not find all necessary libraries. Please check the paths.")
endif()

# 添加可执行文件
add_executable(rtsp_server rtsp/rtsp_server.cpp)

# 链接 live555 和 OpenSSL 库到可执行文件
target_link_libraries(rtsp_server
    ${LIVEMEDIA_LIBRARY}
    ${GROUPSOCK_LIBRARY}
    ${BASICUSAGEENVIRONMENT_LIBRARY}
    ${USAGEENVIRONMENT_LIBRARY}
    ${SSL_LIBRARY}
    ${CRYPTO_LIBRARY}
)
