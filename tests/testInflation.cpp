// Compiling Instruction: g++ tests/testInflation.cpp src/*.cpp -Iinclude -std=c++17 -o testInflation && ./testInflation
#include "Test.h"

#include "Point.h"
#include "Line.h"
#include "Surface.h"
#include "Inflation.h"

#include <cmath>

int main()
{
    Test test;

    // --------------------------------------------------
    // 1. Create geometry
    // --------------------------------------------------

    Point p0(0, 0.0, 0.0);
    Point p1(1, 4.0, 0.0);
    Point p2(2, 4.0, 6.0);
    Point p3(3, 0.0, 6.0);

    Line l0(0, &p0, &p1);
    Line l1(1, &p1, &p2);
    Line l2(2, &p2, &p3);
    Line l3(3, &p3, &p0);

    Surface rectangle(0);

    rectangle.addBoundary(&l0);
    rectangle.addBoundary(&l1);
    rectangle.addBoundary(&l2);
    rectangle.addBoundary(&l3);


    // --------------------------------------------------
    // 2. Generate inflation mesh
    // --------------------------------------------------

    Inflation mesh;

    mesh.generateInflation(
        rectangle,
        0.12,   // h0
        1.3,    // growth ratio
        15,     // requested inflation layers
        20      // Nx
    );


    // --------------------------------------------------
    // 3. Check number of nodes and cells
    // --------------------------------------------------

    test.expectEqual(
        mesh.getNumberOfNodes(),
        252,
        1e-12,
        "Number of nodes = 252"
    );

    test.expectEqual(
        mesh.getNumberOfCells(),
        220,
        1e-12,
        "Number of cells = 220"
    );


    // --------------------------------------------------
    // 4. Check mesh parameters
    // --------------------------------------------------

    test.expectEqual(
        mesh.Nx,
        20,
        1e-12,
        "Nx = 20"
    );

    test.expectEqual(
        mesh.Nlayer,
        9,
        1e-12,
        "Final inflation layers = 9"
    );

    test.expectEqual(
        mesh.Ny,
        2,
        1e-12,
        "Coarse layers = 2"
    );

    test.expectEqual(
        mesh.dx,
        0.2,
        1e-12,
        "dx = 0.2"
    );


    // --------------------------------------------------
    // 5. Check first inflation layer height
    // --------------------------------------------------

    double firstLayerHeight =
        mesh.nodes[mesh.Nx + 1].y
        -
        mesh.nodes[0].y;

    test.expectEqual(
        firstLayerHeight,
        0.12,
        1e-12,
        "First inflation layer height = 0.12"
    );


    // --------------------------------------------------
    // 6. Check last inflation layer height
    // --------------------------------------------------

    double lastLayerHeight =
        mesh.nodes[
            mesh.Nlayer * (mesh.Nx + 1)
        ].y
        -
        mesh.nodes[
            (mesh.Nlayer - 1) * (mesh.Nx + 1)
        ].y;

    double expectedLastLayerHeight =
        0.12 * std::pow(1.3, mesh.Nlayer - 1);

    test.expectEqual(
        lastLayerHeight,
        expectedLastLayerHeight,
        1e-12,
        "Last inflation layer height = 0.9788768652"
    );


    // --------------------------------------------------
    // 7. Check last node reaches xmax and ymax
    // --------------------------------------------------

    int lastNode = mesh.getNumberOfNodes() - 1;

    test.expectEqual(
        mesh.nodes[lastNode].x,
        4.0,
        1e-12,
        "Last node X = xmax"
    );

    test.expectEqual(
        mesh.nodes[lastNode].y,
        6.0,
        1e-12,
        "Last node Y = ymax"
    );


    // --------------------------------------------------
    // 8. Check cell connectivity
    // --------------------------------------------------

    bool validCells = true;

    for(const auto& cell : mesh.cells)
    {
        for(int nodeID : cell.nodeIDs)
        {
            if(nodeID < 0 ||
               nodeID >= mesh.getNumberOfNodes())
            {
                validCells = false;
            }
        }
    }

    test.expectTrue(
        validCells,
        "All cell node indices are valid"
    );


    // --------------------------------------------------
    // 9. Print summary
    // --------------------------------------------------

    test.summary();

    return test.success() ? 0 : 1;
}
