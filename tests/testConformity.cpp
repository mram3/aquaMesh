#include <iostream>

#include <iostream>
#include <cmath>

#include "Point.h"
#include "Line.h"
#include "Surface.h"
#include "Block.h"
#include "MultiBlockMesh.h"
#include "CoordinateMapping.h"
#include "MeshQuality.h"
#include "MeshConformity.h"
#include "Test.h"

using namespace std;

int main(){

    Test test;
    //xi_eta_blocks 

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

    Mesh xi_eta_1, xi_eta_2;

    xi_eta_1.generateCartesian(
        block1Surface,
        10,
        10
    );

    xi_eta_2.generateCartesian(
        block2Surface,
        10,
        10
    );

    CoordinateMapping::PipeBend(
        xi_eta_1,
        acos(-1)/2,
        5,
        10
    );

    CoordinateMapping::outlet(
        xi_eta_2,
        5,
        acos(-1)/2,
        5,
        10
    );

    //testing the conformal check

    test.expectTrue(
        MeshConformity::checkConformity(xi_eta_1, xi_eta_2),
        "Mesh Conformity Check"
    );

    //testing the Mesh Quality class

    auto results = MeshQuality::evaluate(xi_eta_1, 5);
    test.expectTrue(
        !MeshQuality::hasInvertedCells(results),
        "Inverted Cells Check"
    );

    test.expectTrue(
        !MeshQuality::hasLowQualityCells(results),
        "Low Quality Cells Check"
    );

    test.summary();

    return test.success() ? 0 : 1;
}