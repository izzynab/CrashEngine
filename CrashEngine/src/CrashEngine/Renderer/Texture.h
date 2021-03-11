#pragma once

#include <string>
#include <memory>

namespace CrashEngine {

	enum InternalFormat
	{
		RGB16F = 0x881B, 
		RGBA16F = 0x881A,
		RGB32F = 0x8814,
		RGB8 = 0x8051,
		RGBA8 = 0x8058,
		RGBA16  = 0x805B
	};

	enum DataFormat
	{
		RGB = 0x1907,
		RGBA = 0x1908,
	};

	enum WrapParam
	{
		CLAMP = 0x2900,
		REPEAT = 0x2901,
		CLAMP_TO_EDGE = 0x812F,
		CLAMP_TO_BORDER = 0x812D
	};

	enum FilterParam
	{
		NEAREST = 0x2600,
		LINEAR = 0x2601
	};
	
	enum Type
	{
		UNSIGNED_BYTE = 0x1401,
		INT = 0x1404,
		FLOAT = 0x1406
	};
	

	struct TextureSpecification
	{
		uint32_t Width = 0, Height = 0;
		InternalFormat internalFormat = InternalFormat::RGBA8;
		DataFormat DataFormat = DataFormat::RGBA;
		WrapParam WrapParam = WrapParam::CLAMP_TO_EDGE;
		FilterParam FilterParam = FilterParam::LINEAR;
		Type type = Type::UNSIGNED_BYTE;
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;

		virtual void SetData(void* data) = 0;

		virtual void CreateMipmap() = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static std::shared_ptr<Texture2D> Create(TextureSpecification spec);
		static std::shared_ptr<Texture2D> Create(uint32_t width, uint32_t height);
		static std::shared_ptr<Texture2D> Create(const std::string& path);

		std::string m_Path;
	};

	class DepthTexture : public Texture
	{
	public:
		static std::shared_ptr<DepthTexture> Create(uint32_t width, uint32_t height);

		std::string m_Path;
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
