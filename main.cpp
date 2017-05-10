#include <iostream>
#include <cstdlib>

#include "color.h"
#include "vec3.h"
#include "image.h"
#include "ray.h"
#include "sphere.h"
#include <cmath>
#include "plane.h"

#include "camera.h"
#include "scene.h"
#include "material.h"
#include "matte.h"
#include "box.h"
#include "globals.h"


#include "mergehits.h"
#include "interval.h"

#include "csgnode.h"
#include "csgprimitive.h"
#include "csgoperation.h"
#include "csgobject.h"

#include "phong.h"


#define printHitList(A)     cout<<"-------------------------"<<endl<<"inters "<<#A<<": "<<endl;        \
                            for(int l = 0; l < A.size(); l++)       \
                            {                                       \
                                printVar(l);                        \
                                printVec(A[l].hitPoint);             \
                                printVec(r.rayPoint(A[l].t));       \
                                cout<<A[l].t<<endl;                 \
                            } cout<<endl<<endl;



using namespace std;
/*
namespace debug
{
    void T_Color();
    void T_Vec3d();
    void T_Image();
    void T_Ray();
    void T_Sphere();
}*/
void setupScene();
void csgExemple();

Camera CAMERA;
Scene SCENE;
int VERTICAL_RES = 800;
int HORIZONTAL_RES = 800;
float ZOOM = 0.7;

int main(int argc, char** argv)
{
    int i;

    CAMERA = Camera(Vec3d(20, 20, 20.0),
                    Vec3d(0, 0.0, 0.0),
                    Vec3d(0.0, 0.0, 1.0),
                    HORIZONTAL_RES,
                    VERTICAL_RES,
                    500.0);

    CAMERA.pixelSize /= ZOOM;
    SCENE = Scene();
    SCENE.bg = Color();

    csgExemple();

    Light* l = new Light(Vec3d(100.0, 100.0, 100.0),
             1.0,
             Color(1.0));
    SCENE.addLight(l);
    Light* l1 = new Light(Vec3d(-100.0, -100.0, 100.0),
             0.5,
             Color(1.0));
    SCENE.addLight(l1);



//    int NUM_SPH = 10;
//    cin >> NUM_SPH;
//    cout<<"Esferas: " << NUM_SPH<<endl;
//    Sphere * s;
//    for(int j = 0; j < NUM_SPH; j++)
//    {
//        float x = rand() % 200 -100, y = rand() % 200 -100, z = rand() % 200 -100;
//        Color col2 = Color(1.0, 0.0, (j%11)*0.1);
//        s = new Sphere(Vec3d(x, y, z), 15);
//        s->m = new Material(col2);
//        SCENE.addObject(s);
//    }

    Image im = CAMERA.render(SCENE);
    im.save("image.ppm");
    system("start image.ppm");
    return 0;
}


void setupScene()
{


    int nObjs = 0, nLights = 0, i;

    /**
    **  Setup Objects
    **/

    //cin>>nObjs;
    //cout<<nObjs<<" objetos..."<<endl;
    for(i = 0; i < nObjs; i++)
    {
        string type;
        cin >> type;
        cout<<"objeto "<<i<<endl;
        if(type == "sphere")
        {
            /**
                sphere
                <center_coords>
                <radius>
                <color>
            **/
            float a, b, c, r;
            cin>>a>>b>>c;
            Vec3d center = Vec3d(a, b, c);
            cin>>r;
            cin>>a>>b>>c;
            Color col = Color(a, b, c);
            Sphere* s = new Sphere(center, r);
            s->c = col;
            s->m = new Matte(col);
            SCENE.addObject(s);
        }
        else if(type == "plane")
        {
            /**
                plane
                <point_coords>
                <plane_normal>
                <color>
            **/
            float a, b, c;
            cin>>a>>b>>c;
            Vec3d point = Vec3d(a, b, c);
            cin>>a>>b>>c;
            Vec3d normal = Vec3d(a, b, c);
            cin>>a>>b>>c;
            Color col = Color(a, b, c);
            Plane* p = new Plane(point, normal);
            p->c = col;
            p->m = new Matte(col);
            SCENE.addObject(p);
        }
        else if(type == "box")
        {
            /**
                box
                <min_point>
                <max_point>
                <color>
            **/
            float a, b, c;
            cin>>a>>b>>c;
            Vec3d min_point = Vec3d(a, b, c);
            cin>>a>>b>>c;
            Vec3d max_point = Vec3d(a, b, c);
            cin>>a>>b>>c;
            Color col = Color(a, b, c);
            Box* box = new Box(min_point, max_point);
            box->c = col;
            box->m = new Matte(col);
            SCENE.addObject(box);
        }

    }

    /**
    **  Setup Lights
    **/

    //cin>>nLights;

    for(i = 0; i < nLights; i++)
    {
        string type;
        cin >> type;
        cout<<"Luz "<<i<<endl;
        if(type == "point")
        {
            float a, b, c, s;
            cin>>a>>b>>c;
            Vec3d pos = Vec3d(a, b, c);
            cin>>s;
            cin>>a>>b>>c;
            Color col = Color(a, b, c);
            Light* l = new Light(pos, s, col);
            SCENE.addLight(l);
        }
    }


}

void csgExemple()
{
    Color col = Color(0.3, 0.3, 0.3);
    Sphere* s = new Sphere(Vec3d(-10.0, 0.0, 0.0), 8);
    s->c = col;
    s->m = new Matte(col);

    Box* b = new Box(Vec3d(-23, -10, -10), Vec3d(-10, 10, 10));
    b->c = col;
    b->m = new Matte(col);


    Sphere* s2 = new Sphere(Vec3d(0.0, 0.0, 0.0), 6);
    s2->c = col;
    s2->m = new Matte(col);
    //SCENE.addObject(s2);


    Box* b2 = new Box(Vec3d(-5, -5, -5), Vec3d(5, 5, 5));
    b2->c = col;
    b2->m = new Matte(col);


    Sphere* s3 = new Sphere(Vec3d(-20.0, 0.0, 10.0), 5);
    s3->c = col;
    s3->m = new Matte(col);

    ////////////////////////////////////////////////////////////////////////////////////
    // (b - s3) - s
    CSGNode* node = new CSGPrimitive(s);
    CSGNode* node2 = new CSGPrimitive(b);
    CSGNode* node3 = new CSGPrimitive(s3);

    CSGNode* nodeOp1 = new CSGOperation(node2, node3, '-');
    CSGNode* nodeOp2 = new CSGOperation(nodeOp1, node, '-');


    CSGObject * obj = new CSGObject(nodeOp2);
    //printInters(obj->hit(r));
    obj->m = new Matte(col);
    SCENE.addObject(obj);

    ////////////////////////////////////////////////////////////////////////////////////

    CSGNode *box = new CSGPrimitive(b2);
    CSGNode *sph = new CSGPrimitive(s2);
    CSGNode *BSMinus = new CSGOperation(box, sph, '-');
    CSGObject* obj2 = new CSGObject(BSMinus);
    obj2->m = new Matte(Color(0.0, 0.0, 1.0));
    SCENE.addObject(obj2);

    ////////////////////////////////////////////////////////////////////////////////////


    Sphere* s5 = new Sphere(Vec3d(15.0, 0.0, 0.0), 6);
    s5->m = new Matte(col);

    Box* b5 = new Box(Vec3d(13, -2, -10), Vec3d(17, 2, 10));
    b5->m = new Matte(col);

    Box* b6 = new Box(Vec3d(13, -10, -2), Vec3d(17, 10, 2));
    b6->m = new Matte(col);

    CSGNode *sph2 = new CSGPrimitive(s5);
    CSGNode *box2 = new CSGPrimitive(b5);
    CSGNode *box3 = new CSGPrimitive(b6);
    CSGNode *BSMinus2 = new CSGOperation(sph2, box2, '-');
    CSGNode *BSMinus3 = new CSGOperation(BSMinus2, box3, '-');
    CSGObject* obj3 = new CSGObject(BSMinus3);
    obj3->m = new Matte(Color(1.0, 1.0, 0.0));
    SCENE.addObject(obj3);

    Plane* p = new Plane(Vec3d(0, 0, -11), Vec3d(0.0, 0.0, 1.0));
    p->m = new Matte(Color(1.0));
    SCENE.addObject(p);

    Sphere *sphere = new Sphere(Vec3d(0,0,15), 5);
    sphere->m = new Phong(Color(1.0, 0.3, 0.6));
    SCENE.addObject(sphere);
}

/*
void debug::T_Color()
{
    Color red = Color(1.0, 0.0, 0.0);
    Color white = Color(1.0);
    Color black = Color();
    Color red2 = Color(red);
    Color white2 = white;

    printCol(red);
    printCol(white);
    printCol(black);

    printCol(red2);
    printCol(white2);
    printCol(red+black);
    printCol(white * 0.5);
    printCol(white-red);
    printCol(red/2);
}

void debug::T_Vec3d()
{
        Vec3d zero = Vec3d(),
            v1 = Vec3d(1.0),
            X = Vec3d(1.0, 0.0, 0.0),
            X2 = Vec3d(X),
            v2 = v1;
    QP(-----------------------------------Vec3d-----------------------------------------);
    //constructors + assignment
    printVec(zero);
    printVec(v1);
    printVec(X);
    printVec(X2);
    printVec(v2);
    //printVectors(5, &zero, &v1, &X, &X2, &v2);
    // menos unário --------
    QP(Menos unário);
    printVec(-v1);
    // produto por escalar --------
    QP(Produto por escalar);
    Vec3d tmp = 2*X;
    printVec(2*X);
    printVec(X*2);
    // divisão por escalar
    QP(Divisão por escalar);
    printVec(X/2);
    //soma
    QP(Soma);
    printVec(v1+X);
    //soma e atribuição
    QP(Soma e atribuição);
    v2 += X;
    printVec(v2);
    //subtração
    QP(Subtração);
    printVec(v2 - X);
    printVec(X - v2);
    // Dot
    QP(Dot Product);
    cout<<(X*v1)<<endl;
    //cross
    Vec3d c1 = Vec3d(3, -3, 1), c2 = Vec3d(4, 9, 2);
    QP(Cross Product);
    QP(Expected Result: (-15, -2, 39));
    printVec(c1 ^ c2);
}

void debug::T_Image()
{
    QP(--------------------------------------image----------------------------------------);
    Image im = Image();
    for(int i = 0; i < im.w; i++)
    for(int j = 0; j < im.h; j++)
    {
        //cout<<"i = "<<i <<" j = "<<j<<endl;
        //printCol(im.getPixel(i, j));
        if( (int)sqrt(i*i + j*j) % 40 > 35 || (int)sqrt(i*i + j*j) % 40 <10)
            im.setPixel(i, j, Color(2.0, 2.0, 2.0));
    }
    im.save("teste.ppm");
}

void debug::T_Ray()
{
    QP(-------------------------------------- Ray ----------------------------------------);
    Vec3d o1 = Vec3d(1.0), d1 = Vec3d(1.0);
    d1.normalize();
    printVec(o1); printVec(d1);
    Ray r1 = Ray(o1, d1), r2 = Ray(), r3 = Ray(r1), r4 = r2;
    printRay(r1); printRay(r2);
    printRay(r3); printRay(r4);

}

void debug::T_Sphere()
{

    QP(--------------------------------------Sphere----------------------------------------);
    Vec3d v1(1.0, 0.0, 0.0);
    float r1 = 1.0;
    Ray r = Ray(Vec3d(-2.0, 0.0, 0.0), Vec3d(1.0, 0.0, 0.0));
    Intersect i;

    Sphere s1 = Sphere(v1, r1), s2 = Sphere();
    printRay(r);
    printObject((s1));
    printObject(s2);
    //cout<<"hit s1: "<<s1.hit(r, i)<<endl;
    printInters(i);
    printVec(r.rayPoint(i.t));
    printVec(s1.getNormal(r.rayPoint(i.t)));

}
*/




