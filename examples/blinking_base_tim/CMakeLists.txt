include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.cc")

add_executable(blinking_base_tim ${SOURCES})

target_link_libraries(blinking_base_tim
    PRIVATE lp::cc_lib
    PRIVATE lp::bm
    PRIVATE lp::devices
)

setup_custom_target_properties(blinking_base_tim) 
