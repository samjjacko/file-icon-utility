add_library(gui)
target_sources(gui
PRIVATE
gui.cpp
PUBLIC
../include/gui.hpp)
target_include_directories(gui PUBLIC ../include)
add_compile_options(`wx-config --cxxflags --libs`)
find_package(wxWidgets REQUIRED COMPONENTS net core base)
add_library(ricing_tool::gui ALIAS gui)