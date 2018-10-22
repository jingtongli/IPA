// author: mathias, 2017, mathias.rothermel@geod.baug.ethz.ch
#pragma once

#include <string>
#include "MyMesh.h"

namespace MeshIO
{

    void writeMesh( const MyMesh &mesh, const std::string &savename, const bool hasvertcolor ,const bool hasfacecolor,const bool hasvertnormal=false, const bool hasfacetexture=false );

    void readMesh(  MyMesh &ommesh, std::string file, bool hasvertcol, bool hasfacecolor, bool hasvertnormal=false, bool hasfacetexture=false );

}
