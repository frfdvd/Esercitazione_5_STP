#pragma once

#include <iostream>
#include "PolygonalMesh.hpp"

using namespace std;

namespace PolygonalLibrary{
    
    // chiama a cascata le altre funzioni che importano Cell0Ds, Cell1Ds, Cell2Ds e
    bool ImportMesh(PolygonalMesh& mesh);

    // importa i punti e salva tutto nelle strutture dati migliori
    bool ImportCell0Ds(PolygonalMesh& mesh);

    // importa i segmenti e salva tutto nelle strutture dati migliori
    bool ImportCell1Ds(PolygonalMesh& mesh);

    // importa i poligoni e salva tutto nelle strutture dati migliori
    bool ImportCell2Ds(PolygonalMesh& mesh);

    // verifica che non ci siano lati nulli
    bool CalcolaLati(PolygonalMesh& mesh);

    // verifica che non ci siano aree nulle 
    bool CalcolaArea(PolygonalMesh& mesh);

    // verifica che i marker siano stati salvati correttamente
    void VerificaMarker(PolygonalMesh& mesh);

}