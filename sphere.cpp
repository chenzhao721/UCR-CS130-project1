#include "sphere.h"
#include "ray.h"
#include <math.h>
#include <stdio.h>

// Determine if the ray intersects with the sphere
Hit Sphere::Intersection(const Ray& ray, int part) const
{
    //TODO;
    double b,c,d;
    double t1,t2;
    Hit sphereHit;
    b = 2*dot(ray.endpoint-center,ray.direction);
    c = dot(ray.endpoint-center,ray.endpoint-center)-radius*radius;
    d = sqrt(b*b-4*c)/2;
    //printf("endpoint,%lf,%lf,%lf\n",ray.endpoint[0],ray.endpoint[1],ray.endpoint[2]);
    //printf("direction,%lf,%lf,%lf\n",ray.direction[0],ray.direction[1],ray.direction[2]);
    //printf("center,%lf,%lf,%lf\n",center[0],center[1],center[2]);
    //printf("%lf\n",b);
    if(b*b-4*c<=0){
        sphereHit.dist = std::numeric_limits<double>::max();
    }
    else{
        t2 = -b/2-d;
        if(t2>small_t){
            sphereHit.dist=t2;
        }
        else{
            t1 = -b/2+d;
            if(t1>small_t){
                sphereHit.dist=t1;
            }
            else{
                sphereHit.dist = std::numeric_limits<double>::max();
            }
        }
    }
    return sphereHit;
    //return {0,0,0};
}

vec3 Sphere::Normal(const vec3& point, int part) const
{
    vec3 normal;
    //TODO; // compute the normal direction
    normal = (point-center).normalized();
    return normal;
}

Box Sphere::Bounding_Box(int part) const
{
    Box box;
    TODO; // calculate bounding box
    return box;
}
