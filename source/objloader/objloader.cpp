#include "objloader.h"

void BlenderImporter::OBJLoader::VisitVertex(){

    auto [tlx, tly, tlz] = std::make_tuple(std::stof(tokenizedline[1]), std::stof(tokenizedline[2]), std::stof(tokenizedline[3]));
    MeshElement::Vtx3f v(tlx, tly, tlz);
    m_vertices.push_back(v);

}

void BlenderImporter::OBJLoader::VisitUV() {

    auto [tlx, tly] = std::make_tuple(std::stof(tokenizedline[1]), std::stof(tokenizedline[2]));
    MeshElement::Vtx2f v(tlx, tly);
    m_uvs.push_back(v);

}

void BlenderImporter::OBJLoader::VisitNormal() {

    auto [tlx, tly, tlz] = std::make_tuple(std::stof(tokenizedline[1]), std::stof(tokenizedline[2]), std::stof(tokenizedline[3]));
    MeshElement::Vtx3f v(tlx, tly, tlz);
    m_normals.push_back(v);

}

void BlenderImporter::OBJLoader::VisitCamera() {


}

void BlenderImporter::OBJLoader::VisitObject() {

    BlenderImporter::Offset offset{};
    offset.name = tokenizedline[1];
    offset.faces = m_faces_vertices.size();
    offset.normals = m_normals.size();
    offset.uvs = m_uvs.size();
    offset.vertex = m_uvs.size();
    BlenderImporter::Visitor::offset.push_back(offset);

}

void BlenderImporter::OBJLoader::VisitFace() {

    MeshElement::FcIdx face_vertices;
    MeshElement::FcIdx face_uvs;
    MeshElement::FcIdx face_normals;
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
    m_faces_vertices.push_back(face_vertices);
    m_faces_uvs.push_back(face_uvs);
    m_faces_normals.push_back(face_normals);

}

void BlenderImporter::OBJLoader::Visit() {

    auto skip = [](){

    };
    std::map<std::string, std::function<void()>> router {
            {"v", [this](){m_state = State::VERTEX; this->VisitVertex(); }},
            {"vt", [this](){m_state = State::VERTEX_TEXTURE; this->VisitUV();}},
            {"vn", [this](){m_state = State::VERTEX_NORMAL; this->VisitNormal();}},
            {"f", [this](){m_state = State::FACE; this->VisitFace();}},
            {"o", [this](){m_state = State::OBJECT; this->VisitObject();}},
            {"#", [this](){}},
            {"s", [this](){}}
    };

    std::for_each(file.begin(), file.end(), [this, &router](auto& fileLine){

        auto lineReader = std::stringstream {fileLine};
        auto token = std::string{};
        tokenizedline.clear();
        while (getline(lineReader, token, ' '))
        {
            tokenizedline.push_back(token);
        }
        router[tokenizedline[0]]();

    });

}






