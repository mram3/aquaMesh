/*
Compiler instruction:  &&g++ 02_MultiBlock.cpp src/*.cpp -Iinclude -std=c++17 -o MultiBlock ./MultiBlock
*/
#include <iostream>

#include "Point.h"
#include "Line.h"
#include "Surface.h"

#include "Block.h"
#include "MultiBlockMesh.h"

#include "MeshStatistics.h"
#include "MeshWriter.h"

int main()
{
    std::cout << "==========================================" << std::endl;
    std::cout << " Example 02 : Multi Block Mesh" << std::endl;
    std::cout << "==========================================" << std::endl;

    //------------------------------------------------------------
    // Block 1 Geometry
    //------------------------------------------------------------

    Point p0(0,3.0,0.0);
    Point p1(1,6.0,0.0);
    Point p2(2,6.0,3.0);
    Point p3(3,3.0,3.0);

    Line l0(0,&p0,&p1);
    Line l1(1,&p1,&p2);
    Line l2(2,&p2,&p3);
    Line l3(3,&p3,&p0);

    Surface block1Surface(0);

    block1Surface.addBoundary(&l0);
    block1Surface.addBoundary(&l1);
    block1Surface.addBoundary(&l2);
    block1Surface.addBoundary(&l3);

    //------------------------------------------------------------
    // Block 2 Geometry
    //------------------------------------------------------------

    Point p4(4,3.0,0.0);
    Point p5(5,6.0,0.0);
    Point p6(6,6.0,-6.0);
    Point p7(7,3.0,-6.0);

    Line l4(4,&p4,&p5);
    Line l5(5,&p5,&p6);
    Line l6(6,&p6,&p7);
    Line l7(7,&p7,&p4);

    Surface block2Surface(1);

    block2Surface.addBoundary(&l4);
    block2Surface.addBoundary(&l5);
    block2Surface.addBoundary(&l6);
    block2Surface.addBoundary(&l7);

        //------------------------------------------------------------
    // Block 3 Geometry
    //------------------------------------------------------------

    Point p8(8,0.0,-3.0);
    Point p9(9,0.0,-6.0);
    Point p10(10,-3.0,-6.0);
    Point p11(11,-3.0,-3.0);

    Line l8(8,&p8,&p9);
    Line l9(9,&p9,&p10);
    Line l10(10,&p10,&p11);
    Line l11(11,&p11,&p8);

    Surface block3Surface(2);

    block3Surface.addBoundary(&l8);
    block3Surface.addBoundary(&l9);
    block3Surface.addBoundary(&l10);
    block3Surface.addBoundary(&l11);

    //------------------------------------------------------------
    // Create Blocks
    //------------------------------------------------------------

    Block block1
    (
        0,
        block1Surface,
        20,
        10
    );

    Block block2
    (
        1,
        block2Surface,
        20,
        10
    );

    Block block3
    (
        2,
        block3Surface,
        10,
        20
    );

    //------------------------------------------------------------
    // Assemble Multi Block Mesh
    //------------------------------------------------------------

    MultiBlockMesh mesh;

    mesh.addBlock(block1);
    mesh.addBlock(block2);
    mesh.addBlock(block3);
    mesh.generate();

    //------------------------------------------------------------
    // Statistics
    //------------------------------------------------------------

    MeshStatistics::print(mesh);

    //------------------------------------------------------------
    // Export
    //------------------------------------------------------------

    MeshWriter::writeVTK
    (
        mesh,
        "threeBlockMesh.vtk"
    );

    return 0;
}