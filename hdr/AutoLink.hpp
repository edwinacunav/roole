//! \file AutoLink.hpp
//! Contains pragmas that make MSVC link against all the necessary libraries
//! automatically.

#ifdef _MSC_VER
#pragma once

#ifdef NDEBUG
#pragma comment(lib, "Roole.lib")
#else
#pragma comment(lib, "RooleDebug.lib")
#endif

#endif
