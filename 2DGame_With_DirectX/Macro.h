#pragma once

template <typename T>
inline void safeRelease(T& ptr)
{
    if (ptr)
    {
        ptr->Release();
        ptr = NULL;
    }
}
#define SAFE_RELEASE safeRelease            

template <typename T>
inline void safeDelete(T& ptr)
{
    if (ptr)
    {
        delete ptr;
        ptr = NULL;
    }
}
#define SAFE_DELETE safeDelete              

template <typename T>
inline void safeDeleteArray(T& ptr)
{
    if (ptr)
    {
        delete[] ptr;
        ptr = NULL;
    }
}
#define SAFE_DELETE_ARRAY safeDeleteArray

template <typename T>
inline void safeOnLostDevice(T& ptr)
{
    if (ptr)
        ptr->onLostDevice();
}
#define SAFE_ON_LOST_DEVICE safeOnLostDevice 

template <typename T>
inline void safeOnResetDevice(T& ptr)
{
    if (ptr)
        ptr->onResetDevice();
}
#define SAFE_ON_RESET_DEVICE safeOnResetDevice

#define TRANSCOLOR  SETCOLOR_ARGB(0,255,0,255)  // transparent color (magenta)