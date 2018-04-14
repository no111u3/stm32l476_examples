include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.cc")

add_executable(echo_usart ${SOURCES})

target_link_libraries(echo_usart
    PRIVATE lp::cc_lib
    PRIVATE lp::bm
    PRIVATE lp::devices
)

setup_custom_target_properties(echo_usart) 
