#ifndef CONFIGURE_H
#define CONFIGURE_H

#ifdef __linux__

    #define ACT_C_DLL_API
    #define ACT_CPP_DLL_API

#else

    #define ACT_C_DLL_API extern "C" __declspec(dllexport)
    #define ACT_CPP_DLL_API __declspec(dllexport)

#endif

#endif // CONFIGURE_H
