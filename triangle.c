#include "triangle.h"
#include <math.h>

void initTriangle(triangle* t){
    t->a.x = 0;
    t->a.y = 0;
    t->a.z = 0;
    t->b.x = 0;
    t->b.y = 0;
    t->b.z = 0;
    t->c.x = 0;
    t->c.y = 0;
    t->c.z = 0;
}


int isInsideTriangle(vector3 point, triangle triangle){
    vector3 a, b, aux, rightVector;
    int dot1, dot2, dot3;
    copyVec(triangle.a, &a);
    copyVec(triangle.b, &b);
    vec3Sub(b, a, &aux);
    vec3Right(aux, &rightVector);
    vec3Sub(point, a, &aux);
    dot1 = vec3DotProduct(aux, rightVector);
    copyVec(triangle.b, &a);
    copyVec(triangle.c, &b);
    vec3Sub(b, a, &aux);
    vec3Right(aux, &rightVector);
    vec3Sub(point, a, &aux);
    dot2 = vec3DotProduct(aux, rightVector);
    copyVec(triangle.c, &a);
    copyVec(triangle.a, &b);
    vec3Sub(b, a, &aux);
    vec3Right(aux, &rightVector);
    vec3Sub(point, a, &aux);
    dot3 = vec3DotProduct(aux, rightVector);
    return (dot1<0 && dot2<0 && dot3<0) || (dot1>=0 && dot2>=0 && dot3>=0);
}
void triangleOffset(triangle a, vector3 offset, triangle* result){
    result->a.x = a.a.x + offset.x;
    result->a.y = a.a.y + offset.y;
    result->a.z = a.a.z + offset.z;
    result->b.x = a.b.x + offset.x;
    result->b.y = a.b.y + offset.y;
    result->b.z = a.b.z + offset.z;
    result->c.x = a.c.x + offset.x;
    result->c.y = a.c.y + offset.y;
    result->c.z = a.c.z + offset.z;
}
void boundingBox(triangle t, vector3* box){
    box[0].x = fmin(fmin(t.a.x, t.b.x), t.c.x);
    box[0].y = fmin(fmin(t.a.y, t.b.y), t.c.y);
    box[1].x = fmax(fmax(t.a.x, t.b.x), t.c.x);
    box[1].y = fmax(fmax(t.a.y, t.b.y), t.c.y);
    printf("Bounding box min: (%f, %f)\n", box[0].x, box[0].y);
    printf("Bounding box max: (%f, %f)\n", box[1].x, box[1].y);
}

void transformRotation(triangle t1, float rotationAngle, triangle* t2){
    vector3 Xbase, Ybase, aux;
    Xbase.x = cos(rotationAngle);
    Xbase.y = sin(rotationAngle);
    Xbase.z = 0;
    vec3Right(Xbase, &aux);
    vec3Scale(aux, -1.0f, &Ybase);
    

    vector3 Xscaled, Yscaled;
    Xscaled.z = 0.0f; Yscaled.z = 0.0f;
    vec3Scale(Xbase, t1.a.x, &Xscaled);
    vec3Scale(Ybase, t1.a.y, &Yscaled);
    vec3Add(Xscaled, Yscaled, &(t2->a));


    Xscaled.z = 0.0f; Yscaled.z = 0.0f;
    vec3Scale(Xbase, t1.b.x, &Xscaled);
    vec3Scale(Ybase, t1.b.y, &Yscaled);
    vec3Add(Xscaled, Yscaled, &(t2->b));

    Xscaled.z = 0.0f; Yscaled.z = 0.0f;
    vec3Scale(Xbase, t1.c.x, &Xscaled);
    vec3Scale(Ybase, t1.c.y, &Yscaled);
    vec3Add(Xscaled, Yscaled, &(t2->c));
}

void printTriangle(triangle a){
    printf("Point 1:\n");
    printVec3(a.a);
    printf("Point 2:\n");
    printVec3(a.b);
    printf("Point 3:\n");
    printVec3(a.c);
}

void triangleBaricenter(triangle a, vector3 * result){
    result->x = 0;
    result->y = 0;
    result->z = 0;
    vector3 aux;


    vec3Add(a.a, a.b, &aux);
    vec3Add(aux, a.c, result);
    result->x = result->x/3.0f;
    result->y = result->y/3.0f;
    result->z = result->z/3.0f;
}

void triangleScale(triangle a, float scale, triangle* result){
    vector3 c, ca, cb, cc;
    triangleBaricenter(a, &c);
    vec3Sub(a.a, c, &ca);
    vec3Sub(a.b, c, &cb);
    vec3Sub(a.c, c, &cc);
    
    vec3Scale(ca, scale, &ca);
    vec3Scale(cb, scale, &cb);
    vec3Scale(cc, scale, &cc);

    vec3Add(c, ca, &result->a);
    vec3Add(c, cb, &result->b);
    vec3Add(c, cc, &result->c);
}


float triangleArea(triangle t){
    vector3 a, b;
    vec3Sub(t.b, t.a, &a);
    vec3Sub(t.c, t.a, &b);
    return 0.5f * fabsf(a.x * b.y - a.y * b.x);
}

int isTriangle(triangle t){
    return triangleArea(t) > 0;
}

void randomTriangle(triangle* a){
    initTriangle(a);
    while(!isTriangle(*a)){
        float scale = rand()/RAND_MAX*10.0f + 0.1f;
        if(scale == 0) scale = 1.0f;
        float x = (float)abs(rand())/RAND_MAX*PI;
        a->a.x = cos(x);
        a->a.y = sin(x);
        float y = (float)abs(rand())/RAND_MAX*2*PI;
        a->b.x = cos(y);
        a->b.y = sin(y);
        float z = (x+y)/2.0f;
        a->c.x = cos(z);
        a->c.y = sin(z);
        triangleScale(*a, scale, a);
    }
}


float maxCos(triangle t){
    vector3 ab, ba, ac, ca, bc, cb;
    vec3Sub(t.b, t.a, &ab);
    vec3Sub(t.a, t.b, &ba);
    vec3Sub(t.c, t.a, &ac);
    vec3Sub(t.a, t.c, &ca);
    vec3Sub(t.c, t.b, &bc);
    vec3Sub(t.b, t.c, &cb);
    float a = cosVec3(ab, ac);
    float b = cosVec3(ba, bc);
    float c = cosVec3(ca, cb);
    float max = a;
    if(b > max) max = b;
    if(c > max) max = c;
    return max;
}