#[=======================================================================[.rst:
FindVirtualMotion
--------------

Find the VirtualMotion SDK components.

Imported Targets
^^^^^^^^^^^^^^^^

Several :ref:`imported targets <Imported targets>` are provided
if the SDK is found:

``VirtualMotion::VirtualMotion``
  Main library for interacting with VirtualMotion API.

Result Variables
^^^^^^^^^^^^^^^^

This module defines the following variables:

``VIRTUALMOTION_FOUND``
  True if the SDK was found, false otherwise.
``VIRTUALMOTION_INCLUDE_DIRS``
  Include directories needed to find Leap headers.
``VIRTUALMOTION_LIBRARIES``
  The list of all VirtualMotion libraries.

It is recommended to link against the imported targets, rather than using the
latter two variables above.

Cache Variables
^^^^^^^^^^^^^^^

This module uses the following cache variables:

``VIRTUALMOTION_LIBRARY_RELEASE``
  The location of the VirtualMotion API driver library, release build.
``VIRTUALMOTION_LIBRARY_DEBUG``
  The location of the VirtualMotion API driver library, debug build.
``VIRTUALMOTION_INCLUDE_DIR``
  The location of the VirtualMotion SDK include directory containing ``Leap.h``.
``VIRTUALMOTION_ROOT_DIR``
  An optional root to start looking for the VirtualMotion SDK, if it``s not specified
  with the ``VIRTUALMOTION_SDK`` environment variable or in a so-called conventional
  location (x86 Program Files directory + ``VirtualMotion/`` on Windows)

The cache variables should not be used by project code.
They may be set by end users to point at Leap SDK components, though the script
can find the current (as of this writing) version of the SDK in its default
location.

#]=======================================================================]


set(VIRTUALMOTION_ROOT_DIR
    "${VIRTUALMOTION_ROOT_DIR}"
    CACHE
    PATH
    "Directory to search for VirtualMotion API SDK")

# Their docs suggest setting a VIRTUALMOTION_SDK environment variable. We'll use it if we can find it.
if(NOT VIRTUALMOTION_ROOT_DIR)
    get_filename_component(_virtualmotion_env_var "$ENV{VIRTUALMOTION_SDK}" ABSOLUTE)
    if(NOT "${_virtualmotion_env_var}" STREQUAL "")
        set(VIRTUALMOTION_ROOT_DIR "${_virtualmotion_env_var}")
    endif()
endif()

# TODO: handle the libc++ vs libstdc++ variants on Mac OS X
set(VIRTUALMOTION_LIBRARY_PATH_SUFFIX lib)

if(WIN32)
    # Test 32/64 bits
    if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
        set(VIRTUALMOTION_LIBRARY_PATH_SUFFIX "x64")
    else()
        set(VIRTUALMOTION_LIBRARY_PATH_SUFFIX "x86")
    endif()

    # Get desired program files directory
    set(_PF86_ENV "ProgramFiles(x86)")
    set(_PF86 $ENV{${_PF86_ENV}})

    if(NOT "$ENV{${_PF86}}" STREQUAL "")
        # 32-bit dir: only set on win64
        file(TO_CMAKE_PATH "$ENV{_PF86}" _progfiles)
    else()
        # 32-bit dir on win32
        file(TO_CMAKE_PATH "$ENV{ProgramFiles}" _progfiles)
    endif()
endif()

# Try to find the header first, since it's simplest (no 32-64 nonsense in the path)
# and can provide a root directory for finding the rest of the SDK.
find_path(VIRTUALMOTION_INCLUDE_DIR
    NAMES
    libvml.h
    PATHS
    "${VIRTUALMOTION_ROOT_DIR}"
    "${_progfiles}/vml-core/include"
    PATH_SUFFIXES
    include)

if(VIRTUALMOTION_INCLUDE_DIR)
    get_filename_component(_virtualmotion_root "${VIRTUALMOTION_INCLUDE_DIR}" DIRECTORY)
endif()

find_library(VIRTUALMOTION_LIBRARY_RELEASE
    NAMES
    vml
    libvml
    HINTS
    "${_virtualmotion_root}"
    PATHS
    "${VIRTUALMOTION_ROOT_DIR}"
    "${_progfiles}/vml-core/${VIRTUALMOTION_LIBRARY_PATH_SUFFIX}/"
    PATH_SUFFIXES
    ${VIRTUALMOTION_LIBRARY_PATH_SUFFIX})

# No debug build in 1.3.3 but might be in the future?
find_library(VIRTUALMOTION_LIBRARY_DEBUG
    NAMES
    vmld
    libvmld
    HINTS
    "${_virtualmotion_root}"
    PATHS
    "${VIRTUALMOTION_ROOT_DIR}"
    "${_progfiles}/vml-core/${VIRTUALMOTION_LIBRARY_PATH_SUFFIX}/"
    PATH_SUFFIXES
    ${VIRTUALMOTION_LIBRARY_PATH_SUFFIX})

include(SelectLibraryConfigurations)
select_library_configurations(VIRTUALMOTION)

if(WIN32)
    if(VIRTUALMOTION_LIBRARY_RELEASE)
        get_filename_component(_virtualmotion_libdir "${VIRTUALMOTION_LIBRARY_RELEASE}" DIRECTORY)
        find_file(VIRTUALMOTION_RUNTIME_LIBRARY_RELEASE
            NAMES
            libvml.dll
            HINTS
            "${_virtualmotion_libdir}")
    endif()
    if(VIRTUALMOTION_LIBRARY_DEBUG)
        get_filename_component(_virtualmotion_libdir "${VIRTUALMOTION_LIBRARY_DEBUG}" DIRECTORY)
        find_file(VIRTUALMOTION_RUNTIME_LIBRARY_DEBUG
            NAMES
            libvmld.dll
            HINTS
            "${_virtualmotion_libdir}")
    endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(VIRTUALMOTION FOUND_VAR VIRTUALMOTION_FOUND
    REQUIRED_VARS
    VIRTUALMOTION_LIBRARY_RELEASE
    VIRTUALMOTION_INCLUDE_DIR)

if(VIRTUALMOTION_FOUND)
    set(VIRTUALMOTION_LIBRARIES ${VIRTUALMOTION_LIBRARIES})
    set(VIRTUALMOTION_INCLUDE_DIRS "${VIRTUALMOTION_INCLUDE_DIR}")
    mark_as_advanced(VIRTUALMOTION_ROOT_DIR)
    if(NOT TARGET VirtualMotion::VirtualMotion)
        set(_virtualmotion_dll OFF)
        if(NOT WIN32)
            set(_virtualmotion_libtype SHARED)
        elseif(WIN32 AND VIRTUALMOTION_RUNTIME_LIBRARY_RELEASE)
            set(_virtualmotion_dll ON)
            set(_virtualmotion_libtype SHARED)
        else()
            set(_virtualmotion_libtype UNKNOWN)
        endif()
        add_library(VirtualMotion::VirtualMotion ${_virtualmotion_libtype} IMPORTED)
        set_target_properties(VirtualMotion::VirtualMotion PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${VIRTUALMOTION_INCLUDE_DIRS}"
            IMPORTED_LINK_INTERFACE_LANGUAGES "C++")
        if(_virtualmotion_dll)
            set_target_properties(VirtualMotion::VirtualMotion PROPERTIES
                IMPORTED_LOCATION "${VIRTUALMOTION_RUNTIME_LIBRARY_RELEASE}"
                IMPORTED_IMPLIB "${VIRTUALMOTION_LIBRARY_RELEASE}")
        else()
            set_target_properties(VirtualMotion::VirtualMotion PROPERTIES
                IMPORTED_LOCATION "${VIRTUALMOTION_LIBRARY_RELEASE}")
        endif()

        # TODO decide if we leave this in or not
        if(VIRTUALMOTION_LIBRARY_DEBUG)
            if(_virtualmotion_dll)
                if(VIRTUALMOTION_RUNTIME_LIBRARY_DEBUG)
                    # Only add the debug variant if we have both parts lest we confuse CMake.
                    set_target_properties(VirtualMotion::VirtualMotion PROPERTIES
                        IMPORTED_LOCATION_DEBUG "${VIRTUALMOTION_RUNTIME_LIBRARY_DEBUG}"
                        IMPORTED_IMPLIB_DEBUG "${VIRTUALMOTION_LIBRARY_DEBUG}")
                endif()
            else()
                set_target_properties(VirtualMotion::VirtualMotion PROPERTIES
                IMPORTED_LOCATION_DEBUG "${VIRTUALMOTION_LIBRARY_DEBUG}")
            endif()

        endif()
    endif()
endif()

mark_as_advanced(VIRTUALMOTION_INCLUDE_DIR
    VIRTUALMOTION_LIBRARY_RELEASE
    VIRTUALMOTION_LIBRARY_DEBUG
    VIRTUALMOTION_RUNTIME_LIBRARY_RELEASE
    VIRTUALMOTION_RUNTIME_LIBRARY_DEBUG)
