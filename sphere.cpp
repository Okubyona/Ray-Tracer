#include "sphere.h"
#include "ray.h"

#include <cmath>

// Determine if the ray intersects with the sphere
Hit Sphere::Intersection(const Ray& ray, int part) const
{
    // this fuction is essentially just solving the quadratic equation.
    Hit intersection; //declare empty hit
    intersection.object = NULL;
    intersection.part = -1;

    // values for a,b,c determined from equation in the textbook
    double a;
    double b;
    double c;
    double descriminant;
    double t_1; // solution 1
    double t_2; // soultion 2

    a = ray.direction.magnitude_squared();
    b = 2 * dot(ray.direction, (ray.endpoint - center));
    c = (ray.endpoint - center).magnitude_squared() - (radius * radius);
    descriminant = (b * b) - (4 * a * c);

    if (descriminant < 0) {
        // ray does not intersect sphere, return null hit
        return intersection;
    }

    t_1 = (-b + sqrt(descriminant)) / (2 * a);
    t_2 = (-b - sqrt(descriminant)) / (2 * a);

    //now we determine which t is closest and higher than threshold small_t
    if (t_1 < t_2 && t_1 >= small_t) {
        intersection.dist = t_1;
        intersection.object = this;

    }

    else if (t_2 < t_1 && t_2 >= small_t) {
        intersection.dist = t_2;
        intersection.object = this;

    }

    return intersection;
}

vec3 Sphere::Normal(const vec3& point, int part) const
{
    vec3 normal;
    normal = point - center;
    return normal.normalized();
}

Box Sphere::Bounding_Box(int part) const
{
    Box box;
    TODO; // calculate bounding box
    return box;
}
