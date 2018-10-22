#pragma once

#include <vector>
#include "MyMesh.h"

namespace MeshConv 
{
	void vectorsToOpenMesh(const std::vector<std::vector<float> > &verts, const std::vector<std::vector<int> > &faces, const std::vector<short> &lables, MyMesh &ommesh );	

	void openMeshToVectors( const MyMesh &ommesh, std::vector<std::vector<float> > &verts, std::vector<std::vector<int> > &faces, std::vector<short> &lables);	

	void vectorsToOpenMesh(float** &verts, int** faces, short** &lables, MyMesh &ommesh );

	void openMeshToVectors( const MyMesh &ommesh, float** &verts, int** &faces,short** &lables);

	void vertColorToFaceColor(MyMesh &mesh);

	void faceColorToFaceLabel(MyMesh &mesh);

	void faceLabelToFaceColor(MyMesh &mesh);

	void faceLabelToFaceColorRandom(MyMesh &mesh);

	void faceLabelToFaceColorICCV(MyMesh &mesh);

	void faceLabelToFaceColorAudrey(MyMesh &mesh);

	void faceColorICCVToFacelabel(MyMesh &mesh);
	
	void vertLabelToFaceLabel(MyMesh &mesh, int numClasses); // Added by Audrey in order to convert the colored mesh per vertex to a colored mesh per face 

	void faceColorToFaceLabelAudrey(MyMesh &mesh);
}
