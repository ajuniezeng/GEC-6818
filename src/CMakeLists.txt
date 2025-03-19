add_library(src lcd_control.c render.c bmp.c)
target_include_directories(src PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})