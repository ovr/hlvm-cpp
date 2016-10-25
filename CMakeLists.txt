cmake_minimum_required(VERSION 3.6)
project(llvm_simple_generator)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")
set(CMAKE_CXX_STANDARD 11)

find_package(LLVM 3.9 REQUIRED
        all-targets
        analysis
        asmparser
        asmprinter
        bitreader
        bitwriter
        codegen
        core
        debuginfocodeview
        debuginfodwarf
        debuginfomsf
        debuginfopdb
        globalisel
        instcombine
        ipa
        ipo
        instrumentation
        irreader
        linker
        lto
        mc
        mcdisassembler
        mcparser
        objcarcopts
        object
        option
        profiledata
        scalaropts
        selectiondag
        support
        tablegen
        target
        transformutils
        interpreter
        executionengine
        vectorize ${EXTRA_LLVM_MODULES})

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS} -std=c++11)

add_subdirectory(ast)

set(SOURCE_FILES main.cpp ast.h)
add_executable(llvm_simple_generator ${SOURCE_FILES})

llvm_map_components_to_libnames(llvm_libs mcjit x86codegen interpreter executionengine)
target_link_libraries(llvm_simple_generator ast ${llvm_libs})
