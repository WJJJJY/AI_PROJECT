# 最低版本3.6
CMAKE_MINIMUM_REQUIRED(VERSION 3.6)

message("Start compile...")

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "pkg-config --cflags --libs gtk+-2.0")

#查找gtk库，指定生成 IMPORT_GARGET
include(FindPkgConfig)
pkg_check_modules(LIBCONFIG REQUIRED IMPORTED_TARGET gtk+-2.0)

# 设置源码路径
set(SRC_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
message(${SRC_ROOT_PATH})

file(GLOB FILE_SRCS "${SRC_ROOT_PATH}/srcs/*.c")

set(
    SOURCE_FILES
    ${FILE_SRCS}
)


include_directories(${SRC_ROOT_PATH}/include/)

message("---------------")
add_executable(AI_PROJECT ${SOURCE_FILES})

# 将调用pkg_check_modules 生成的 IMPORTED_TARGET PKGCONFIG::LIGCONFIG作为依赖加入到自己的项目中
target_link_libraries(AI_PROJECT PkgConfig::LIBCONFIG)
# 使用imported_target方式引入依赖库不需要手工设置include_directory和library

