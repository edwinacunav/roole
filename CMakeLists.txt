cmake_minimum_required(VERSION 2.9)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)
Project(roole)

set(BIN_RPATH "\$ORIGIN/lib")
set(LIB_PATH "lib")
set(CMAKE_SKIP_BUILD_RPATH TRUE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH ${BIN_RPATH})
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)
#option(SHARED_FLUID "Dynamically link fluidsynth at build time" OFF)
option(WORKDIR_CURRENT "Keep current directory on startup" OFF)
#set(BINDING "MRI" CACHE STRING "The Binding Type (MRI)")
set(EXTERNAL_LIB_PATH "" CACHE PATH "External precompiled lib prefix")

SET(LINUX ON)

if (LINUX)
  set(OS_STRING "linux")
  set(OS_REAL_STRING "Debian 10 Buster")
endif()

if (WINDOWS)
  set(OS_STRING "windows")
  set(OS_REAL_STRING "Windows 8")
endif()

IF(WORKDIR_CURRENT)
  list(APPEND DEFINES WORKDIR_CURRENT)
ENDIF()

if (EXTERNAL_LIB_PATH)
  set(CMAKE_PREFIX_PATH ${EXTERNAL_LIB_PATH})
  if(EXISTS "${EXTERNAL_LIB_PATH}/${LIB_PATH}/pkgconfig/")
    SET(ENV{PKG_CONFIG_PATH} "${EXTERNAL_LIB_PATH}/${LIB_PATH}/pkgconfig/")
  endif()
endif()

find_package(PkgConfig REQUIRED)
#pkg_check_modules(OPENGL     REQUIRED gl)
pkg_check_modules(SDL2       REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
pkg_check_modules(MRI        REQUIRED ruby-2.5)
pkg_check_modules(SNDFILE    REQUIRED sndfile)
pkg_check_modules(VORBISFILE REQUIRED vorbisfile)
pkg_check_modules(MPG123     REQUIRED libmpg123)
pkg_check_modules(FONTCONFIG REQUIRED fontconfig)
pkg_check_modules(FREETYPE   REQUIRED freetype2)

#cmake_policy(SET CMP0072 OLD)

find_package(OpenAL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(ZLIB REQUIRED)

set(MAIN_HEADERS
  hdr/Audio.hpp
  hdr/AudioFile.hpp
  hdr/AudioImpl.hpp
  hdr/AudioToolboxFile.hpp
  hdr/AutoLink.hpp
  hdr/Bitmap.hpp
  hdr/BlockAllocator.hpp
  hdr/Buttons.hpp
  hdr/ClipRectStack.hpp
  hdr/Color.hpp
  hdr/Directories.hpp
  hdr/DrawOp.hpp
  hdr/DrawOpQueue.hpp
  hdr/EmptyImageData.hpp
  hdr/extras.hpp
  hdr/Font.hpp
  hdr/Fwd.hpp
  hdr/Roole.hpp
  hdr/GLView.hpp
  hdr/ViewController.hpp
  hdr/Graphics.hpp
  hdr/GraphicsBase.hpp
  hdr/GraphicsImpl.hpp
  hdr/IO.hpp
  hdr/Iconv.hpp
  hdr/Image.hpp
  hdr/ImageData.hpp
  hdr/Input.hpp
  hdr/Inspection.hpp
  hdr/LargeImageData.hpp
  hdr/Log.hpp
  hdr/MPEGFile.hpp
  hdr/Macro.hpp
  hdr/MarkupParser.hpp
  hdr/Math.hpp
  hdr/OffScreenTarget.hpp
  hdr/OggFile.hpp
  hdr/Platform.hpp
  hdr/RenderState.hpp
  hdr/Scene.hpp
  hdr/SndFile.hpp
  hdr/TexChunk.hpp
  hdr/Text.hpp
  hdr/TextBuilder.hpp
  hdr/TextInput.hpp
  hdr/Texture.hpp
  hdr/Timing.hpp
  hdr/TransformStack.hpp
  hdr/TrueTypeFont.hpp
  hdr/Utility.hpp
  hdr/Version.hpp
  hdr/WinUtility.hpp
  hdr/Window.hpp
  hdr/stb_image.h
  hdr/stb_image_write.h
  hdr/stb_truetype.h
  hdr/utf8proc.h
  hdr/utf8proc_data.h
  xxd/icon.xxd                            
)

set(MAIN_SOURCE
  src/Audio.cpp
  src/AudioImpl.cpp
  src/Bitmap.cpp
  src/BitmapIO.cpp
  src/BlockAllocator.cpp
  src/Channel.cpp
  src/Color.cpp
  src/Constants.cpp
  src/DirectoriesApple.cpp
  src/DirectoriesUnix.cpp
  src/DirectoriesWin.cpp
  src/FileUnix.cpp
  src/FileWin.cpp
  src/Font.cpp
  src/GLView.cpp
  src/ViewController.cpp
  src/Graphics.cpp
  src/IO.cpp
  src/Image.cpp
  src/Input.cpp
  src/InputUIKit.cpp
  src/Inspection.cpp
  src/LargeImageData.cpp
  src/Macro.cpp
  src/MarkupParser.cpp
  src/Math.cpp
  src/OffScreenTarget.cpp
  src/Resolution.cpp
  src/TexChunk.cpp
  src/Text.cpp
  src/TextBuilder.cpp
  src/TextInput.cpp
  src/Texture.cpp
  src/TimingApple.cpp
  src/TimingUnix.cpp
  src/TimingWin.cpp
  src/Transform.cpp
  src/TrueTypeFont.cpp
  src/TrueTypeFontApple.cpp
  src/TrueTypeFontUnix.cpp
  src/TrueTypeFontWin.cpp
  src/Utility.cpp
  src/UtilityApple.cpp
  src/UtilityWin.cpp
  src/Version.cpp
  src/WinMain.cpp
  src/WinUtility.cpp
  src/Window.cpp
  src/WindowUIKit.cpp
  src/stb_vorbis.c
  src/utf8proc.c
  src/Scene.cpp
  src/Sprite.cpp
  src/module.cpp
  src/ruby_input.cpp
  src/ruby_roole.cpp
  src/ruby_load.cpp
  src/ruby_color.cpp
  src/ruby_sprite.cpp
  src/ruby_image.cpp
  src/ruby_font.cpp
  src/ruby_audio.cpp
  src/ruby_scene.cpp
  src/ruby_window.cpp
  main.cpp
)

source_group("Game Source" FILES ${MAIN_SOURCE} ${MAIN_HEADERS})

set(EMBEDDED_INPUT
  assets/icon
)
  #assets/gosu
  #"assets/constants
  #assets/patches
  #assets/swig_patches
  #assets/game )

#find_program(XXD_EXE xxd DOC "Location of the xxd executable")

#macro(ProcessWithXXD outvar inputfile outdir)
#  get_filename_component(basefile ${inputfile} NAME)
#  set(outputfile ${outdir}/${basefile}.xxd)
#  set_source_files_properties(${outputfile} PROPERTIES HEADER_ONLY TRUE)
#  add_custom_command(
#    OUTPUT ${outputfile}
#    COMMAND ${XXD_EXE} -i ${inputfile} ${outputfile}
#    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#    DEPENDS ${inputfile}
#    COMMENT "Generating XXD for ${inputfile}"
#  )
#  list(APPEND ${outvar} ${outputfile})
#endmacro()

#file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${PROJECT_NAME}.dir/xxd)
#include_directories(${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${PROJECT_NAME}.dir/xxd)

#foreach(item ${EMBEDDED_INPUT})
#  ProcessWithXXD(EMBEDDED_SOURCE ${item} ${CMAKE_CURRENT_BINARY_DIR})
#endforeach()

source_group("Embedded Source" FILES ${EMBEDDED_INPUT} ${EMBEDDED_SOURCE})

link_directories(
  ${SDL2_LIBRARY_DIRS}
  ${SNDFILE_LIBRARY_DIRS}
  ${VORBISFILE_LIBRARY_DIRS}
  ${MPG123_LIBRARY_DIRS}
  ${FREETYPE_LIBRARY_DIRS}
  ${FONTCONFIG_LIBRARY_DIRS}
  ${OPENGL_LIBRARY_DIR}
  ${MRI_LIBDIR}
)

add_executable(${PROJECT_NAME}
  ${MAIN_HEADERS}
  ${MAIN_SOURCE}
  ${EMBEDDED_SOURCE}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE ${DEFINES})
target_compile_options(${PROJECT_NAME} PRIVATE -w)

target_include_directories(${PROJECT_NAME} PRIVATE
  src
  hdr
  ${SDL2_INCLUDE_DIRS}
  ${SNDFILE_INCLUDE_DIRS}
  ${VORBISFILE_INCLUDE_DIRS}
  ${MPG123_INCLUDE_DIRS}
  ${FREETYPE_INCLUDE_DIR}
  ${FREETYPE_INCLUDE_DIRS}
  ${FONTCONFIG_INCLUDE_DIR}
  ${FONTCONFIG_INCLUDE_DIRS}
  ${MRI_INCLUDE_DIRS}
  ${OPENAL_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
  ${SDL2_LIBRARIES}
  ${SDL2_IMAGE_LIBRARIES}
  ${MRI_LIBRARIES}
  ${FREETYPE_LIBRARIES}
  ${FONTCONFIG_LIBRARIES}
  ${VORBISFILE_LIBRARIES}
  ${SNDFILE_LIBRARIES}
  ${MPG123_LIBRARIES}
  ${OPENAL_LIBRARY}
  ${OPENGL_LIBRARIES}
  ${ZLIB_LIBRARY}
  ${PLATFORM_LIBRARIES}
)
