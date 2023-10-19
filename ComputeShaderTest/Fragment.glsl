#version 430 core
out vec4 FragColor;
  

uniform vec2 windowDimensions;
uniform vec3 cameraLoc;
uniform vec3 cameraDirection;
uniform vec3 cameraUp;
uniform vec3 cameraRight;

struct Voxel
{
    vec4 pos;    // 16
    vec4 col;    // 16
};

layout (std430, binding=2) buffer voxelData
{
    Voxel voxels[];
};

struct Ray{
    vec3 origin;
    vec3 dir;
};

struct CameraParameters{
     float aspectRatio;
     float imageWidth;
     float imageHeight;
     vec3 center;
     vec3 pixel00Loc;
     vec3 pixelDeltaH;
     vec3 pixelDeltaV;
};

void initializeCameraParameters(inout CameraParameters cp){
    cp.imageWidth = float(windowDimensions.x);
    cp.imageHeight = float(windowDimensions.y);

    cp.aspectRatio = cp.imageHeight / cp.imageWidth;

    cp.center = cameraLoc;

    float focalLenght = 1.0;
    float viewportHeight = 2.0;
    float viewportWidth = viewportHeight * (cp.imageWidth / cp.imageHeight);
     
    //vec3 viewportH = vec3(viewportWidth, 0, 0);
    //vec3 viewportV = vec3(0, viewportHeight, 0);

    vec3 viewportH = cameraRight * viewportWidth;
    vec3 viewportV = cameraUp * viewportHeight;

    cp.pixelDeltaH = viewportH / cp.imageWidth;
    cp.pixelDeltaV = viewportV / cp.imageHeight;

    cp.pixel00Loc = cp.center + cameraDirection - viewportH/2.0 - viewportV/2.0;
}

#define MAXDIST 199999999.0 

float rayboxintersect(Ray ray, Voxel voxel)
{
    vec3 boxmin = voxel.pos.xyz;
    vec3 boxmax = boxmin + vec3(1.0);

    float t1 = (boxmin.x - ray.origin.x) / ray.dir.x;
    float t2 = (boxmax.x - ray.origin.x) / ray.dir.x;
    float t3 = (boxmin.y - ray.origin.y) / ray.dir.y;
    float t4 = (boxmax.y - ray.origin.y) / ray.dir.y;
    float t5 = (boxmin.z - ray.origin.z) / ray.dir.z;
    float t6 = (boxmax.z - ray.origin.z) / ray.dir.z;

    float tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
    float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

    if (tmax < 0.0) // box on ray but behind ray origin
    {
        return MAXDIST;
    }

    if (tmin > tmax) // ray doesn't intersect box
    {
        return MAXDIST;
    }

    return tmin;
}


void main()
{
    CameraParameters cp;
    initializeCameraParameters(cp);

    vec2 currPixel = gl_FragCoord.xy;

    vec3 pixelCenter = cp.pixel00Loc + (currPixel.x * cp.pixelDeltaH) + (currPixel.y * cp.pixelDeltaV);

    vec3 rayDirection = pixelCenter - cp.center;

    rayDirection = normalize(rayDirection);

    Ray ray = Ray(cp.center, rayDirection);

    vec4 color = vec4(1.0, 1.0, 1.0, 1.0);

    float minT = 99999999.0;
    for(int voxelIndex = 0; voxelIndex < voxels.length(); voxelIndex++){
        float currT = rayboxintersect(ray, voxels[voxelIndex]);
        if(currT < minT){
            //color = voxels[voxelIndex].col;
            color = vec4(0.0, 0.0, 0.0, 1.0);
            minT = currT;
       }
    }


    FragColor = vec4(color);
} 