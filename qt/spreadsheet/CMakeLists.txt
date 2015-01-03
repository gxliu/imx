#dir: 
#datte: 2015-01-01
#author: zuokongxiao
#discription: qt cmake files

#��������
PROJECT(sort)

#CMAKE�汾
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#QT
FIND_PACKAGE(Qt4 COMPONENTS QtCore QtGui REQUIRED)
INCLUDE(${QT_USE_FILE})

#����û�������Ϣ
MESSAGE(STATUS "source dir: " ${PROJECT_SOURCE_DIR})
MESSAGE(STATUS "binary dir: " ${PROJECT_BINARY_DIR})

#����������
if(WIN32)
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
else(WIN32)
 IF(${TARGET_P} MATCHES "x86")
  SET(CMAKE_CXX_COMPILER g++)
 ELSE()
  SET(CMAKE_CXX_COMPILER arm-linux-g++)
 ENDIF()
 SET(CMAKE_CXX_FLAGS "-g -Wno-psabi")
endif(WIN32)
MESSAGE(STATUS "compiler: ${CMAKE_CXX_COMPILER}, flags: ${CMAKE_C_FLAGS}")

#ִ���ļ�����
set(EXEC_NAME sortdialog)

#ͷ�ļ�
SET(HEADER_FILES 
    sortdialog.h
    )

#Դ�ļ�
SET(SOURCE_FILES 
    main.cpp
    sortdialog.cpp
    )

#moc�ļ�Q_OBJECT
set(MOC_FILES 
    sortdialog.h
    )

#ui�ļ�
set(UI_FILES
    sortdialog.ui
    )

#�򹤳�����ͷ�ļ�����·��
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${QT_INCLUDES}
                    )

#uic
qt4_wrap_ui(UI_OUTPUTS ${UI_FILES})

#MOC meta-object compiler
QT4_WRAP_CPP(MOC_OUTPUTS ${MOC_FILES})
#SOURCE_GROUP("moc_cxx" FILES ${MOC_OUTPUTS})

#��ִ���ļ�
ADD_EXECUTABLE(${EXEC_NAME} 
               ${SOURCE_FILES}
               ${MOC_OUTPUTS}
               ${UI_OUTPUTS}
               )

#ָ����ִ���ļ���Ҫ���ӵ����ӿ�
TARGET_LINK_LIBRARIES(${EXEC_NAME} ${QT_LIBRARIES})

