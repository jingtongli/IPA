// author: mathias rothermel, 2016, mathias.rothermel@geod.baug.ethz.ch

#include "MeshConv.h"
#include <iostream>

namespace MeshConv
{

void openMeshToVectors( const MyMesh &ommesh, std::vector<std::vector<float> > &verts, std::vector<std::vector<int> > &faces, std::vector<short> &labels)
{

	int numverts=ommesh.n_vertices();
	int numfaces=ommesh.n_faces();
	int v,i;

	// Resize vectors
	verts.resize(numverts); for(i=0; i<numverts; i++){ verts[i].resize(3); }
	faces.resize(numfaces); for(i=0; i<numfaces; i++){ faces[i].resize(3); }
	labels.resize(numfaces);

	// Copy verts and labels
	int vertcount=0;
	for (MyMesh::VertexIter v_it=ommesh.vertices_begin(); v_it!=ommesh.vertices_end(); ++v_it){
		verts[vertcount][0]=ommesh.point(*v_it)[0];
		verts[vertcount][1]=ommesh.point(*v_it)[1];
		verts[vertcount][2]=ommesh.point(*v_it)[2];
		vertcount++;
	}

	// Copy faces
	int facecount=0;
	MyMesh::CFVIter fv_it;
	for (MyMesh::FaceIter f_it=ommesh.faces_begin(); f_it!=ommesh.faces_end(); ++f_it)
	{
   		v=0;
    		for(fv_it=ommesh.cfv_iter(*f_it); fv_it.is_valid(); ++fv_it)
		{
    			faces[facecount][v]=fv_it->idx(); 
			v++;
	   	}
		labels[facecount]=ommesh.data(*f_it).labelid();
    		facecount++;
	}
}

void vectorsToOpenMesh(const std::vector<std::vector<float> > &verts, const std::vector<std::vector<int> > &faces, const std::vector<short> &labels, MyMesh &ommesh )
{

	int numfaces=faces.size();
	int numverts=verts.size();

	// Copy Verts
	MyMesh::VertexHandle* vhandles=new MyMesh::VertexHandle[numverts];
	int v=0;
	for(std::vector<std::vector<float> >::const_iterator it=verts.begin(); it !=verts.end(); ++it){
	    vhandles[v] = ommesh.add_vertex(MyMesh::Point((*it)[0],(*it)[1],(*it)[2])); v++;
	}
	// Copy Faces
	std::vector<MyMesh::VertexHandle> face_vhandles(3); v=0;
	for( std::vector<std::vector<int> >::const_iterator it=faces.begin(); it!=faces.end(); ++it){

		face_vhandles[0]=vhandles[(*it)[0]];
		face_vhandles[1]=vhandles[(*it)[1]];
		face_vhandles[2]=vhandles[(*it)[2]];
		ommesh.add_face(face_vhandles); v++; 
	}
	delete [] vhandles;

	// Copy Labels
	int f=0;
	for (MyMesh::FaceIter f_it=ommesh.faces_begin(); f_it!=ommesh.faces_end(); ++f_it)
	{
	    ommesh.data(*f_it).setlabelid(labels[f]); f++;
	}
}

void vertColorToFaceColor(MyMesh &mesh)
{
	int label;
    	MyMesh::Color c0,c1,c2,fc;
	MyMesh::CFVIter fv_it;
	mesh.request_face_colors();

	for (MyMesh::FaceIter f_it=mesh.faces_begin(); f_it!=mesh.faces_end(); ++f_it)
	{
	    	fv_it=mesh.cfv_iter(*f_it);
		c0=mesh.color(*fv_it); fv_it++;
	    	c1=mesh.color(*fv_it);fv_it++;
	    	c2=mesh.color(*fv_it);fv_it++;

		if(c0==c1 || c0==c2 ) fc=c0;
		else if(c1==c0 || c1==c2 ) fc=c1;
		else fc=c0;

		mesh.set_color(*f_it,fc);
	}
	
}

void faceColorToFaceLabel(MyMesh &mesh)
{
    	unsigned char first, second;
	unsigned short s;

    	for (MyMesh::FaceIter f_it=mesh.faces_begin(); f_it!=mesh.faces_end(); ++f_it)
	{
		const MyMesh::Color& c=mesh.color(*f_it);
		first=c[0];
		second=c[1];

//		std::cout << "\n first="<<(int)first<<" second="<<(int)second;
		s=(((unsigned short)first) << 8) | second;
		mesh.data(*f_it).setlabelid(s);

	}
}

void faceColorToFaceLabelAudrey(MyMesh &mesh)
{
    	unsigned char first, second;
	unsigned short s;

	for(MyMesh::FaceIter f_it=mesh.faces_begin(); f_it!=mesh.faces_end(); ++f_it)
	{
	    	MyMesh::Color c=mesh.color(*f_it);
		if(c==MyMesh::Color(75,255,56))
		{
		    mesh.data(*f_it).setlabelid(4);
		}
		if(c==MyMesh::Color(139,139,139))
		{
		    mesh.data(*f_it).setlabelid(0);
		}
		if(c== MyMesh::Color(11,88,3))
		{
		    mesh.data(*f_it).setlabelid(1);
		}
		if(c== MyMesh::Color(230,154,0))
		{
		    mesh.data(*f_it).setlabelid(2);
		}
		if(c== MyMesh::Color(0,100,230))
		{
		    mesh.data(*f_it).setlabelid(3);
		}
	}
}

void faceLabelToFaceColor(MyMesh &mesh)
{

    	unsigned char first, second;
	unsigned short s;

    	for (MyMesh::FaceIter f_it=mesh.faces_begin(); f_it!=mesh.faces_end(); ++f_it)
	{
		s=mesh.data(*f_it).labelid();
		first= s >> 8;
		second = s & 0x00ff;
		mesh.set_color(*f_it, MyMesh::Color(first,second,first+second));
	}
}

void faceLabelToFaceColorAudrey(MyMesh &mesh)
{
	unsigned short s;
	for(MyMesh::FaceIter f_it=mesh.faces_begin(); f_it!=mesh.faces_end(); ++f_it)
	{
		s=mesh.data(*f_it).labelid();
		if(s==4) //Facade
		{
			mesh.set_color(*f_it, MyMesh::Color(75,255,56));
		}
		else if(s==0) //Ground
		{
			mesh.set_color(*f_it, MyMesh::Color(139,139,139));
		}
		else if(s==1) //Vegetation
		{
			mesh.set_color(*f_it, MyMesh::Color(11,88,3));
		}
		else if(s==2) //Roof
		{
			mesh.set_color(*f_it, MyMesh::Color(230,154,0));
		}
		else if(s==3) //Vegetation
		{
			mesh.set_color(*f_it, MyMesh::Color(0, 100,230));
		}
	}
}

void faceLabelToFaceColorICCV(MyMesh &mesh)
{
	unsigned short s;
	for(MyMesh::FaceIter f_it=mesh.faces_begin(); f_it!=mesh.faces_end(); ++f_it)
	{
		s=mesh.data(*f_it).labelid();
		if(s==0) //Facade
		{
			mesh.set_color(*f_it, MyMesh::Color(150,0,150));
		}
		else if(s==1) //Ground
		{
			mesh.set_color(*f_it, MyMesh::Color(60,60,60));
		}
		else if(s==2) //Vegetation
		{
			mesh.set_color(*f_it, MyMesh::Color(0,200,0));
		}
		else if(s==3) //Roof
		{
			mesh.set_color(*f_it, MyMesh::Color(200,200,0));
		}
		else if(s==4) //Vegetation
		{
			mesh.set_color(*f_it, MyMesh::Color(0, 0,200));
		}
	}
}

void faceColorICCVToFacelabelAudrey(MyMesh &mesh)
{
	unsigned short s;
	for(MyMesh::FaceIter f_it=mesh.faces_begin(); f_it!=mesh.faces_end(); ++f_it)
	{
		if(mesh.color(*f_it)[0]==150) //Facade
		{
			mesh.data(*f_it).setlabelid(0);
		}
		else if(mesh.color(*f_it)[0]==60) //Ground
		{
			mesh.data(*f_it).setlabelid(1);
		}
		else if(mesh.color(*f_it)[0]==0 && mesh.color(*f_it)[2]==0) // Vegetation
		{
			mesh.data(*f_it).setlabelid(2);
		}
		else if(mesh.color(*f_it)[0]==200) // Roof
		{
			mesh.data(*f_it).setlabelid(3);
		}
		else if(mesh.color(*f_it)[0]==0 && mesh.color(*f_it)[2]==200) // Vegetation
		{
			mesh.data(*f_it).setlabelid(4);
		}
	}
}
void faceColorICCVToFacelabel(MyMesh &mesh)
{
	unsigned short s;
	for(MyMesh::FaceIter f_it=mesh.faces_begin(); f_it!=mesh.faces_end(); ++f_it)
	{
		if(mesh.color(*f_it)[0]==150) //Facade
		{
			mesh.data(*f_it).setlabelid(0);
		}
		else if(mesh.color(*f_it)[0]==60) //Ground
		{
			mesh.data(*f_it).setlabelid(1);
		}
		else if(mesh.color(*f_it)[0]==0 && mesh.color(*f_it)[2]==0) // Vegetation
		{
			mesh.data(*f_it).setlabelid(2);
		}
		else if(mesh.color(*f_it)[0]==200) // Roof
		{
			mesh.data(*f_it).setlabelid(3);
		}
		else if(mesh.color(*f_it)[0]==0 && mesh.color(*f_it)[2]==200) // Vegetation
		{
			mesh.data(*f_it).setlabelid(4);
		}
	}
}

void faceLabelToFaceColorRandom(MyMesh &mesh)
{
	unsigned short s;
	std::vector<MyMesh::Color> colorvec(mesh.n_faces());

	//Build vector with random numbers 
	for(int i=0;i<mesh.n_faces();i++)
	{
	    colorvec[i][0]=rand()%255;
	    colorvec[i][1]=rand()%255;
	    colorvec[i][2]=rand()%255;
	}
	for(MyMesh::FaceIter f_it=mesh.faces_begin(); f_it!=mesh.faces_end(); ++f_it)
	{
		s=mesh.data(*f_it).labelid();
		mesh.set_color(*f_it, colorvec[s]);
	}
}

} // namespace 

