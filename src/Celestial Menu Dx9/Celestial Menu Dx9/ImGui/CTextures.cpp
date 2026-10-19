#include "CTextures.h"

#include "Resources/Textures/image_head.h"
#include "Resources/Textures/image_neck.h"
#include "Resources/Textures/image_chest.h"
#include "Resources/Textures/image_arms.h"
#include "Resources/Textures/image_other.h"

Textures::Textures()
{
	tHead = nullptr;
	tNeck = nullptr;
	tChest = nullptr;
	tArms = nullptr;
	tOther = nullptr;
}

Textures::Textures(const Textures&)
{
}

Textures::~Textures()
{
}

bool Textures::Initialize(IDirect3DDevice9* pDevice)
{
	D3DXCreateTextureFromFileInMemoryEx(pDevice, &head_bytes, sizeof(head_bytes), 190, 304, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &tHead);
	D3DXCreateTextureFromFileInMemoryEx(pDevice, &neck_bytes, sizeof(neck_bytes), 190, 304, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &tNeck);
	D3DXCreateTextureFromFileInMemoryEx(pDevice, &chest_bytes, sizeof(chest_bytes), 190, 304, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &tChest);
	D3DXCreateTextureFromFileInMemoryEx(pDevice, &arms_bytes, sizeof(arms_bytes), 190, 304, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &tArms);
	D3DXCreateTextureFromFileInMemoryEx(pDevice, &other_bytes, sizeof(other_bytes), 190, 304, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &tOther);

	return true;
}

void Textures::Shutdown()
{
	tHead->Release();
	tNeck->Release();
	tChest->Release();
	tArms->Release();
	tOther->Release();
}