#dir: app/uartprinter
#datte: 2014-10-23
#author: zuokongxiao
#discription: 串口打印机

#工程名称
PROJECT(uartprinter)

#CMAKE版本
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#输出用户定义信息
MESSAGE(STATUS "source dir: " ${PROJECT_SOURCE_DIR})
MESSAGE(STATUS "binary dir: " ${PROJECT_BINARY_DIR})

#编译器设置
IF(${TARGET_P} MATCHES "x86")
 MESSAGE(STATUS "compiler: gcc ")
 SET(CMAKE_C_COMPILER gcc)
ELSE()
 #MESSAGE(STATUS "target_p: arm-fsl-linux-gnueabi-gcc")
 #SET(CMAKE_C_COMPILER arm-fsl-linux-gnueabi-gcc)
 #MESSAGE(STATUS "target_p: arm-linux-gnueabihf-gcc")
 #SET(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
 MESSAGE(STATUS "target_p: arm-linux-gcc")
 SET(CMAKE_C_COMPILER arm-linux-gcc)
ENDIF()

#设置编译器
#SET(CMAKE_THREAD_PREFER_PTHREAD TRUE)
#SET(CMAKE_C_FLAGS "-lpthread")
SET(CMAKE_C_FLAGS "-g")
MESSAGE(STATUS "c flags: " ${CMAKE_C_FLAGS})

#线程
#FIND_PACKAGE(Threads REQUIRED)

#使用命令ADD_SUBDIRECTORY指明本项目包含一个子目录printerlib
ADD_SUBDIRECTORY(printerlib)

#向工程添加头文件搜索路径
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../include)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/printerlib)

#将当前目录中的源文件名称赋值给变量DIR_SRCS
AUX_SOURCE_DIRECTORY(. DIR_SRCS)

#指示变量DIR_SRCS中的源文件需要编译成可执行文件
ADD_EXECUTABLE(uartprinter uartprinter)

#指明可执行文件需要连接的链接库
#TARGET_LINK_LIBRARIES(testxml ${CMAKE_THREAD_LIBS_INIT})
TARGET_LINK_LIBRARIES(uartprinter ${PROJECT_SOURCE_DIR}/../common/libcommon.a)
TARGET_LINK_LIBRARIES(uartprinter printerlib)


