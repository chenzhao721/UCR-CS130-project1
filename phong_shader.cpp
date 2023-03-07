#include "light.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"
#include "object.h"
#include <algorithm>

vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    vec3 color;
    vec3 ambient_component;
    vec3 diffuse_component;
    vec3 specular_component;
    vec3 light_vec;
    Light *light_source;
        
    //TODO; //determine the color
    vec3 light_intensity;
    vec3 reflect_light;
    Ray lightray;
    //compute ambient
    ambient_component = world.ambient_color*world.ambient_intensity*color_ambient;
    //compute diffuse
    for(size_t i=0;i<world.lights.size();i++){
        light_source = world.lights[i];
        light_vec = light_source->position - intersection_point; //not sure if reverse
        //compute shadow
        lightray.endpoint = intersection_point;
        lightray.direction =  light_vec.normalized();
       // std::cout<<"light_direction"<<light_direction<<std::endl;
        if(world.enable_shadows==true && (world.Closest_Intersection(lightray).dist!=std::numeric_limits<double>::max())){ 
             //std::cout<<(world.Closest_Intersection(lightray).object)<<std::endl;
            if(std::sqrt(dot(light_vec,light_vec))>world.Closest_Intersection(lightray).dist){
                continue; 
            }           
        }              
        light_intensity = light_source->Emitted_Light(light_vec);       
        diffuse_component = diffuse_component + color_diffuse*light_intensity*std::max(dot(normal.normalized(),light_vec.normalized()),double(0));
    
    //compute specular
    
        reflect_light = 2*dot(normal.normalized(),light_vec.normalized())*normal.normalized()-light_vec.normalized();
        specular_component = specular_component+color_specular*light_intensity*std::pow(std::max(dot(reflect_light.normalized(),(ray.endpoint-intersection_point).normalized()),double(0)),specular_power);
        //std::cout<<color_specular<<std::endl<<light_intensity<<std::endl<<reflect_light<<std::endl<<specular_power<<std::endl;
        //std::cout<<normal.normalized()<<std::endl;
    }
    color = ambient_component + diffuse_component + specular_component;
    //std::cout<<ambient_component<<std::endl<<diffuse_component<<std::endl<<specular_component<<std::endl;
    return color;
}
