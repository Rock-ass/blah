#pragma once
#include <inttypes.h>
#include <blah/math/rect.h>
#include <blah/containers/str.h>
#include <blah/graphics/texture.h>
#include <blah/graphics/framebuffer.h>
#include <blah/graphics/mesh.h>
#include <blah/graphics/shader.h>
#include <blah/graphics/material.h>
#include <memory>

namespace Blah
{
	class Stream;
	class Image;

	enum class Compare
	{
		None,
		Always,
		Never,
		Less,
		Equal,
		LessOrEqual,
		Greater,
		NotEqual,
		GreatorOrEqual
	};

	enum class Cull
	{
		None = 0,
		Front = 1,
		Back = 2,
		Both = 3
	};

	enum class BlendOp
	{
		Add,
		Subtract,
		ReverseSubtract,
		Min,
		Max
	};

	enum class BlendFactor
	{
		Zero,
		One,
		SrcColor,
		OneMinusSrcColor,
		DstColor,
		OneMinusDstColor,
		SrcAlpha,
		OneMinusSrcAlpha,
		DstAlpha,
		OneMinusDstAlpha,
		ConstantColor,
		OneMinusConstantColor,
		ConstantAlpha,
		OneMinusConstantAlpha,
		SrcAlphaSaturate,
		Src1Color,
		OneMinusSrc1Color,
		Src1Alpha,
		OneMinusSrc1Alpha
	};

	enum class BlendMask
	{
		None	= 0,
		Red		= 1,
		Green   = 2,
		Blue	= 4,
		Alpha   = 8,
		RGB		= Red | Green | Blue,
		RGBA	= Red | Green | Blue | Alpha,
	};

	struct BlendMode
	{
		BlendOp colorOp;
		BlendFactor colorSrc;
		BlendFactor colorDst;
		BlendOp alphaOp;
		BlendFactor alphaSrc;
		BlendFactor alphaDst;
		BlendMask mask;
		uint32_t rgba;

		BlendMode() = default;

		BlendMode(BlendOp op, BlendFactor src, BlendFactor dst)
		{
			colorOp = op;
			colorSrc = src;
			colorDst = dst;
			alphaOp = op;
			alphaSrc = src;
			alphaDst = dst;
			mask = BlendMask::RGBA;
			rgba = 0xffffffff;
		}
		
		BlendMode(BlendOp rgbOp, BlendFactor rgbSrc, BlendFactor rgbDst, BlendOp aOp, BlendFactor aSrc, BlendFactor aDst, BlendMask blendMask, uint32_t blendColor)
		{
			colorOp = rgbOp;
			colorSrc = rgbSrc;
			colorDst = rgbDst;
			alphaOp = aOp;
			alphaSrc = aSrc;
			alphaDst = aDst;
			mask = blendMask;
			rgba = blendColor;
		}

		inline bool operator==(const BlendMode& rhs) const {
			return colorOp == rhs.colorOp && colorSrc == rhs.colorSrc && colorDst == rhs.colorDst &&
				   alphaOp == rhs.alphaOp && alphaSrc == rhs.alphaSrc && alphaDst == rhs.alphaDst &&
				   mask == rhs.mask && rgba == rhs.rgba;
		}
		inline bool operator!=(const BlendMode& rhs) const { return !(*this == rhs); }

		static const BlendMode Normal;
		static const BlendMode Subtract;
	};

	struct RenderCall
	{
		// Framebuffer to draw to
		FrameBufferRef target;

		// Mesh to draw with
		MeshRef mesh;

		// Material to draw with
		MaterialRef material;

		// Whether the Render Call should use a specific viewport
		bool has_viewport;

		// Whether the Render Call should use a scissor rectangle
		bool has_scissor;

		// The viewport (only used if hasViewport is true)
		Rect viewport;

		// The scissor rectangle (only used if hasScissor is true)
		Rect scissor;

		// First index in the Mesh to draw from
		int64_t index_start;

		// Total amount of indices to draw from the Mesh
		int64_t index_count;

		// Total amount of instances to draw from the Mesh
		int64_t instance_count;

		// Depth Compare Function
		Compare depth;

		// Cull Mode
		Cull cull;

		// Blend Mode
		BlendMode blend;

		// Initializes a default Render Call
		RenderCall();
	};

	enum class GraphicsRenderer
	{
		None = -1,
		OpenGL,
		D3D11,
		Metal,
		Count
	};

	struct GraphicsInfo
	{
		bool instancing = false;
		bool origin_bottom_left = false;
		int max_texture_size = 0;
	};

	namespace Graphics
	{
		const FrameBufferRef backbuffer;

		// Gets graphics information from the graphics device
		const GraphicsInfo* info();

		// Gets the Renderer implementation type
		GraphicsRenderer renderer();

		// Creates a new Texture.
		// If the Texture creation fails, it will return an invalid TextureRef.
		TextureRef create_texture(const Image& image);

		// Creates a new Texture.
		// If the Texture creation fails, it will return an invalid TextureRef.
		TextureRef create_texture(int width, int height, unsigned char* rgba);

		// Creates a new Texture.
		// If the Texture creation fails, it will return an invalid TextureRef.
		TextureRef create_texture(int width, int height, TextureFormat format);

		// Creates a new Texture from a Stream.
		// If the Texture creation fails, it will return an invalid TextureRef.
		TextureRef create_texture(Stream& stream);

		// Creates a new Texture from a File.
		// If the Texture creation fails, it will return an invalid TextureRef.
		TextureRef create_texture(const char* file);

		// Creates a new FrameBuffer with a single Color attachment
		// If the FrameBuffer creation fails, it will return an invalid FrameBufferRef.
		FrameBufferRef create_framebuffer(int width, int height);

		// Creates a new FrameBuffer with the given Texture Attachments. You must provide at least one Attachment.
		// If the FrameBuffer creation fails, it will return an invalid FrameBufferRef.
		FrameBufferRef create_framebuffer(int width, int height, const TextureFormat* attachments, int attachmentCount);

		// Creates a Shader with the given Shader Data.
		// If the Shader creation fails, it will return an invalid ShaderRef.
		ShaderRef create_shader(const ShaderData* data);

		// Creates a new Material from the given Shader.
		// If the Shader is invalid, it will return an invalid MaterialRef.
		MaterialRef create_material(const ShaderRef& shader);

		// Creates a new Mesh.
		// If the Mesh creation fails, it will return an invalid Mesh.
		MeshRef create_mesh();

		// Submits and Flushes the given render call to the graphics device.
		void render(const RenderCall& render_call);

		// Clears the given FrameBuffer to the given color.
		void clear(const FrameBufferRef& target, uint32_t rgba);
	}
}