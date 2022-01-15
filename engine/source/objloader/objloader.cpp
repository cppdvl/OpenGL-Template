#include "objloader.h"

void BlenderImporter::OBJLoader::VisitVertex(){

    auto& obj = m_objectname_data[objectname];
    auto [tlx, tly, tlz] = std::make_tuple(std::stof(tokenizedline[1]), std::stof(tokenizedline[2]), std::stof(tokenizedline[3]));
    GraphEntity::Vtx3f v(tlx, tly, tlz);
    obj.m_vertices.push_back(v);

}

void BlenderImporter::OBJLoader::VisitUV() {

    auto& obj = m_objectname_data[objectname];
    auto [tlx, tly] = std::make_tuple(std::stof(tokenizedline[1]), std::stof(tokenizedline[2]));
    GraphEntity::Vtx2f v(tlx, tly);
    obj.m_uvs.push_back(v);

}

void BlenderImporter::OBJLoader::VisitNormal() {

    auto& obj = m_objectname_data[objectname];
    auto [tlx, tly, tlz] = std::make_tuple(std::stof(tokenizedline[1]), std::stof(tokenizedline[2]), std::stof(tokenizedline[3]));
    GraphEntity::Vtx3f v(tlx, tly, tlz);
    obj.m_normals.push_back(v);

}

void BlenderImporter::OBJLoader::VisitObject() {

    auto data = OBJVisitor::Data{};
    data.m_mtllib = mtllib;
    mtlname = std::string{};
    objectname = tokenizedline[1];
    m_objectname_data[objectname] = data;
}

void BlenderImporter::OBJLoader::VisitFace() {

    auto& obj = m_objectname_data[objectname];
    GraphEntity::FcIdx face_vertices;
    GraphEntity::FcIdx face_uvs;
    GraphEntity::FcIdx face_normals;
    for (auto index = 1; index <= 3; ++index)
    {
        auto vertexreader = std::stringstream {tokenizedline[index]};
        std::string token;
        unsigned int faceIndex = 0;

        while(getline(vertexreader, token, '/'))
        {
            auto& face = faceIndex % 3 == 0 ? face_vertices : ( faceIndex % 3 == 1 ? face_uvs : face_normals );
            face[index-1] = std::stoul(token) - 1; //  V / UV / NORMAL
            faceIndex++;
        }
    }


    if (!mtllib.empty() && !mtlname.empty()) obj.m_material_facesindex[mtlname] = obj.m_faces.m_faces_vertices.size();

    obj.m_faces.m_faces_vertices.push_back(face_vertices);
    obj.m_faces.m_faces_uvs.push_back(face_uvs);
    obj.m_faces.m_faces_normals.push_back(face_normals);

}

void BlenderImporter::MTLLoader::VisitNewMtl()
{
    lastIndex = tokenizedline[1];
    m_data[lastIndex] = MTLVisitor::Data();

}

void BlenderImporter::MTLLoader::VisitNs() {

    auto& m_data_ref = m_data[lastIndex];
    m_data_ref.m_Ns_specular_exp = std::stof(tokenizedline[1]);

}

void BlenderImporter::MTLLoader::VisitKa() {

    auto [r,g,b] = std::make_tuple(std::stof(tokenizedline[1]), std::stof(tokenizedline[2]), std::stof(tokenizedline[3]));
    auto& ref = m_data[lastIndex];
    ref.m_Ka_ambient_color = GraphEntity::Color4 (r,g,b,ref.m_d_opacity);

}

void BlenderImporter::MTLLoader::VisitKd() {

    auto [r,g,b] = std::make_tuple(std::stof(tokenizedline[1]), std::stof(tokenizedline[2]), std::stof(tokenizedline[3]));
    auto& ref = m_data[lastIndex];
    ref.m_Kd_diffuse_color = GraphEntity::Color4 (r,g,b,ref.m_d_opacity);


}

void BlenderImporter::MTLLoader::VisitKs() {

    auto [r,g,b] = std::make_tuple(std::stof(tokenizedline[1]), std::stof(tokenizedline[2]), std::stof(tokenizedline[3]));
    auto& ref = m_data[lastIndex];
    ref.m_Ks_specular_color = GraphEntity::Color4 (r,g,b,ref.m_d_opacity);

}

void BlenderImporter::MTLLoader::VisitKe() {

    auto [r,g,b] = std::make_tuple(std::stof(tokenizedline[1]), std::stof(tokenizedline[2]), std::stof(tokenizedline[3]));
    auto& ref = m_data[lastIndex];
    ref.m_Ke_emission_color = GraphEntity::Color4 (r,g,b,ref.m_d_opacity);

}

void BlenderImporter::MTLLoader::VisitNi() {

    auto& ref = m_data[lastIndex];
    ref.m_Ni_refraction_index = std::stof(tokenizedline[1]);

}

void BlenderImporter::MTLLoader::Visitillum() {

    auto& ref = m_data[lastIndex];
    ref.m_illum = std::stoul(tokenizedline[1]);

}

void BlenderImporter::MTLLoader::Visitd() {

    auto& ref = m_data[lastIndex];
    ref.m_d_opacity = std::stof(tokenizedline[1]);

    ref.m_Ka_ambient_color.a = ref.m_d_opacity;
    ref.m_Kd_diffuse_color.a = ref.m_d_opacity;
    ref.m_Ks_specular_color.a = ref.m_d_opacity;
    ref.m_Ke_emission_color.a = ref.m_d_opacity;

}

void BlenderImporter::Visitor::Visit() {

    std::for_each(file.begin(), file.end(), [this](auto& fileLine){

        auto lineReader = std::stringstream {fileLine};
        auto token = std::string{};
        tokenizedline.clear();
        while (getline(lineReader, token, ' '))
        {
            tokenizedline.push_back(token);
        }
        this->m_router[tokenizedline[0]]();

    });

}

void BlenderImporter::OBJLoader::VisitMTLLib() {

    mtllib = tokenizedline[1];
    mtlname = std::string{};
    objectname = std::string{};

}
void BlenderImporter::OBJLoader::VisitUseMtl() {
    mtlname = tokenizedline[1];
}



