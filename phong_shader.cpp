#include "light.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"
#include "object.h"

#include <cmath>

vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 L;
    vec3 R;

    // Should first use world's ambient color and intensity first
    // to determine the ambient reflection

    ambient = color_ambient * world.ambient_color * world.ambient_intensity;

    //iterate through the number of lights in the world
    for (unsigned i = 0; i < world.lights.size(); ++i) {
        L = world.lights[i]->position - intersection_point;
        //equation for R is taken from the textbook
        R = (-L + 2 * dot(L, normal) * normal).normalized();

        diffuse += color_diffuse * world.lights[i]->Emitted_Light(L)
                    * std::max(dot(normal, L.normalized()), 0.0);

        // Not neccessary for Checkpoint 1, but L and R are computed and formula
        // is given in the lecture slides
        specular += color_specular * world.lights[i]->Emitted_Light(L)
                    * std::pow(std::max(dot(R, -(ray.direction)), 0.0), specular_power);

    }

    return ambient + diffuse + specular;
}
