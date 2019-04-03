/*
 * A speed-improved simplex noise algorithm for 2D, 3D and 4D in Java.
 *
 * Based on example code by Stefan Gustavson (stegu@itn.liu.se).
 * Optimisations by Peter Eastman (peastman@drizzle.stanford.edu).
 * Better rank ordering method for 4D by Stefan Gustavson in 2012.
 *
 * This could be speeded up even further, but it's useful as it is.
 *
 * Version 2012-03-09
 *
 * This code was placed in the public domain by its original author,
 * Stefan Gustavson. You may use it as you see fit, but
 * attribution is appreciated.
 *
 */
#include <math.h>
struct Grad
{
	double x, y, z, w;
	Grad() {
		x = 0;
		y = 0;
		z = 0;
		w = 0;
	}
	Grad(double px, double py, double pz) {
		x = px;
		y = py;
		z = pz;
	}

	Grad(double px, double py, double pz, double pw) {
		x = px;
		y = py;
		z = pz;
		w = pw;
	}
};

class SimplexIslands {  // Simplex noise in 2D, 3D and 4D
		// Inner class to speed upp gradient computations
	// (In Java, array access is a lot slower than member access)
public:
	std::vector<Grad> grad3;
	std::vector<Grad> grad4;

	static short pbak[];
	static short p[];

	// To remove the need for index wrapping, double the permutation table length
	short perm[512];
	short permMod12[512];
	
	double F2;
	double G2;
	double F3;
	double G3;
	double F4;
	double G4;

	SimplexIslands();

	// This method is a *lot* faster than using (int)Math.floor(x)
	static inline int32_t fastfloor(double fp) {
		int32_t i = static_cast<int32_t>(fp);
		return (fp < i) ? (i - 1) : (i);
	}

	double dot(Grad g, double x, double y) {
		return g.x*x + g.y*y;
	}

	// 2D simplex noise
	double noise(double xin, double yin) {
		double n0, n1, n2; // Noise contributions from the three corners
		// Skew the input space to determine which simplex cell we're in
		double s = (xin + yin)*F2; // Hairy factor for 2D
		int i = fastfloor(xin + s);
		int j = fastfloor(yin + s);
		double t = (i + j)*G2;
		double X0 = i - t; // Unskew the cell origin back to (x,y) space
		double Y0 = j - t;
		double x0 = xin - X0; // The x,y distances from the cell origin
		double y0 = yin - Y0;
		// For the 2D case, the simplex shape is an equilateral triangle.
		// Determine which simplex we are in.
		int i1, j1; // Offsets for second (middle) corner of simplex in (i,j) coords
		if (x0 > y0) { i1 = 1; j1 = 0; } // lower triangle, XY order: (0,0)->(1,0)->(1,1)
		else { i1 = 0; j1 = 1; }      // upper triangle, YX order: (0,0)->(0,1)->(1,1)
		// A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
		// a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
		// c = (3-sqrt(3))/6
		double x1 = x0 - i1 + G2; // Offsets for middle corner in (x,y) unskewed coords
		double y1 = y0 - j1 + G2;
		double x2 = x0 - 1.0 + 2.0 * G2; // Offsets for last corner in (x,y) unskewed coords
		double y2 = y0 - 1.0 + 2.0 * G2;
		// Work out the hashed gradient indices of the three simplex corners
		int ii = i & 255;
		int jj = j & 255;
		int gi0 = permMod12[ii + perm[jj]];
		int gi1 = permMod12[ii + i1 + perm[jj + j1]];
		int gi2 = permMod12[ii + 1 + perm[jj + 1]];
		// Calculate the contribution from the three corners
		double t0 = 0.5 - x0 * x0 - y0 * y0;
		if (t0 < 0) n0 = 0.0;
		else {
			t0 *= t0;
			n0 = t0 * t0 * dot(grad3[gi0], x0, y0);  // (x,y) of grad3 used for 2D gradient
		}
		double t1 = 0.5 - x1 * x1 - y1 * y1;
		if (t1 < 0) n1 = 0.0;
		else {
			t1 *= t1;
			n1 = t1 * t1 * dot(grad3[gi1], x1, y1);
		}
		double t2 = 0.5 - x2 * x2 - y2 * y2;
		if (t2 < 0) n2 = 0.0;
		else {
			t2 *= t2;
			n2 = t2 * t2 * dot(grad3[gi2], x2, y2);
		}
		// Add contributions from each corner to get the final noise value.
		// The result is scaled to return values in the interval [-1,1].
		return 70.0 * (n0 + n1 + n2);
	}

	std::vector<std::vector<float>> generateOctavedSimplexNoise(int width, int height, int octaves, float roughness, float scale, short seed) {
		std::vector<std::vector<float>> totalNoise;
		totalNoise.resize(width);
		for (auto& row : totalNoise)
			row.resize(height);

		// UTILISE THE SEED

		for (int i = 0; i < 256; i++) {
			p[i] = (short)((pbak[i] + seed) % 255);
		}

		// CREATE PERMUTATION TABLES AGAIN
		for (int i = 0; i < 512; i++)
		{
			perm[i] = p[i & 255];
			permMod12[i] = (short)(perm[i] % 12);
		}

		float layerFrequency = scale;
		float layerWeight = 1;
		float weightSum = 0;

		for (int octave = 0; octave < octaves; octave++) {
			//Calculate single layer/octave of simplex noise, then add it to total noise
			for (int x = 0; x < width; x++) {
				for (int y = 0; y < height; y++) {
					totalNoise[x][y] += (float)noise(x * layerFrequency, y * layerFrequency) * layerWeight;
				}
			}

			//Increase variables with each incrementing octave
			layerFrequency *= 2;
			weightSum += layerWeight;
			layerWeight *= roughness;

		}
		return totalNoise;
	}

	void normalizeArray(std::vector<std::vector<float>>& pNoise) {
		float min = 0;
		float max = 0;

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

		float divisor = max - min;

		for (size_t i = 0; i < pNoise.size(); i++) {
			for (size_t j = 0; j < pNoise[i].size(); j++) {

				pNoise[i][j] = (pNoise[i][j] - min) / divisor;

			}
		}
	}

	void flipVertically(std::vector<std::vector<float>>& pNoise) {
		std::vector<std::vector<float>> tempMap = pNoise;

		for (size_t x = 0; x < pNoise.size(); x++) {
			for (size_t y = 0; y < pNoise[x].size(); y++) {
				tempMap[x][y] = pNoise[x][pNoise[x].size() - y - 1];
			}
		}

		pNoise = tempMap;
	}

	void createEdgeMask(std::vector<std::vector<float>>& pNoise, float pEdgeFade) {

		float percentToGradient = pEdgeFade;
		// this is the percentage of the entire width/height from edge to fade in

		for (size_t i = 0; i < pNoise.size(); i++) {
			for (size_t j = 0; j < pNoise[i].size(); j++) {

				if (i < pNoise.size() / 2) { // left side of map
					float percentIn = (float)i / pNoise.size(); // calculate how far in the map we are
					if (percentIn < percentToGradient) {
						pNoise[i][j] *= (percentIn / percentToGradient);
					}
				}

				if (i > pNoise.size() / 2) { // right side of map
					float percentIn = 1 - (float)i / pNoise.size();
					if (percentIn < percentToGradient) {
						pNoise[i][j] *= (percentIn / percentToGradient);
					}
				}

				if (j < pNoise[i].size() / 2) { // top side of map
					float percentIn = (float)j / pNoise[i].size();
					if (percentIn < percentToGradient) {
						pNoise[i][j] *= (percentIn / percentToGradient);
					}
				}

				if (j > pNoise[i].size() / 2) { // top side of map
					float percentIn = 1 - (float)j / pNoise[i].size();
					if (percentIn < percentToGradient) {
						pNoise[i][j] *= (percentIn / percentToGradient);
					}
				}

			}
		}
	}
	void createRadialMask(std::vector<std::vector<float>>& pNoise) {
		float centerX = (float) pNoise.size() / 2;
		float centerY = (float) pNoise[0].size() / 2;

		float furthestDistance = (float)sqrt((centerX * centerX) + (centerY * centerY));

		for (size_t i = 0; i < pNoise.size(); i++) {
			for (size_t j = 0; j < pNoise[i].size(); j++) {

				//Simple squaring, you can use whatever math libraries are available to you to make this more readable
				//The cool thing about squaring is that it will always give you a positive distance! (-10 * -10 = 100)
				float distanceX = (centerX - i) * (centerX - i);
				float distanceY = (centerY - j) * (centerY - j);

				float distanceToCenter = (float)sqrt(distanceX + distanceY);

				//Make sure this value ends up as a float and not an integer
				//If you're not outputting this to an image, get the correct 1.0 white on the furthest edges by dividing by half the map size, in this case 64. You will get higher than 1.0 values, so clamp them!
				distanceToCenter = distanceToCenter / furthestDistance;

				pNoise[i][j] *= (1 - distanceToCenter);
			}
		}
	}

	std::vector<std::vector<float>> CreateArray( size_t pWidth, size_t pHeight, size_t pOctaves, float pRoughness, float pScale, short pSeed, bool pRadialEnabled, float pEdgeFade) {

		auto map = generateOctavedSimplexNoise(pWidth, pHeight, pOctaves, pRoughness, pScale, pSeed);

		normalizeArray(map);
		if (pRadialEnabled) createRadialMask(map);
		createEdgeMask(map, pEdgeFade);
		flipVertically(map);

		return map;
	}
};
