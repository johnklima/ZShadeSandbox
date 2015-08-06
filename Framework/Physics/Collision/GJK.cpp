#include "GJK.h"
//==============================================================================================================================
//==============================================================================================================================
GJK::GJK(EngineOptions* eo)
:  m_EngineOptions(eo)
{
}
//==============================================================================================================================
bool GJK::Intersect(PhysicsBody3D* body1, PhysicsBody3D* body2)
{
	// If there is no mesh to check then there is no collision
	if (!body1) return false;
	if (!body2) return false;
	
	// This assumes that the convex hull has been created for each mesh
	if (!body1->IsHullCreated()) return false;
	if (!body2->IsHullCreated()) return false;
	
	// Make sure the simplex starts out fresh
	m_Simplex.clear();
	
	/*
	Actually, I’m really glad you asked this because the center for shape1 was wrong. I fixed it and thankfully the
	  flow of the post remains unchanged.
	Finding the center of a 2D polygon can be done a number of ways. In this post I just picked a point that looked
	  like the center to make the math easier. Typically you would do this using some algorithm.
	In dyn4j there are two algorithms used to calculate the center:
	  1. Average Method – Add all the x values of the vertices and divide by the number of them; likewise with the y values. See here
	  2. Area Weighted Method – See here
	*/
	// Get the direction from the vector from the center of shape1 to the center of shape2
	// d = c2 - c1 = (9, 5) - (5.5, 8.5) = (3.5, -3.5) = (1, -1);
	ZShadeSandboxMath::Vec3<XMFLOAT3> mesh1Center(body1->Center());
	ZShadeSandboxMath::Vec3<XMFLOAT3> mesh2Center(body2->Center());
	ZShadeSandboxMath::Vec3<XMFLOAT3> direction = mesh2Center - mesh1Center;
	
	mesh1Points = body1->GetPoints();
	mesh2Points = body2->GetPoints();
	
	ZShadeSandboxMath::Vec3<XMFLOAT3> point = Support(direction);
	
	m_Simplex.Add(point);
	
	// Set the search direction to be opposite the point
	direction = point.Negate(false);
	
	// Begin the Collision check, If something goes wrong we can bail out after so many iterations and not have an infinite loop
	uint32 iterations = 0;
	for (; iterations < 1000; iterations++)
	{
		point = Support(direction);
		
		// termination condition if V is no more extremal in direction -P than P itself
		//if( (V* (-1*P.Normalized() ) <= (P.Magnitude()*-1) + EPSALON))
		//if( dot(v - p, -p) <= epsilon * max )
		//	return false;
		// If the dot product is less than 0, than the angle is 90 degrees and lies outside the region
		if (point.Dot(direction) <= 0)
			break;
		else
		{
			m_Simplex.Add(point);
			
			if (CheckSimplex(direction))
			{
				// The collision has been detected, calculate the contact manifold for contact resolution
				EPA(body1, body2);
				
				return true;
			}
		}
	}
	
	return false;
}
//==============================================================================================================================
bool GJK::CheckFace(ZShadeSandboxMath::Vec3<XMFLOAT3>& direction)
{
	ZShadeSandboxMath::Vec3<XMFLOAT3> A = m_Simplex.A;
	ZShadeSandboxMath::Vec3<XMFLOAT3> B = m_Simplex.B;
	ZShadeSandboxMath::Vec3<XMFLOAT3> C = m_Simplex.C;
	ZShadeSandboxMath::Vec3<XMFLOAT3> D = m_Simplex.D;
	
	// compute AO (same thing as -A)
	ZShadeSandboxMath::Vec3<XMFLOAT3> AO = A.Negate(false);
	
	// compute the edges, AB, AC and AD
	ZShadeSandboxMath::Vec3<XMFLOAT3> AB = B - A;
	ZShadeSandboxMath::Vec3<XMFLOAT3> AC = C - A;
	ZShadeSandboxMath::Vec3<XMFLOAT3> AD = D - A;
	
	// Compute the triangles normals
	ZShadeSandboxMath::Vec3<XMFLOAT3> perpABC = AB.Cross(AC);
	
	// compute the normals
	ZShadeSandboxMath::Vec3<XMFLOAT3> perpACD = AC.Cross(AD);
	ZShadeSandboxMath::Vec3<XMFLOAT3> perpADB = AD.Cross(AB);
	ZShadeSandboxMath::Vec3<XMFLOAT3> perpABP = AB.Cross(perpABC);
	
	// The simplex is equal to the triangle that the origin is in front of
	// this is exactly the same as the triangular simplex test except that we know
	// that the origin is not behind the triangle.

	if (perpABP.Dot( AO ) > 0)
	{
		m_Simplex.Set(m_Simplex.A, m_Simplex.B);
		
		direction = AB.Cross( AO );
		
		return false;
	}
	
	ZShadeSandboxMath::Vec3<XMFLOAT3> perpACP = AC.Cross(perpABC);
	
	if (perpACP.Dot( AO ) > 0)
	{
		m_Simplex.Set(m_Simplex.A, m_Simplex.C);
		
		direction = AC.Cross( AO );
		
		return false;
	}
	
	m_Simplex.Set(m_Simplex.A, m_Simplex.B, m_Simplex.C);
	
	direction = perpABC;
	
	return false;
}
//==============================================================================================================================
bool GJK::CheckSimplex(ZShadeSandboxMath::Vec3<XMFLOAT3>& direction)
{
	//Google: does a 3D GJK need more than 3 points in the simplex
	//http://vec3.ca/gjk/implementation/
	
	// We only have a single point so do nothing
	if (m_Simplex.num <= 1) return false;
	
	ZShadeSandboxMath::Vec3<XMFLOAT3> A = m_Simplex.A;
	
	// compute AO (same thing as -A)
	ZShadeSandboxMath::Vec3<XMFLOAT3> AO = A.Negate(false);
	
	// This is a line segment
	if (m_Simplex.num == 2)
	{
		ZShadeSandboxMath::Vec3<XMFLOAT3> B = m_Simplex.B;

		ZShadeSandboxMath::Vec3<XMFLOAT3> AB = B - A;

		// Check a line segment
		// In 3D, Triple Cross product from the line AB in the direction of the origin
		ZShadeSandboxMath::Vec3<XMFLOAT3> perpAB = AB.Cross(AO, AB);

		// Set the direction to the perpAB
		direction = perpAB;
	}
	else if (m_Simplex.num == 3)// It's a triangle
	{
		// Check triangle case

		ZShadeSandboxMath::Vec3<XMFLOAT3> B = m_Simplex.B;
		ZShadeSandboxMath::Vec3<XMFLOAT3> C = m_Simplex.C;

		// compute the edges, AB and BC
		ZShadeSandboxMath::Vec3<XMFLOAT3> AB = B - A;
		ZShadeSandboxMath::Vec3<XMFLOAT3> AC = C - A;

		// compute the normals
		ZShadeSandboxMath::Vec3<XMFLOAT3> perpAB = AC.Cross(AB, AB);
		ZShadeSandboxMath::Vec3<XMFLOAT3> perpAC = AB.Cross(AC, AC);

		if (perpAB.Dot(AO) > 0)
		{
			// The origin lies outside the triangle near the edge AB
			// Reset the simplex to the line AB and continue
			// search direction is perpendicular to AB and parallel to AO

			//m_Simplex.RemovePoint(C);
			m_Simplex.Set(m_Simplex.A, m_Simplex.B);

			direction = perpAB;
		}
		else
		{
			if (perpAC.Dot(AO) > 0)
			{
				// The origin lies outside the triangle near the edge AC
				// Reset the simplex to the line AC and continue
				// search direction is perpendicular to AC and parallel to AO

				//m_Simplex.RemovePoint(B);
				m_Simplex.Set(m_Simplex.A, m_Simplex.C);

				direction = perpAC;
			}
			else
			{
				// Check the engine options to see if we are in 2D or 3D
				if (m_EngineOptions->m_DimType == DimType::ZSHADE_3D)
				{
					// Origin is within the triangular prism defined by the triangle
					// check above or below the triangle

					// Compute the triangles normals
					ZShadeSandboxMath::Vec3<XMFLOAT3> perpABC = AB.Cross(AC);

					if (perpABC.Dot(AO) > 0)
					{
						// The origin is above the triangle, so the simplex is not modified,
						// the search direction is the triangle's face normal
						direction = perpABC;
					}
					else
					{
						// Origin is below the triangle, so the simplex is rewound the oppisite direction
						// the search direction is the new triangle's face normal
						m_Simplex.Set(m_Simplex.A, m_Simplex.C, m_Simplex.B);

						direction = perpABC.Negate(false);
					}
				}
				else// We are in 2D so collision is found
				{
					return true;
				}
			}
		}
	}
	else if (m_Simplex.num == 4)// It's a tetrahedron
	{
		// Need to check if the origin is contained above or below the triangle
		
		ZShadeSandboxMath::Vec3<XMFLOAT3> B = m_Simplex.B;
		ZShadeSandboxMath::Vec3<XMFLOAT3> C = m_Simplex.C;
		ZShadeSandboxMath::Vec3<XMFLOAT3> D = m_Simplex.D;
		
		// compute the edges, AB, AC and AD
		ZShadeSandboxMath::Vec3<XMFLOAT3> AB = B - A;
		ZShadeSandboxMath::Vec3<XMFLOAT3> AC = C - A;
		ZShadeSandboxMath::Vec3<XMFLOAT3> AD = D - A;
		
		// Compute the triangles normals
		ZShadeSandboxMath::Vec3<XMFLOAT3> perpABC = AB.Cross(AC);
		
		// compute the normals
		ZShadeSandboxMath::Vec3<XMFLOAT3> perpACD = AC.Cross(AD);
		ZShadeSandboxMath::Vec3<XMFLOAT3> perpADB = AD.Cross(AB);
		
		if (perpABC.Dot(AO) > 0)
		{
			// Origin is in Front of triangle ABC, simplex is already what it needs to be
			return CheckFace(direction);
		}
		
		if (perpACD.Dot(AO) > 0)
		{
			// Origin is in Front of triangle ACD, simplex is set to this triangle
			m_Simplex.Set(m_Simplex.A, m_Simplex.C, m_Simplex.D);

			// Origin is in Front of triangle ACD
			return CheckFace(direction);
		}
		
		if (perpADB.Dot(AO) > 0)
		{
			// Origin is in Front of triangle ABD, simplex is set to this triangle
			m_Simplex.Set(m_Simplex.A, m_Simplex.B, m_Simplex.D);
			
			// Origin is in Front of triangle ABD
			return CheckFace(direction);
		}
		
		// behind all three faces, the origin is in the tetrahedron, we're done
		// Collision Found
		return true;
	}
	
	// Nothing passed
	return false;
}
//==============================================================================================================================
ZShadeSandboxMath::Vec3<XMFLOAT3> GJK::FindMax(std::vector< ZShadeSandboxMath::Vec3<XMFLOAT3> > meshPoints, ZShadeSandboxMath::Vec3<XMFLOAT3> direction)
{
	ZShadeSandboxMath::Vec3<XMFLOAT3> point;

	float new_dist = 0;
	float dist = 0;

	ZShadeSandboxMath::Vec3<XMFLOAT3> p = meshPoints[0];
	new_dist = p.Magnitude(direction);

	for (uint32 i = 1; i < meshPoints.size(); i++)
	{
		p = meshPoints[i];

		dist = p.Magnitude(direction);

		if (dist > new_dist)
		{
			point = p;
			new_dist = dist;
		}
	}

	return point;
}
//==============================================================================================================================
ZShadeSandboxMath::Vec3<XMFLOAT3> GJK::Support(ZShadeSandboxMath::Vec3<XMFLOAT3> direction)
{
	/*
	Dealing with Rotations

	One snag you will run into when generating support points is how to deal with a shape that has a
	rotation associated with it, the solution is to rotate the direction by the inverse of the objects
	rotation before generating the point, then you can just transform the point by the object’s transform
	matrix to rotate and translate it where it’s supposed to be.
	
	// Support mapping for an OBB
	const HDXQuaternion &trans_rot = body->getComp()->getTrans()->getOrientation();
	HDXVector3 rd = HDXVector3::gen::transformQuaternion(d,trans_rot.getInverse()).getNormal();
	HDXVector3 ret = HDXVector3(SIGN(rd.x)*halfsize.x,
                            SIGN(rd.y)*halfsize.y,
                            SIGN(rd.z)*halfsize.z) * getTransform();
	return ret;
	*/

	ZShadeSandboxMath::Vec3<XMFLOAT3> furthestPointA = FindMax(mesh1Points, direction);
	ZShadeSandboxMath::Vec3<XMFLOAT3> furthestPointB = FindMax(mesh2Points, direction.Negate(false));
	
	// Calculate the minkowski point
	ZShadeSandboxMath::Vec3<XMFLOAT3> minkowskiPoint = furthestPointA - furthestPointB;
	
	return minkowskiPoint;
}
//==============================================================================================================================
Edge GJK::FindClosestEdge2D()
{
	Edge closest;

	closest.distance = FLT_MAX;

	ZShadeSandboxMath::Vec3<XMFLOAT3> A = m_Simplex.A;
	ZShadeSandboxMath::Vec3<XMFLOAT3> B = m_Simplex.B;
	ZShadeSandboxMath::Vec3<XMFLOAT3> C = m_Simplex.C;

	CheckEdgeDistance(A, B, 1, closest); // 1
	CheckEdgeDistance(A, C, 2, closest); // 2
	CheckEdgeDistance(B, C, 0, closest); // 0

	return closest;
}
//==============================================================================================================================
Edge GJK::FindClosestEdge(ZShadeSandboxMath::TrianglePrimitive tri)
{
	Edge closest;

	closest.distance = FLT_MAX;

	ZShadeSandboxMath::Vec3<XMFLOAT3> A = tri.Point1();
	ZShadeSandboxMath::Vec3<XMFLOAT3> B = tri.Point2();
	ZShadeSandboxMath::Vec3<XMFLOAT3> C = tri.Point3();

	CheckEdgeDistance(A, B, 1, closest); // 1
	CheckEdgeDistance(A, C, 2, closest); // 2
	CheckEdgeDistance(B, C, 0, closest); // 0

	return closest;
}
//==============================================================================================================================
void GJK::CheckEdgeDistance(ZShadeSandboxMath::Vec3<XMFLOAT3> A, ZShadeSandboxMath::Vec3<XMFLOAT3> B, int j, Edge& closest)
{
	// Create the edge vector
	ZShadeSandboxMath::Vec3<XMFLOAT3> E = B - A;

	// Vector from the origin to a
	ZShadeSandboxMath::Vec3<XMFLOAT3> OA = A;

	// Get the vector from the face normal towards the origin
	ZShadeSandboxMath::Vec3<XMFLOAT3> N = E.Cross(OA, E);

	// Normalize the vector
	N.Normalize();

	// Calculate the distance from the origin to the edge
	float d = N.Dot(A);

	// Check the distance against the other distances
	if (d > closest.distance)
	{
		// If this edge is closer then use it
		closest.distance = d;
		closest.normal = N;
		closest.index = j;
	}
}
//==============================================================================================================================
float GJK::find_distance(ZShadeSandboxMath::Vec3<XMFLOAT3> point, ZShadeSandboxMath::Vec3<XMFLOAT3> faceNormal)
{
	// Vector from the origin to a
	ZShadeSandboxMath::Vec3<XMFLOAT3> OA = point;
	
	// Get the vector from the face normal towards the origin
	ZShadeSandboxMath::Vec3<XMFLOAT3> N = faceNormal.Cross(OA, faceNormal);
	
	// Normalize the vector
	N.Normalize();
	
	// Calculate the distance from the origin to the face normal
	return N.Dot(point);
}
//==============================================================================================================================
ZShadeSandboxMath::TrianglePrimitive GJK::FindClosestTriangle()
{
	ZShadeSandboxMath::TrianglePrimitive triangle;
	
	ZShadeSandboxMath::TrianglePrimitive tri = m_EPASimplex3D[0];
	ZShadeSandboxMath::PlanePrimitive new_plane(tri.Point1(), tri.Point2(), tri.Point3());
	
	// Calculate the distance of the face normal to the origin
	float new_dist = find_distance(tri.Point1(), new_plane.Normal());
	
	// Find the Face that is closer to the origin
	for (int i = 1; i < m_EPASimplex3D.tri_simplex.size(); i++)
	{
		tri = m_EPASimplex3D[i];
		ZShadeSandboxMath::PlanePrimitive plane(tri.Point1(), tri.Point2(), tri.Point3());
		
		// Calculate the distance of the face normal to the origin
		float dist = find_distance(tri.Point1(), plane.Normal());
		
		// Check the distance against the other distances
		if (dist > new_dist)
		{
			// If this face is closer then use it
			new_dist = dist;
			new_plane = plane;
			triangle = tri;
		}
	}
	
	return triangle;
}
//==============================================================================================================================
void GJK::EPA(PhysicsBody3D* body1, PhysicsBody3D* body2)
{
	// Google: expanding polytope algorithm 3d
	// http://www.codezealot.org/archives/180
	
	// Finding Contact Points
	// http://www.codezealot.org/archives/394
	
	if (m_EngineOptions->m_DimType == DimType::ZSHADE_3D)
	{
		// Projects the origin onto each triangle of the simplex.
		// Point normal called v.
		// Finds the triangle whose v is closest to the origin, and
		// calls the support function using v as the direction
		// The triangle is subdivided, and new triangles are created using
		// the result of the support function as a new vertex.
		// This is repeated over and over again.
		// Once the distance between v and the support point is less than
		// a certain threshold, we have found the penetration depth, and
		// can determine the contact point and normal.
		
		m_EPASimplex3D.clear();

		// Add the tetrahedron simplex that contains the origin
		m_EPASimplex3D.AddTriangle( m_Simplex.A, m_Simplex.B, m_Simplex.C );
		m_EPASimplex3D.AddTriangle( m_Simplex.A, m_Simplex.B, m_Simplex.D );
		m_EPASimplex3D.AddTriangle( m_Simplex.B, m_Simplex.C, m_Simplex.D );
		m_EPASimplex3D.AddTriangle( m_Simplex.A, m_Simplex.C, m_Simplex.D );
		
		// Just in case something goes wrong and we have an infinite loop
		uint32 iterations = 0;
		for (; iterations < 10000; iterations++)
		{
			// Obtain the feature (triangle for 3D) closest to the origin on the
			// Minkowski Difference
			ZShadeSandboxMath::TrianglePrimitive closestTriangle = FindClosestTriangle();
			
			// Find the closest edge to the origin on the triangle
			Edge e = FindClosestEdge(closestTriangle);
			
			// Obtain a new support point in the direction of the edge normal
			ZShadeSandboxMath::Vec3<XMFLOAT3> p = Support(e.normal);
			
			// Check the distance from the origin to the edge against the
			// distance p is along e.normal
			float d = p.Dot(e.normal);
			
			// If the distance is less than the tolerance than we can assume
			// that we cannot expand the simplex any further and we
			// have our collision information.
			if (d - e.distance < TOLERANCE)
			{
				m_mtv = new GJK_MTV();
				m_mtv->normal = e.normal;
				m_mtv->penetration = e.distance;
				
				break;
			}
			else
			{
				// We haven't reached the edge of the Minkowski Difference
				// so continue expanding by adding the new points to the
				// simplex subdividing the triangle.
				
				// Compute auxiliary support points for the edges of the closest triangle
				ZShadeSandboxMath::Vec3<XMFLOAT3> p1(closestTriangle.Point1());
				ZShadeSandboxMath::Vec3<XMFLOAT3> p2(closestTriangle.Point2());
				ZShadeSandboxMath::Vec3<XMFLOAT3> p3(closestTriangle.Point3());

				ZShadeSandboxMath::Vec3<XMFLOAT3> AB = p2 - p1;
				ZShadeSandboxMath::Vec3<XMFLOAT3> AC = p3 - p2;
				ZShadeSandboxMath::Vec3<XMFLOAT3> CB = p3 - p1;
				
				// Add six new triangles based on the new subdivision
				m_EPASimplex3D.AddTriangle(AC, closestTriangle.Point2(), p);
				m_EPASimplex3D.AddTriangle(AC, closestTriangle.Point3(), p);
				m_EPASimplex3D.AddTriangle(CB, closestTriangle.Point3(), p);
				m_EPASimplex3D.AddTriangle(CB, closestTriangle.Point1(), p);
				m_EPASimplex3D.AddTriangle(AB, closestTriangle.Point2(), p);
				m_EPASimplex3D.AddTriangle(AB, closestTriangle.Point1(), p);
			}
		}
	}
	else// 2D
	{
		// Just in case something goes wrong and we have an infinite loop
		uint32 iterations = 0;
		for (; iterations < 10000; iterations++)
		{
			// Obtain the feature (edge for 2D) closest to the origin on the
			// Minkowski Difference
			Edge e = FindClosestEdge2D();

			// Obtain a new support point in the direction of the edge normal
			ZShadeSandboxMath::Vec3<XMFLOAT3> p = Support(e.normal);

			// Check the distance from the origin to the edge against the
			// distance p is along e.normal
			float d = p.Dot(e.normal);

			// If the distance is less than the tolerance than we can assume
			// that we cannot expand the simplex any further and we
			// have our collision information.
			if (d - e.distance < TOLERANCE)
			{
				m_mtv = new GJK_MTV();
				m_mtv->normal = e.normal;
				m_mtv->penetration = e.distance;
				
				break;
			}
			else
			{
				// We haven't reached the edge of the Minkowski Difference
				// so continue expanding by adding the new point to the
				// simplex in between the points that made the closest edge.
				if (e.index == 0) m_Simplex.A = p;
				if (e.index == 1) m_Simplex.B = p;
				if (e.index == 2) m_Simplex.C = p;
			}
		}
	}
}
//==============================================================================================================================