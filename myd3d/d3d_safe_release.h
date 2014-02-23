#pragma once

template<class T>
void d3d_safe_release(T* obj)
{
    if(obj)
    {
        obj->Release();
        obj = 0;
    }
}


template<class T>
void d3d_safe_unacquire_release(T* obj)
{
    if(obj)
    {
        obj->Unacquire();
        obj->Release();
        obj = 0;
    }
}
