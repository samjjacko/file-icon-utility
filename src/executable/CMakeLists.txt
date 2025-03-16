add_executable(ricing_tool_binary main.cpp)
target_link_libraries(ricing_tool_binary
    ricing_tool::objc_lib
    ricing_tool::gui
    "-framework Foundation" # Required for objective-c compilation
    ${wxWidgets_LIBRARIES} # Required for gui with wxWidgets
)