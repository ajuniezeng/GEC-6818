add_library(src lcd_control.c display.c)
target_include_directories(src PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})