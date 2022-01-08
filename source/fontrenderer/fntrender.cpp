//
// Created by User on 12/22/2021.
//
#include <shader.h>
#include <vector>
#include <map>
#include <utility>
#include <filesystem>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "fntrenderer.h"


namespace Text{


    static std::map<char, Text::AGlyph> GetCharacterSet (const std::string& fontPath)
    {
        std::map<char, Text::AGlyph>Characters{};
        FT_Library ft;
        // All functions return a value different than 0 whenever an error occurred
        if (FT_Init_FreeType(&ft))
        {
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
            return std::map<char, Text::AGlyph>{};
        }

        // find path to font
        std::filesystem::path fsFontPath = fontPath;
        if (fsFontPath.empty())
        {
            std::cout << "ERROR::FREETYPE: Failed to load fsFontPath" << std::endl;
            return std::map<char, Text::AGlyph>{};
        }

        // load font as face
        FT_Face face;
        if (FT_New_Face(ft, fsFontPath.string().c_str(), 0, &face)) {
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
            return std::map<char, Text::AGlyph>{};
        }
        else {
            // set size to load glyphs as
            FT_Set_Pixel_Sizes(face, 0, 48);

            // disable byte-alignment restriction
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            // load first 128 characters of ASCII set
            for (unsigned char c = 0; c < 128; c++)
            {
                // Load character glyph
                if (FT_Load_Char(face, c, FT_LOAD_RENDER))
                {
                    std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                    continue;
                }
                // generate texture
                unsigned int texture;
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);
                glTexImage2D(
                        GL_TEXTURE_2D,
                        0,
                        GL_RED,
                        face->glyph->bitmap.width,
                        face->glyph->bitmap.rows,
                        0,
                        GL_RED,
                        GL_UNSIGNED_BYTE,
                        face->glyph->bitmap.buffer
                );
                // set texture options
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                // now store character for later use
                AGlyph character = {
                        texture,
                        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                        static_cast<unsigned int>(face->glyph->advance.x)
                };
                Characters.insert(std::pair<char, Text::AGlyph>(c, character));
            }
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        // destroy FreeType once we're finished
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
        return Characters;
    }

    void Renderer::Draw(const std::string& str, float x, float y, float scale, glm::vec3 color)
    {
        shd.use();
        glUniform3f(glGetUniformLocation(shd.ID, "textColor"), color.x, color.y, color.z);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        for(auto const& ctext : str)
        {
            auto cglyph = cs[ctext];

            auto position = glm::vec2(x,y);
            position += glm::vec2 (cglyph.Bearing.x  , cglyph.Bearing.y - cglyph.Size.y) * scale;
            auto size = glm::vec2 (cglyph.Size.x, cglyph.Size.y) * scale;
            auto position_size = position+size;
            //auto vertices_ = std::vector<float>{
            float vertices[6][4] = {

                position.x,         position_size.y,    0.0f,   0.0f,
                position.x,         position.y,         0.0f,   1.0f,
                position_size.x,    position.y,         1.0f,   1.0f,

                position.x,         position_size.y,    0.0f,   0.0f,
                position_size.x,    position.y,         1.0f,   1.0f,
                position_size.x,    position_size.y,    1.0f,   0.0f

            };
            //auto vertices = vertices_.data();
            glBindTexture(GL_TEXTURE_2D, cglyph.TextureID);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            //Draw the Quad : 2 triangles x 3 vert / trngl = 6 vert
            glDrawArrays(GL_TRIANGLES,0 ,6);

            //NEXT GLYPH
            auto glyphAdvanceInPixels = cglyph.Advance >> 6;
            x += glyphAdvanceInPixels * scale;
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    Renderer::Renderer(const std::string& vtxPath, const std::string& frgPath, const std::string& fontPath) : shd(Shader(vtxPath.c_str(), frgPath.c_str()))
    {

        cs = GetCharacterSet(fontPath);
        if (cs.empty()) return;
        m_valid = true;
        //Reserve memory (GPU) for data:
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    void Renderer::SetCanvas(unsigned int width, unsigned int height)
    {
        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height));
        shd.use();
        glUniformMatrix4fv(glGetUniformLocation(shd.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    }

}