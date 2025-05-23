cmake_minimum_required(VERSION 3.10.0)

include(../InitRootPath.cmake)

set(c_ProjectName Oopecs)

include(${c_RootCMakeProjectFrameworkDirPath}/ProjectBegin.cmake)
project(${c_ProjectName})
include(${c_RootCMakeProjectFrameworkDirPath}/ProjectEnd.cmake)


set(ModuleName ${c_ProjectName})

set(ModuleRootPath ${c_RootSourceDirPath}/${ModuleName})
set(ModuleSourcePath ${ModuleRootPath}/src)
set(ModuleIncludePath ${ModuleRootPath}/include)

set(IncludePathsPrivate "")
set(IncludePathsPublic "")
list(APPEND IncludePathsPrivate ${ModuleSourcePath})
list(APPEND IncludePathsPublic ${ModuleIncludePath})

file(GLOB_RECURSE ModuleSources ${ModuleSourcePath}/*.cpp ${ModuleSourcePath}/*.h)
create_source_group(${ModuleSourcePath} ${ModuleSources})
file(GLOB_RECURSE ModuleHeaders ${ModuleIncludePath}/*.h)
create_source_group(${ModuleIncludePath} ${ModuleHeaders})
set(SrcAll "")
list(APPEND SrcAll ${ModuleSources})
list(APPEND SrcAll ${ModuleHeaders})

add_executable(${ModuleName} ${SrcAll})

target_precompile_headers(${ModuleName} PUBLIC ${ModuleSourcePath}/OopecsCommon.h)

target_include_directories(${ModuleName}
	PRIVATE ${IncludePathsPrivate}
	PUBLIC ${IncludePathsPublic}
)

list(APPEND v_ListAccessorSettingHeaderFilePath ${ModuleSourcePath}/AccessorSetting.h)
list(APPEND v_ListModuleHeaderFilePath ${ModuleHeaders})
list(APPEND v_ListToolOption "-gsr")
#set(v_EnabledDebuggerAttaching ON)
include(${c_RootProjectDirPath}/Niflect/Default.cmake)

if(MSVC)
	set_target_properties(${ModuleName} PROPERTIES
		VS_DEBUGGER_WORKING_DIRECTORY "$<IF:$<CONFIG:Debug>,${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG},${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}>"
	)
endif()

if(MSVC)
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${c_ProjectName})
endif()

include(${c_RootCMakeProjectFrameworkDirPath}/Release.cmake)
include(${c_RootCMakeProjectFrameworkDirPath}/Cleanup.cmake)