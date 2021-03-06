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
