#ifndef BLOCK_H
#define BLOCK_H

#include "Surface.h"
#include "Mesh.h"

class Block
{
public:

    int id;

    Surface surface;

    Mesh mesh;

    int Nx;

    int Ny;

    Block();

    Block
    (
        int id_,
        const Surface& surface_,
        int Nx_,
        int Ny_
    );

    Block
    (
        int id_,
        const Mesh& mesh_
    );

    void generateMesh();

};

#endif