#include "p_pch.h"
#include "TextRenderer.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

namespace Paper
{

	void TextRenderer::Init()
	{
		PAPER_PROFILE_FUNCTION();

		PAPER_CORE_ASSERT(!FT_Init_FreeType(&s_FTLib), "Couldn't initialize FreeType Library");
	}

	void TextRenderer::LoadFont(const std::string& fontpath, uint32_t size, uint32_t resolution, bool flip_font)
	{
		FT_Set_Pixel_Sizes(s_Face, 0, size);
		FT_New_Face(s_FTLib, fontpath.c_str(), 0, &s_Face);
		FT_Set_Char_Size(s_Face, 0, size << 6, resolution, resolution);

		int max_dim = (1 + (s_Face->size->metrics.height >> 6)) * ceilf(sqrtf(NUM_GLYPHS));
		int tex_width = 1;

		while(tex_width < max_dim) tex_width <<= 1;
		int tex_height = tex_width;

		char* pixels = (char*) calloc(tex_width * tex_height, 1);
		int pen_x = 0, pen_y = 0;

		for (unsigned char c = 0; c < NUM_GLYPHS; c++) {
			FT_Load_Char(s_Face, c, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT | FT_LOAD_TARGET_LIGHT);
			FT_Bitmap* bmp = &s_Face->glyph->bitmap;

			if(pen_x + bmp->width >= tex_width){
				pen_x = 0;
				pen_y += ((s_Face->size->metrics.height >> 6) + 1);
			}

			for (int row = 0; row < bmp->rows; row++){
				for (int col = 0; col < bmp->width; col++){
					int x = pen_x + col;
					int y = pen_y + row;
					pixels[y * tex_width + x] = bmp->buffer[row * bmp->pitch + col];
				}
			}

			s_Glyphs[c].Top = {pen_x, pen_y};
			s_Glyphs[c].Bottom = {pen_x + bmp->width, pen_y + bmp->rows};
			s_Glyphs[c].Offset = {s_Face->glyph->bitmap_left, s_Face->glyph->bitmap_top};
			s_Glyphs[c].Advance = s_Face->glyph->advance.x >> 6;

			pen_x += bmp->width + 1;
		}

		FT_Done_FreeType(s_FTLib);

		constexpr size_t color_bytes = 4;

		// Vertically Flipping Pixels
		for (int y = 0; y < tex_height / 2; y++) {
			for (int x = 0; x < tex_width; x++) {
				int _y = tex_height - y - 1;
				int _x = x;

				char t = pixels[y * tex_width + x];
				pixels[y * tex_width + x] = pixels[_y * tex_width + _x];
				pixels[_y * tex_width + _x] = t;
			}
		}

		unsigned char* png_data = (unsigned char*)calloc(tex_width * tex_height * color_bytes, 1);
		for (int i = 0; i < (tex_width * tex_height); ++i){
			png_data[i * 4 + 0] |= pixels[i];
			png_data[i * 4 + 1] |= pixels[i];
			png_data[i * 4 + 2] |= pixels[i];
			png_data[i * 4 + 3] = pixels[i] ? pixels[i] : 0x00;
		}

		s_TextureAtlas = Texture2D::Create(tex_width, tex_height);
		s_TextureAtlas->SetData(png_data, tex_width * tex_height * color_bytes);

		stbi_write_png("font_output.png", tex_width, tex_height, color_bytes, png_data, tex_width * color_bytes);

		free(png_data);
		free(pixels);
	}

	GlyphInfo TextRenderer::s_Glyphs[NUM_GLYPHS];
	Ref<Texture2D> TextRenderer::s_TextureAtlas = nullptr;
	FT_Library TextRenderer::s_FTLib;
	FT_Face TextRenderer::s_Face;
}