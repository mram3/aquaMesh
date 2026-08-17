/*
Compiling Instruction: g++ 05_InflationMesh.cpp src/*.cpp -Iinclude -std=c++17 -o inflationMesh && ./inflationMesh
*/
#include <iostream>

#include "Point.h"
#include "Line.h"
#include "Surface.h"

#include "Inflation.h"
#include "MeshStatistics.h"
#include "MeshWriter.h"

int main()
{
    std::cout << "==========================================" << std::endl;
    std::cout << " Homework 1.1 : Inflated Mesh Generation" << std::endl;
    std::cout << "==========================================" << std::endl;

    //------------------------------------------------------------
    // Step 1 : Create Geometry
    //------------------------------------------------------------

    Point p0(0,0.0,0.0);
    Point p1(1,4.0,0.0);
    Point p2(2,4.0,6.0);
    Point p3(3,0.0,6.0);

    Line l0(0,&p0,&p1);
    Line l1(1,&p1,&p2);
    Line l2(2,&p2,&p3);
    Line l3(3,&p3,&p0);

    Surface rectangle(0);

    rectangle.addBoundary(&l0);
    rectangle.addBoundary(&l1);
    rectangle.addBoundary(&l2);
    rectangle.addBoundary(&l3);

    //------------------------------------------------------------
    // Print Geometry
    //------------------------------------------------------------

    std::cout << "\n";
    std::cout << "Geometry Created" << std::endl;
    std::cout << "----------------" << std::endl;

    rectangle.print();

    //------------------------------------------------------------
    // Step 2 : Generate Cartesian Mesh
    //------------------------------------------------------------

    Inflation mesh;

    try{
        mesh.generateInflation(
            rectangle,
            0.12, //h0
            1.3,  //r
            15,   //Layer(Ny)
            20    //Nx
        );

    //------------------------------------------------------------
    // Step 3 : Print Mesh Statistics
    //------------------------------------------------------------

        MeshStatistics::print(mesh);

    //------------------------------------------------------------
    // Step 4 : Export Mesh
    //------------------------------------------------------------

        MeshWriter::writeVTK
        (
            mesh,
            "Homework1.vtk"
        );
    }
    catch(const std::invalid_argument& err){
        std::cerr << "\n[PROGRAM TERMINATED]\n" << err.what() << std::endl;
        return 1;
    }
    
    return 0;
}