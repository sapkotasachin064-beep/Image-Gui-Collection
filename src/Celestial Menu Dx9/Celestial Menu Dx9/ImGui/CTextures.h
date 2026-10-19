#pragma once

#include <d3d9.h>
#include <d3dx9.h>

class Textures
{
public:
    Textures();
    Textures(const Textures&);
    ~Textures();

    bool Initialize(IDirect3DDevice9* pDevice);
    void Shutdown();

public:
    IDirect3DTexture9* tHead;
    IDirect3DTexture9* tNeck;
    IDirect3DTexture9* tChest;
    IDirect3DTexture9* tArms;
    IDirect3DTexture9* tOther;
};
inline Textures dtextures;