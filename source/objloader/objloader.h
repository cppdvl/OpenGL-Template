//
// Created by the CPPDVL on 11/21/2021.
//

#pragma once
#include <map>
#include <array>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <functional>

#include "glm/glm.hpp"


namespace MeshElement
{
    using Vtx3f = glm::vec3;
    using Vtx2f = glm::vec2;
    using FcIdx = std::array<unsigned int, 3>;

}

namespace BlenderImporter {

    struct Offset {
        std::string name{};
        unsigned int vertex{0};
        unsigned int uvs{0};
        unsigned int normals{0};
        unsigned int faces{0};
    };

    struct Visitor {


        std::vector<BlenderImporter::Offset>offset{};
        std::vector<std::string> file{};
        std::vector<std::string> tokenizedline{};

        std::vector<MeshElement::Vtx3f> m_vertices;
        std::vector<MeshElement::Vtx3f> m_normals;
        std::vector<MeshElement::Vtx2f> m_uvs;
        std::vector<MeshElement::FcIdx> m_faces_vertices;
        std::vector<MeshElement::FcIdx> m_faces_uvs;
        std::vector<MeshElement::FcIdx> m_faces_normals;

        virtual void VisitVertex() = 0;
        virtual void VisitUV() = 0;
        virtual void VisitNormal() = 0;
        virtual void VisitObject() = 0;
        virtual void VisitFace() = 0;
        virtual void VisitCamera() = 0;

        virtual void Visit() = 0;

        Visitor(const std::string& path)
        {
            auto file = std::ifstream (path);
            if (file.is_open())
            {
                auto line = std::string{};
                while (getline(file, line))
                {
                    this->file.emplace_back(line);
                }
                file.close();
            }
        }


    };

    struct OBJLoader : public Visitor {

        enum class State {
            VERTEX,
            VERTEX_TEXTURE,
            VERTEX_NORMAL,
            FACE,
            OBJECT
        };

        State m_state;

        void VisitVertex() override;
        void VisitUV() override;
        void VisitNormal() override;
        void VisitCamera() override;
        void VisitObject() override;
        void VisitFace() override;
        void Visit() override;
        OBJLoader(const std::string& path) : Visitor(path){}
    };
}