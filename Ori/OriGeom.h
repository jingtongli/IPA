// author: mathias, 2017, 2018, mathias.rothermel@geod.baug.ethz.ch
#include "Orientation.h"

// Standard header stuff
namespace OriGeom
{
    void computeRelative( const Orientation &identity, const Orientation &other, Orientation &out );

    void computeSnippet( const Orientation& original,
	    		 const int uplx,
			 const int uply,
			 const int newcols,
			 const int newrows,
			 Orientation out);

    double angleBetweenCams(const Orientation& ori0, const Orientation& ori);
}
