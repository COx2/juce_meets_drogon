# Example GUI App CMakeLists.txt

# To get started on a new GUI app, copy this entire folder (containing this file and C++ sources) to
# a convenient location, and then start making modifications. For other examples of CMakeLists for
# GUI apps, check `extras/Projucer` and `examples/DemoRunner` in the JUCE repo.

# The first line of any CMake project should be a call to `cmake_minimum_required`, which checks
# that the installed CMake will be able to understand the following CMakeLists, and ensures that
# CMake's behaviour is compatible with the named version. This is a standard CMake command, so more
# information can be found in the CMake docs.

cmake_minimum_required(VERSION 3.15)

# To be enabled integrate vcpkg installed packages.
# NOTE: CMAKE_TOOLCHAIN_FILE is should setted in startup
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/External/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE PATH "toolchain file")

# The top-level CMakeLists.txt file for a project must contain a literal, direct call to the
# `project()` command. `project()` sets up some helpful variables that describe source/binary
# directories, and the current project version. This is a standard CMake command.

project(JUCE_MEETS_DROGON VERSION 0.0.1)

# static linking in Windows
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Adds all the module sources so they appear correctly in the IDE
set_property(GLOBAL PROPERTY USE_FOLDERS YES)
option(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Enable Module Source Groups" ON)

# If you've installed JUCE somehow (via a package manager, or directly using the CMake install
# target), you'll need to tell this project that it depends on the installed copy of JUCE. If you've
# included JUCE directly in your source tree (perhaps as a submodule), you'll need to tell CMake to
# include that subdirectory as part of the build.

# find_package(JUCE CONFIG REQUIRED)        # If you've installed JUCE to your system
# or
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/External/JUCE)                    # If you've put JUCE in a subdirectory called JUCE

# Add drogon package
find_package(Drogon CONFIG REQUIRED)

# If your app depends the VST2 SDK, perhaps to host VST2 plugins, CMake needs to be told where
# to find the SDK on your system. This setup should be done before calling `juce_add_gui_app`.

# juce_set_vst2_sdk_path(...)

# `juce_add_gui_app` adds an executable target with the name passed as the first argument
# (GuiAppExample here). This target is a normal CMake target, but has a lot of extra properties set
# up by default. This function accepts many optional arguments. Check the readme at
# `docs/CMake API.md` in the JUCE repo for the full list.

juce_add_gui_app(JuceMeetsDrogon
    # VERSION ...                       # Set this if the app version is different to the project version
    # ICON_BIG ...                      # ICON_* arguments specify a path to an image file to use as an icon
    # ICON_SMALL ...
    # DOCUMENT_EXTENSIONS ...           # Specify file extensions that should be associated with this app
    # COMPANY_NAME ...                  # Specify the name of the app's author
    PRODUCT_NAME "JuceMeetsDrogon")     # The name of the final executable, which can differ from the target name

# `juce_generate_juce_header` will create a JuceHeader.h for a given target, which will be generated
# into your build tree. This should be included with `#include <JuceHeader.h>`. The include path for
# this header will be automatically added to the target. The main function of the JuceHeader is to
# include all your JUCE module headers; if you're happy to include module headers directly, you
# probably don't need to call this.

juce_generate_juce_header(JuceMeetsDrogon)

# `target_sources` adds source files to a target. We pass the target that needs the sources as the
# first argument, then a visibility parameter for the sources which should normally be PRIVATE.
# Finally, we supply a list of source files that will be built into the target. This is a standard
# CMake command.

set_property(TARGET JuceMeetsDrogon PROPERTY CXX_STANDARD 17)

# Test that the target_path should be excluded
# if the path is excluded, IS_EXCLUDED will be set to TRUE
function(TEST_EXCLUDED TARGET_PATH EXCLUSION_PATTERN_LIST IS_EXCLUDED)
  set(IS_EXCLUDED FALSE PARENT_SCOPE)
  foreach(PAT ${EXCLUSION_PATTERN_LIST})
    if(TARGET_PATH MATCHES ${PAT})
      set(IS_EXCLUDED TRUE PARENT_SCOPE)
      break()
    endif()
  endforeach()
endfunction()

# set target extension list
set(TARGET_SOURCE_PATTERNS "*.c" "*.cc" "*.cpp" "*.h" "*.hpp")

if(MSVC)
  list(APPEND TARGET_SOURCE_PATTERNS "*.rc")
elseif(XCODE)
  list(APPEND TARGET_SOURCE_PATTERNS "*.mm")
endif()

# generate_source_list(
#   output
#   BASE_DIR dir
#   SOURCE_DIRS dir1 [dir2...]
#   EXCLUSION_PATTERNS [pattern1...]
#   )
function(generate_source_list output)
  cmake_parse_arguments(GSL "" "BASE_DIR" "SOURCE_DIRS;EXCLUSION_PATTERNS" ${ARGN})

  if(NOT DEFINED GSL_BASE_DIR)
    message(FATAL_ERROR "BASE_DIR must be specified.")
  endif()

  if(NOT DEFINED GSL_SOURCE_DIRS)
    message(FATAL_ERROR "SOURCE_DIRS must be specified.")
  endif()

  message("Print EXCLUSION_PATTERNS: ${GSL_EXCLUSION_PATTERNS}")

  # set the root directory of the source file tree in IDE
  get_filename_component(GSL_BASE_DIR "${GSL_BASE_DIR}" ABSOLUTE)

  foreach(SOURCE_DIR ${GSL_SOURCE_DIRS})
    get_filename_component(SOURCE_DIR "${SOURCE_DIR}" ABSOLUTE)

    set(PATTERNS "")
    foreach(PATTERN ${TARGET_SOURCE_PATTERNS})
      list(APPEND PATTERNS "${SOURCE_DIR}/${PATTERN}")
    endforeach()

    file(GLOB_RECURSE FILES ${PATTERNS})

    # Define SourceGroup reflecting filesystem hierarchy.
    foreach(FILE_PATH ${FILES})
      get_filename_component(FILEPATH "${FILE_PATH}" ABSOLUTE)
      TEST_EXCLUDED(${FILE_PATH} "${GSL_EXCLUSION_PATTERNS}" IS_EXCLUDED)
      if(IS_EXCLUDED)
        continue()
      endif()

      get_filename_component(PARENT_DIR "${FILE_PATH}" DIRECTORY)
      file(RELATIVE_PATH GROUP_NAME "${GSL_BASE_DIR}" "${PARENT_DIR}")
      string(REPLACE "/" "\\" GROUP_NAME "${GROUP_NAME}")
      source_group("${GROUP_NAME}" FILES "${FILE_PATH}")
      list(APPEND SOURCE_FILES "${FILE_PATH}")
    endforeach()
  endforeach()

  message("Files ${SOURCE_FILES}")
  set(${output} "${SOURCE_FILES}" PARENT_SCOPE)
endfunction()

generate_source_list(
    APP_SOURCES
    BASE_DIR "./Source"
    SOURCE_DIRS "./Source"
#     EXCLUSION_PATTERNS ".+/Test/.+"
    )

target_sources(JuceMeetsDrogon
    PRIVATE
    ${APP_SOURCES}
    )

# target_sources(JuceMeetsDrogon
#     PRIVATE
#         Source/Main.cpp
#         Source/MainComponent.cpp
#         Source/MainComponent.h)

# `target_compile_definitions` adds some preprocessor definitions to our target. In a Projucer
# project, these might be passed in the 'Preprocessor Definitions' field. JUCE modules also make use
# of compile definitions to switch certain features on/off, so if there's a particular feature you
# need that's not on by default, check the module header for the correct flag to set here. These
# definitions will be visible both to your code, and also the JUCE module code, so for new
# definitions, pick unique names that are unlikely to collide! This is a standard CMake command.

target_compile_definitions(JuceMeetsDrogon
    PRIVATE
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_gui_app` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_gui_app` call
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:JuceMeetsDrogon,JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:JuceMeetsDrogon,JUCE_VERSION>")

# If your target needs extra binary assets, you can add them here. The first argument is the name of
# a new static library target that will include all the binary resources. There is an optional
# `NAMESPACE` argument that can specify the namespace of the generated binary data class. Finally,
# the SOURCES argument should be followed by a list of source files that should be built into the
# static library. These source files can be of any kind (wav data, images, fonts, icons etc.).
# Conversion to binary-data will happen when your target is built.

# juce_add_binary_data(GuiAppData SOURCES ...)

# `target_link_libraries` links libraries and JUCE modules to other libraries or executables. Here,
# we're linking our executable target to the `juce::juce_gui_extra` module. Inter-module
# dependencies are resolved automatically, so `juce_core`, `juce_events` and so on will also be
# linked automatically. If we'd generated a binary data target above, we would need to link to it
# here too. This is a standard CMake command.

target_link_libraries(JuceMeetsDrogon
    PRIVATE
        # GuiAppData            # If we'd created a binary data target, we'd link to it here
        juce::juce_gui_extra
        Drogon::Drogon
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
