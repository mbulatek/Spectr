include(FetchContent)

# Spdlog
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.11.0
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(spdlog)

# FFTW
FetchContent_Declare(
    fftw
    GIT_REPOSITORY https://github.com/FFTW/fftw3.git
    GIT_TAG master
    GIT_SHALLOW TRUE
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_GetProperties(fftw)
if (NOT fftw_POPULATED)
    FetchContent_Populate(fftw)
    
    set(FFTW_BUILD_DIR ${fftw_BINARY_DIR})
    set(FFTW_SOURCE_DIR ${fftw_SOURCE_DIR})

    set(ENABLE_SSE ON CACHE BOOL "Enable SSE support")
    set(ENABLE_AVX ON CACHE BOOL "Enable AVX support")
    set(ENABLE_THREADS ON CACHE BOOL "Enable multi-threading support")
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static FFTW library")

    add_subdirectory(${FFTW_SOURCE_DIR} ${FFTW_BUILD_DIR})
endif()

# Boost
FetchContent_Declare(
    Boost
    GIT_REPOSITORY https://github.com/boostorg/boost.git
    GIT_TAG boost-1.83.0
    GIT_SHALLOW TRUE
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_GetProperties(Boost)
if (NOT Boost_POPULATED)
    FetchContent_Populate(Boost)

    set(BOOST_BUILD_DIR ${Boost_BINARY_DIR})
    set(BOOST_SOURCE_DIR ${Boost_SOURCE_DIR})

    set(BOOST_COMPONENTS filesystem system)
    set(Boost_USE_STATIC_LIBS ON CACHE BOOL "Use static Boost libraries" FORCE)

    add_subdirectory(${BOOST_SOURCE_DIR} ${BOOST_BUILD_DIR})
    
    get_property(ALL_TARGETS GLOBAL PROPERTY TARGETS)
	message(STATUS "Available CMake Targets: ${ALL_TARGETS}")
    
	add_dependencies(${PROJECT_NAME} Boost)
endif()
