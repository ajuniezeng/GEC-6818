add_library(utils lcd_control.c render.c touch.c led_control.c module_control.c)
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})