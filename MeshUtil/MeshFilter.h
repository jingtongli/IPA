// author: mathias, 2017, mathias.rothermel@geod.baug.ethz.ch
#pragma once

#include "MyMesh.h"

namespace MeshFilter
{
    
	// Smoothing
    	void smoothJacobiLapalce(MyMesh& mesh, const int iterations, const bool skipfeatures=false);
	
	// Decimation
	void simQuadricError(MyMesh& mesh, const float maximalerror, const float tolerancefac);
	void simEdgeLength(MyMesh& mesh, const float maximallength, const float tolerancefac);
	void simEdgePlusQuadric(MyMesh& mesh, const float maximallength, const float maximalerror, const float tolerancefac);

	// Filter according to valid verts specified in validverts
	void simValidity( 	std::vector< std::vector<float> > &verts,
				std::vector<std::vector<int> > &faces,
				bool* validverts);
	
	// Filter according to valid verts specified in validverts
	void filterBB(	MyMesh& mesh,
			const float minx, const float maxx,
			const float miny, const float maxy,
			const float minz, const float maxz);
}
