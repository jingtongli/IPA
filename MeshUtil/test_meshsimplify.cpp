// author: mathias, 2017, mathias.rothermel@googlemail.com
#include "MyMesh.h"
#include "MeshIO.h"
#include "MeshFilter.h"
#include "../MeshMetaData/MeshMetaData.h"
#include "../IOList/IOList.h"

#include <iostream>
#include <boost/filesystem.hpp>

void thinoutArea(MyMesh& mesh, MeshMetaData& mmd, double trimfac )
{
	// Now what do we do?
	double xwidth=mmd._maxx-mmd._minx;
	double ywidth=mmd._maxy-mmd._miny;

	// Local coords
	double width=std::max(xwidth,ywidth);
	double red=trimfac*width;

	double minx_loc=mmd._minx-mmd._offsetx+red;
	double maxx_loc=mmd._maxx-mmd._offsetx-red;
	double miny_loc=mmd._miny-mmd._offsety+red;
	double maxy_loc=mmd._maxy-mmd._offsety-red;

	MeshFilter::filterBB(	mesh,
				minx_loc, maxx_loc, miny_loc, maxy_loc,
				-1.0*std::numeric_limits<float>::infinity(),
				std::numeric_limits<float>::infinity());
}


static void show_usage(std::string name )
{
	std::cerr << "Usage: " <<  name << " <option(s)> SOURCES"
		  << "Options:\n"
		  << "\t-h,--help\t\t\t\tShow this help message\n"
		  << "\t-imesh, --inputmesh INPUT MESH LIST  \tSpecify the input list holding dsms\n"
		  << "\t-imeshmmd, --inputmeshmd INPUT METADATA LIST  \tSpecify the input list holding metadata\n"
		  << "\t-simfac, --simfac OUTPUT FOLDER\t\tSpecify the simplification factor\n"
		  << "\t-trimfac, --trimfac OUTPUT FOLDER\t\tSpecify the simplification factor\n"
		  << "\t-o,--output OUTPUT FOLDER\t\tSpecify the destination path\n"
		  << std::endl;
}

int main(int argc, char* argv[])
{
	if (argc < 5)
	{
	       show_usage(argv[0]);
	       return 1;
	}

	double gsd=1.0f;
	double trimfac=0.0;
	double simfac=1.0;

	std::string output;
	std::string inputmesh;
	std::string inputmeshmd;

	float factor=1.0f;

	for(int i=1; i<argc; ++i)
	{
	   	std::string arg = argv[i]; 
		if ((arg == "-h") || (arg == "--help"))
		{
			show_usage(argv[0]);
			return 0;
		}
		else if ((arg == "-o") || (arg == "--output"))
		{
			if (i + 1 < argc)
			{ // Make sure we aren't at the end of argv!
				output = argv[++i]; // Increment 'i' so we don't get the argument as the next argv[i].
			}
			else
			{ // Uh-oh, there was no argument to the destination option.
				std::cerr << "--destination option requires one argument." << std::endl;
			        return 1;
			}
		}
		else if ((arg == "-imesh") || (arg == "--inputmesh"))
		{
			if (i + 1 < argc)
			{ // Make sure we aren't at the end of argv!
				inputmesh = argv[++i]; // Increment 'i' so we don't get the argument as the next argv[i].
			}
			else
			{ // Uh-oh, there was no argument to the destination option.
				std::cerr << "--destination option requires one argument." << std::endl;
			        return 1;
			}
		}
		else if ((arg == "-imeshmmd") || (arg == "--inputmeshmmd"))
		{
			if (i + 1 < argc)
			{ // Make sure we aren't at the end of argv!
				inputmeshmd = argv[++i]; // Increment 'i' so we don't get the argument as the next argv[i].
			}
			else
			{ // Uh-oh, there was no argument to the destination option.
				std::cerr << "--destination option requires one argument." << std::endl;
			        return 1;
			}
		}
		else if ((arg == "-simfac") || (arg == "--simfac"))
		{
			if (i + 1 < argc)
			{ // Make sure we aren't at the end of argv!
				simfac=atof(argv[++i]); // Increment 'i' so we don't get the argument as the next argv[i].
			}
			else
			{ // Uh-oh, there was no argument to the destination option.
				std::cerr << "--simfac option requires one argument." << std::endl;
			        return 1;
			}
		}
		else if ((arg == "-trimfac") || (arg == "--trimfac"))
		{
			if (i + 1 < argc)
			{ // Make sure we aren't at the end of argv!
				trimfac= atof(argv[++i]); // Increment 'i' so we don't get the argument as the next argv[i].
			}
			else
			{ // Uh-oh, there was no argument to the destination option.
				std::cerr << "--destination option requires one argument." << std::endl;
			        return 1;
			}
		}
		else
		{
		    std::cout<<"Cannt parse "<< arg; return 1;
	        }
        }

	std::cout<<"\nInputMesh="<<inputmesh;
	std::cout<<"\nInputMeta="<<inputmeshmd;
	std::cout<<"\nOutput="<<output;

	// Check if file and dir exsist
	if ( !boost::filesystem::exists( inputmesh ) )
	{
      		std::cout << "\nCan't find file" << inputmesh;
		return 1;
	}
	if ( !boost::filesystem::exists( inputmeshmd) )
	{
      		std::cout << "\nCan't find file" << inputmeshmd;
		return 1;
	}
	if ( !boost::filesystem::exists(output) )
	{
      		std::cout << "\nCan't find folder" << output;
		return 1;
	}

	float TOLERANCEFAC=1.0f;
	float errorfactor;

	// Load lists of input
	IOList mmdlist(inputmeshmd);
	IOList meshlist(inputmesh);

	if(mmdlist.size()!=meshlist.size())
	{
		std::cout<<"\nMmd list and meshlist sizes different.";
		exit(1);
	}

    	for(int m=0; m<meshlist.size(); m++)
	{
		// Load the Mesh
		MyMesh mesh;
	    	MeshIO::readMesh(mesh, meshlist.getElement(m), false, true, false);
		mesh.request_vertex_status();
		mesh.request_face_status();
		mesh.request_edge_status();

		// Load the Meta data
		MeshMetaData mmd(mmdlist.getElement(m));
		errorfactor=mmd._gsd*mmd._gsd*simfac;

		// Simplify the mesh
		MeshFilter::simQuadricError(mesh, errorfactor,TOLERANCEFAC);
		
		// Also thin out the area
		thinoutArea(mesh, mmd, trimfac);

		// Make the savenames
		boost::filesystem::path fspath(meshlist.getElement(m));
		std::string savename(output);
		savename.append("/");
		savename.append(fspath.filename().string());

		std::string savenamemmd(output);
		savenamemmd.append("/");
		savenamemmd.append(fspath.stem().string());
		savenamemmd.append(".mmd");


		// Store the mesh ... mmd should remain the same
		MeshIO::writeMesh(mesh,savename,false,true,false);

		// Store the meta data
		mmd.writeMetaData(savenamemmd);

	}
}
