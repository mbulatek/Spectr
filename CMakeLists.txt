cmake_minimum_required(VERSION 3.21.0)

#cmake_policy(SET CMP0054 NEW)
#cmake_policy(SET CMP0135 NEW)  

option(SMTG_ENABLE_VST3_PLUGIN_EXAMPLES "Enable VST 3 Plug-in Examples" OFF)
option(SMTG_ENABLE_VST3_HOSTING_EXAMPLES "Enable VST 3 Hosting Examples" OFF)

set(CMAKE_OSX_DEPLOYMENT_TARGET 10.13 CACHE STRING "")

set(SOURCES
    source/SpectrEntry.cpp
    source/SpectrController.cpp
    source/SpectrEditor.cpp
    source/SpectrProcessor.cpp
    source/Utils/Logger.cpp
)

set(HEADERS
    source/version.h
    source/SpectrCIds.h
    source/SpectrController.h
    source/SpectrEditor.h
    source/SpectrProcessor.h
    source/UI/SpectrogramView.h
    source/Utils/Logger.h
)

if(DEFINED ENV{VST3SDK_PATH})
  set(vst3sdk_SOURCE_DIR "$ENV{VST3SDK_PATH}" CACHE PATH "Path to VST3 SDK")
else()
  set(vst3sdk_SOURCE_DIR "C:/vst3sdk" CACHE PATH "Path to VST3 SDK")
endif()

if(NOT vst3sdk_SOURCE_DIR)
    message(FATAL_ERROR "Path to VST3 SDK is empty!")
endif()

project(Spectr
    # This is your plug-in version number. Change it here only.
    # Version number symbols usable in C++ can be found in
    # source/version.h and ${PROJECT_BINARY_DIR}/projectversion.h.
    VERSION 1.0.0.0 
    DESCRIPTION "Spectr VST 3 Plug-in"
)

set(SMTG_VSTGUI_ROOT "${vst3sdk_SOURCE_DIR}")

add_subdirectory(${vst3sdk_SOURCE_DIR} ${PROJECT_BINARY_DIR}/vst3sdk)
smtg_enable_vst3_sdk()

smtg_add_vst3plugin(Spectr
        ${SOURCES} 
        ${HEADERS}
)

target_include_directories(Spectr
    PUBLIC
    ${CMAKE_SOURCE_DIR}/source
)

#- VSTGUI Wanted ----
if(SMTG_ENABLE_VSTGUI_SUPPORT)
    target_sources(Spectr
        PRIVATE
            resource/myplugineditor.uidesc
    )
    target_link_libraries(Spectr
        PRIVATE
            vstgui_support
    )
    smtg_target_add_plugin_resources(Spectr
        RESOURCES
            "resource/myplugineditor.uidesc"
    )
endif(SMTG_ENABLE_VSTGUI_SUPPORT)
# -------------------

smtg_target_add_plugin_snapshots (Spectr
    RESOURCES
        resource/875965F5366558DFB4C01848017AA95C_snapshot.png
        resource/875965F5366558DFB4C01848017AA95C_snapshot_2.0x.png
)

target_link_libraries(Spectr
    PRIVATE
        sdk
)

smtg_target_configure_version_file(Spectr)

if(SMTG_MAC)
    smtg_target_set_bundle(Spectr
        BUNDLE_IDENTIFIER com.maro.spectr
        COMPANY_NAME "My Plug-in Company"
    )
    smtg_target_set_debug_executable(Spectr
        "/Applications/VST3PluginTestHost.app"
        "--pluginfolder;$(BUILT_PRODUCTS_DIR)"
    )
elseif(SMTG_WIN)
    target_sources(Spectr PRIVATE 
        resource/win32resource.rc
    )
    if(MSVC)
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Spectr)

        smtg_target_set_debug_executable(Spectr
            "$(ProgramW6432)/Steinberg/VST3PluginTestHost/VST3PluginTestHost.exe"
            "--pluginfolder \"$(OutDir)/\""
        )
    endif()
endif(SMTG_MAC)

include(dependencies.cmake)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog fftw3 Boost::filesystem Boost::system)


find_program(CLANG_FORMAT_EXE NAMES clang-format)

if(CLANG_FORMAT_EXE)
    message(STATUS "Found clang-format: ${CLANG_FORMAT_EXE}")

    file(GLOB_RECURSE ALL_SOURCE_FILES 
        ${SOURCES} 
        ${HEADERS}
    )

    add_custom_target(clang-format
        COMMAND ${CLANG_FORMAT_EXE} -i ${ALL_SOURCE_FILES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running clang-format on source files..."
        VERBATIM
    )
else()
    message(WARNING "clang-format not found, skipping clang-format target.")
endif()

