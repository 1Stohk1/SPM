# This file will be configured to contain variables for CPack. These variables
# should be set in the CMake list file of the project before CPack module is
# included. The list of available CPACK_xxx variables and their associated
# documentation may be obtained using
#  cpack --help-variable-list
#
# Some variables are common to all generators (e.g. CPACK_PACKAGE_NAME)
# and some are specific to a generator
# (e.g. CPACK_NSIS_EXTRA_INSTALL_COMMANDS). The generator specific variables
# usually begin with CPACK_<GENNAME>_xxxx.


set(CPACK_BINARY_CYGWIN "ON")
set(CPACK_BINARY_IFW "OFF")
set(CPACK_BUILD_SOURCE_DIRS "/cygdrive/c/Users/chris/Documents/SPM/Jacobi_PRJ;/cygdrive/c/Users/chris/Documents/SPM/Jacobi_PRJ/cmake-build-debug")
set(CPACK_CMAKE_GENERATOR "Unix Makefiles")
set(CPACK_COMPONENTS_ALL "")
set(CPACK_COMPONENT_UNSPECIFIED_HIDDEN "TRUE")
set(CPACK_COMPONENT_UNSPECIFIED_REQUIRED "TRUE")
set(CPACK_DEFAULT_PACKAGE_DESCRIPTION_FILE "/cygdrive/c/Users/chris/AppData/Local/JetBrains/CLion2021.3/cygwin_cmake/share/cmake-3.21.1/Templates/CPack.GenericDescription.txt")
set(CPACK_DEFAULT_PACKAGE_DESCRIPTION_SUMMARY "project built using CMake")
set(CPACK_GENERATOR "CygwinBinary")
set(CPACK_INSTALL_CMAKE_PROJECTS "/cygdrive/c/Users/chris/Documents/SPM/Jacobi_PRJ/cmake-build-debug;project;ALL;/")
set(CPACK_INSTALL_PREFIX "/usr/local")
set(CPACK_MODULE_PATH "")
set(CPACK_NSIS_DISPLAY_NAME "project 0.1.0")
set(CPACK_NSIS_INSTALLER_ICON_CODE "")
set(CPACK_NSIS_INSTALLER_MUI_ICON_CODE "")
set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES")
set(CPACK_NSIS_PACKAGE_NAME "project 0.1.0")
set(CPACK_NSIS_UNINSTALL_NAME "Uninstall")
set(CPACK_OUTPUT_CONFIG_FILE "/cygdrive/c/Users/chris/Documents/SPM/Jacobi_PRJ/cmake-build-debug/CPackConfig.cmake")
set(CPACK_PACKAGE_DEFAULT_LOCATION "/")
set(CPACK_PACKAGE_DESCRIPTION_FILE "/cygdrive/c/Users/chris/AppData/Local/JetBrains/CLion2021.3/cygwin_cmake/share/cmake-3.21.1/Templates/CPack.GenericDescription.txt")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "project built using CMake")
set(CPACK_PACKAGE_FILE_NAME "project-0.1.0-CYGWIN")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "project 0.1.0")
set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "project 0.1.0")
set(CPACK_PACKAGE_NAME "project")
set(CPACK_PACKAGE_RELOCATABLE "true")
set(CPACK_PACKAGE_VENDOR "Humanity")
set(CPACK_PACKAGE_VERSION "0.1.0")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PROJECT_NAME "project")
set(CPACK_PROJECT_VERSION "0.1.0")
set(CPACK_RESOURCE_FILE_LICENSE "/cygdrive/c/Users/chris/AppData/Local/JetBrains/CLion2021.3/cygwin_cmake/share/cmake-3.21.1/Templates/CPack.GenericLicense.txt")
set(CPACK_RESOURCE_FILE_README "/cygdrive/c/Users/chris/AppData/Local/JetBrains/CLion2021.3/cygwin_cmake/share/cmake-3.21.1/Templates/CPack.GenericDescription.txt")
set(CPACK_RESOURCE_FILE_WELCOME "/cygdrive/c/Users/chris/AppData/Local/JetBrains/CLion2021.3/cygwin_cmake/share/cmake-3.21.1/Templates/CPack.GenericWelcome.txt")
set(CPACK_SET_DESTDIR "OFF")
set(CPACK_SOURCE_CYGWIN "ON")
set(CPACK_SOURCE_GENERATOR "CygwinSource")
set(CPACK_SOURCE_OUTPUT_CONFIG_FILE "/cygdrive/c/Users/chris/Documents/SPM/Jacobi_PRJ/cmake-build-debug/CPackSourceConfig.cmake")
set(CPACK_SYSTEM_NAME "CYGWIN")
set(CPACK_THREADS "1")
set(CPACK_TOPLEVEL_TAG "CYGWIN")
set(CPACK_WIX_SIZEOF_VOID_P "8")

if(NOT CPACK_PROPERTIES_FILE)
  set(CPACK_PROPERTIES_FILE "/cygdrive/c/Users/chris/Documents/SPM/Jacobi_PRJ/cmake-build-debug/CPackProperties.cmake")
endif()

if(EXISTS ${CPACK_PROPERTIES_FILE})
  include(${CPACK_PROPERTIES_FILE})
endif()
