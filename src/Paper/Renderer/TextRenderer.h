#pragma once

#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>

#include "Texture.h"

#define NUM_GLYPHS 128

namespace Paper
{
	struct GlyphInfo
	{
		glm::vec2 Bottom;
		glm::vec2 Top;
		glm::vec2 Offset;
		float Advance;
	};

	class TextRenderer
	{
	public:
		static void Init();
		static void LoadFont(const std::string& fontpath, uint32_t size, uint32_t resolution, bool flip_font = true);

		static GlyphInfo* GetGlyphInfos() { return s_Glyphs; }
		static Ref<Texture2D> GetTexture() { return s_TextureAtlas; }
	private:
		static GlyphInfo s_Glyphs[NUM_GLYPHS];

		static Ref<Texture2D> s_TextureAtlas;

		static FT_Library s_FTLib;
		static FT_Face s_Face;
	};
}