

cmake_minimum_required(VERSION 3.4.1)

#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI})
find_library(log-lib
              log )

#添加ffmpeg的库
set(distribution_DIR ${CMAKE_SOURCE_DIR}/libs)

add_library(ffmpeg
            SHARED
            IMPORTED
)
set_target_properties(ffmpeg
            PROPERTIES IMPORTED_LOCATION
           ${distribution_DIR}/${ANDROID_ABI}/libffmpeg.so
)
#引入外部库的头文件
include_directories(libs/include)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

add_library( native-lib
             SHARED
             src/main/cpp/native-lib.cpp
             src/main/cpp/decoder.c )
target_link_libraries(#把需要的so库 连接起来
                       native-lib
                        ffmpeg
                       ${log-lib} )