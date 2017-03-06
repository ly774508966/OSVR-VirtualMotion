#[=======================================================================[.rst:
FindVM30
--------------

Find the VM30 SDK components.

Imported Targets
^^^^^^^^^^^^^^^^

Several :ref:`imported targets <Imported targets>` are provided
if the SDK is found:

``VM30::VM30``
  Main library for interacting with VM30 API.

Result Variables
^^^^^^^^^^^^^^^^

This module defines the following variables:

``VM30_FOUND``
  True if the SDK was found, false otherwise.
``VM30_INCLUDE_DIRS``
  Include directories needed to find Leap headers.
``VM30_LIBRARIES``
  The list of all VM30 libraries.

It is recommended to link against the imported targets, rather than using the
latter two variables above.

Cache Variables
^^^^^^^^^^^^^^^

This module uses the following cache variables:

``VM30_LIBRARY_RELEASE``
  The location of the VM30 API driver library, release build.
``VM30_LIBRARY_DEBUG``
  The location of the VM30 API driver library, debug build.
``VM30_INCLUDE_DIR``
  The location of the VM30 SDK include directory containing ``Leap.h``.
``VM30_ROOT_DIR``
  An optional root to start looking for the VM30 SDK, if it``s not specified
  with the ``VM30_SDK`` environment variable or in a so-called conventional
  location (x86 Program Files directory + ``VM30/LeapSDK/`` on Windows)

The cache variables should not be used by project code.
They may be set by end users to point at Leap SDK components, though the script
can find the current (as of this writing) version of the SDK in its default
location.

#]=======================================================================]


set(VM30_ROOT_DIR
    "${VM30_ROOT_DIR}"
    CACHE
    PATH
    "Directory to search for VM30 API SDK")

# Their docs suggest setting a VM30_SDK environment variable. We'll use it if we can find it.
if(NOT VM30_ROOT_DIR)
    get_filename_component(_vm30_env_var "$ENV{VM30_SDK}" ABSOLUTE)
    if(NOT "${_vm30_env_var}" STREQUAL "")
        set(VM30_ROOT_DIR "${_vm30_env_var}")
    endif()
endif()

# TODO: handle the libc++ vs libstdc++ variants on Mac OS X
set(VM30_LIBRARY_PATH_SUFFIX lib)

if(WIN32)
    # Test 32/64 bits
    if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
        set(VM30_LIBRARY_PATH_SUFFIX "x64")
    else()
        set(VM30_LIBRARY_PATH_SUFFIX "x86")
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
find_path(VM30_INCLUDE_DIR
    NAMES
    libvml.h
    PATHS
    "${VM30_ROOT_DIR}"
    "${_progfiles}/vml-core/include"
    PATH_SUFFIXES
    include)

if(VM30_INCLUDE_DIR)
    get_filename_component(_vm30_root "${VM30_INCLUDE_DIR}" DIRECTORY)
endif()

find_library(VM30_LIBRARY_RELEASE
    NAMES
    vml
    libvml
    HINTS
    "${_vm30_root}"
    PATHS
    "${VM30_ROOT_DIR}"
    "${_progfiles}/vml-core/${VM30_LIBRARY_PATH_SUFFIX}/"
    PATH_SUFFIXES
    ${VM30_LIBRARY_PATH_SUFFIX})

# No debug build in 1.3.3 but might be in the future?
find_library(VM30_LIBRARY_DEBUG
    NAMES
    vmld
    libvmld
    HINTS
    "${_vm30_root}"
    PATHS
    "${VM30_ROOT_DIR}"
    "${_progfiles}/vml-core/${VM30_LIBRARY_PATH_SUFFIX}/"
    PATH_SUFFIXES
    ${VM30_LIBRARY_PATH_SUFFIX})

include(SelectLibraryConfigurations)
select_library_configurations(VM30)

if(WIN32)
    if(VM30_LIBRARY_RELEASE)
        get_filename_component(_vm30_libdir "${VM30_LIBRARY_RELEASE}" DIRECTORY)
        find_file(VM30_RUNTIME_LIBRARY_RELEASE
            NAMES
            libvml.dll
            HINTS
            "${_vm30_libdir}")
    endif()
    if(VM30_LIBRARY_DEBUG)
        get_filename_component(_vm30_libdir "${VM30_LIBRARY_DEBUG}" DIRECTORY)
        find_file(VM30_RUNTIME_LIBRARY_DEBUG
            NAMES
            libvmld.dll
            HINTS
            "${_vm30_libdir}")
    endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(VM30 FOUND_VAR VM30_FOUND
    REQUIRED_VARS
    VM30_LIBRARY_RELEASE
    VM30_INCLUDE_DIR)

if(VM30_FOUND)
    set(VM30_LIBRARIES ${VM30_LIBRARIES})
    set(VM30_INCLUDE_DIRS "${VM30_INCLUDE_DIR}")
    mark_as_advanced(VM30_ROOT_DIR)
    if(NOT TARGET VM30::VM30)
        set(_vm30_dll OFF)
        if(NOT WIN32)
            set(_vm30_libtype SHARED)
        elseif(WIN32 AND VM30_RUNTIME_LIBRARY_RELEASE)
            set(_vm30_dll ON)
            set(_vm30_libtype SHARED)
        else()
            set(_vm30_libtype UNKNOWN)
        endif()
        add_library(VM30::VM30 ${_vm30_libtype} IMPORTED)
        set_target_properties(VM30::VM30 PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${VM30_INCLUDE_DIRS}"
            IMPORTED_LINK_INTERFACE_LANGUAGES "C++")
        if(_vm30_dll)
            set_target_properties(VM30::VM30 PROPERTIES
                IMPORTED_LOCATION "${VM30_RUNTIME_LIBRARY_RELEASE}"
                IMPORTED_IMPLIB "${VM30_LIBRARY_RELEASE}")
        else()
            set_target_properties(VM30::VM30 PROPERTIES
                IMPORTED_LOCATION "${VM30_LIBRARY_RELEASE}")
        endif()

        # TODO decide if we leave this in or not
        if(VM30_LIBRARY_DEBUG)
            if(_vm30_dll)
                if(VM30_RUNTIME_LIBRARY_DEBUG)
                    # Only add the debug variant if we have both parts lest we confuse CMake.
                    set_target_properties(VM30::VM30 PROPERTIES
                        IMPORTED_LOCATION_DEBUG "${VM30_RUNTIME_LIBRARY_DEBUG}"
                        IMPORTED_IMPLIB_DEBUG "${VM30_LIBRARY_DEBUG}")
                endif()
            else()
                set_target_properties(VM30::VM30 PROPERTIES
                IMPORTED_LOCATION_DEBUG "${VM30_LIBRARY_DEBUG}")
            endif()

        endif()
    endif()
endif()

mark_as_advanced(VM30_INCLUDE_DIR
    VM30_LIBRARY_RELEASE
    VM30_LIBRARY_DEBUG
    VM30_RUNTIME_LIBRARY_RELEASE
    VM30_RUNTIME_LIBRARY_DEBUG)
