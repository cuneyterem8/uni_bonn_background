#include <icp/ICP.h>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <math.h>
#include <Eigen/SVD> 

using namespace std;
namespace icp
{
/**
 * \brief Compute the Euclidean distance between a pair of 2D points.
 * \param[in] p1: The first 2D point.
 * \param[in] p2: The second 2D point.
 * \return The Euclidean distance between the two input 2D points.
 */
	double ICP::distance(const Eigen::Vector2d& p1, const Eigen::Vector2d& p2)
	{
		double result = -1.0;
		//TODO: Calculate the distance between the points.
		result = sqrt(pow(p1(0) - p2(0), 2) + pow(p1(1) - p2(1), 2));
		return result;
	}
/**
 * \brief Compute the closest point that lies on a given line to a given 2D point.
 * \param[in] pX: The given 2D point, to which we need to compute the closest point that lies on a line.
 * \param[in] pL1: A point that lies on the line.
 * \param[in] pL2: Another point that lies on the line.
 * \return The closest point on the line.
 */
	Eigen::Vector2d ICP::closestPointOnLine(const Eigen::Vector2d& pX, const Eigen::Vector2d& pL1, const Eigen::Vector2d& pL2)
	{
		Eigen::Vector2d result(0., 0.);
		//TODO: Compute the point on the line (pL1-pL2) that is closest to pX.
		double slope1 = 0.0;
		double temp_x = 0.0;
		double temp_y = 0.0;
		if (pL2(1) - pL1(0) != 0) {
			slope1 = (pL2(1) - pL1(1)) / (pL2(0) - pL1(0));
			temp_x = (pow(slope1, 2)*pL1(0) + slope1 * (pX(1) - pL1(1)) + pX(0)) / (pow(slope1, 2) + 1);
			temp_y = slope1 * (temp_x - pL1(0)) + pL1(1);
			result << temp_x, temp_y;

		}
		else {
			result << pL1(0), pX(1);
		}
		return result;


	}
/**
 * \brief Get the minimum value within vector.
 * \param[in] dist: A vector of values.
 * \return The minimum value.
 */
	double ICP::minDistance(const std::vector<double>& dist)
	{
		double result = 0.0;
		//TODO: Find and return the minimum value in the vector dist
		result = *min_element(dist.begin(), dist.end());
		return result;
	}
/**
 * \brief Compute the corresponding points in list P to those points in list Q, using the 'closest point' matching method.
 * \param[in] Q: A vector of 2D points.
 * \param[in] P: A vector of 2D points.
 * \return A vector of the corresponding 2D points matched to points of list Q in list P.
 *
 * StdVectorOfVector2d is equivalent to std::vector<Eigen::Vector2d>.
 * The result vector will have the same length as Q.
 */
	StdVectorOfVector2d ICP::euclideanCorrespondences(const StdVectorOfVector2d& Q, const StdVectorOfVector2d& P)
	{
		StdVectorOfVector2d result;

		//TODO: Compute corresponding points using the "closest point" method
		double temp = 0.0;
		int index = 0;
		for (int i = 0; i < Q.size(); i++ ) {
			temp = distance(Q[i], P[0]);
			for (int j = 1; j < P.size(); j++ ) {
					if (distance(Q[i], P[j]) < temp) {
						temp = distance(Q[i], P[j]);
						index = j;
					}
				}
			result.push_back(P[index]);
		}
		return result;
	}
/**
 * \brief Compute the corresponding points in list P to those points in list Q, using the 'point-to-line' matching method .
 * \param[in] Q: A vector of 2D points.
 * \param[in] P: A vector of 2D points.
 * \return A vector of the corresponding 2D points matched to points of list Q in list P.
 *
 * StdVectorOfVector2d is equivalent to std::vector<Eigen::Vector2d>.
 */
	StdVectorOfVector2d ICP::closestPointToLineCorrespondences(const StdVectorOfVector2d& Q, const StdVectorOfVector2d& P)
	{
		StdVectorOfVector2d  result;
		//TODO: Compute corresponding points using the "point-to-line" method
		StdVectorOfVector2d  vector_temp;
		
		Eigen::Vector2d point1(0., 0.);
		Eigen::Vector2d point2(0., 0.);
		Eigen::Vector2d point(0., 0.);
		double temp = 0.0;
		int index = 0;
		vector_temp = euclideanCorrespondences(Q, P);
		for (int i = 0; i < Q.size(); i++) {
			point1 = vector_temp[i];
			if (i == 0) {
				temp = distance(point1, vector_temp[1]);
			}
			else {
				temp = distance(point1, vector_temp[0]);
			}
			for (int j = 1; j < vector_temp.size(); j++) {
				if (distance(point1, vector_temp[j]) < temp && distance(point1, vector_temp[j]) != 0) {
					temp = distance(point1, vector_temp[j]);
					index = j;
				}
				}
			point2 = vector_temp[index];
			point = closestPointOnLine(Q[i], point1, point2);
			if (distance(point, point1) < distance(point, point2)) {
				result.push_back(point1);
			}
			else {
				result.push_back(point2);
			}
			
		}


		return result;

	}
    Eigen::Vector2d homogeneousToEuclidean(const Eigen::Vector3d& point)
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
 * \brief Compute the affine transformation matrix needed to align the previously computed corresponding points (list C) to the points of list Q.
 * \param[in] Q: A vector of 2D points.
 * \param[in] C: A vector of 2D points, that corresponds to points in list Q.
 * \return An Affine transformation matrix.
 *
 * StdVectorOfVector2d is equivalent to std::vector<Eigen::Vector2d>.
 */
	Eigen::Matrix3d ICP::calculateAffineTransformation(const StdVectorOfVector2d& Q, const StdVectorOfVector2d& C)
	{
		Eigen::Matrix3d result = Eigen::Matrix3d::Zero();
        Eigen::Vector2d com1(0., 0.);
        Eigen::Vector2d com2(0., 0.);
        StdVectorOfVector2d  newQ;
        StdVectorOfVector2d  newP;
        for (int i=0; i< Q.size();i++){
            com1(0) += Q[i].operator()(0);
            com1(1) += Q[i].operator()(1);
            com2(0) += C[i].operator()(0);
            com2(1) += C[i].operator()(1);

        }
        com1(0) = (1.0/C.size()) *com1(0);
        com1(1) = (1.0/C.size()) *com1(1);
        com2(0) = (1.0/C.size()) *com2(0);
        com2(1) = (1.0/C.size()) *com2(1);
        for (int i=0; i< Q.size();i++){
           newQ.push_back({Q[i].operator()(0)-com1(0), Q[i].operator()(1)-com1(1)});
           newP.push_back({C[i].operator()(0)-com2(0), C[i].operator()(1)-com2(1)});

        }

        Eigen::Matrix3d W = Eigen::Matrix3d::Zero();
        for (int i=0; i< C.size();i++){
            Eigen::Vector3d homoTransQ= Eigen::Vector3d::Zero();
            homoTransQ << newQ[i].operator()(0), newQ[i].operator()(1), 1;
            Eigen::Vector3d homoTransP = Eigen::Vector3d::Zero();
            homoTransP << newP[i].operator()(0), newP[i].operator()(1), 1;
            W += homoTransQ*homoTransP.transpose();
        }
        Eigen::Vector3d homoTranscom2 = Eigen::Vector3d::Zero();
        homoTranscom2 << com2(0), com2(1), 1;
        Eigen::JacobiSVD<Eigen::Matrix3d> svd(W, Eigen::ComputeFullU | Eigen::ComputeFullV);
        Eigen::Matrix3d U = svd.matrixU();
        Eigen::Matrix3d V = svd.matrixV();
        Eigen::Matrix3d R_ = U * (V.transpose());
        Eigen::Vector2d t_ (com1(0) - (homogeneousToEuclidean(R_ * homoTranscom2))(0),
        com1(1) - (homogeneousToEuclidean(R_ * homoTranscom2))(1));

        result << R_(0,0),R_(0,1),t_(0),
                  R_(1,0),R_(1,1),t_(1),
                  0,0,1;
        //TODO: Compute the affine transformation matrix


		return result;

	}

/**
 * \brief Apply the affine transformation matrix on the points in list P.
 * \param[in] A: Affine transformation matrix.
 * \param[in] P: A vector of 2D points, on which the affine transformation will be applied.
 * \return The vector of transformed points.
 *
 * StdVectorOfVector2d is equivalent to std::vector<Eigen::Vector2d>.
 */
	StdVectorOfVector2d ICP::applyTransformation(const Eigen::Matrix3d& A, const StdVectorOfVector2d& P)
	{
		StdVectorOfVector2d  result;
        Eigen::Vector3d homoTrans = Eigen::Vector3d::Zero();
        //TODO
        for (int i = 0 ; i< P.size(); i++){
            homoTrans << P[i](0), P[i](1), 1;
            //homoTrans = homoTrans.transpose();
            result.push_back(homogeneousToEuclidean(A*homoTrans));
        }



		//TODO: Apply the affine transformation A to the points in list P.

		return result;
	}
/**
 * \brief Compute the error between the points in Q list and the transformed corresponding points.
 * \param[in] Q: A vector of 2D points.
 * \param[in] C: A vector of 2D points corresponding to point in Q.
 * \param[in] A: Affine transformation matrix.
 * \return The error of applying the transformation.
 *
 * StdVectorOfVector2d is equivalent to std::vector<Eigen::Vector2d>.
 */
	double ICP::computeError(const StdVectorOfVector2d& Q, const StdVectorOfVector2d& C, const Eigen::Matrix3d& A)
	{
		double result = 0.0;
        Eigen::Vector3d homoTrans = Eigen::Vector3d::Zero();
        Eigen::Vector2d eucTrans = Eigen::Vector2d::Zero();
		for(int i=0 ; i< C.size(); i++){
            homoTrans << C[i](0), C[i](1), 1;

		    result += pow(distance(Q[i] , homogeneousToEuclidean(A*homoTrans)),2);


		}
		//TODO: Compute the error after the transformation.
		return result;		
	}


/**
 * \brief Perform one iteration of ICP and prints the error of that iteration.
 * \param[in] Q: A vector of 2D points.
 * \param[in] P: A vector of 2D points, that we need to transform them to be aligned with points in Q list.
 * \param[out] convergenceFlag: A flag passed by reference to determine whether the alignment error has crossed the convergence threshold or not. The flag should be set to 'true' in case of convergence.
 * \param[in] pointToLineFlag: A flag that states the matching method to be used. Its value is set to 'true' when point-to-line method is needed, and to 'false' when closest point method is needed.
 * \param[in] threshold: The maximum value of acceptable error.
 * \return The vector of transformed points.
 *
 * StdVectorOfVector2d is equivalent to std::vector<Eigen::Vector2d>.
 */
	StdVectorOfVector2d ICP::iterateOnce(const StdVectorOfVector2d& Q, const StdVectorOfVector2d& P, bool & convergenceFlag, const bool pointToLineFlag, const double threshold)
	{
		StdVectorOfVector2d result;
        Eigen::Matrix3d A = Eigen::Matrix3d::Zero();
        StdVectorOfVector2d C;
        C= euclideanCorrespondences(Q,P);
        A = calculateAffineTransformation(Q,C);
        result = applyTransformation(A,P);


		
		//TODO: Perform one iteration of ICP
	
		double error = 0.0;
		//TODO: Compute the error
		error = computeError(Q,C,A);

		if (error <= threshold) {
			convergenceFlag = true;
		}

		return result;
	}


}
