# check if wxWidgets is already installed in the system - using CMake's built in script FindwxWidgets
find_package(wxWidgets QUIET)

if(wxWidgets_FOUND)
    message(STATUS "Found preinstalled wxWidgets libraries at ${wxWidgets_LIBRARIES}")
    add_library(wxWidgets_external INTERFACE)
else()
    message(STATUS "Preinstalled wxWidgets not found.")
    message(STATUS "Will download and install wxWidgets in ${STAGED_INSTALL_PREFIX}")

    include(ExternalProject)
    
    ExternalProject_Add(wxWidgets_external
        GIT_REPOSITORY https://github.com/wxWidgets/wxWidgets.git
        GIT_TAG v3.2.2
        UPDATE_COMMAND ""
        CMAKE_ARGS
            -DCMAKE_INSTALL_PREFIX=${STAGED_INSTALL_PREFIX}
            -DCMAKE_BUILD_TYPE=Release
            -DwxBUILD_SHARED=OFF
            -DwxBUILD_MONOLITHIC=OFF
            -DwxBUILD_PRECOMP=OFF
            -DwxUSE_OPENGL=ON
            -DwxUSE_LIBPNG=sys
        TEST_AFTER_INSTALL 0
        DOWNLOAD_NO_PROGRESS 0
        LOG_CONFIGURE 1
        LOG_BUILD 1
        LOG_INSTALL 1
    )


    set(wxWidgets_ROOT_DIR ${STAGED_INSTALL_PREFIX})
    file(TO_NATIVE_PATH "${wxWidgets_ROOT_DIR}" wxWidgets_ROOT_DIR)
    set(wxWidgets_ROOT_DIR ${wxWidgets_ROOT_DIR} CACHE INTERNAL "wxWidgets installation dir")

    set(ENV_WX_CONFIG ${STAGED_INSTALL_PREFIX}/bin/wx-config)
    file(TO_NATIVE_PATH "${ENV_WX_CONFIG}" ENV_WX_CONFIG)
    set(ENV_WX_CONFIG ${ENV_WX_CONFIG} CACHE INTERNAL "wx-config dir")
endif()
