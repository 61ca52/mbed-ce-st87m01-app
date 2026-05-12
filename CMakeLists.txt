cmake_minimum_required(VERSION 3.19)
cmake_policy(VERSION 3.19...3.22)

set(MBED_APP_JSON_PATH mbed_app.json5)

include(mbed-os/tools/cmake/mbed_toolchain_setup.cmake)
project(MbedCE_ST87M01_EVAL_KIT_APP
        LANGUAGES C CXX ASM)

list(APPEND MBED_CONFIG_SOURCE_DIRS
    ${CMAKE_CURRENT_LIST_DIR}/st87m01
    ${CMAKE_CURRENT_LIST_DIR}/cellular-test
)

include(mbed_project_setup)

add_subdirectory(mbed-os)
add_subdirectory(cellular-test)
add_subdirectory(st87m01)

add_executable(${PROJECT_NAME} main.cpp)

if("MBED_CONF_TARGET_APPLICATION_PROFILE=bare-metal" IN_LIST MBED_CONFIG_DEFINITIONS)
    message(FATAL_ERROR "Bare-metal profile not allowed")
endif()

target_link_libraries(${PROJECT_NAME}
    mbed-os
    mbed-netsocket
    mbed-cellular
    cellular-test
    st87m01
)

mbed_set_post_build(${PROJECT_NAME})