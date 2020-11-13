#pragma once

#include <string>

namespace CrashEngine {


	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void CreateMipmap() = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static std::shared_ptr<Texture2D> Create(uint32_t width, uint32_t height);
		static std::shared_ptr<Texture2D> Create(const std::string& path);
	};

	class TextureHDR : public Texture
	{
	public:
		static std::shared_ptr<TextureHDR> Create(const std::string& path);
	};

	class CubemapTexture : public Texture
	{
	public:
		static std::shared_ptr<CubemapTexture> Create(uint32_t width, uint32_t height, bool mipmap);
	};

}