#ifndef MESHCONFORMITY_H
#define MESHCONFORMITY_H

#include "Point.h"
#include "Mesh.h"
#include <vector>

enum class Boundary{
    Top, 
    Bottom,
    Right,
    Left
};

class MeshConformity
{
public:

    static bool checkConformity
    (
        const Mesh& mesh1,
        const Mesh& mesh2
    );

private:

    static std::vector<const Point*> getBoundary
    (
        const Mesh& mesh,
        Boundary b
    );

    static bool checkInterface
    (
        const Mesh& mesh1,
        Boundary b1,
        const Mesh& mesh2,
        Boundary b2
    );

};

#endif