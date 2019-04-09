/* (c) https://github.com/andeplane/master */

/*! \brief This class creates a randomly generated heightmap using either successive random additions algorithm or
successive random displacement algorithm.

The program starts with generating a 2d grid of size (2^power2+1)x(2^power2+1). We initialize the system by setting
the z-value in the corners to a given value. We then generate the rest of the z-values by iteration.

The algorithm consists of two separate steps, called "diamond" and "square".

In the diamond-step we find the z-value of a point (x,y) in the grid by taking the average of four points:
(x+d, y), (x-d,y), (x,y+d), (x,y-d), where d is a steplength, and adding a random number (usually a normally
distributed random number, but uniform distribution is also available). The four points make up a diamond centered
around (x,y), thus we call it the diamond-step. See the ASCII-illustration below.

o o D o o
o o o o o
D o X o D
o o o o o
o o D o o
Figure: The diamond-step. We interpolate the points marked "D" and add a random number to find the value in "X"

In the square-step we find the z-value of a point (x,y) average of four diagonal neighbors of the point: (x+d, y+d),
(x+d,y-d), (x-d,y+d), (x-d,y-d). These four neighbors make up a square witht he point we want to find in the center,
thus we call it the square-step. See the ASCII-illustration below.

S o o o S
o o o o o
o o X o o
o o o o o
S o o o S
Figure: The square-step. We interpolate the points marked "S" and add a random number to find the value in "X"

When generating the heightmap we first do the square step once, interpolating the four initial points. We then do the
diamond-step four times, filling in the values in the center of the four edges of the grid. We then continue doing
this until the whole grid is filled in. The standard deviation of the random displacement in each step is multiplied
with sigma_(i-1)*0.5^(0.5*H), where sigma_(i-1) is the standard deviation in the last step. This is to produce
Brownian motion with a Hurst-exponent of H.

By default the program also adds a random displacement to the points used in the interpolation. This is called the
successive random additions algorithm. For more info on what this does see the book "The Science of Fractal Images",
chapter 2 "Algorithms for random fractals", section 2.3.3 (available here:
http://bringerp.free.fr/Files/Captain%20Blood/Saupe87d.pdf). To turn of this behaviour, and use the successive
random displacement algorithm instead, set the boolean input variable "addition" to false.

The methods used to produce the heightmaps are more thoroughly described in the book "Fractals" by Jens Feder,
sections 9.8 and 13.4 (1988 version), in the book "The Science of Fractal Images", chapters 1 and 2 (1988 version),
and in the book "Fundamental Algorithms for Computer Graphics" in the chapter "Random fractal forgeries" by Voss R. F.
*/

#ifndef DIAMONDSQUARE_H
#define DIAMONDSQUARE_H

#include <iostream>
#include <vector>
#include <cmath>        // pow, sqrt,

typedef unsigned int uint;


#define IA 16807
#define IM 2147483647
#define AM (1.0/IM)
#define IQ 127773
#define IR 2836
#define NTAB 32
#define NDIV (1+(IM-1)/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)

class Random {
public:
	long     iy;
	long     iv[NTAB];
	long     idum[1];

	Random(long seed) {
		*idum = seed;
		iy = 0;
	}

	double next_gauss() {

		return sqrt(-2.0*log(1.0 - next_double()))
			* cos(6.283185307 * next_double());
	}

	double next_double()
	{
		int             j;
		long            k;
		double          temp;

		if (*idum <= 0 || !iy) {
			if (-(*idum) < 1) *idum = 1;
			else *idum = -(*idum);
			for (j = NTAB + 7; j >= 0; j--) {
				k = (*idum) / IQ;
				*idum = IA*(*idum - k*IQ) - IR*k;
				if (*idum < 0) *idum += IM;
				if (j < NTAB) iv[j] = *idum;
			}
			iy = iv[0];
		}
		k = (*idum) / IQ;
		*idum = IA*(*idum - k*IQ) - IR*k;
		if (*idum < 0) *idum += IM;
		j = iy / NDIV;
		iy = iv[j];
		iv[j] = *idum;
		if ((temp = AM*iy) > RNMX) return RNMX;
		else return temp;
	}
};


class cDiamondSquare {
public:

	cDiamondSquare(const uint power2, const long seed) :
		power2(power2),
		systemSize((uint) pow(2.0, power2) + 1),
		R(std::vector<std::vector<double> >(systemSize, std::vector<double>(systemSize))),
		rnd(new Random(-std::abs(seed)))
		{	}

	/*!
	\brief generate The method used to generate a heightmap. Returns a reference to a member vector<vector<double>> "matrix".

	This method generates the heightmap as described in the main description of this class.

	The size of the grid is calculated as 2^\a power2 + 1.

	If periodic boundaries is enabled the values in the bottom and top row are the same, as are the values in the
	right and left column. The biggest impact this has is that when we do the diamond step for a point close to or
	at the edge, we will use a point from the opposite end of the grid in the interpolation, instead of just using
	three points. See the ASCII-art below for a visual explanation

	1 o o o # o o o #
	o o o o o o o o o
	D o 2 o o o 4 o o
	o o o o o o o o o
	3 o o o # o o o #
	o o o o o o o o o
	o o # o o o # o o
	o o o o o o o o o
	# o o o # o o o #
	diamond-square    with PBC: the point "D" is calculated by interpolating the points 1,2,3, and 4.
	diamond-square without PBC: the point "D" is calculated by interpolating the points 1,2, and 3.
	(when using PBC all four corners of the grid would be point 1, and points 3 and D would also appear on the right
	edge)

	Each iteration the standard deviation of the random numbers is multiplied by a factor
	pow(randomRangeReductionFactor, H).

	The lacunarity of the surface can be adjusted with the parameter \a randomRangeReductionFactor. If addition is
	disabled this should be set to 1/sqrt(2). See "Fundamental Algorithms for Computer Graphics" in the chapter
	"Random fractal forgeries" by Voss R. F. for more details.

	If addition is enabled we add a random displacement to all the points used in the interpolation for the new
	points in both the diamond- and the square-step. This is called the successive random addition algorithm. If
	addition is disabled we use the successive random displacement algorithm.

	If periodic boundary conditions are enabled, the first element in \a corners is used for all corners.

	\param power2 an integer argument that sets the size of the grid.
	\param H the Hurst-exponent
	\param corners an armadillo std::vector with the initial values of the corners.
	\param seed the seed used for the random number generator.
	\param sigma the initial standard deviation of the random displacement.
	\param randomRangeReductionFactor the range of the random numbers gets reduced by this factor each iteration.
	\param addition a bool controls random displacement of the points used in the interpolation of a new point.
	If true we use the successive random additions algorithm, if false we use the successive random
	displacement algorithm.
	\param PBC a bool that controls periodic boundary conditions.
	\param RNG an unsigned in the selects which random number generator to use (0 just returns 0.0, 1 uses a uniform
	distribution, 2 uses a normal distribution).
	*/
	std::vector<std::vector<double> >& generate(
		double H = 0.75,
		std::vector<double> corners = std::vector<double>(),
		double sigma = 1.0,
		double randomRangeReductionFactor = 1.0 / sqrt(2.0),
		bool addition = true,
		bool PBC = false) {

		if (corners.size() == 0) { // Use random corners if no values are supplied
			corners.resize(4);
			if (PBC) {
				corners[0] = sigma*random();
				corners[1] = corners[0];
				corners[2] = corners[0];
				corners[3] = corners[0];
			}
			else {
				corners[0] = sigma*random();
				corners[1] = sigma*random();
				corners[2] = sigma*random();
				corners[3] = sigma*random();
			}
			sigma *= pow(randomRangeReductionFactor, H);
		}

		R[0][0] = corners[0];
		R[0][systemSize - 1] = corners[1];
		R[systemSize - 1][0] = corners[2];
		R[systemSize - 1][systemSize - 1] = corners[3];

		runDiamondSquare(R, H, sigma, randomRangeReductionFactor, addition, PBC);

		normalizeArray(R);
		return R;
	}


	void normalizeArray(std::vector<std::vector<double>>& pNoise) {
		double min = 0;
		double max = 0;

		for (size_t i = 0; i < pNoise.size(); i++) {
			for (size_t j = 0; j < pNoise[i].size(); j++) {

				if (pNoise[i][j] < min) {
					min = pNoise[i][j];
				}
				else if (pNoise[i][j] > max) {
					max = pNoise[i][j];
				}
			}
		}

		double divisor = max - min;

		for (size_t i = 0; i < pNoise.size(); i++) {
			for (size_t j = 0; j < pNoise[i].size(); j++) {

				pNoise[i][j] = (pNoise[i][j] - min) / divisor;

			}
		}
	}
private:

	void runDiamondSquare(
		std::vector<std::vector<double> >& R,
		const double H,
		double initialSigma,
		const double randomRangeReductionFactor,
		const bool addition,
		const bool _PBC) {

		double sigma = initialSigma;
		uint stepLength = systemSize - 1;
		uint halfStepLength = stepLength / 2;
		this->PBC = _PBC;

		for (uint depth = 1; depth <= power2; depth++) {

			// Squares //
			for (uint x = halfStepLength; x < systemSize - halfStepLength; x += stepLength) {
				for (uint y = halfStepLength; y < systemSize - halfStepLength; y += stepLength) {
					R[x][y] = meanOfSquare(x, y, halfStepLength, R) + sigma*random();
				}
			}
			if (addition) {
				// Add random number to all old points
				uint limit;
				if (PBC) {
					limit = systemSize - halfStepLength; // Skip right and bottom edge if using periodic boundary conditions
				}
				else {
					limit = systemSize;
				}
				for (uint x = 0; x < limit; x += stepLength) {
					for (uint y = 0; y < limit; y += stepLength) {
						R[x][y] += sigma*random();
					}
				}
				if (PBC) {
					// Set corners equal to R[0][0]
					R[0][systemSize - 1] = R[0][0];
					R[systemSize - 1][0] = R[0][0];
					R[systemSize - 1][systemSize - 1] = R[0][0];

					// Set bottom edge equal to top edge, and right edge equal to left edge
					for (uint idx = 0; idx < systemSize; idx += stepLength) {
						R[idx][systemSize - 1] = R[idx][0]; // Right/left edge
						R[systemSize - 1][idx] = R[0][idx]; // Bottom/top edge
					}
				}
			}
			sigma *= pow(randomRangeReductionFactor, H);

			// Diamonds //
			// Every other row of diamond points, starting with the one at (0, halfStepLength)
			for (uint x = 0; x < systemSize - halfStepLength; x += stepLength) {
				for (uint y = halfStepLength; y < systemSize - halfStepLength; y += stepLength) {
					R[x][y] = meanOfDiamond(x, y, halfStepLength, R) + sigma*random();
				}
			}
			// Every other row of diamond points, starting with the one at (halfStepLength, 0)
			for (uint x = halfStepLength; x < systemSize - halfStepLength; x += stepLength) {
				for (uint y = 0; y < systemSize - halfStepLength; y += stepLength) {
					R[x][y] = meanOfDiamond(x, y, halfStepLength, R) + sigma*random();
				}
			}
			if (PBC) {
				// Set bottom edge equal to top, and right edge equal to left
				for (uint idx = halfStepLength; idx < systemSize - 1; idx += halfStepLength) {
					R[idx][systemSize - 1] = R[idx][0]; // Right/left edge
					R[systemSize - 1][idx] = R[0][idx]; // Bottom/top edge
				}
			}
			else {
				// Bottom edge diamonds
				for (uint y = halfStepLength; y < systemSize - halfStepLength; y += stepLength) {
					uint x = systemSize - 1;
					R[x][y] = nonPBCbottomEdgeDiamonds(x, y, halfStepLength, R) + sigma*random();
				}

				// Right edge diamonds
				for (uint x = halfStepLength; x < systemSize - halfStepLength; x += stepLength) {
					uint y = systemSize - 1;
					R[x][y] = nonPBCrightEdgeDiamonds(x, y, halfStepLength, R) + sigma*random();
				}
			}
			if (addition) {
				// Add a random number to all old points
				uint limit;
				if (PBC) {
					limit = systemSize - halfStepLength; // Skip right and bottom edge if using periodic boundary conditions
				}
				else {
					limit = systemSize;
				}
				for (uint x = 0; x < limit; x += stepLength) {
					for (uint y = 0; y < limit; y += stepLength) {
						R[x][y] += sigma*random();
					}
				}
				for (uint x = halfStepLength; x < systemSize - halfStepLength; x += stepLength) {
					for (uint y = halfStepLength; y < systemSize - halfStepLength; y += stepLength) {
						R[x][y] += sigma*random();
					}
				}
				if (PBC) {
					// Set corners equal
					R[0][systemSize - 1] = R[0][0];
					R[systemSize - 1][0] = R[0][0];
					R[systemSize - 1][systemSize - 1] = R[0][0];

					// Set bottom edge equal to top, and right edge equal to left
					for (uint idx = halfStepLength; idx < systemSize - 1; idx += halfStepLength) {
						R[idx][systemSize - 1] = R[idx][0]; // Right/left edge
						R[systemSize - 1][idx] = R[0][idx]; // Bottom/top edge
					}
				}
			}
			sigma *= pow(randomRangeReductionFactor, H);

			stepLength /= 2;
			halfStepLength /= 2;
		}
	}

	double meanOfSquare(
		const uint x,
		const uint y,
		const uint halfStepLength,
		const std::vector<std::vector<double> >&R) {

		return 0.25*(
			R[x + halfStepLength][y + halfStepLength] +
			R[x + halfStepLength][y - halfStepLength] +
			R[x - halfStepLength][y + halfStepLength] +
			R[x - halfStepLength][y - halfStepLength]);
	}

	double meanOfDiamond(
		const uint x,
		const uint y,
		const uint halfStepLength,
		const std::vector<std::vector<double> > &R) {

		double average;

		if (x == 0) { // At top edge of system
			if (PBC) {
				average = 0.25*(
					R[x][y + halfStepLength] +
					R[x][y - halfStepLength] +
					R[x + halfStepLength][y] +
					R[R.size() - 1 - halfStepLength][y]);
			}
			else {
				average = (1.0 / 3.0)*(
					R[x][y + halfStepLength] +
					R[x][y - halfStepLength] +
					R[x + halfStepLength][y]);
			}
		}
		else if (y == 0) { // At left edge of system
			if (PBC) {
				average = 0.25*(
					R[x][y + halfStepLength] +
					R[x][R[0].size() - 1 - halfStepLength] +
					R[x + halfStepLength][y] +
					R[x - halfStepLength][y]);
			}
			else {
				average = (1.0 / 3.0)*(
					R[x][y + halfStepLength] +
					R[x + halfStepLength][y] +
					R[x - halfStepLength][y]);
			}
		}
		else {
			average = 0.25*(
				R[x][y + halfStepLength] +
				R[x][y - halfStepLength] +
				R[x + halfStepLength][y] +
				R[x - halfStepLength][y]);
		}

		return average;
	}

	double nonPBCbottomEdgeDiamonds(const uint x, const uint y, const uint halfStepLength, std::vector<std::vector<double> >& R) {

		return (1.0 / 3.0)*(
			R[x - halfStepLength][y] +
			R[x][y + halfStepLength] +
			R[x][y - halfStepLength]);
	}

	double nonPBCrightEdgeDiamonds(const uint x, const uint y, const uint halfStepLength, std::vector<std::vector<double> >& R) {

		return (1.0 / 3.0)*(
			R[x][y - halfStepLength] +
			R[x + halfStepLength][y] +
			R[x - halfStepLength][y]);
	}

	inline double random() {
		return rnd->next_gauss();
	}

	uint power2, systemSize;
	std::vector<std::vector<double> > R;
	uint PBC;
	Random* rnd;
};


#endif // DIAMONDSQUARE_H
