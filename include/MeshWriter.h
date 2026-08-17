#ifndef MESHWRITER_H
#define MESHWRITER_H

#include <string>

#include "Mesh.h"
#include "Inflation.h"
#include "Bump.h"
class MeshWriter
{

public:

    static void writeVTK
    (
        const Mesh& mesh,
        const std::string& filename
    );

    static void writeVTK
    (
        const Inflation& inflation,
        const std::string& filename
    );

    static void writeVTK
    (
        const Bump& bump,
        const std::string& filename
    );
};

#endif