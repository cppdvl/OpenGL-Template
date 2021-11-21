#include "objloader.h"

int main(void)
{
    BlenderImporter::OBJLoader importer("E:/WorkingDirectory/ModelSamples/sample.obj");
    importer.Visit();
    return 0;
}
