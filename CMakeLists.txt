cmake_minimum_required(VERSION 3.0)
project(OleBow CXX)

set(SOURCES
    OleBow.cpp
    MDispInterface.cpp
    MInterface.cpp
    MMethod.cpp
    MModule.cpp
    MNode.cpp
    MTypeDesc.cpp
    MTypeLib.cpp
    MTypeInfoExtra.cpp)

# libolebow_static.a
add_library(olebow_static STATIC ${SOURCES})
target_compile_definitions(olebow_static PRIVATE -DOLEBOW_BUILD)

# olebow.dll
add_library(olebow SHARED ${SOURCES} OleBow.def)
target_compile_definitions(olebow PRIVATE -DOLEBOW_DLL -DOLEBOW_BUILD)
target_link_libraries(olebow PRIVATE ole32 oleaut32 shlwapi)
set_target_properties(olebow PROPERTIES PREFIX "")

# OleBow.exe
add_executable(olebow_exe main.cpp)
if(FALSE) # Use DLL
    target_link_libraries(olebow_exe PRIVATE olebow ole32 oleaut32 shlwapi)
    target_compile_definitions(olebow_exe PRIVATE -DOLEBOW_DLL)
    set_target_properties(olebow_exe PROPERTIES OUTPUT_NAME "olebow")
else() # Don't use DLL
    target_link_libraries(olebow_exe PRIVATE olebow_static ole32 oleaut32 shlwapi)
    set_target_properties(olebow_exe PROPERTIES OUTPUT_NAME "olebow")
endif()

if(CMAKE_SIZEOF_VOID_P MATCHES "8") # sizeof(void*) == 8
  set_target_properties(olebow PROPERTIES OUTPUT_NAME "olebow64")
  set_target_properties(olebow_static PROPERTIES OUTPUT_NAME "olebow64_static")
  set_target_properties(olebow_exe PROPERTIES OUTPUT_NAME "olebow64")
else()
  set_target_properties(olebow PROPERTIES OUTPUT_NAME "olebow32")
  set_target_properties(olebow_static PROPERTIES OUTPUT_NAME "olebow32_static")
  set_target_properties(olebow_exe PROPERTIES OUTPUT_NAME "olebow32")
endif()
