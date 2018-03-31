cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(jsring VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(GNUInstallDirs)

enable_testing()

add_library(jsring INTERFACE)

target_include_directories(jsring INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/jsringConfigVersion.cmake"
    VERSION 0.1
    COMPATIBILITY AnyNewerVersion
)

install(TARGETS jsring
    EXPORT jsringTargets
    LIBRARY DESTINATION lib COMPONENT Runtime
    ARCHIVE DESTINATION lib COMPONENT Development
    RUNTIME DESTINATION bin COMPONENT Runtime
    PUBLIC_HEADER DESTINATION include COMPONENT Development
    BUNDLE DESTINATION bin COMPONENT Runtime
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/jsringConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/jsringConfig.cmake"
    INSTALL_DESTINATION lib/cmake/jsring
)

install(EXPORT jsringTargets DESTINATION lib/cmake/jsring)
install(FILES "${PROJECT_BINARY_DIR}/jsringConfigVersion.cmake"
              "${PROJECT_BINARY_DIR}/jsringConfig.cmake"
        DESTINATION lib/cmake/jsring)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)

find_program(
  CLANG_TIDY_EXE
  NAMES "clang-tidy"
  DOC "Path to clang-tidy executable"
  )
if(NOT CLANG_TIDY_EXE)
  message(STATUS "clang-tidy not found.")
else()
  message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
  set(DO_CLANG_TIDY "${CLANG_TIDY_EXE}")
endif()

add_subdirectory(test)
