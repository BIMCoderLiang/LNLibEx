set(TARGET_NAME LNData)
project(${TARGET_NAME} LANGUAGES CXX)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
add_library(${TARGET_NAME} SHARED "")
target_compile_definitions(LNData PRIVATE LNData_HOME)

target_include_directories(${TARGET_NAME} PUBLIC
	"${SOURCE_DIR}/public"
	"${LNLib_DIR}/include"
    "${OCC_DIR}/include"
)

target_link_options(${TARGET_NAME} PRIVATE
    "/DELAYLOAD:TKBRep.dll"
    "/DELAYLOAD:TKDEIGES.dll"
    "/DELAYLOAD:TKDESTEP.dll"
    "/DELAYLOAD:TKernel.dll"
    "/DELAYLOAD:TKG3d.dll"
    "/DELAYLOAD:TKMath.dll"
    "/DELAYLOAD:TKTopAlgo.dll"
    "/DELAYLOAD:TKXSBase.dll"
    "/DELAYLOAD:TKGeomBase.dll"
    "/DELAYLOAD:TKG2d.dll"
    "/DELAYLOAD:TKDE.dll"
    "/DELAYLOAD:TKBool.dll"
    "/DELAYLOAD:TKXCAF.dll"
    "/DELAYLOAD:TKLCAF.dll"
    "/DELAYLOAD:TKShHealing.dll"
    "/DELAYLOAD:TKPrim.dll"
    "/DELAYLOAD:TKGeomAlgo.dll"
    "/DELAYLOAD:jemalloc.dll"
    "/DELAYLOAD:TKBO.dll"
    "/DELAYLOAD:TKVCAF.dll"
    "/DELAYLOAD:TKV3d.dll"
    "/DELAYLOAD:TKService.dll"
    "/DELAYLOAD:TKCAF.dll"
    "/DELAYLOAD:TKCDF.dll"
    "/DELAYLOAD:TKMesh.dll"
    "/DELAYLOAD:TKHLR.dll"
    "/DELAYLOAD:openvr_api.dll"
    "/DELAYLOAD:FreeImage.dll"
    "/DELAYLOAD:freetype.dll"
    "/DELAYLOAD:avcodec-57.dll"
    "/DELAYLOAD:avformat-57.dll"
    "/DELAYLOAD:swscale-4.dll"
    "/DELAYLOAD:avutil-55.dll"
)

target_link_libraries(${TARGET_NAME} ${LIBS} ${LNLib_DIR}/lib/$<CONFIG>/LNLib.lib)
target_link_libraries(${TARGET_NAME} ${LIBS} ${OCC_DIR}/lib/*.lib)

file(GLOB rootfiles *.cpp *.h)
source_group("" FILES ${rootfiles})
target_sources(${TARGET_NAME} PRIVATE ${rootfiles})
SUBDIRLIST(SUBDIRS ${SOURCE_DIR})
foreach(subdir ${SUBDIRS})
    file(GLOB subdirFiles ${subdir}/*.cpp ${subdir}/*.h)
    string(REPLACE "/" "\\" subdir ${subdir})
    source_group(${subdir} FILES ${subdirFiles})
    target_sources(${TARGET_NAME} PRIVATE ${subdirFiles})
endforeach()

if(MSVC)
    set_target_properties(${TARGET_NAME} PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=${LNLib_DIR}/bin/$<CONFIG>;")
endif()