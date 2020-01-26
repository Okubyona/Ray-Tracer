#include "render_world.h"
#include "flat_shader.h"
#include "object.h"
#include "light.h"
#include "ray.h"

#include <limits>
// implemented for setting min_t to a large value

extern bool disable_hierarchy;

Render_World::Render_World()
    :background_shader(0),ambient_intensity(0),enable_shadows(true),
    recursion_depth_limit(3)
{}

Render_World::~Render_World()
{
    delete background_shader;
    for(size_t i=0;i<objects.size();i++) delete objects[i];
    for(size_t i=0;i<lights.size();i++) delete lights[i];
}

// Find and return the Hit structure for the closest intersection.  Be careful
// to ensure that hit.dist>=small_t.
Hit Render_World::Closest_Intersection(const Ray& ray)
{
    //initialize empty hit
    Hit closestInt = {NULL, 0, 0};
    double min_t = std::numeric_limits<double>::max();

    // loop checks each Object inside of Render_World's object vector
    for (unsigned i = 0; i < objects.size(); ++i) {

        // Intersection function operation explained in object.h
        Hit intersect = objects[i]->Intersection(ray, -13);

        // if intersect has an object, and its distance is >= small_t and
        // distance is less than min_t
        if ((intersect.object && intersect.dist >= small_t) && intersect.dist < min_t) {
            closestInt = intersect;
        }
    }

    return closestInt;
}

// set up the initial view ray and call
void Render_World::Render_Pixel(const ivec2& pixel_index)
{
    vec3 viewRay = camera.World_Position(pixel_index) - camera.position;

    //uses constructor to assign values to ray, instead of directly doing so
    Ray ray(camera.position, viewRay);

    vec3 color=Cast_Ray(ray,1);
    camera.Set_Pixel(pixel_index,Pixel_Color(color));
}

void Render_World::Render()
{
    if(!disable_hierarchy)
        Initialize_Hierarchy();

    for(int j=0;j<camera.number_pixels[1];j++)
        for(int i=0;i<camera.number_pixels[0];i++)
            Render_Pixel(ivec2(i,j));
}

// cast ray and return the color of the closest intersected surface point,
// or the background color if there is no object intersection
vec3 Render_World::Cast_Ray(const Ray& ray,int recursion_depth)
{
    vec3 color;
    vec3 intersectPoint;
    Hit closestInt;

    closestInt = Closest_Intersection(ray);
    intersectPoint = ray.Point(closestInt.dist);

    if (closestInt.object) {
        color = closestInt.object->material_shader->Shade_Surface(ray,
            intersectPoint, closestInt.object->Normal(intersectPoint, -13), recursion_depth);
    }

    else {
        //background shader, can use any 3d shader as a parameter
        color = background_shader->Shade_Surface(ray, color, color, 0);
    }
    return color;
}

void Render_World::Initialize_Hierarchy()
{
    TODO; // Fill in hierarchy.entries; there should be one entry for
    // each part of each object.

    hierarchy.Reorder_Entries();
    hierarchy.Build_Tree();
}
