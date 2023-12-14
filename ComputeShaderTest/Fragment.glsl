#version 430 core
out vec4 FragColor;
  

uniform vec2 windowDimensions;
uniform vec3 cameraLoc;
uniform vec3 cameraDirection;
uniform vec3 cameraUp;
uniform vec3 cameraRight;

uniform int selectedDataStructure;

struct Voxel
{
    vec4 pos;    // 16
    vec4 col;    // 16
    ivec4 data;   //hasData isDivided isLeaf childrenIndex   
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

bool isInOctant(vec3 point, Voxel voxel)
{
	return point.x >= voxel.pos.x && point.x < voxel.pos.x + voxel.pos.w &&
		point.y >= voxel.pos.y && point.y < voxel.pos.y + voxel.pos.w &&
		point.z >= voxel.pos.z && point.z < voxel.pos.z + voxel.pos.w;
}

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
#define MAXTRAVERSE 100
#define MAXHEIGHT 20

float rayboxintersect(Ray ray, Voxel voxel, out vec2 tValues)
{
    vec3 boxmin = voxel.pos.xyz;
    vec3 boxmax = boxmin + voxel.pos.w;

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

    tValues = vec2(tmin + 0.01, tmax + 0.01);
    return tmin;
}

bool traversal(inout vec2 tValues, in Ray ray){
    
    Voxel currVoxel = voxels[0];

    int count = 0;
    while(currVoxel.data[0] != 0 && count < MAXHEIGHT){ //hasData
        //FragColor = vec4(1.0, 0.0, 0.0, 1.0);
        //FragColor = vec4(1.0, 0.0, 0.0, 1.0);
        if(currVoxel.data[2] != 0){
            return true;
        }
        for(int i = 0; i < 8; i++){
            int newIndex = currVoxel.data.w;
            Voxel currChild = voxels[newIndex +i];
            vec3 point = ray.origin + tValues.x * ray.dir;
            if(isInOctant(point, currChild)){
                currVoxel = currChild;
                break;
            }
        }
        count++;
    }
    rayboxintersect(ray, currVoxel, tValues);

    return false;
}


vec4 rayCastOctree(Ray ray){
    vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
    vec2 tValues;
    rayboxintersect(ray, voxels[0], tValues);

    if(tValues.y < tValues.x || tValues.y < 0.0) return color;
    int count = 0;
    vec3 point = ray.origin + tValues.x * ray.dir;
    while(isInOctant(point, voxels[0]) && count < MAXTRAVERSE){
        if(traversal(tValues, ray)){
            color = vec4(0.0, 0.0, 0.0, 1.0) + (vec4(1.0, 1.0, 1.0, 1.0) * (tValues.x/2000));
            break;
        }
        tValues.x = tValues.y;
        point = ray.origin + tValues.x * ray.dir;
        count++;
    }
    return color;
}

vec4 rayCastList(Ray ray){
    vec4 color = vec4(1.0, 1.0, 1.0, 1.0);

    float minT = MAXDIST;
    vec2 placeHolder;

    for(int voxelIndex = 0; voxelIndex < voxels.length(); voxelIndex++){
        float currT = rayboxintersect(ray, voxels[voxelIndex], placeHolder);
        if(currT < minT){
            //color = voxels[voxelIndex].col;
            minT = currT;
            color = vec4(0.0, 0.0, 0.0, 1.0) + (vec4(1.0, 1.0, 1.0, 1.0) * (minT/2000));
       }
    }

    return color;
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

    vec4 color;

    switch(selectedDataStructure){
    case 0:
        color = rayCastOctree(ray);
        break;
    case 1:
        color = rayCastList(ray);
        break;
    }


    FragColor = color;
} 