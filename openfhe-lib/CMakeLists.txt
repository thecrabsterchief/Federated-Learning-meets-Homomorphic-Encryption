cmake_minimum_required (VERSION 3.5.1)

project(Test-openFHE
  VERSION
    1.0
  DESCRIPTION
    "This is sample for testing openFHE library"
  LANGUAGES
    CXX
)

set(CMAKE_CXX_STANDARD 17)
option( BUILD_STATIC "Set to ON to include static versions of the library" OFF)

### Include openFHE library to our project
find_package(OpenFHE)
set( CMAKE_CXX_FLAGS ${OpenFHE_CXX_FLAGS} )
include_directories( ${OPENMP_INCLUDES} )
include_directories( ${OpenFHE_INCLUDE} )
include_directories( ${OpenFHE_INCLUDE}/third-party/include )
include_directories( ${OpenFHE_INCLUDE}/core )
include_directories( ${OpenFHE_INCLUDE}/pke )

### add directories for other OpenFHE modules as needed for your project
link_directories( ${OpenFHE_LIBDIR} )
link_directories( ${OPENMP_LIBRARIES} )
if(BUILD_STATIC)
    set( CMAKE_EXE_LINKER_FLAGS "${OpenFHE_EXE_LINKER_FLAGS} -static")
    link_libraries( ${OpenFHE_STATIC_LIBRARIES} )
else()
    set( CMAKE_EXE_LINKER_FLAGS ${OpenFHE_EXE_LINKER_FLAGS} )
    link_libraries( ${OpenFHE_SHARED_LIBRARIES} )
endif()

### ADD YOUR EXECUTABLE(s) HERE
add_executable(key_generation key_generation.cpp)
add_executable(client client.cpp)
add_executable(server server.cpp)