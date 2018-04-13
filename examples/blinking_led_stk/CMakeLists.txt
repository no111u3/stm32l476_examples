include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.cc")

add_executable(blinking_led_stk ${SOURCES})

target_link_libraries(blinking_led_stk
    PRIVATE lp::cc_lib
    PRIVATE lp::bm
    PRIVATE lp::devices
)

setup_custom_target_properties(blinking_led_stk) 
