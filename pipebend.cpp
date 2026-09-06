/*
Compiling Instructions : g++ pipebend.cpp src/*.cpp -Iinclude -std=c++17 -o pipebend && ./pipebend
*/
#include <iostream>

#include "Point.h"
#include "Line.h"
#include "Surface.h"
#include "Block.h"
#include "MultiBlockMesh.h"
#include "CoordinateMapping.h"
#include "MeshStatistics.h"
#include "MeshWriter.h"
#include "MeshQuality.h"

void inlet(Mesh& mesh, double L, double rInner, double rOuter){

    for(auto& node : mesh.nodes){
        double xi = node.x;
        double eta = node.y;
        node.x = rInner + (rOuter-rInner)*xi;
        node.y = L * eta;
    }
}

void outlet(Mesh& mesh, double L, double rInner, double rOuter){

    for(auto& node : mesh.nodes){
        double xi = node.x;
        double eta = node.y;
        node.y = -(rInner + (rOuter-rInner)*eta);
        node.x = -L * xi;
    }
}

int main(){

    //------------------------------------------------------------
    // User Inputs
    //------------------------------------------------------------

    double rInner = 5.0; //Pipe inner radius
    double rOuter = 10.0;//Pipe outer radius
    double lI     = 5.0; //input pipe length
    double lO     = 5.0; //output pipe length
    int Nr        = 200;  //radial node
    int Na        = 100;  //axial node
    int Nl        = 10;  //lateral node for pipes
    //---------------------------------------------------------------------
    // Blocks Geometry (Creating three xi eta domains (0<=xi<=1, 0<=eta<=1)
    //---------------------------------------------------------------------

    Point p0(0,0.0,0.0);
    Point p1(1,1.0,0.0);
    Point p2(2,1.0,1.0);
    Point p3(3,0.0,1.0);

    Line l0(0,&p0,&p1);
    Line l1(1,&p1,&p2);
    Line l2(2,&p2,&p3);
    Line l3(3,&p3,&p0);

    Surface block1Surface(0);

    block1Surface.addBoundary(&l0);
    block1Surface.addBoundary(&l1);
    block1Surface.addBoundary(&l2);
    block1Surface.addBoundary(&l3);

    Point p4(4,0.0,0.0);
    Point p5(5,1.0,0.0);
    Point p6(6,1.0,1.0);
    Point p7(7,0.0,1.0);

    Line l4(4,&p0,&p1);
    Line l5(5,&p1,&p2);
    Line l6(6,&p2,&p3);
    Line l7(7,&p3,&p0);

    Surface block2Surface(1);

    block2Surface.addBoundary(&l4);
    block2Surface.addBoundary(&l5);
    block2Surface.addBoundary(&l6);
    block2Surface.addBoundary(&l7);

    Point p8(8,0.0,0.0);
    Point p9(9,1.0,0.0);
    Point p10(10,1.0,1.0);
    Point p11(11,0.0,1.0);

    Line l8(8,&p0,&p1);
    Line l9(9,&p1,&p2);
    Line l10(10,&p2,&p3);
    Line l11(11,&p3,&p0);

    Surface block3Surface(2);

    block3Surface.addBoundary(&l0);
    block3Surface.addBoundary(&l1);
    block3Surface.addBoundary(&l2);
    block3Surface.addBoundary(&l3);

    //Create 3 xi eta mesh

    Mesh xi_eta_1, xi_eta_2, xi_eta_3;

    xi_eta_1.generateCartesian(
        block1Surface,
        Nr,
        Na
    );

    xi_eta_2.generateCartesian(
        block2Surface,
        Na,
        Nl
    );

    xi_eta_3.generateCartesian(
        block2Surface,
        Nl,
        Na
    );

    //Transformations
    Mesh pipemesh = xi_eta_1; 
    //transforming the first block to pipebend
    CoordinateMapping::PipeBend
    (
        pipemesh,
        rInner,
        rOuter
    );
    //transforming the rest of the blocks to inlet and outlet
    Mesh inletmesh = xi_eta_2;
    Mesh outletmesh = xi_eta_3;
    inlet(inletmesh, lI, rInner, rOuter);
    outlet(outletmesh, lO, rInner, rOuter);

    //------------------------------------------------------------
    // Add blocks with a id
    //------------------------------------------------------------

    Block block1(
        0, 
        inletmesh
    );

    Block block2(
        1,
        pipemesh
    );

    Block block3(
        2, 
        outletmesh
    );

    MultiBlockMesh mesh;
    mesh.addBlock(block1);
    mesh.addBlock(block2);
    mesh.addBlock(block3);
    mesh.generateFromMesh();

    //------------------------------------------------------------
    // Check Mesh Quality
    //------------------------------------------------------------

    auto results = MeshQuality::evaluate(mesh,5.0);
    MeshQuality::report(results);
    //------------------------------------------------------------
    // Export
    //------------------------------------------------------------

    MeshWriter::writeVTK
    (
        mesh,
        "pipebendMultiBlock.vtk"
    );

    return 0;
}