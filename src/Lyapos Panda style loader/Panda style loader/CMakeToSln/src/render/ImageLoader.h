#ifndef IMAGE_LOADER_HPP
#define IMAGE_LOADER_HPP

#if _HAS_CXX17
#include <string_view>
#else
#include <string>
#endif

#include <d3d11.h>

class ImageLoader {
public:
#if _HAS_CXX17
	using string_t = std::string_view;
#else
	using string_t = const std::string&;
#endif

	ImageLoader() = default;
	ImageLoader(ID3D11Device* dx_device, string_t image_url);
	~ImageLoader();

	/* Same as ImageLoader constructor just might be called later */
	void initialize(ID3D11Device* dx_device, string_t image_url);

	/* Get image instance */
	ID3D11ShaderResourceView* image() const noexcept;

private:;
	ID3D11ShaderResourceView* m_image;
};

#endif