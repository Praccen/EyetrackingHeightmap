#include "Intersections.h"

Intersections::Intersections() {

}

Intersections::~Intersections() {

}

float Intersections::rayTriTest(glm::vec3 rayStart, glm::vec3 rayVec, glm::vec3 triangleCorners[3]) {
	float returnValue = -1.0f;
	rayVec = glm::normalize(rayVec); //Normalize ray direction vec just to be sure

	//Calculate triangle edges
	glm::vec3 edge0 = triangleCorners[1] - triangleCorners[0];
	glm::vec3 edge1 = triangleCorners[2] - triangleCorners[0];

	//Determines s to use in cramer's rule
	glm::vec3 cramersS = rayStart - triangleCorners[0];

	glm::vec4 tuvw;

	rayVec *= -1.0f;

	if (glm::determinant(glm::mat3(rayVec, edge0, edge1)) != 0) { //Makes sure no division by 0
		glm::vec3 detVec(glm::determinant(glm::mat3(cramersS, edge0, edge1)), glm::determinant(glm::mat3(rayVec, cramersS, edge1)), glm::determinant(glm::mat3(rayVec, edge0, cramersS))); //Vector containing determinant of the three matrixes
		glm::vec3 tuv = detVec * (1 / glm::determinant(glm::mat3(rayVec, edge0, edge1)));
		tuvw = glm::vec4(tuv.x, tuv.y, tuv.z, 1 - tuv.y - tuv.z);
	}

	if (tuvw.x >= 0.0f &&
		tuvw.y >= 0.0f && tuvw.y <= 1.0f &&
		tuvw.z >= 0.0f && tuvw.z <= 1.0f &&
		tuvw.w >= 0.0f && tuvw.w <= 1.0f) { //Hit if t is not negative and if u, v, and w are all between 0 and 1)
		returnValue = tuvw.x;
	}

	return returnValue;
}

bool Intersections::triTriSupport(glm::vec3 U[3], glm::vec3 V[3], glm::vec3 segment[2])
{
	// Compute the plane normal for triangle U.
	glm::vec3 edge1 = U[1] - U[0];
	glm::vec3 edge2 = U[2] - U[0];
	glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

	// Test whether the edges of triangle V transversely intersect the
	// plane of triangle U.
	float d[3];
	int positive = 0, negative = 0, zero = 0;
	for (int i = 0; i < 3; ++i)
	{
		d[i] = glm::dot(normal, V[i] - U[0]);
		if (d[i] > 0.0f)
		{
			++positive;
		}
		else if (d[i] < 0.0f)
		{
			++negative;
		}
		else
		{
			++zero;
		}
	}
	// positive + negative + zero == 3

	if (positive > 0 && negative > 0)
	{
		if (positive == 2)  // and negative == 1
		{
			if (d[0] < 0.0f)
			{
				segment[0] = (d[1] * V[0] - d[0] * V[1]) / (d[1] - d[0]);
				segment[1] = (d[2] * V[0] - d[0] * V[2]) / (d[2] - d[0]);
			}
			else if (d[1] < 0.0f)
			{
				segment[0] = (d[0] * V[1] - d[1] * V[0]) / (d[0] - d[1]);
				segment[1] = (d[2] * V[1] - d[1] * V[2]) / (d[2] - d[1]);
			}
			else  // d[2] < 0.0f
			{
				segment[0] = (d[0] * V[2] - d[2] * V[0]) / (d[0] - d[2]);
				segment[1] = (d[1] * V[2] - d[2] * V[1]) / (d[1] - d[2]);
			}
		}
		else if (negative == 2)  // and positive == 1
		{
			if (d[0] > 0.0f)
			{
				segment[0] = (d[1] * V[0] - d[0] * V[1]) / (d[1] - d[0]);
				segment[1] = (d[2] * V[0] - d[0] * V[2]) / (d[2] - d[0]);
			}
			else if (d[1] > 0.0f)
			{
				segment[0] = (d[0] * V[1] - d[1] * V[0]) / (d[0] - d[1]);
				segment[1] = (d[2] * V[1] - d[1] * V[2]) / (d[2] - d[1]);
			}
			else  // d[2] > 0.0f
			{
				segment[0] = (d[0] * V[2] - d[2] * V[0]) / (d[0] - d[2]);
				segment[1] = (d[1] * V[2] - d[2] * V[1]) / (d[1] - d[2]);
			}
		}
		else  // positive == 1, negative == 1, zero == 1
		{
			if (d[0] == 0.0f)
			{
				segment[0] = V[0];
				segment[1] = (d[2] * V[1] - d[1] * V[2]) / (d[2] - d[1]);
			}
			else if (d[1] == 0.0f)
			{
				segment[0] = V[1];
				segment[1] = (d[0] * V[2] - d[2] * V[0]) / (d[0] - d[2]);
			}
			else  // d[2] == 0.0f
			{
				segment[0] = V[2];
				segment[1] = (d[1] * V[0] - d[0] * V[1]) / (d[1] - d[0]);
			}
		}
		return true;
	}

	// Triangle V does not transversely intersect triangle U, although it is
	// possible a vertex or edge of V is just touching U.  In this case, we
	// do not call this an intersection.
	return false;
}

bool Intersections::triTriTest(glm::vec3 U[3], glm::vec3 V[3]) {
	glm::vec3 S0[2], S1[2];
	if (triTriSupport(V, U, S0) && triTriSupport(U, V, S1))
	{
		// Theoretically, the segments lie on the same line.  A direction D
		// of the line is the Cross(NormalOf(U),NormalOf(V)).  We choose the
		// average A of the segment endpoints as the line origin.
		glm::vec3 uNormal = glm::cross(U[1] - U[0], U[2] - U[0]);
		glm::vec3 vNormal = glm::cross(V[1] - V[0], V[2] - V[0]);
		glm::vec3 D = glm::normalize(glm::cross(uNormal, vNormal));
		glm::vec3 A = 0.25f*(S0[0] + S0[1] + S1[0] + S1[1]);

		// Each segment endpoint is of the form A + t*D.  Compute the
		// t-values to obtain I0 = [t0min,t0max] for S0 and I1 = [t1min,t1max]
		// for S1.  The segments intersect when I0 overlaps I1.  Although this
		// application acts as a "test intersection" query, in fact the
		// construction here is a "find intersection" query.
		float t00 = glm::dot(D, S0[0] - A), t01 = glm::dot(D, S0[1] - A);
		float t10 = glm::dot(D, S1[0] - A), t11 = glm::dot(D, S1[1] - A);
		auto I0 = std::minmax(t00, t01);
		auto I1 = std::minmax(t10, t11);
		return (I0.second > I1.first && I0.first < I1.second);
	}
	return false;
}