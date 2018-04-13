include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.cc")

add_executable(greeting_usart ${SOURCES})

target_link_libraries(greeting_usart
    PRIVATE lp::cc_lib
    PRIVATE lp::bm
    PRIVATE lp::devices
)

setup_custom_target_properties(greeting_usart) 
