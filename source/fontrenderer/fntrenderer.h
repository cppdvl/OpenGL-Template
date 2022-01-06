//
// Created by User on 1/4/2022.
//

#pragma once


#include <shader.h>
#include <vector>
#include <map>
#include <utility>
#include <filesystem>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace Text {

    class AGlyph {
    public:
        unsigned int TextureID; // ID handle of the glyph texture
        glm::ivec2   Size;      // Size of glyph
        glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
        unsigned int Advance;   // Horizontal offset to advance to next glyph

    };

    static std::map<char, Text::AGlyph> GetCharacterSet(const std::string& fontPath);


    class Renderer {
        bool m_valid{false};
        std::map<GLchar, Text::AGlyph> cs;
        Shader shd;
    public:
        unsigned int VAO{0};
        unsigned int VBO{0};
        void Draw(const std::string&str, float x, float y, float scale, glm::vec3 color);
        Renderer(const std::string& vtxPath, const std::string& frgPath, const std::string& fontPath);

    };


}
