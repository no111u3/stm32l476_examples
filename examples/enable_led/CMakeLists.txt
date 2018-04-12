include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.cc")

add_executable(enable_led ${SOURCES})

target_link_libraries(enable_led
    PRIVATE lp::cc_lib
    PRIVATE lp::bm
    PRIVATE lp::devices
)

setup_custom_target_properties(enable_led) 
