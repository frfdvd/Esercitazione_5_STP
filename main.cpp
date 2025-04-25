#include <iostream>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"
#include "UCDUtilities.hpp"

using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;
using namespace Gedim;

int main()
{
    // creo un oggetto mesh
    PolygonalMesh mesh;


    // importo la mesh e verifico che avvenga correttamente
    if(!ImportMesh(mesh))
    {
        cerr << "file not found" << endl;
        return 1;
    }

    // calcolo se esistono dei lati nulli
    if(!CalcolaLati(mesh))
    {
        cerr << "c'è un lato nullo" << endl;
        return 1;
    }

    // calcolo se esistono dei poligoni con area nulla
    if(!CalcolaArea(mesh))
    {
        cerr << "c'è una figura di area nulla" << endl;
        return 1;
    };

    VerificaMarker(mesh);

    
    UCDUtilities utilities;
    utilities.ExportPoints("./Cell0Ds.inp",
                           mesh.Cell0DsCoordinates);

    utilities.ExportSegments("./Cell1Ds.inp",
                             mesh.Cell0DsCoordinates,
                             mesh.Cell1DsExtrema);

    return 0;
}
