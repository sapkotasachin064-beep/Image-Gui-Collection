#include "ImageLoader.h"

#include <Windows.h>
#include <wininet.h>
#include <vector>
#include <stdexcept>
#include <memory>
#include <d3dx11.h>

#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "wininet.lib")

#define MAX_FILESIZE_TO_READ 4096

ImageLoader::ImageLoader(ID3D11Device* dx_device, string_t image_url) : m_image(nullptr)
{
	if (m_image != nullptr)
		return;

	initialize(dx_device, image_url);
}

ImageLoader::~ImageLoader() {}

void ImageLoader::initialize(ID3D11Device* dx_device, string_t image_url)
{
	if (m_image != nullptr)
		return;

	std::unique_ptr<std::remove_pointer_t<HINTERNET>, decltype(&InternetCloseHandle)>
		hInternet(InternetOpenA("ImGui ImageLoader", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0), InternetCloseHandle);

	if (!hInternet) {
		throw std::runtime_error("InternetOpenA failed");
	}

	std::unique_ptr<std::remove_pointer_t<HINTERNET>, decltype(&InternetCloseHandle)>
		hURL(InternetOpenUrlA(hInternet.get(), image_url.data(), NULL, 0, INTERNET_FLAG_RELOAD, 0), InternetCloseHandle);

	if (!hURL) {
		throw std::runtime_error("InternetOpenUrl failed");
	}

	std::vector<BYTE> image_data;
	std::vector<BYTE> buffer(MAX_FILESIZE_TO_READ); // Why the heap and not the stack? Because that's the way i want it.
	DWORD bytes_read = 0;
	while (InternetReadFile(hURL.get(), buffer.data(), MAX_FILESIZE_TO_READ, &bytes_read) && bytes_read > 0) {
		image_data.insert(image_data.end(), buffer.data(), buffer.data() + bytes_read);
	}

	D3DX11_IMAGE_LOAD_INFO image_load_info;
	ID3DX11ThreadPump* thread_pump{ nullptr };
	HRESULT hr = D3DX11CreateShaderResourceViewFromMemory(dx_device, image_data.data(), image_data.size(), &image_load_info, thread_pump, &m_image, 0);
	if (FAILED(hr)) {
		throw std::runtime_error("D3DX11CreateShaderResourceViewFromMemory failed");
	}
}

ID3D11ShaderResourceView* ImageLoader::image() const noexcept
{
	return m_image;
}