include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.cc")

add_executable(empty ${SOURCES})

target_link_libraries(empty
    PRIVATE lp::cc_lib
    PRIVATE lp::bm
    PRIVATE lp::devices
)

setup_custom_target_properties(empty) 
