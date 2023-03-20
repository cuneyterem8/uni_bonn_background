#include <irm/IRM.h>
#include <angles/angles.h>
#include <vector>

namespace irm {

/**
 * \brief Draws an angle for each joint randomly from a uniform distribution.
 * \return The vector (q0, q1, q2) of random joint angles.
 */
Eigen::Vector3d IRM::sampleConfiguration() const {
	Eigen::Vector3d randomJointAngles = Eigen::Vector3d::Zero();

	// TODO Sample the joint angles.

	// CAUTION: Do not call srand() in this method. The random generator is already seeded.

	std::pair<double, double> ranges[3] = {std::make_pair(0., M_PI/2), std::make_pair(-M_PI, M_PI), std::make_pair(-M_PI, M_PI)};

	for(size_t i = 0; i < 3; i++) {
		randomJointAngles[i] = static_cast<double>(rand()) / RAND_MAX * (ranges[i].second - ranges[i].first) + ranges[i].first;
	}

	return randomJointAngles;
}

/**
 * \brief Computes the manipulability of a robot configuration.
 * \param[in] jointAngles The angles of each joint (q_0, q_1, q_2).
 * \param[in] endeffectorPose The endeffector pose (e_x, e_y, e_theta).
 * \return The manipulability score between 0.0 (bad pose) and 1.0 (good pose).
 */
double IRM::computeManipulability(const Eigen::Vector3d& jointAngles, const Eigen::Vector3d& endeffectorPose) const {
	double manipulability = 0.0;
	// TODO Compute the manipulability score according to the function on the exercise sheet.

	manipulability = 1.0 - (fabs((double)(4*jointAngles[0] - M_PI)) + fabs((double)jointAngles[1]) 
	+ fabs((double)jointAngles[2]) + fabs((double)endeffectorPose[2])) / (4*M_PI);

	return manipulability;
}

/**
 * \brief Computes the reachability map of the robot.
 * \param[in] numSamples The number of samples that the method should add to the reachability map.
 */
void IRM::computeRM(const size_t& numSamples) {
	// TODO: Compute the reachability map.

	/* Available methods:
	 *
	 * - sampleConfiguration(): defined above, draws a random joint configuration
	 *
	 * - computeManipulability(): defined above, computes the manipulability score.
	 *
	 * - addToRM(const Eigen::Vector3d& jointAngles, const Eigen::Vector3d& endeffectorPose, const double& manipulability):
	 *       Stores the given values to the reachability map.
	 *
	 * - bool forwardKinematics(const Eigen::Vector3d& jointAngles, Eigen::Vector3d& endeffectorPose):
	 *       Calculates the forward kinematics for jointAngles and stores the result in endeffectorPose.
	 *       The method returns true on success and false on failure (e.g., if the joint angles are
	 *       beyond the robot's limits).
	 *
	 * - bool inverseKinematics(const Eigen::Vector3d& endeffectorPose, Eigen::Vector3d& jointAngles):
	 *       Calculates the inverse kinematics for an endeffectorPose and stores the result in jointAngles.
	 *       The method returns true on success and false on failure (e.g., if the endeffector pose is
	 *       impossible to reach).
	 *
	 */

	for(size_t i = 0; i < numSamples; ) {
		const const Eigen::Vector3d jointAngles = sampleConfiguration();
		Eigen::Vector3d endeffectorPose;
		if(forwardKinematics(jointAngles, endeffectorPose) && endeffectorPose[1] > 0.0) {
			const double manipulability = computeManipulability(jointAngles, endeffectorPose);
			addToIRM(jointAngles, endeffectorPose, manipulability);
			i++;
		}
	}

}

/**
 * \brief Computes the inverse reachability map.
 * \param[in] voxels The voxels of the reachability map computed by the method above.
 */
void IRM::computeIRM(const std::vector<RMVoxel>& voxels) {
	// TODO: Compute the inverse reachability map.

	/* Available methods and fields:
	 * - voxels[i].configurations: a standard vector containing the configurations of the voxel.
	 *
	 * - voxels[i].configurations[j]->jointAngles: The joint angles of the given configuration.
	 * - voxels[i].configurations[j]->manipulability: The manipulability score of the given configuration.
	 *
	 * - addToIRM(const Eigen::Vector3d& basePose, const Eigen::Vector3d& jointAngles, const double& manipulability):
	 *       Stores the given values to the reachability map.
	 *
	 * - bool forwardKinematics(const Eigen::Vector3d& jointAngles, Eigen::Vector3d& endeffectorPose):
	 *       Calculates the forward kinematics for jointAngles and stores the result in endeffectorPose.
	 *       The method returns true on success and false on failure (e.g., if the joint angles are
	 *       beyond the robot's limits).
	 *
	 * - bool inverseKinematics(const Eigen::Vector3d& endeffectorPose, Eigen::Vector3d& jointAngles):
	 *       Calculates the inverse kinematics for an endeffectorPose and stores the result in jointAngles.
	 *       The method returns true on success and false on failure (e.g., if the endeffector pose is
	 *       impossible to reach).
	 *
	 */

	for(size_t i = 0; i < voxels.size(); i++) {
		for(size_t j = 0; j < voxels[i].configurations.size(); j++) {
			const JointConfiguration *configuration = voxels[i].configurations[j];
			Eigen::Vector3d eef;
			forwardKinematics(configuration->jointAngles, eef);
			Eigen::Matrix3d eefTransform;
			const double theta = eef[2];
			eefTransform << cos(theta), -sin(theta), eef[0],
							sin(theta), cos(theta), eef[1],
							0, 0, 1;
			const Eigen::Matrix3d baseTransform = eefTransform.inverse();
			Eigen::Matrix3d base;
			base << baseTransform(0, 2), baseTransform(1, 2), atan2((double)baseTransform(1, 0), (double)baseTransform(0, 0));
			addToIRM(base, configuration->jointAngles, configuration->manipulability);
		}
	}

}


}  // namespace irm
