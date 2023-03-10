#include "render_world.h"
#include "flat_shader.h"
#include "object.h"
#include "light.h"
#include "ray.h"

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
    //TODO;
    double min_t = std::numeric_limits<double>::max();
    Hit closestHit;
    closestHit.dist = min_t;
    Hit objectHit;
    for(size_t i=0;i<objects.size();i++){
        for(size_t j=0; j<objects[i]->number_parts;j++){
            objectHit = objects[i]->Intersection(ray,j);
            if(objectHit.dist < min_t and objectHit.dist > small_t){
                min_t = objectHit.dist;
                closestHit = objectHit;
                closestHit.object = objects[i];
             }
        }
    }        
    //std::cout<< closestHit.dist <<std::endl;
    return closestHit;
}

// set up the initial view ray and call
void Render_World::Render_Pixel(const ivec2& pixel_index)
{
    //TODO; // set up the initial view ray here
    Ray ray;
    //TODO;
    ray.endpoint = camera.position;
    ray.direction = (camera.World_Position(pixel_index)-camera.position).normalized();
    //end;
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
    //TODO; // determine the color here
    vec3 hitPoint;
    vec3 normal;
    Hit rayHit = Closest_Intersection(ray);
    if(rayHit.dist!=std::numeric_limits<double>::max()){
        hitPoint = ray.Point(rayHit.dist);
        normal = (rayHit.object)->Normal(hitPoint,rayHit.part);
        color=(rayHit.object)->material_shader->Shade_Surface(ray,hitPoint,normal,recursion_depth);
    }
    else{
        color = background_shader->Shade_Surface(ray,hitPoint,normal,recursion_depth);
    }   
    //end
    return color;
}

void Render_World::Initialize_Hierarchy()
{
    TODO; // Fill in hierarchy.entries; there should be one entry for
    // each part of each object.

    hierarchy.Reorder_Entries();
    hierarchy.Build_Tree();
}
