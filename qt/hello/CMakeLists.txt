#dir: /imx/qt/hello
#datte: 2014-12-25
#author: zuokongxiao
#discription: qt hello

#工程名称
PROJECT(hello)

#CMAKE版本
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#QT
FIND_PACKAGE(Qt4 COMPONENTS QtCore QtGui REQUIRED)
INCLUDE(${QT_USE_FILE})

#输出用户定义信息
MESSAGE(STATUS "source dir: " ${PROJECT_SOURCE_DIR})
MESSAGE(STATUS "binary dir: " ${PROJECT_BINARY_DIR})

#编译器设置
IF(${TARGET_P} MATCHES "x86")
 SET(CMAKE_CXX_COMPILER g++)
ELSE()
 SET(CMAKE_CXX_COMPILER arm-linux-g++)
ENDIF()
SET(CMAKE_CXX_FLAGS "-g")
MESSAGE(STATUS "compiler: ${CMAKE_CXX_COMPILER}, flags: ${CMAKE_C_FLAGS}")

#执行文件名称
set(EXEC_NAME hello)

#头文件
#SET(HEADER_FILES 
#    mainwindow.h
#    )

#源文件
SET(SOURCE_FILES 
    hello.cpp
    )

#向工程添加头文件搜索路径
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}
                    ${QT_INCLUDES}
                    #${HEADER_FILES}
                    )

#将当前目录中的源文件名称赋值给变量DIR_SRCS
#AUX_SOURCE_DIRECTORY(. DIR_SRCS)

#指示变量DIR_SRCS中的源文件需要编译成可执行文件
ADD_EXECUTABLE(${EXEC_NAME} 
               ${SOURCE_FILES}
               )

#指明可执行文件需要连接的链接库
TARGET_LINK_LIBRARIES(${EXEC_NAME} ${QT_LIBRARIES})


