cmake_minimum_required(VERSION 3.20)

project(VaultRenderer)

set(AssetsPath ${PROJECT_SOURCE_DIR}/Assets)
set(BinaryPath ${PROJECT_SOURCE_DIR}/Binaries)
set(SourcePath ${PROJECT_SOURCE_DIR}/Source)
set(ExternalPath ${PROJECT_SOURCE_DIR}/External)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (OutputBinary ${PROJECT_SOURCE_DIR}/Binaries)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OutputBinary})

file (COPY ${PROJECT_SOURCE_DIR}/Assets DESTINATION ${OutputBinary})

# add_subdirectory(External)
add_subdirectory(Source)
add_subdirectory(Test)