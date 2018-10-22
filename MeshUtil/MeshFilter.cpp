// author: mathias, 2017, mathias.rothermel@geod.baug.ethz.ch

#include "MeshFilter.h"

#include <iostream>

// Headers from the library ... hits from the bong
#include <OpenMesh/Tools/Smoother/JacobiLaplaceSmootherT.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Tools/Subdivider/Uniform/LongestEdgeT.hh>
#include <OpenMesh/Tools/Decimater/DecimaterT.hh>
#include <OpenMesh/Tools/Decimater/ModQuadricT.hh>
#include <OpenMesh/Tools/Decimater/ModEdgeLengthT.hh>

namespace MeshFilter
{

    	// Decimater type
    	typedef OpenMesh::Decimater::DecimaterT< MyMesh > Decimater;

    	// Decimation Module Handle type
    	typedef OpenMesh::Decimater::ModQuadricT< MyMesh >::Handle HModQuadric;

    	void smoothJacobiLapalce(MyMesh& mesh, const int iterations, const bool skipfeatures)
	{
	    	mesh.request_face_status();
		mesh.request_edge_status();
		mesh.request_halfedge_status();

	    	OpenMesh::Smoother::JacobiLaplaceSmootherT<MyMesh> smoother(mesh);
	    	OpenMesh::Smoother::SmootherT<MyMesh>::Continuity cont=OpenMesh::Smoother::JacobiLaplaceSmootherT<MyMesh>::C1;
		OpenMesh::Smoother::SmootherT<MyMesh>::Component component=OpenMesh::Smoother::JacobiLaplaceSmootherT<MyMesh>::Tangential_and_Normal;

		smoother.initialize(component, cont);
		smoother.skip_features(skipfeatures);
		smoother.smooth(iterations);
	}

   	void simQuadricError(MyMesh& mesh, const float maximalerror, const float tolerancefac)
	{
	    	Decimater filter(mesh);

		HModQuadric hModQuadric;
		filter.add(hModQuadric);
		filter.module(hModQuadric).set_max_err(maximalerror);
		filter.module(hModQuadric).set_error_tolerance_factor(tolerancefac);

		filter.initialize();
		filter.decimate();
		mesh.garbage_collection();
	}


   	void simEdgeLength(MyMesh& mesh, const float maximallength, const float tolerancefac)
	{
		Decimater filter(mesh);

		HModQuadric hModEdgeLegth;
		filter.add(hModEdgeLegth);
		filter.module(hModEdgeLegth).set_max_err(maximallength);
		filter.module(hModEdgeLegth).set_error_tolerance_factor(tolerancefac);

		filter.initialize();
		filter.decimate();
		mesh.garbage_collection();
	}

   	void simEdgePlusQuadric(MyMesh& mesh, const float maximallength, const float maximalerror, const float tolerancefac)
	{
		Decimater filter(mesh);

		HModQuadric hModEdgeLegth;
		filter.add(hModEdgeLegth);
		filter.module(hModEdgeLegth).set_binary(0);
		filter.module(hModEdgeLegth).set_max_err(maximallength);
		filter.module(hModEdgeLegth).set_error_tolerance_factor(tolerancefac);

		HModQuadric hModQuadric;
		filter.add(hModQuadric);
		filter.module(hModQuadric).set_binary(1);
		filter.module(hModQuadric).set_max_err(maximalerror);
		filter.module(hModQuadric).set_error_tolerance_factor(tolerancefac);

		filter.initialize();
		filter.decimate();
		mesh.garbage_collection();
	}

	void simValidity(	std::vector<std::vector<float> > &verts,
				std::vector<std::vector<int> > &faces,
				bool* validverts)
	{
		int numverts=verts.size();
		int numfaces=faces.size();

    		// Derive num valid faces
	   	int* newids=new int[numverts];
		for(int i=0; i<numverts; i++){ newids[i]=-1; }

		int numnewfaces=0;
    		for(int f=0; f<numfaces; f++)
    		{

        		if( 	validverts[faces[f][0]]
				&& validverts[faces[f][1]]
				&& validverts[faces[f][2]])
			{
			    numnewfaces++;
			    newids[faces[f][0]]=1;
			    newids[faces[f][1]]=1;
			    newids[faces[f][2]]=1;
			}
		}
    		// Derive num valids, update vert validity, establish mapping
		int numnewverts=0;
    		for(int v=0; v<numverts; v++)
		{
        		if(newids[v]==1)
			{
            			newids[v]=numnewverts;
				numnewverts++;
			}
        	}

    		// Allocate newstuff
    		float** newverts=new float*[numnewverts];
    		int** newfaces =new int*[numnewfaces];
		for(int v=0; v<numnewverts; v++){newverts[v]=new float[3]; }
		for(int f=0; f<numnewfaces; f++){newfaces[f]=new int[3]; }

		int oldid=0;
		int newid=0;
		for(oldid=0; oldid<numverts; oldid++)
		{
		    	if(newids[oldid]!=-1) // vert not valid
		    	{
				newverts[newid][0]=verts[oldid][0];
				newverts[newid][1]=verts[oldid][1];
				newverts[newid][2]=verts[oldid][2];
				newid++;
		    	}
		}

		newid=0;
		for(oldid=0; oldid<numfaces; oldid++)
		{
			// If face is valid
        		if( 	validverts[faces[oldid][0]]
				&& validverts[faces[oldid][1]]
				&& validverts[faces[oldid][2]])
			{
			    	// Copy to new position
			    	newfaces[newid][0]=newids[faces[oldid][0]];
			    	newfaces[newid][1]=newids[faces[oldid][1]];
			    	newfaces[newid][2]=newids[faces[oldid][2]];
			    	newid++;
			}
		}

		// Now this is done... erase old struct and copy content 
		verts.resize(numnewverts);
		for(int v=0; v<numnewverts; v++) { verts[v].resize(3); }
		faces.resize(numnewfaces);
		for(int f=0; f<numnewfaces; f++) { faces[f].resize(3); }

		for(int v=0; v<numnewverts; v++)
		{
			verts[v][0]=newverts[v][0];
			verts[v][1]=newverts[v][1];
			verts[v][2]=newverts[v][2];
		}
		for(int f=0; f<numnewfaces; f++)
		{
			faces[f][0]=newfaces[f][0];
			faces[f][1]=newfaces[f][1];
			faces[f][2]=newfaces[f][2];
		}
		// Some garbage collection
		delete[] newids;
		for(int v=0; v<numnewverts; v++) { delete[] newverts[v]; }
		delete[] newverts;
		for(int f=0; f<numnewfaces; f++) { delete[] newfaces[f]; }
		delete[] newfaces;

	}

	void filterBB(	MyMesh& mesh,
			const float minx, const float maxx,
			const float miny, const float maxy,
			const float minz, const float maxz)
	{
		MyMesh::Point pt;
		for(MyMesh::FaceIter f_it=mesh.faces_begin();f_it!=mesh.faces_end(); ++f_it)
		{
			for(MyMesh::FaceVertexIter fv_it=mesh.cfv_iter(*f_it); fv_it.is_valid(); ++fv_it)
			{
			    	pt=mesh.point(*fv_it);
				if(	pt[0]>maxx || pt[0]<minx || 
					pt[1]>maxy || pt[1]<miny ||
					pt[2]>maxz || pt[2]<minz)
				{
				    mesh.delete_face(*f_it,true);
				    break;
				}
			}
		}
		mesh.garbage_collection();
	}

}
