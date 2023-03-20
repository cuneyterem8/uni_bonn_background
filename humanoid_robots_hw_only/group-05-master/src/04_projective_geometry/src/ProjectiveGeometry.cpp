#include <projective_geometry/ProjectiveGeometry.h>
#include <math.h>
#include <iostream>
namespace projective_geometry 
{
const double ProjectiveGeometry::PI = 3.141592654;

/**
 * \brief Converts a 3D Euclidean coordinates to homogeneous coordinates.
 * \param[in] 3D point in Euclidean coordinates.
 * \return 3D point in homogeneous coordinates.
 */
Eigen::Vector4d ProjectiveGeometry::euclideanToHomogeneous(const Eigen::Vector3d& point)
{
    Eigen::Vector4d result = Eigen::Vector4d::Zero();
    //TODO
    result << point(0), point(1), point(2), 1;
    return result;
}
/**
 * \brief Converts a 2D point in homogeneous coordinates into Euclidean coordinates.
 * \param[in] 2D point in homogeneous coordinates.
 * \return 2D point in Euclidean coordinates.
 */
Eigen::Vector2d ProjectiveGeometry::homogeneousToEuclidean(const Eigen::Vector3d& point)
{
    Eigen::Vector2d result = Eigen::Vector2d::Zero();
    //TODO
    if(point(2) != 0) {
        result << point(0)/point(2), point(1)/point(2);
    }
    else {
        result << point(0), point(1);
    }
    return result;
}
/**
 * \brief Assigns the values of the camera's extrinsic and intrinsic parameters.
 * \param[in] alpha The camera's current rotation angle.
 * \return a struct 'cameraParameters' which contains the camera parameters.
 */
CameraParameters ProjectiveGeometry::setCameraParameters(const double alpha)
{
    CameraParameters results;
    //TODO
    results.xH = 400;
    results.yH = 300;
    results.m = 0.0025;
    results.c = 550;

    results.X0 << 0.4, 0, 10;
	results.rotX = 0;
    //results.rotY = (cos(alpha), 0, sin(alpha), 0, 1, 0, -sin(alpha), 0, cos(alpha));
	results.rotY = alpha;
    results.rotZ = 0;
    return results;
}
/**
 * \brief Computes the calibration matrix based on the camera's intrinsic parameters.
 * \param[in] camera parameters (CameraParameters struct).
 * \return Calibration matrix.
 */
Eigen::Matrix3d ProjectiveGeometry::calibrationMatrix(const CameraParameters& param)
{
    Eigen::Matrix3d result = Eigen::Matrix3d::Zero();
    //TODO
    result << param.c, 0, param.xH,
            0, param.c*(1 + param.m), param.yH,
            0, 0, 1;
    return result;
}

/**
 * \brief Computes the projection matrix based on the camera's parameters and the pre-computed calibration matrix.
 * \param[in] Calibration matrix.
 * \param[in] Camera parameters (cameraParameters struct).
 * \return Projection matrix.
 */
Eigen::MatrixXd ProjectiveGeometry::projectionMatrix(const Eigen::Matrix3d& calibrationMatrix, const CameraParameters& param)
{
	Eigen::MatrixXd result = Eigen::MatrixXd::Zero(3, 4);
	Eigen::MatrixXd temp = Eigen::MatrixXd::Zero(3, 4);
	temp << 1,0,0,-param.X0[0],
			0,1,0,-param.X0[1],
    		0,0,1,-param.X0[2];
    Eigen::Matrix3d rot = Eigen::Matrix3d::Zero();
    rot << cos(param.rotY), 0 , sin(param.rotY),
    		0,1,0,
    		-sin(param.rotY),0, cos(param.rotY);

    //TODO
    result = calibrationMatrix*rot*temp;


    return result;
}
/**
 * \brief Applies the pre-computed projection matrix on a 3D point in Euclidean coordinates.
 * \param[in] 3D point in Euclidean coordinates.
 * \param[in] Projection matrix.
 * \return 2D point in Euclidean coordinates.
 */
Eigen::Vector2d ProjectiveGeometry::projectPoint(const Eigen::Vector3d& point, const Eigen::MatrixXd& projectionMatrix)
{
    Eigen::Vector2d result = Eigen::Vector2d::Zero();
    Eigen::Vector3d tempresult = Eigen::Vector3d::Zero();
    //TODO
    tempresult = projectionMatrix*euclideanToHomogeneous(point);

    result = homogeneousToEuclidean(tempresult);

    return result;
}

/**
 * \brief Reprojects an image pixel to a 3D point on a given horizontal plane in Euclidean coordinates.
 * \param[in] imagePoint The image point in pixels where a feature (e.g., the corner of a chess board) was detected.
 * \param[in] calibrationMatrix The calibration matrix calculated above.
 * \param[in] param The intrinsic and extrinsic camera parameters.
 * \param[in] tableHeight The height of the table in front of the robot. The reprojected point will lie on that table.
 * \return 3D point in Eucldiean coordinates.
 */
Eigen::Vector3d ProjectiveGeometry::reprojectTo3D(const Eigen::Vector2d& imagePoint, const Eigen::Matrix3d& calibrationMatrix,
		const CameraParameters& param, const double tableHeight) {
    Eigen::Vector3d coordinates3D = Eigen::Vector3d::Zero();

    // TODO
	Eigen::MatrixXd rotation = Eigen::MatrixXd::Zero(3, 3);
	Eigen::MatrixXd rotation_inv = Eigen::MatrixXd::Zero(3, 3);
	Eigen::MatrixXd temp = Eigen::MatrixXd::Zero(3, 3);
	/*transM << cos(param.rotY), 0, sin(param.rotX), -param.X0[0] * cos(param.rotY) - param.X0[2] * sin(param.rotY),
		0, 1, 0, 0,
		-sin(param.rotY), 0, cos(param.rotY), sin(param.rotY)*param.X0[0] - param.X0[2] * cos(param.rotY),
		0, 0, 0, 1;
	*/
	rotation << cos(param.rotY), 0, sin(param.rotY),
		0, 1, 0,
		-sin(param.rotY), 0, cos(param.rotY);

	rotation_inv = rotation.inverse();
	temp = rotation_inv * calibrationMatrix.inverse();
	//double point_z = (tableHeight - param.X0[2] - temp(2, 0)*imagePoint(0) - temp(2, 1)*imagePoint(1)) / temp(2, 2);
	double point_z = (tableHeight - param.X0[2]) / (temp(2, 0)*imagePoint(0) + temp(2, 1)*imagePoint(1) + temp(2, 2));
	double point_x = (temp(0, 0)*imagePoint(0) + temp(0, 1)*imagePoint(1) + temp(0, 2))*point_z + param.X0[0];
	double point_y = (temp(1, 0)*imagePoint(0) + temp(1, 1)*imagePoint(1) + temp(1, 2))*point_z + param.X0[1];
	coordinates3D << point_x, point_y, tableHeight;

    return coordinates3D;
}


}  // namespace projective_geometry
