#include <OpenMesh/Core/IO/MeshIO.hh>

#include <OpenMesh/Core/IO/writer/PLYWriter.hh>
#include <OpenMesh/Core/IO/writer/OBJWriter.hh>
#include <OpenMesh/Core/IO/reader/PLYReader.hh>
#include <OpenMesh/Core/IO/reader/OBJReader.hh>
#include "MyMesh.h"

#include <string>
#include <boost/filesystem.hpp>
#include <iostream>

#include "MeshIO.h"

namespace MeshIO
{

void readMesh( MyMesh &ommesh, std::string savename, bool hasvertcolor, bool hasfacecolor, bool hasvertnormal, bool hasfacetexture )
{

    	// Check which extension
    	std::string extension=boost::filesystem::extension(savename); 
	if(extension==".obj") { OpenMesh::IO::_OBJWriter_(); }
	else if (extension==".ply") { OpenMesh::IO::_PLYWriter_(); }
	else { std::cout << "\n MeshUtil::readReadMeshVFN: could not figure extension. " << extension; exit(1); }

	OpenMesh::IO::Options ropt, wopt;
	if (hasfacecolor)
	{
	    	ommesh.request_face_colors();
        	ropt += OpenMesh::IO::Options::FaceColor;
	}
	if (hasvertcolor)
	{
	    	ommesh.request_vertex_colors();
        	ropt += OpenMesh::IO::Options::VertexColor;
	}
	if( hasvertnormal)
	{
	    	ommesh.request_vertex_normals();
        	ropt += OpenMesh::IO::Options::VertexNormal;
	}
	if( hasfacetexture )
	{
	    	ommesh.request_halfedge_texcoords2D();
		ommesh.request_face_texture_index();
        	ropt += OpenMesh::IO::Options::FaceTexCoord;
	}

	if(extension==".obj") {

	    	OpenMesh::IO::_OBJReader_(); 
		if ( ! OpenMesh::IO::read_mesh(ommesh, savename.c_str(), ropt ))
        	{
        		std::cerr << "Error loading mesh from file " << savename << std::endl;
		}
	}
	else if(extension==".ply")
	{
	    	OpenMesh::IO::_PLYReader_();

		ropt+=OpenMesh::IO::Options::Binary;
		if ( ! OpenMesh::IO::read_mesh(ommesh, savename.c_str(), ropt ))
        	{
        		std::cerr << "Error loading mesh from file " << savename << std::endl;
		}
	}
}

void writeMesh( const MyMesh &mesh, const std::string &savename, const bool hasvertcolor, const bool hasfacecolor, const bool hasvertnormal, const bool hasfacetexture )
{


    	// Check which extension
    	std::string extension=boost::filesystem::extension(savename); 
	if(extension==".obj") { /*OpenMesh::IO::_OBJWriter_();*/ }
	else if (extension==".ply") { /*OpenMesh::IO::_PLYWriter_();*/ }
	else {std::cout<<"\n MeshIO::writeMesh: could not figure extension."; exit(1); }
	
	OpenMesh::IO::Options wopt;
        if(hasfacecolor) wopt += OpenMesh::IO::Options::FaceColor;
        if(hasvertcolor) wopt += OpenMesh::IO::Options::VertexColor;
        if(hasvertnormal) wopt += OpenMesh::IO::Options::VertexNormal;
        if(hasfacetexture) wopt += OpenMesh::IO::Options::FaceTexCoord;

    	if (!OpenMesh::IO::write_mesh(mesh, savename.c_str(), wopt))
	{
		std::cerr << "MeshUtil: Write error\n";
		exit(1);
	}
}
}
