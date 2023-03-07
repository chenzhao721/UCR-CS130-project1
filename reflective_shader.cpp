#include "reflective_shader.h"
#include "ray.h"
#include "render_world.h"

vec3 Reflective_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    vec3 color;
    Ray reflectiveRay;
    vec3 viewray;
    vec3 reflectdir;
    //TODO; // determine the color
    color = shader->Shade_Surface(ray,intersection_point,normal,recursion_depth);
    if(recursion_depth==world.recursion_depth_limit){
        return color;
    }
    viewray = (intersection_point-ray.endpoint).normalized();
    reflectdir = viewray - 2.0*normal.normalized()*dot(normal.normalized(),viewray);
    reflectiveRay.endpoint = intersection_point;
    reflectiveRay.direction = reflectdir;
    color = (1-reflectivity)*color + reflectivity*world.Cast_Ray(reflectiveRay,recursion_depth+1);
    return color;
}
