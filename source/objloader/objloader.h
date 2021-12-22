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


namespace GraphEntity
{
    using Color4 = glm::vec4;
    using Vtx3f = glm::vec3;
    using Vtx2f = glm::vec2;
    using FcIdx = std::array<unsigned int, 3>;

}

namespace BlenderImporter {


    struct Visitor {

        virtual void Visit();
        std::vector<std::string> file{};
        std::vector<std::string> tokenizedline{};
        std::map<std::string, std::function<void()>> m_router;
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
    struct OBJVisitor : public Visitor {

        struct Faces {
            std::vector<GraphEntity::FcIdx> m_faces_vertices;
            std::vector<GraphEntity::FcIdx> m_faces_uvs;
            std::vector<GraphEntity::FcIdx> m_faces_normals;
        };
        struct Data {
            std::string                                 m_mtllib;
            std::vector<GraphEntity::Vtx3f>             m_vertices;
            std::vector<GraphEntity::Vtx3f>             m_normals;
            std::vector<GraphEntity::Vtx2f>             m_uvs;
            std::map<std::string, unsigned long>        m_material_facesindex;
            OBJVisitor::Faces                           m_faces;
        };
        std::string mtllib;
        std::string mtlname;
        std::string objectname;
        std::map<std::string, OBJVisitor::Data> m_objectname_data;


        virtual void VisitMTLLib() = 0;
        virtual void VisitUseMtl() = 0;
        virtual void VisitVertex() = 0;
        virtual void VisitUV() = 0;
        virtual void VisitNormal() = 0;
        virtual void VisitObject() = 0;
        virtual void VisitFace() = 0;

        OBJVisitor(const std::string& path) : Visitor(path)
        {
            std::map<std::string, std::function<void()>> router {
                    {"mtllib", [this](){this->VisitMTLLib();}},
                    {"usemtl", [this](){this->VisitUseMtl();}},
                    {"v", [this](){this->VisitVertex(); }},
                    {"vt", [this](){this->VisitUV();}},
                    {"vn", [this](){this->VisitNormal();}},
                    {"f", [this](){this->VisitFace();}},
                    {"o", [this](){this->VisitObject();}},
                    {"#", [this](){}},
                    {"s", [this](){}}
            };
        }
    };
    struct MTLVisitor : public Visitor
    {
        struct Data {
            float                  m_d_opacity{1.0f};
            float                  m_Ns_specular_exp{0.0f};
            float                  m_Ni_refraction_index{0.0f};
            GraphEntity::Color4    m_Ka_ambient_color{0.0f, 0.0f, 0.0f, m_d_opacity};
            GraphEntity::Color4    m_Kd_diffuse_color{0.0f, 0.0f, 0.0f, m_d_opacity};
            GraphEntity::Color4    m_Ks_specular_color{0.0f, 0.0f, 0.0f, m_d_opacity};
            GraphEntity::Color4    m_Ke_emission_color{0.0f, 0.0f, 0.0f, m_d_opacity};
            unsigned long          m_illum{0u};
        };
        std::map<std::string, MTLVisitor::Data> m_data{};
        std::string lastIndex{};

        virtual void VisitNewMtl()  = 0;
        virtual void VisitNs()      = 0;
        virtual void VisitNi()      = 0;
        virtual void VisitKa()      = 0;
        virtual void VisitKd()      = 0;
        virtual void VisitKs()      = 0;
        virtual void VisitKe()      = 0;
        virtual void Visitd()       = 0;
        virtual void Visitillum()   = 0;

        MTLVisitor(const std::string& path) : Visitor(path)
        {
            m_router = std::map<std::string, std::function<void()>>{
                    {"newmtl", [this]() {this->VisitNewMtl();}},
                    {"d", [this]() {this->Visitd();}},
                    {"Ns", [this]() {this->VisitNs();}},
                    {"Ni", [this]() {this->VisitNi();}},
                    {"Ka", [this]() {this->VisitKa();}},
                    {"Kd", [this]() {this->VisitKd();}},
                    {"Ks", [this]() {this->VisitKs();}},
                    {"Ke", [this]() {this->VisitKe();}},
                    {"illum", [this]() {this->Visitillum();}},
                    {"#", [this]() {}}
            };

        }
    };
    struct MTLLoader : public MTLVisitor {

        void VisitNs()      override;
        void VisitNi()      override;
        void VisitKa()      override;
        void VisitKd()      override;
        void VisitKs()      override;
        void VisitKe()      override;
        void Visitd()       override;
        void Visitillum()   override;
        void VisitNewMtl()  override;
        MTLLoader(const std::string& path) : MTLVisitor(path){}

    };
    struct OBJLoader : public OBJVisitor {


        void VisitMTLLib() override;
        void VisitUseMtl() override;
        void VisitVertex() override;
        void VisitUV() override;
        void VisitNormal() override;
        void VisitObject() override;
        void VisitFace() override;
        OBJLoader(const std::string& path) : OBJVisitor(path){}
    };


}