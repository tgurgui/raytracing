#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"
#include "vec3.h"

class triangle : public hittable
{
public:
	triangle(const point3& p_1, const point3& p_2, const point3& p_3, shared_ptr<material> material):
		p1(p_1), p2(p_2), p3(p_3), mat(material) {}
		
	bool hit(const ray& r, interval ray_t, hit_record& rec) const override 
	{
		// Check if intersection
		const vec3 p2p1 = p2 - p1;
		const vec3 p3p1 = p3 - p1;
		const vec3 normal = cross(p2p1, p3p1);
		const double det = -dot(r.direction(), normal);
		const double invdet = 1.0 / det;		
		const vec3 AO = r.origin() - p1;
		const vec3 DAO = cross(AO, r.direction());
		
		// Compute baricentric coordinates
		const double u =  dot(p3p1,DAO) * invdet;
   		const double v = -dot(p2p1,DAO) * invdet;
		const double t =  dot(AO,normal)  * invdet;


		if(det >= 1e-6 && ray_t.surrounds(t) && u >= 0.0 && v >= 0.0 && (u+v) <= 1.0)
		{
			rec.t = t;
			rec.set_face_normal(r, normal);
			rec.p = r.at(rec.t);
			rec.mat = mat;
			
			return true;
		}
		else
		{
			return false;
		}

		

		return true;
	}

private:
	point3 p1, p2, p3;
	shared_ptr<material> mat;
};

#endif
