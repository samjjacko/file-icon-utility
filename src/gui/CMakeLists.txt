# find wx widgets
find_package(wxWidgets REQUIRED COMPONENTS core base)
# include wxwidgets
include(${wxWidgets_USE_FILE})

add_library(gui)
target_sources(gui
PRIVATE
gui.cpp
imagepanel.cpp
PUBLIC
../include/gui.hpp)

target_link_libraries(gui ${wxWidgets_LIBRARIES})

# add_compile_options(`wx-config --cxxflags --libs`) # does nothing?
target_include_directories(gui 
PUBLIC 
../include
PRIVATE
${wxWidgets_INCLUDE_DIRS})

add_library(ricing_tool::gui ALIAS gui)