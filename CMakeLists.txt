cmake_minimum_required(VERSION 3.2)
project(ParallelSystems)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "E:/Workspace/ParallelSystems/Debug")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

set(SOURCE_FILES
    Debug/MPI_TEST
    src/bucketSort.c
    src/bucketSort.h
    src/dataHolder.c
    src/dataHolder.h
    src/fileHandler.c
    src/fileHandler.h
    src/MPI_TEST.c)

add_executable(ParallelSystems ${SOURCE_FILES})