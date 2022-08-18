cmake_minimum_required(VERSION 3.8)	
project(Dx12Renderer)

option(GROUP_BY_EXPLORER ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

LIST(APPEND ComponentAllSubDir "Dx12lib")
LIST(APPEND ComponentAllSubDir "Ext/DirectXTex")
LIST(APPEND ComponentAllSubDir "Math")
LIST(APPEND ComponentAllSubDir "RenderGraph")

if (MSVC)
	add_definitions(-D_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING)	## enable wstring_convert
endif(MSVC)

set(DX12RENDERER_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(AllTestDir "")
foreach(dirname ${ComponentAllSubDir}) 
	if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${dirname}/CMakeLists.txt")
		add_subdirectory(${dirname})
		LIST(APPEND AllTestDir "${dirname}")
	endif()
endforeach()

