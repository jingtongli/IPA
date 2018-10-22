// author: mathias, 2018, mathias.rothermel@geod.baug.ethz.ch
#pragma once
#include <eigen3/Eigen/Dense>
// #include <unsupported/Eigen/FFT>
#include "Orientation.h"

namespace GeomUtil
{
	void intersect( const Orientation& ori0, const Eigen::Vector3d x0,
			const Orientation& ori1, const Eigen::Vector3d x1,
			Eigen::Vector3d& X);

	void planeFrom3Pts(const Eigen::Vector3d& pt0, const Eigen::Vector3d& pt1, const Eigen::Vector3d& pt2, Eigen::Vector4d& plane);

	void intersectRayPlane(const Eigen::Vector3d& X0, const Eigen::Vector3d& X1, const Eigen::Vector4d& plane, Eigen::Vector3d& pt);

}
