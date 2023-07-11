# RayTraceApp
This is a offline raytracer with multithreding and some postprocessing effects
It is just for fun currently can only handle spheres 
Triangles coming soon....
## Input File Format 
Scene
SAMPLES [number of samples]
NEAR [near plane]
LEFT [left]
RIGHT [right]
BOTTOM [bottom]
TOP [top]
RES [width] [height]
MATERIAL[name] [color as 3 0-1 values] [raughness 0 - 1] [mettalic [0 - 1] [specular component]]
SPHERE [name] [position as 3 floats] [radius as a float] [material name]
LIGHT [name] [position as 3 floats] [color as 3 0-1 values]
AMBIENT [color as 3 0-1 values]
OUTPUT [Name of the output file .ppm format]