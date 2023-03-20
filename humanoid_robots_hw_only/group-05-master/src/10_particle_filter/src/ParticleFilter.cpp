#include <particle_filter/ParticleFilter.h>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <random>
#include <numeric>

namespace particle_filter {

/**
 * \brief Calculate the probability phi(d, stdev) of a measurement according to a Gaussian distribution.
 * \param[in] d The difference between the measurement and the mean
 * \param[in] stdev The standard deviation of the Gaussian.
 * \return Probability of the measurement.
 */
double ParticleFilter::gaussianProbability(const double& d, const double& stdev) {
	double probability = 0.0;
	/*TODO: Calculate the probability of the measurement for a Gaussian distribution with
	  the given mean and standard deviation */
	probability = exp(-0.5*pow(d, 2) / pow(stdev, 2)) / (stdev*sqrt(2 * M_PI));
	return probability;
}

/**
 * \brief Draw a sample from a Gaussian distribution.
 * \param[in] mean The mean of the Gaussian.
 * \param[in] stdev The standard deviation of the Gaussian.
 * \return A random sample drawn from the given Gaussian distribution.
 */
double ParticleFilter::sampleFromGaussian(const double& mean, const double& stdev) {
	double result = 0.0;
	//TODO: draw a sample from a 1D Gaussian
	double sum = 0.0;
	for (int i = 0; i < 12; i++) {
		sum += -stdev + 2.0 * stdev * rand() / (RAND_MAX + 1.0);
	}
	result = mean + 0.5*sum;
	return result;
}


/**
 * \brief Initializes the position and weights of the particles.
 * \param[in,out] particles The list of particles.
 *
 * The positions should be distributed uniformly in the interval [0, 10].
 * The weights should be equal and sum up to 1.
 */
void ParticleFilter::initParticles(std::vector<Particle>& particles) {
	//TODO: Distribute the particles randomly between [0, 10] with equal weights
	for (int i = 0; i < particles.size(); i++) {
		particles[i].x = rand() % 11;
		particles[i].weight = 1.0 / particles.size();
		
	}
}

/**
 * \brief Normalizes the weights of the particle set so that they sum up to 1.
 * \param[in,out] particles The list of particles.
 */
void ParticleFilter::normalizeWeights(std::vector<Particle>& particles) {
	//TODO: normalize the particles' weights so that they sum up to 1.
	double sum = 0.0;
	for (int i = 0; i < particles.size(); i++) {
		sum += particles[i].weight;
	}
	for (int i = 0; i < particles.size(); i++) {
		particles[i].weight = particles[i].weight / sum;

	}

}

/**
 * \brief Displace the particles according to the robot's movements.
 * \param[in,out] particles The list of particles.
 * \param[in] ux The odometry (displacement) of the robot along the x axis.
 * \param[in] stdev The standard deviation of the motion model.
 */
void ParticleFilter::integrateMotion(std::vector<Particle>& particles, const double& ux, const double& stdev) {
	//TODO: Prediction step: Update each sample by drawing the a pose from the motion model.
	for (int i = 0; i < particles.size(); i++){
		particles[i].x = particles[i].x + sampleFromGaussian(ux, stdev);
		
	}
}


/**
 * \brief Returns the distance between the given x position and the nearest light source.
 * \param[in] x The position on the x axis.
 * \return The distance to the nearest light source.
 */
double ParticleFilter::getDistanceToNearestLight(const double& x) {
	double dist = 0.0;
	//TODO Return the distance from the robot's position x to the nearest light source.
	double dis1 = fabs(x - 2.0);
	double dis2 = fabs(x - 6.0);
	double dis3 = fabs(x - 8.0);
	if ((dis1 <= dis2) && (dis1 <= dis3)) {
		dist = dis1;
	}
	else if ((dis2 <= dis1) && (dis2 <= dis3)) {
		dist = dis2;
	}
	else  {
		dist = dis3;
	}
	return dist;
}

/**
 * \brief Updates the particle weights according to the measured distance to the nearest light source.
 * \param[in,out] particles The list of particles.
 * \param[in] measurement The measured distance between the robot and the nearest light source.
 * \param[in] stdev The standard deviation of the observation model.
 */
void ParticleFilter::integrateObservation(std::vector<Particle>& particles, const double measurement,
		const double& stdev) {
	//TODO: Correction step: weight the samples according to the observation model.
	double zDistance = 0.0;
	for (int i = 0; i < particles.size(); i++) {
		zDistance = getDistanceToNearestLight(particles[i].x);
		particles[i].weight = gaussianProbability(zDistance - measurement, stdev);
	}

	// Normalize the weights after updating so that they sum up to 1 again:
	normalizeWeights(particles);
}

/**
 * \brief Resamples the particle set by throwing out unlikely particles and duplicating more likely ones.
 * \param[in] particles The old list of particles.
 * \return The new list of particles after resampling.
 */
std::vector<ParticleFilter::Particle> ParticleFilter::resample(const std::vector<Particle>& particles) {
	std::vector<Particle> newParticles;
	/*TODO: Use stochastic universal resampling (also called low variance resampling)
	 * to draw a new set of particles according to the old particles' weights */

    std::vector<Particle> particlesCopy = particles;
	std::vector<double> weights;
	std::mt19937 gen;

	for(size_t i = 0; i < particlesCopy.size(); i++){
		weights.push_back(particlesCopy[i].weight);
	}

	std::discrete_distribution<int> distribution(weights.begin(), weights.end());
    
    for (int i = 0; i < particlesCopy.size(); i++){
        newParticles.push_back(particlesCopy[distribution(gen)]);
    }

	return newParticles;
}

}  // namespace particle_filter

