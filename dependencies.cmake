include(FetchContent)

# Spdlog
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.11.0
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(spdlog)

# FFTW
FetchContent_Declare(
    fftw
    GIT_REPOSITORY https://github.com/FFTW/fftw3.git
    GIT_TAG master
    GIT_SHALLOW TRUE
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    GIT_PROGRESS TRUE
)
FetchContent_GetProperties(fftw)
if (NOT fftw_POPULATED)
    FetchContent_Populate(fftw)
    
    set(FFTW_BUILD_DIR ${fftw_BINARY_DIR})
    set(FFTW_SOURCE_DIR ${fftw_SOURCE_DIR})
    target_include_directories(Spectr
	    PUBLIC
	    ${FFTW_SOURCE_DIR}/api
    )
    
    set(ENABLE_SSE ON CACHE BOOL "Enable SSE support")
    set(ENABLE_AVX ON CACHE BOOL "Enable AVX support")
    set(ENABLE_THREADS ON CACHE BOOL "Enable multi-threading support")
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static FFTW library")

    add_subdirectory(${FFTW_SOURCE_DIR} ${FFTW_BUILD_DIR})
    
    set(ENABLE_SSE ON CACHE BOOL "Enable SSE support" FORCE)
	set(ENABLE_AVX ON CACHE BOOL "Enable AVX support" FORCE)
	set(ENABLE_THREADS ON CACHE BOOL "Enable multi-threading support" FORCE)
	set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static FFTW library" FORCE)
endif()

# Boost
FetchContent_Declare(
    Boost
    GIT_REPOSITORY https://github.com/boostorg/boost.git
    GIT_TAG boost-1.87.0
    GIT_SHALLOW FALSE
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    SUBMODULES ""
    GIT_PROGRESS TRUE
)

FetchContent_GetProperties(Boost)
if (NOT Boost_POPULATED)
    FetchContent_Populate(Boost)

    set(Boost_INCLUDE_DIR "${Boost_SOURCE_DIR}")
    set(Boost_LIBRARY_DIR "${Boost_SOURCE_DIR}/stage/lib")
    set(Boost_ROOT "${Boost_SOURCE_DIR}")

    set(Boost_NO_SYSTEM_PATHS TRUE CACHE BOOL "Disable system paths for Boost")
    set(Boost_USE_STATIC_LIBS ON CACHE BOOL "Use static Boost libraries")
    
    set(Boost_SOURCE_DIR ${CMAKE_BINARY_DIR}/_deps/boost-src)
	set(Boost_BINARY_DIR ${CMAKE_BINARY_DIR}/_deps/boost-build)
	
	set(Boost_INCLUDE_DIR "${Boost_SOURCE_DIR}")
	set(Boost_LIBRARY_DIR "${Boost_SOURCE_DIR}/stage/lib")
	set(Boost_ROOT "${Boost_SOURCE_DIR}")

    message(STATUS "Boost_ROOT: ${Boost_ROOT}")
    message(STATUS "Boost_SOURCE_DIR: ${Boost_SOURCE_DIR}")
    message(STATUS "Boost_INCLUDE_DIR: ${Boost_INCLUDE_DIR}")
    message(STATUS "Boost_LIBRARY_DIR: ${Boost_LIBRARY_DIR}")

    execute_process(
        COMMAND git submodule update --init --recursive --progress
        WORKING_DIRECTORY ${Boost_SOURCE_DIR}
        RESULT_VARIABLE BOOST_SUBMODULE_RESULT
    )
    if (NOT BOOST_SUBMODULE_RESULT EQUAL 0)
        message(FATAL_ERROR "Failed to update Boost submodules!")
    endif()
    
    execute_process(
	    COMMAND ${Boost_SOURCE_DIR}/bootstrap.bat
	    WORKING_DIRECTORY ${Boost_SOURCE_DIR}
	    RESULT_VARIABLE BOOST_BOOTSTRAP_RESULT
	)
	if (NOT BOOST_BOOTSTRAP_RESULT EQUAL 0)
	    message(FATAL_ERROR "Failed to run Boost bootstrap.bat!")
	endif()
    
    
    execute_process(
	    COMMAND ${Boost_SOURCE_DIR}/b2 headers
	    WORKING_DIRECTORY ${Boost_SOURCE_DIR}
	    RESULT_VARIABLE BOOST_B2_HEADERS_RESULT
	)
	if (NOT BOOST_B2_HEADERS_RESULT EQUAL 0)
	    message(FATAL_ERROR "Failed to run Boost b2 headers!")
	endif()
	
	execute_process(
	    COMMAND ${Boost_SOURCE_DIR}/b2 --with-filesystem --with-system  --with-lockfree link=static runtime-link=shared threading=multi variant=release
	    WORKING_DIRECTORY ${Boost_SOURCE_DIR}
	    RESULT_VARIABLE BOOST_B2_LIBRARIES_RESULT
	)
	if (NOT BOOST_B2_LIBRARIES_RESULT EQUAL 0)
	    message(FATAL_ERROR "Failed to build Boost libraries!")
	endif()
	

    add_subdirectory(${Boost_SOURCE_DIR} ${Boost_BINARY_DIR})

    find_package(Boost REQUIRED COMPONENTS filesystem system)
    if (Boost_FOUND)
        message(STATUS "Boost found: ${Boost_INCLUDE_DIRS}")
        include_directories(${Boost_INCLUDE_DIRS})
        target_link_libraries(${PROJECT_NAME} PRIVATE Boost::filesystem Boost::system Boost::lockfree)
    else()
        message(FATAL_ERROR "Boost NOT found! Check if it's correctly installed.")
    endif()
endif()
