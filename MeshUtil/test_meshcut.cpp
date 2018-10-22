// author: mathias, 2017, mathias.rothermel@geod.baug.ethz.ch
#include "../IOList/IOList.h"
#include "../MeshUtil/MyMesh.h"
#include "../MeshUtil/MeshIO.h"
#include "../MeshUtil/MeshFilter.h"
#include "../MeshMetaData/MeshMetaData.h"

#include <iostream>
#include <string>

int main()
{
    	// Load all the stuff
	std::string mmdname("/media/data/mathias/Scene1_Z/Mesh/mmdlist.txt");
	std::string meshname("/media/data/mathias/Scene1_Z/Mesh/meshlist.txt");
	std::string outputpath("/media/data/mathias/Scene1_Z/Mesh/Cut");

	IOList mmdlist(mmdname);
	IOList meshlist(meshname);

	// Sanity...
	if(meshlist.size()!=mmdlist.size())
	{
	    std::cout<<"\nDifferent sizes of list";
	    exit(1);
	}

	double minx, maxx, miny, maxy, minz, maxz;
	double rangex, rangey, rangez;
	double percext=0.0;

	for(int m=0; m<meshlist.size(); m++)
	{
	    	// Load mmd
	    	MeshMetaData mmd(mmdlist.getElement(m));

		rangex=mmd._maxx-mmd._minx;
		rangey=mmd._maxy-mmd._miny;
		rangez=mmd._maxz-mmd._minz;

		minx=mmd._minx-mmd._offsetx+rangex*percext;
		miny=mmd._miny-mmd._offsety+rangey*percext;
		minz=-std::numeric_limits<double>::infinity();
		maxx=mmd._maxx-mmd._offsetx-rangex*percext;
		maxy=mmd._maxy-mmd._offsety-rangey*percext;
		maxz=std::numeric_limits<double>::infinity();

 		// Load the mesh
		MyMesh mesh;
		MeshIO::readMesh(mesh,meshlist.getElement(m),false,false);
		MeshFilter::filterBB(mesh,minx,maxx,miny,maxy,minz,maxz);

		// Assemble Output path
		std::string filename=meshlist.getName(m);
		std::string savename=outputpath;
		savename.append("/");
		savename.append(filename);

		// Save the stuff
		MeshIO::writeMesh(mesh,savename,false,false);

	}
}
