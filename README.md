# Roole

This engine is focused on providing you with a simple binary executable capable of running on any Debian based Linux OS
without forcing the player to previously install Ruby on it.

You can compile it on any other Linux OS thanks to tools like CMake that let me build it on Debian.

## About Gosu for Ruby

Because of people like Julian Raschke, Jan Lücker and other contributors that made Gosu for Ruby what it is now, this project has come into existence.

## Warnings

This executable does not embed your custom Ruby scripts or any resources in this engine.

## Instructions:

This engine requires you to create a load.txt file where you can enter as many Ruby script filenames as needed.

Examples:
main
lib/scene

The file extension is optional.
Leave a blank line at the end of the file!

## Dependencies / Building

* OpenAL
* SDL2*
* SDL2_image
* vorbisfile
* zlib (only for Ruby)
* OpenGL header (alternatively GLES2 with `DEFINES+=GLES2_HEADER`)
* libiconv (on Windows, optional with INI_ENCODING)

### CMake

You can compile it with CMake by running my custom bash script compile.sh on your terminal by typing the following in the project's root directory:

./compile.sh

You might need to use chmod +x compile.sh first to make it an executable script.

It will create a build directory where it will run CMake and later execute make to compile the HiddenChest binary. If succeeded it will move the binary executable file to the root directory for you.

### Supported image/audio formats

File formats like MP3, OGG, BMP, JPG, PNG and TGA are supported.