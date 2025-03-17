find_package(wxWidgets REQUIRED COMPONENTS net core base)
add_executable(ricing_tool_binary main.cpp)
add_compile_options(-fcxx-modules -fmodules) #doesn't do a thing
# add_compile_options(-fcxx-modules -fmodules `wx-config --cxxflags --libs`) #doesn't do a thing
target_include_directories(ricing_tool_binary
    PRIVATE
    ${wxWidgets_INCLUDE_DIRS}
    ../include
)
target_link_libraries(ricing_tool_binary
    ricing_tool::objc_lib
    ricing_tool::gui
    ${wxWidgets_LIBRARIES} # do i need to specify this twice??
)
include(${wxWidgets_USE_FILE})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# note that the other libraries are not linked here, but required libraries 
# are linked in their own CMakeLists.txt files