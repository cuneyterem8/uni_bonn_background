#include <odometry_calibration/OdometryCalibration.h>
#include <cmath>

namespace odometry_calibration {

/**
 * \brief Computes the odometry error function.
 * \param[in] groundTruth The ground truth odometry measured by an external sensor (called u* in the slides).
 * \param[in] observation The odometry measurement observed by the robot itself, e.g., using wheel sensors or joint angles (called u in the slides).
 * \param[in] calibrationMatrix The calibration matrix of the current iteration (called X in the slides).
 * \return The error function vector (called e in the slides).
 */
Eigen::Vector3d OdometryCalibration::errorFunction(const Odometry& groundTruth, const Odometry& observation, const Eigen::Matrix3d& calibrationMatrix) {
	Eigen::Vector3d error;
	Eigen::Vector3d obs;
	Eigen::Vector3d gt;
	obs << observation.ux,
	observation.uy,
	observation.utheta;
	gt << groundTruth.ux,
	groundTruth.uy,
	groundTruth.utheta;

	error = gt - calibrationMatrix*obs;
	
	//TODO: Compute the error vector.
	return error;
}

/**
 * \brief Computes the Jacobian (matrix derivative) of the error function for a given odometry measurement.
 * \param[in] measurement The odometry measured by the robot (called u in the slides).
 * \return The Jacobian matrix of the error function.
 */
Eigen::Matrix3Xd OdometryCalibration::jacobian(const Odometry& observation) {
	Eigen::Matrix3Xd jacobian(3, 9);
	Eigen::Vector3d obs;
	obs << observation.ux,
	observation.uy,
	observation.utheta;
	//TODO: Calculate the 3x9 Jacobian matrix of the error function for the given observation.
	jacobian << -obs[0],-obs[1],-obs[2],0,		0,		0,		0,		0,0,

				0,		0,		0,		-obs[0],-obs[1],-obs[2],0,		0,0,
	
				0,		0,		0,		0,		0,		0,-obs[0],-obs[1],-obs[2];
	return jacobian;
}

/**
 * \brief Calibrates the odometry of a robot.
 * \param[in] measurementData A vector containing ground truth and observed odometry measurements.
 * ŗeturn The calibration matrix that can be used to correct the odometry.
 */
Eigen::Matrix3d OdometryCalibration::calibrateOdometry(const std::vector<MeasurementData>& measurements) {
	Eigen::Matrix3d calibrationMatrix = Eigen::Matrix3d::Identity();
	/** TODO: Calculate the calibration matrix. The steps to do are:
	 * - Start with an arbitrary initial calibration matrix
	 * - Iterate through the calibration data
	 * - Compute the error function and Jacobian for each data set
	 * - Accumulate the linear system components H and b
	 * - Solve the linear system
	 * - Update the calibration matrix
	 */
	
	Eigen::Vector3d error = Eigen::Vector3d::Zero(3, 1);
	Eigen::Matrix3Xd A = Eigen::Matrix3Xd::Zero(3, 9);
	Eigen::MatrixXd H = Eigen::MatrixXd::Zero(9, 9);
	Eigen::VectorXd b = Eigen::VectorXd::Zero(9, 1);

	for (int i = 0; i < measurements.size(); i++) {
		error << errorFunction(measurements.data()[i].groundTruth, measurements.data()[i].uncalibrated, calibrationMatrix);
		A << jacobian(measurements.data()[i].uncalibrated);
		H = H + A.transpose()*A;
		b = b + A.transpose()*error;
	}

	Eigen::VectorXd partial_X = -H.colPivHouseholderQr().solve(b);
	Eigen::Matrix3d d_X;
	d_X << partial_X(0), partial_X(1), partial_X(2),
			partial_X(3), partial_X(4), partial_X(5),
			partial_X(6), partial_X(7), partial_X(8);
	//d_X = d_X.transpose();
	calibrationMatrix += d_X;

	return calibrationMatrix;
}

/**
 * \brief Applies the calibration matrix to an odometry measurement in order to get a corrected estimate.
 * \param[in] uncalibratedOdometry An uncalibrated odometry measurement (called u in the slides).
 * \param[in] calibrationMatrix The calibration matrix computed by calibrateOdometry in a previous step (called X in the slides).
 * \return The corrected odometry estimate.
 */
Odometry OdometryCalibration::applyOdometryCorrection(const Odometry& uncalibratedOdometry, const Eigen::Matrix3d& calibrationMatrix) {
	Odometry calibratedOdometry;
	/*TODO: Given the calibration matrix, return the corrected odometry measurement so that the robot has
	 * a better estimate of the location where it is currently.
	 */
	Eigen::Vector3d uo;
	uo << uncalibratedOdometry.ux, uncalibratedOdometry.uy, uncalibratedOdometry.utheta;
	Eigen::Vector3d co = calibrationMatrix*uo;

	calibratedOdometry.ux = co[0];
	calibratedOdometry.uy = co[1];
	calibratedOdometry.utheta = co[2];

	return calibratedOdometry;
}

/**
 * \brief Converts an odometry reading into an affine 2D transformation.
 * \param[in] odometry The odometry reading.
 * \returns The corresponding affine transformation as a 3x3 matrix.
 */
Eigen::Matrix3d OdometryCalibration::odometryToAffineTransformation(const Odometry& odometry) {
	Eigen::Matrix3d transformation;
	//TODO: Convert the odometry measurement to an affine transformation matrix.
	transformation << cos(odometry.utheta), -sin(odometry.utheta), odometry.ux,
					sin(odometry.utheta), cos(odometry.utheta), odometry.uy,
					0, 0, 1;

	return transformation;
}

/**
 * \brief Converts an affine 2D transformation matrix into a 2D robot pose (x, y, and theta).
 * \param[in] transformation An affine transformation as a 3x3 matrix.
 * \returns The corresponding 2D pose (x, y, and theta).
 */
Pose2D OdometryCalibration::affineTransformationToPose(const Eigen::Matrix3d& transformation) {
	Pose2D pose;
	/* TODO: replace the following lines by the x and y position and the rotation of the robot.
	 * Hint: x and y can be directly read from the matrix. To get the rotation, use the acos/asin
	 * functions on the rotation matrix and take extra care of the sign, or (better) use the
	 * atan2 function.
	 */
	pose.x = 0.0;
	pose.y = 0.0;
	pose.theta = 0.0;

	pose.x = transformation(0, 2);
	pose.y = transformation(1, 2);
	pose.theta = atan2(transformation(1, 0), transformation(0, 0));

	return pose;
}

/**
 * \brief Calculate the robot's trajectory in Cartesian coordinates given a list of calibrated odometry measurements.
 * \param[in] calibratedOdometry Odometry measurements that have already been corrected using the applyOdometryCorrection method.
 * \returns A vector of 2D poses in the global coordinate system.
 */
std::vector<Pose2D> OdometryCalibration::calculateTrajectory(const std::vector<Odometry>& calibratedOdometry) {
	std::vector<Pose2D> trajectory;
	/* TODO: Compute the trajectory of the robot.
	 * - Start at the position x = 0, y = 0, theta = 0. (Do not add this point to the trajectory).
	 * - Iterate through the odometry measurements.
	 * - Convert each odometry measurement to an affine transformation using the
	 *   odometryToAffineTransformation method from above.
	 * - Chain the affine transformation to get the next pose.
	 * - Convert the affine transformation back to a 2D pose using the
	 *   affineTransformationToPose method from above.
	 * - Store the pose in the trajectory vector.
	 */

	trajectory = std::vector<Pose2D>(calibratedOdometry.size());
	Eigen::Matrix3d otT = Eigen::Matrix3d::Zero(3, 3);
	Pose2D pose;

	for (int i = 0; i < calibratedOdometry.size(); i++) {
		otT = odometryToAffineTransformation(calibratedOdometry[i]);
		trajectory[i] = affineTransformationToPose(otT);
	}

	return trajectory;
}




} /* namespace odometry_calibration */
