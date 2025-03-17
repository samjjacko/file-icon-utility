add_library(objc_lib)

target_sources(objc_lib
PRIVATE
objcWrapper.mm
MacInterface.mm
PUBLIC
../include/objcWrapper.hpp
../include/MacInterface.h)

target_include_directories(objc_lib PUBLIC ../include)

target_link_libraries(objc_lib "-framework Foundation")

add_compile_options(-fcxx-modules -fmodules)
add_library(ricing_tool::objc_lib ALIAS objc_lib)