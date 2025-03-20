add_executable(utils lcd_control.c render.c touch.c)
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/utils)