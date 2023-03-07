#include "mesh.h"
#include <fstream>
#include <string>
#include <limits>

// Consider a triangle to intersect a ray if the ray intersects the plane of the
// triangle with barycentric weights in [-weight_tolerance, 1+weight_tolerance]
static const double weight_tolerance = 1e-4;

// Read in a mesh from an obj file.  Populates the bounding box and registers
// one part per triangle (by setting number_parts).
void Mesh::Read_Obj(const char* file)
{
    std::ifstream fin(file);
    if(!fin)
    {
        exit(EXIT_FAILURE);
    }
    std::string line;
    ivec3 e;
    vec3 v;
    box.Make_Empty();
    while(fin)
    {
        getline(fin,line);

        if(sscanf(line.c_str(), "v %lg %lg %lg", &v[0], &v[1], &v[2]) == 3)
        {
            vertices.push_back(v);
            box.Include_Point(v);
        }

        if(sscanf(line.c_str(), "f %d %d %d", &e[0], &e[1], &e[2]) == 3)
        {
            for(int i=0;i<3;i++) e[i]--;
            triangles.push_back(e);
        }
    }
    number_parts=triangles.size();
}

// Check for an intersection against the ray.  See the base class for details.
Hit Mesh::Intersection(const Ray& ray, int part) const
{
    //TODO;
    Hit planeHit;   
    planeHit.dist = std::numeric_limits<double>::max();
    if(Intersect_Triangle(ray, part, planeHit.dist)){  
        planeHit.object = this;
        planeHit.part = part;
    }
    return planeHit;
}

// Compute the normal direction for the triangle with index part.
vec3 Mesh::Normal(const vec3& point, int part) const
{
    assert(part>=0);
    //TODO;
    
   int idxA, idxB, idxC;
    vec3 A, B, C;
    idxA = triangles[part][0];
    idxB = triangles[part][1];
    idxC = triangles[part][2];
    A = vertices[idxA];
    B = vertices[idxB];
    C = vertices[idxC];

    vec3 normal;
    normal = cross(B-A,C-A).normalized();

    return normal;
}

// This is a helper routine whose purpose is to simplify the implementation
// of the Intersection routine.  It should test for an intersection between
// the ray and the triangle with index tri.  If an intersection exists,
// record the distance and return true.  Otherwise, return false.
// This intersection should be computed by determining the intersection of
// the ray and the plane of the triangle.  From this, determine (1) where
// along the ray the intersection point occurs (dist) and (2) the barycentric
// coordinates within the triangle where the intersection occurs.  The
// triangle intersects the ray if dist>small_t and the barycentric weights are
// larger than -weight_tolerance.  The use of small_t avoid the self-shadowing
// bug, and the use of weight_tolerance prevents rays from passing in between
// two triangles.
bool Mesh::Intersect_Triangle(const Ray& ray, int tri, double& dist) const
{
    //TODO;
    vec3 normal;
    vec3 x1;
    ivec3 idx;
    vec3 A,B,C,P;
    float alpha, beta, gamma;
    

    idx = triangles[tri];
    A = vertices[idx[0]];
    B = vertices[idx[1]];
    C = vertices[idx[2]];

    
    x1 = A;
    normal = Normal(P, tri);

    if(dot(ray.endpoint-x1,normal)==0){
        return false;
    }
    else{
        if(dot(ray.direction,normal)!=0){
            dist = dot(x1-ray.endpoint,normal)/dot(ray.direction,normal);
            if(dist<small_t){
                return false;
            }  
        } 
        else{
           return false;
        }
    }
    
    P = ray.endpoint + dist*ray.direction;   
    alpha = dot(cross(B-P,C-P),normal)/std::sqrt(dot(cross(B-A,C-A),cross(B-A,C-A)));
    beta =  dot(cross(A-P,B-P),normal)/std::sqrt(dot(cross(A-C,B-C),cross(A-C,B-C)));
    gamma = dot(cross(C-P,A-P),normal)/std::sqrt(dot(cross(C-B,A-B),cross(C-B,A-B)));

    if(alpha>-weight_tol&&alpha<1+weight_tolerance&& beta>-weight_tol&&beta<1+weight_tolerance && gamma>-weight_tol&&gamma<1+weight_tolerance){
        return true;
    }
    else{
        dist = std::numeric_limits<double>::max();
        return false;
    }
}

// Compute the bounding box.  Return the bounding box of only the triangle whose
// index is part.
Box Mesh::Bounding_Box(int part) const
{
    Box b;
    vec3 A,B,C;
    ivec3 idx;
    //TODO;
    if(part>=0){
        idx = triangles[part];
        A = vertices[idx[0]];
        B = vertices[idx[1]];
        C = vertices[idx[2]];
        for(int i=0;i<3;i++){
            if(A[i]<B[i]){
                if(C[i]<A[i]){
                    b.lo[i] = C[i]; 
                    b.hi[i] = B[i];
                }
                else if (C[i]>B[i]){
                    b.lo[i] = A[i]; 
                    b.hi[i] = C[i];
 
                }
                else{
                    b.lo[i] = A[i]; 
                    b.hi[i] = B[i];
                }
            }
            else{
                if(C[i]<B[i]){
                    b.lo[i] = C[i]; 
                    b.hi[i] = A[i];
                }
                else if (C[i]>A[i]){
                    b.lo[i] = B[i]; 
                    b.hi[i] = C[i];
 
                }
                else{
                    b.lo[i] = B[i]; 
                    b.hi[i] = A[i];
                }
            }
          
        }
    }
    //part<0 not completed
    return b;
}
