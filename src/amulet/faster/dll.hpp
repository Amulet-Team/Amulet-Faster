#pragma once

#ifndef AMULET_FASTER_EXPORT
    #ifdef _WIN32
        #ifdef ExportAmuletFaster
            #define AMULET_FASTER_EXPORT __declspec(dllexport)
        #else
            #define AMULET_FASTER_EXPORT __declspec(dllimport)
        #endif
    #else
        #define AMULET_FASTER_EXPORT
    #endif
#endif

#if !defined(AMULET_FASTER_EXPORT_EXCEPTION)
    #if defined(_LIBCPP_EXCEPTION)
        #define AMULET_FASTER_EXPORT_EXCEPTION __attribute__((visibility("default")))
    #else
        #define AMULET_FASTER_EXPORT_EXCEPTION
    #endif
#endif
