// g++ tests/testBumpInflation.cpp src/*.cpp -Iinclude -std=c++17 -o testBumpInflation && ./testBumpInflation
#include "Test.h"

#include "Point.h"
#include "Line.h"
#include "Surface.h"
#include "Bump.h"

#include <cmath>
#include <stdexcept>

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
    // 2. Generate Bump inflation mesh
    // --------------------------------------------------

    Bump mesh;

    mesh.generateInflation(
        rectangle,
        0.12,   // h0
        1.2,    // r
        8,      // Nlayer
        20      // Nx
    );


    // --------------------------------------------------
    // 3. Check mesh parameters
    // --------------------------------------------------

    test.expectEqual(
        mesh.Nx,
        20,
        1e-12,
        "Nx = 20"
    );

    test.expectEqual(
        mesh.Nlayer,
        8,
        1e-12,
        "Inflation layers = 8"
    );

    test.expectEqual(
        mesh.Ny,
        2,
        1e-12,
        "Coarse layers = 2"
    );


    // --------------------------------------------------
    // 4. Check number of nodes and cells
    // --------------------------------------------------

    test.expectEqual(
        mesh.getNumberOfNodes(),
        441,
        1e-12,
        "Number of nodes = 441"
    );

    test.expectEqual(
        mesh.getNumberOfCells(),
        400,
        1e-12,
        "Number of cells = 400"
    );


    // --------------------------------------------------
    // 5. Check dx
    // --------------------------------------------------

    test.expectEqual(
        mesh.dx,
        0.2,
        1e-12,
        "dx = 0.2"
    );


    // --------------------------------------------------
    // 6. Check dy
    // --------------------------------------------------

    double expectedDy = 0.510054912;

    test.expectEqual(
        mesh.dy,
        expectedDy,
        1e-12,
        "Coarse layer height dy"
    );


    // --------------------------------------------------
    // 7. Check first lower inflation layer
    // --------------------------------------------------

    double firstLowerLayer =
        mesh.nodes[mesh.Nx + 1].y
        -
        mesh.nodes[0].y;

    test.expectEqual(
        firstLowerLayer,
        0.12,
        1e-12,
        "First lower inflation layer = h0"
    );


    // --------------------------------------------------
    // 8. Check last lower inflation layer
    // --------------------------------------------------

    double lastLowerLayer =
        mesh.nodes[
            mesh.Nlayer * (mesh.Nx + 1)
        ].y
        -
        mesh.nodes[
            (mesh.Nlayer - 1) * (mesh.Nx + 1)
        ].y;

    double expectedLastLowerLayer =
        0.12 * std::pow(1.2, 7);

    test.expectEqual(
        lastLowerLayer,
        expectedLastLowerLayer,
        1e-12,
        "Last lower inflation layer"
    );


    // --------------------------------------------------
    // 9. Check upper inflation region
    // --------------------------------------------------

    int rowStart = mesh.Nlayer + mesh.Ny;

    double upperFirstLayer =
        mesh.nodes[
            (rowStart-mesh.Ny) * (mesh.Nx + 1)
        ].y
        -
        mesh.nodes[
            (rowStart-mesh.Ny-1) * (mesh.Nx + 1)
        ].y;

    test.expectEqual(
        upperFirstLayer,
        0.429981696,
        1e-12,
        "First upper inflation layer"
    );


    // --------------------------------------------------
    // 10. Check last upper inflation layer
    // --------------------------------------------------

    double upperLastLayer =
        mesh.nodes[
            (2 * (mesh.Nlayer + mesh.Ny)) * (mesh.Nx + 1)
        ].y
        -
        mesh.nodes[
            (2 * (mesh.Nlayer + mesh.Ny) - 1) * (mesh.Nx + 1)
        ].y;

    test.expectEqual(
        upperLastLayer,
        0.12,
        1e-12,
        "Last upper inflation layer"
    );


    // --------------------------------------------------
    // 11. Check domain limits
    // --------------------------------------------------

    int lastNode = mesh.getNumberOfNodes() - 1;

    test.expectEqual(
        mesh.nodes[0].y,
        0.0,
        1e-12,
        "First node Y = ymin"
    );

    test.expectEqual(
        mesh.nodes[lastNode].x,
        4.0,
        1e-12,
        "Last node X = xmax"
    );


    // --------------------------------------------------
    // 12. Check cell connectivity
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
    // 13. Summary
    // --------------------------------------------------

    test.summary();

    return test.success() ? 0 : 1;
}