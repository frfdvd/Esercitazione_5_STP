#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <cmath>

namespace PolygonalLibrary{
    bool ImportMesh(PolygonalMesh& mesh)
{

    if(!ImportCell0Ds(mesh))
        return false;

    if(!ImportCell1Ds(mesh))
        return false;

    if(!ImportCell2Ds(mesh))
        return false;

    return true;

}

/**********************************/

bool ImportCell0Ds(PolygonalMesh& mesh)
{
    ifstream file("./Cell0Ds.csv");

    if(file.fail())
        return false;

    list<string> listLines;

    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    // remove header
    listLines.pop_front();

    mesh.NumCell0Ds = listLines.size();

    if (mesh.NumCell0Ds == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }


    // alloco lo spazio adatto per le mie matrici 
    mesh.Cell0DsId.reserve(mesh.NumCell0Ds);
    mesh.Cell0DsCoordinates = Eigen::MatrixXd::Zero(3, mesh.NumCell0Ds);

    
    // leggo riga per riga leggendo ogni valore separato da ;
    for (const string& line : listLines)
    {
        istringstream converter(line);
        string field;
        unsigned int id;
        unsigned int marker;
        Vector2d coord;

        // separo ogni valore e lo inserisco delle strutture dati preposte 
        getline(converter, field, ';');
        id = stoi(field);

        getline(converter, field, ';');
        marker = stoi(field);

        getline(converter, field, ';');
        mesh.Cell0DsCoordinates(0, id) = stod(field);

        getline(converter, field, ';');
        mesh.Cell0DsCoordinates(1, id) = stod(field);

        mesh.Cell0DsId.push_back(id);

        // salvo i marker con un iteratore usando il metodo find
        if(marker!=0){
            auto it = mesh.MarkerCell0Ds.find(marker);
            if(it != mesh.MarkerCell0Ds.end()){
                (*it).second.push_back(id);

            }else{
                mesh.MarkerCell0Ds.insert({marker, {id}});
            }
        }

    }

    return true;
}

/**********************************/

bool ImportCell1Ds(PolygonalMesh& mesh)
{
    ifstream file("./Cell1Ds.csv");

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    
    // salvo tutte le righe del csv in una lista di stringhe
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    // remove header
    listLines.pop_front();

    // salvo e conto il numero di righe
    mesh.NumCell1Ds = listLines.size();

    // verifico che ci siano effettivamente dei lati 
    if (mesh.NumCell1Ds == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    // alloco lo spazio necessario
    mesh.Cell1DsId.reserve(mesh.NumCell1Ds);
    mesh.Cell1DsExtrema = Eigen::MatrixXi(2, mesh.NumCell1Ds);

    // itero su ogni riga del csv
    for (const string& line : listLines)
    {
        istringstream converter(line);
        string field;
        unsigned int id;
        unsigned int marker;
        
        // separo in base al ; e salvo i valori 
        getline(converter, field, ';');
        id = stoi(field);

        getline(converter, field, ';');
        marker = stoi(field);

        getline(converter, field, ';');
        mesh.Cell1DsExtrema(0, id) = stoi(field);

        getline(converter, field, ';');
        mesh.Cell1DsExtrema(1, id) = stoi(field);
        
        
        mesh.Cell1DsId.push_back(id);

        // Memorizza i marker
        if(marker!=0){
            auto it = mesh.MarkerCell1Ds.find(marker);
            if(it != mesh.MarkerCell1Ds.end()){
                (*it).second.push_back(id);

            }else{
                mesh.MarkerCell1Ds.insert({marker, {id}});
            }
        }
    }


    return true;
}

/**********************************/
bool ImportCell2Ds(PolygonalMesh& mesh){
    ifstream file;
    file.open("./Cell2Ds.csv");

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    // remove header
    listLines.pop_front();

    mesh.NumCell2Ds = listLines.size();

    if (mesh.NumCell2Ds == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }

    // alloco lo spazio necessario per salvare i dati
    mesh.Cell2DsId.reserve(mesh.NumCell2Ds);
    mesh.Cell2DsVertices.reserve(mesh.NumCell2Ds);
    mesh.Cell2DsEdges.reserve(mesh.NumCell2Ds);

    string field;
    for(const string& line : listLines){
        istringstream converter(line);

        unsigned int id;
        unsigned int NumVertices;
        unsigned int NumEdges;
        unsigned int marker;
        vector<unsigned int> vertices;
        vector<unsigned int> edges;
    
        // separo i valori in base al ;
        getline(converter, field, ';');
        id = stoi(field);

        getline(converter, field, ';');
        marker = stoi(field);

        getline(converter, field, ';');
        NumVertices = stoi(field);
        
        // ciclo in base al numero di vertici
        for(unsigned int i = 0; i < NumVertices; i++){
            getline(converter, field, ';');
            vertices.push_back(stoi(field));
        }

        getline(converter, field, ';');
        NumEdges = stoi(field);
        
        // ciclo in base al numero di lati
        for(unsigned int i = 0; i < NumEdges; i++){
            getline(converter, field, ';');
            edges.push_back(stoi(field));
        }

        mesh.Cell2DsId.push_back(id);
        mesh.Cell2DsVertices.push_back(vertices);
        mesh.Cell2DsEdges.push_back(edges);

        // salvo i marker
        if(marker!=0){
            auto it = mesh.MarkerCell2Ds.find(marker);
            if(it != mesh.MarkerCell2Ds.end()){
                (*it).second.push_back(id);

            }else{
                mesh.MarkerCell2Ds.insert({marker, {id}});
            }
        }

    }
    return true;
}

/**********************************/

bool CalcolaArea(PolygonalMesh& mesh){
    for (const auto& riga : mesh.Cell2DsEdges) {
        double Area = 0.0;
        for(unsigned int i = 0; i<riga.size();i++){
            int idLato = riga[i];
            
            // trovo i punti del lato
            int idPunto1 = mesh.Cell1DsExtrema(0, idLato);
            int idPunto2 = mesh.Cell1DsExtrema(1, idLato);

            // trovo le coordinate dei punti
            double x1 = mesh.Cell0DsCoordinates(0,idPunto1);
            double y1 = mesh.Cell0DsCoordinates(1,idPunto1);

            double x2 = mesh.Cell0DsCoordinates(0,idPunto2);
            double y2 = mesh.Cell0DsCoordinates(1,idPunto2);

            // calcolo l'area usando la formula del prodotto vettoriale 
            // usando i punti come vettori che partono dall'origine
            Area = Area + x1*y2 - x2*y1;
        }
        Area = 0.5 * abs(Area); 

        // verifico che l'area non sia nulla, non faccio il valore assoluto all'area perchè l'ho già fatto prima
        if(Area < numeric_limits<double>::epsilon()*sqrt(3)/4.0){
            return false;
        }
    }     
    return true;
}

/***********************************/

bool CalcolaLati(PolygonalMesh& mesh){
    for(int i = 0; i<mesh.Cell1DsExtrema.cols(); i++){
        
        // trovo gli id dei punti
        int idPunto1 = mesh.Cell1DsExtrema(0,i);
        int idPunto2 = mesh.Cell1DsExtrema(1,i);

        // calcolo la lunghezza dei cateti (proiezioni del lato lungo gli assi)
        double lato1 = (mesh.Cell0DsCoordinates(0,idPunto1)-mesh.Cell0DsCoordinates(0,idPunto2));
        double lato2 = (mesh.Cell0DsCoordinates(1,idPunto1)-mesh.Cell0DsCoordinates(1,idPunto2));
        double distanza_quadra =  sqrt(lato1 * lato1 + lato2 * lato2);

        // verifico che la lunghezza non sia nulla
        if(abs(distanza_quadra) < numeric_limits<double>::epsilon()){
            return false;
        }

    }
    return true;
}

/***********************************/

void VerificaMarker(PolygonalMesh& mesh){

    cout << "Verifico che i marker dei punti siano corretti" << endl;
    cout << endl;
    for(const auto& pair : mesh.MarkerCell0Ds){
        cout << "Marker: "<< pair.first << endl;
        
        cout << "Id dei Punti associati: ";
        for(const unsigned int& IdPunti : pair.second){
            cout << IdPunti << " ";
        }
        cout << endl;
    }

    cout << endl;
    cout << "Verifico che i marker dei lati siano corretti" << endl;
    cout << endl;
    for(const auto& pair : mesh.MarkerCell1Ds){
        cout << "Marker: "<< pair.first << endl;
        
        cout << "Id dei Lati associati: ";
        for(const unsigned int& IdLati : pair.second){
            cout << IdLati << " ";
        }
        cout << endl;
    }
}
}