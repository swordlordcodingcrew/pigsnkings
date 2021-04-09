// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <iostream>
#include <fonts/barcadebrawl.h>
#include "Gear.hpp"
#include "SettingsLayer.h"
#include "Layer.hpp"
#include "Imagesheet.hpp"

#include "tinyraytracer.h" // see https://github.com/ssloy/tinyraytracer
#include "type.h"
#include "mesh_monkey.h"

bool bRendered = false;

#define FOV              64

void raytrace() {
    Material      ivory(1.0, Vec4f(0.6,  0.3, 0.1, 0.0), Vec3f(0.4, 0.4, 0.3),   50.);
    Material      glass(1.5, Vec4f(0.0,  0.5, 0.1, 0.8), Vec3f(0.6, 0.7, 0.8),  125.);
    Material red_rubber(1.0, Vec4f(0.9,  0.1, 0.0, 0.0), Vec3f(0.4, 0.1, 0.1),   10.);
    Material     mirror(1.0, Vec4f(0.0, 10.0, 0.8, 0.0), Vec3f(1.0, 1.0, 1.0), 1425.);

    std::vector<Sphere> spheres;
    spheres.push_back(Sphere(Vec3f(-3,    0,   -16), 2,      ivory));
    spheres.push_back(Sphere(Vec3f(-1.0, -1.5, -12), 2,      glass));
    spheres.push_back(Sphere(Vec3f( 1.5, -0.5, -18), 3, red_rubber));
//    spheres.push_back(Sphere(Vec3f( 7,    5,   -18), 4,     mirror));

    std::vector<Light>  lights;
    lights.push_back(Light(Vec3f(-20, 20,  20), 1.5));
    lights.push_back(Light(Vec3f( 30, 50, -25), 1.8));
    lights.push_back(Light(Vec3f( 30, 20,  30), 1.7));

    render(spheres, lights);
}

struct point {
    float initialx;
    float initialy;
    float initialz;
    float x;
    float y;
    float z;
};

uint32_t framenum = 0;
point ivorySphereCoords = { 0, -1.5, -14 };

// ----------------------------------------------
// global variables
// ----------------------------------------------
Matrix4 m_world;
Vector3i mesh_rotation = {0, 0, 0};
Vector3i mesh_position = {0, 0, 0};

#define HALFW            160
#define HALFH            120

const unsigned int lut[] = {         // 0 to 90 degrees fixed point COSINE look up table
        16384, 16381, 16374, 16361, 16344, 16321, 16294, 16261, 16224, 16182, 16135, 16082, 16025, 15964, 15897, 15825, 15749, 15668, 15582, 15491, 15395, 15295, 15190, 15081, 14967, 14848, 14725, 14598, 14466, 14329, 14188, 14043, 13894, 13740, 13582, 13420, 13254, 13084, 12910, 12732, 12550, 12365, 12175, 11982, 11785, 11585, 11381, 11173, 10963, 10748, 10531, 10310, 10086, 9860, 9630, 9397, 9161, 8923, 8682, 8438, 8191, 7943, 7691, 7438, 7182, 6924, 6663, 6401, 6137, 5871, 5603, 5334, 5062, 4790, 4516, 4240, 3963, 3685, 3406, 3126, 2845, 2563, 2280, 1996, 1712, 1427, 1142, 857, 571, 285, 0
};

#define LUT(a) (long)(&lut[a])// return value from LUT

static int proj_nodes[NODECOUNT][2];         // projected nodes (x,y)
static int old_nodes[NODECOUNT][2];          // projected nodes of previous frame to check if we need to redraw
static unsigned char i;
static int loops;
static double next_tick;
static double last_btn;                      // used for checking when the button was last pushed
static unsigned char draw_type = 3;          // 0 - vertex | 1 - wireframe | 2 - flat colors | ...

// ----------------------------------------------
// SIN/COS from 90 degrees LUT
// ----------------------------------------------
long SIN(unsigned int angle) {
    angle += 90;
    if (angle > 450) return LUT(0);
    if (angle > 360 && angle < 451) return -LUT(angle-360);
    if (angle > 270 && angle < 361) return -LUT(360-angle);
    if (angle > 180 && angle < 271) return  LUT(angle-180);
    return LUT(180-angle);
}

long COS(unsigned int angle) {
    if (angle > 360) return LUT(0);
    if (angle > 270 && angle < 361) return  LUT(360-angle);
    if (angle > 180 && angle < 271) return -LUT(angle-180);
    if (angle > 90  && angle < 181) return -LUT(180-angle);
    return LUT(angle);
}

// ----------------------------------------------
// Matrix operation
// ----------------------------------------------
Matrix4 mMultiply(const Matrix4 &mat1, const Matrix4 &mat2) {
    Matrix4 mat;
    unsigned char r,c;
    for (c=0; c<4; c++)
        for (r=0; r<4; r++)
            mat.m[c][r] = pMultiply(mat1.m[0][r], mat2.m[c][0]) +
                          pMultiply(mat1.m[1][r], mat2.m[c][1]) +
                          pMultiply(mat1.m[2][r], mat2.m[c][2]) +
                          pMultiply(mat1.m[3][r], mat2.m[c][3]);
    return mat;
}

Matrix4 mRotateX(const unsigned int angle) {
    Matrix4 mat;
    mat.m[1][1] =  COS(angle);
    mat.m[1][2] =  SIN(angle);
    mat.m[2][1] = -SIN(angle);
    mat.m[2][2] =  COS(angle);
    return mat;
}

Matrix4 mRotateY(const unsigned int angle) {
    Matrix4 mat;
    mat.m[0][0] =  COS(angle);
    mat.m[0][2] = -SIN(angle);
    mat.m[2][0] =  SIN(angle);
    mat.m[2][2] =  COS(angle);
    return mat;
}

Matrix4 mRotateZ(const unsigned int angle) {
    Matrix4 mat;
    mat.m[0][0] =  COS(angle);
    mat.m[0][1] =  SIN(angle);
    mat.m[1][0] = -SIN(angle);
    mat.m[1][1] =  COS(angle);
    return mat;
}

Matrix4 mTranslate(const long x, const long y, const long z) {
    Matrix4 mat;
    mat.m[3][0] =  x << PSHIFT;
    mat.m[3][1] =  y << PSHIFT;
    mat.m[3][2] =  z << PSHIFT;
    return mat;
}

Matrix4 mScale(const float ratio) {
    Matrix4 mat;
    mat.m[0][0] *= ratio;
    mat.m[1][1] *= ratio;
    mat.m[2][2] *= ratio;
    return mat;
}

// ----------------------------------------------
// Shoelace algorithm to get the surface
// ----------------------------------------------
int shoelace(const int (*n)[2], const int index) {
    unsigned char t = 0;
    int surface = 0;
    for (; t<3; t++) {
        // (x1y2 - y1x2) + (x2y3 - y2x3) ...
        surface += (n[faces[index][t]][0]           * n[faces[index][t<2?t+1:0]][1]) -
                   (n[faces[index][t<2?t+1:0]][0] * n[faces[index][t]][1]);
    }
    return surface * 0.5;
}

// ----------------------------------------------
// Shoelace algorithm for triangle visibility
// ----------------------------------------------
bool is_hidden(const int n[NODECOUNT][2], const int index) {
    // (x1y2 - y1x2) + (x2y3 - y2x3) ...
    return ( ( (n[faces[index][0]][0] * n[faces[index][1]][1]) -
               (n[faces[index][1]][0] * n[faces[index][0]][1])   ) +
             ( (n[faces[index][1]][0] * n[faces[index][2]][1]) -
               (n[faces[index][2]][0] * n[faces[index][1]][1])   ) +
             ( (n[faces[index][2]][0] * n[faces[index][0]][1]) -
               (n[faces[index][0]][0] * n[faces[index][2]][1])   ) ) < 0 ? false : true;
}

// ----------------------------------------------
// draw projected nodes
// ----------------------------------------------
void draw_vertex(const int (*n)[2], const uint16_t color) {
    i = NODECOUNT-1;
    do {
        blit::Pen backgroundColour = blit::Pen(255, 255, 255, 255);
        blit::screen.pen = backgroundColour;
        blit::screen.pixel(blit::Point(n[i][0],n[i][1]));

        //TFT.drawPixel(n[i][0],n[i][1], color);
    } while(i--);
}

// ----------------------------------------------
// draw edges between projected nodes
// ----------------------------------------------
void draw_wireframe(const int n[NODECOUNT][2], const uint16_t color) {
    i = TRICOUNT-1;
    do {
        // don't draw triangle with negative surface value
        if (!is_hidden(n, i)) {
            blit::Pen backgroundColour = blit::Pen(255, 20, 255, 255);
            blit::screen.pen = backgroundColour;
            // draw triangle edges - 0 -> 1 -> 2 -> 0
            blit::screen.line(blit::Point(n[faces[i][0]][0], n[faces[i][0]][1]), blit::Point(n[faces[i][1]][0], n[faces[i][1]][1]));
            blit::screen.line(blit::Point(n[faces[i][1]][0], n[faces[i][1]][1]), blit::Point(n[faces[i][2]][0], n[faces[i][2]][1]));
            blit::screen.line(blit::Point(n[faces[i][2]][0], n[faces[i][2]][1]), blit::Point(n[faces[i][0]][0], n[faces[i][0]][1]));
            /*
            TFT.drawLine(n[EDGE(i,0)][0], n[EDGE(i,0)][1], n[EDGE(i,1)][0], n[EDGE(i,1)][1], color);
            TFT.drawLine(n[EDGE(i,1)][0], n[EDGE(i,1)][1], n[EDGE(i,2)][0], n[EDGE(i,2)][1], color);
            TFT.drawLine(n[EDGE(i,2)][0], n[EDGE(i,2)][1], n[EDGE(i,0)][0], n[EDGE(i,0)][1], color);
             */
        }
    } while(i--);
}

// ----------------------------------------------
// draw flat color (not flat shading)
// ----------------------------------------------
void draw_flat_color(const int (*n)[2], uint16_t color) {
    i = TRICOUNT-1;
    int surface;
    uint16_t col = color;
    do {
        // draw only triangles facing us
        if ((surface=shoelace(n, i)) < 0) {
            // this is an ugly hack but it 'somehow' fakes shading
            // depending on the size of the surface of the triangle
            // change the color toward brighter/darker
            color = col * (surface * 0.001);

            /*
            TFT.fillTriangle(n[EDGE(i,0)][0], n[EDGE(i,0)][1],
                             n[EDGE(i,1)][0], n[EDGE(i,1)][1],
                             n[EDGE(i,2)][0], n[EDGE(i,2)][1],
                             color);
                             */
        }
    } while(i--);
}

// ----------------------------------------------
// main loop
// ----------------------------------------------
void do_render() {
    loops = 0;


        // rotation
        m_world = mRotateX(mesh_rotation.x);
        m_world = mMultiply(mRotateY(mesh_rotation.y), m_world);
        m_world = mMultiply(mRotateZ(mesh_rotation.z), m_world);
        // scaling
        //m_world = mMultiply(mScale(1.5), m_world);

        // project nodes with world matrix
        Vector3i p;
        for (i=0; i<NODECOUNT; i++) {
            p.x = (m_world.m[0][0] * (nodes[i][0] >> PSHIFT) +
                   m_world.m[1][0] * (nodes[i][1] >> PSHIFT) +
                   m_world.m[2][0] * (nodes[i][2] >> PSHIFT) +
                   m_world.m[3][0]) / PRES;

            p.y = (m_world.m[0][1] * (nodes[i][0] >> PSHIFT) +
                   m_world.m[1][1] * (nodes[i][1] >> PSHIFT) +
                   m_world.m[2][1] * (nodes[i][2] >> PSHIFT) +
                   m_world.m[3][1]) / PRES;

            p.z = (m_world.m[0][2] * (nodes[i][0] >> PSHIFT) +
                   m_world.m[1][2] * (nodes[i][1] >> PSHIFT) +
                   m_world.m[2][2] * (nodes[i][2] >> PSHIFT) +
                   m_world.m[3][2]) / PRES;

            // store projected node
            proj_nodes[i][0] = (FOV * p.x) / (FOV + p.z) + HALFW;
            proj_nodes[i][1] = (FOV * p.y) / (FOV + p.z) + HALFH;
        }

        // default auto-rotation mode
        mesh_rotation.x+=3;
        mesh_rotation.y+=2;
        mesh_rotation.z++;

        if (mesh_rotation.x > 360) mesh_rotation.x = 0;
        if (mesh_rotation.y > 360) mesh_rotation.y = 0;
        if (mesh_rotation.z > 360) mesh_rotation.z = 0;
        // ...
        loops++;


    // ===============
    // draw
    // ===============
    // render frame
    switch(draw_type) {
        case 0:
            draw_vertex(old_nodes, 0);
            draw_vertex(proj_nodes, 0);
            break;
        case 1:
            draw_wireframe(old_nodes, 0);
            draw_wireframe(proj_nodes, 0);
            break;
        case 2:
            draw_flat_color(proj_nodes, 0);
            break;
        case 3:
            draw_flat_color(proj_nodes, 0);
            draw_wireframe(proj_nodes, 0);
            break;
        case 4:
            draw_flat_color(proj_nodes, 0);
            break;
    }
}

namespace pnk
{
    SettingsLayer::SettingsLayer() : Layer(dang::Layer::LT_UNDEFINED)
    {
        backgroundColour = blit::Pen(255, 255, 255, 255);
        foregroundColour = blit::Pen(0, 0, 0, 255);
        highlightColour = blit::Pen(221, 107, 6, 255);

        // paint houses^h^h^h clear the background
        blit::screen.pen = backgroundColour;
        displayRect = blit::Rect(0, 0, blit::screen.bounds.w, blit::screen.bounds.h);
        blit::screen.rectangle(displayRect);

        // Init tables
        for (int i = 0; i < SINE_VALUES; i++) {
            SinTable[i] = sin(i / 180.0 * M_PI);
            CosTable[i] = cos(i / 180.0 * M_PI);
        }

        // Init palette

        int light = 350;
        int reflect = 130;
        int ambient = 0;
        for (int i = 0; i < COLOURS; i++) {
            double intensity = cos((255 - i) / 512.0 * M_PI);
            int r = CLAMP255(63 * ambient / 255 + 63 * intensity + pow(intensity, reflect) * light);
            int g = CLAMP255(72 * ambient / 255 + 72 * intensity + pow(intensity, reflect) * light);
            int b = CLAMP255(128 * ambient / 255 + 128 * intensity + pow(intensity, reflect) * light);
            //RETRO_SetColor(i, r, g, b);

            colourpalette[i] = blit::Pen(r, g, b);
        }

        // 3d here
        /*
        for (int i = 0; i < 64; i++) {
            colourpalette[i] = blit::Pen(i * 4, i * 2, i * 3);
        }
         */

        // blob here
        // Init palette
        /*
        for (int i = 0; i < COLOURS; i++) {
            colourpalette[i] = blit::Pen(int(i / 2.5), int(i / 1.5), i, 255);
        }
         */


        // Init blob positions
        for (int i = 0; i < NUM_BLOBS; i++) {
            Blobs[i].x = (WIDTH / 2) - BLOB_RADIUS;
            Blobs[i].y = (HEIGHT / 2) - BLOB_RADIUS;
        }

        // Init blob shape
        for (int y = 0; y < BLOB_DRADIUS; y++) {
            for (int x = 0; x < BLOB_DRADIUS; x++) {
                float distance = (y - BLOB_RADIUS) * (y - BLOB_RADIUS) + (x - BLOB_RADIUS) * (x - BLOB_RADIUS);

                if (distance <= BLOB_SRADIUS) {
                    float fraction = distance / BLOB_SRADIUS;
                    Blob[y * BLOB_DRADIUS + x] = (unsigned char) (pow((0.7 - (fraction * fraction)), 3.3) * 255);
                } else {
                    Blob[y * BLOB_DRADIUS + x] = 0;
                }
            }
        }
    }

    void SettingsLayer::update(uint32_t dt, const dang::Gear &gear)
    {
        // Todo dont rely on blit::now but use the delta time instead
        //if (lastStartTime == 0) {
        //    lastStartTime = blit::now();
        //}

        static int frame = 0;
        frame = (frame + 1) % SINE_VALUES;

        // Move balls
        Balls[0].r = 1000;
        Balls[0].x = CosTable[frame] * -100 + (WIDTH / 2);
        Balls[0].y = SinTable[frame] * -10 + (HEIGHT / 2);
        Balls[1].r = 4000;
        Balls[1].x = CosTable[frame] * 10 + (WIDTH / 2);
        Balls[1].y = SinTable[frame] * 60 + (HEIGHT / 2);
        Balls[2].r = 7000;
        Balls[2].x = CosTable[frame] * -130 + (WIDTH / 2);
        Balls[2].y = SinTable[frame] * -80 + (HEIGHT / 2);
        Balls[3].r = 10000;
        Balls[3].x = CosTable[frame] * -80 + (WIDTH / 2);
        Balls[3].y = SinTable[frame] * 70 + (HEIGHT / 2);

        // blobs here
        // Calculate movement
        for (int i = 0; i < NUM_BLOBS; i++) {
            Blobs[i].x += -2 + (5.0 * (rand() / (RAND_MAX + 2.0)));
            Blobs[i].y += -2 + (5.0 * (rand() / (RAND_MAX + 2.0)));
        }

        framenum += dt;
        ivorySphereCoords.x = ivorySphereCoords.initialx + (4*sin( ((float)framenum)*M_PI*2 ));
        ivorySphereCoords.z = ivorySphereCoords.initialz + (2*cos( ((float)framenum)*M_PI*2 ));
        ivorySphereCoords.y = ivorySphereCoords.initialy + fabs(2.75*cos( ((float)((framenum*2)))*M_PI*2 ));
    }

    void SettingsLayer::render(const dang::Gear& gear)
    {

        /*
        // Draw balls
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                float color = 0;
                for (int i = 0; i < NUM_BALLS; i++) {
                    color += Balls[i].r / float ((x - Balls[i].x) * (x - Balls[i].x) + (y - Balls[i].y) * (y - Balls[i].y));
                }
                color = CLAMP255(20 * color);

                blit::screen.pen = colourpalette[int(color)];
                blit::screen.pixel(blit::Point(x, y));
            }
        }
         */

        blit::screen.pen = {200, 20, 200, 255};
        blit::screen.pixel(blit::Point(10,10));

//        if(bRendered) return;

        //raytrace();
        do_render();

        bRendered = true;


        // blobs here
        // Draw blobs
        /*
        for (int i = 0; i < NUM_BLOBS; i++) {
            if (Blobs[i].x > 0 && Blobs[i].x < WIDTH - BLOB_DRADIUS && Blobs[i].y > 0 && Blobs[i].y < HEIGHT - BLOB_DRADIUS) {
                for (int y = 0; y < BLOB_DRADIUS; y++) {
                    for (int x = 0; x < BLOB_DRADIUS; x++) {

                        // getpixel: Pointer to the pixel, so depends on the pixel format. p = screen.ptr(...); p[0] is either the palette index (if format == P) or the red channel (RGB/RGBA)
                        // Though maybe there should be a method on Surface that returns a Pen...

                        // Surface *backdrop = Surface::load(pic)
                        // auto p = backdrop>ptr(Vec2(x,y));
                        // Pen *pen = Pen( p[0], p[1], p[2] );

                        unsigned char color = CLAMP255(RETRO_GetPixel(Blobs[i].x + x, Blobs[i].y + y) + Blob[y * BLOB_DRADIUS + x]);

                        blit::screen.pen = colourpalette[int(color)];
                        blit::screen.pixel(blit::Point(Blobs[i].x + x, Blobs[i].y + y));
                    }
                }
            } else {
                Blobs[i].x = (WIDTH / 2) - BLOB_RADIUS;
                Blobs[i].y = (HEIGHT / 2) - BLOB_RADIUS;
            }
        }
        */

        /*
        // 3d here
        static double angle = 0;
        angle += 0.01;

        Point2D projected[VERTICES];

        for (int i = 0; i < VERTICES; i++) {
            Point3Df rotated = Rotate(Shape[i], cos(angle), sin(angle));
            projected[i] = Project(rotated, EYEDISTANCE, SCALE);
        }

        for (int i = 0; i < FACES; i++) {
            DrawGlenzTriangleFixedPoint(projected[Faces[i][0]], projected[Faces[i][1]], projected[Faces[i][2]], FaceColor[i]);
        }
         */

    }

    /*
    Point3D SettingsLayer::Rotate(Point3D point, float cosvalue, float sinvalue)
    {
        Point3D rotated;

        // Rotate on X
        rotated.y = point.y * cosvalue - point.z * sinvalue;
        rotated.z = point.y * sinvalue + point.z * cosvalue;

        // Rotate on Y
        rotated.x = point.x * cosvalue + rotated.z * sinvalue;
        rotated.z = -point.x * sinvalue + rotated.z * cosvalue;

        // Rotate on Z
        int tmpx = rotated.x * cosvalue - rotated.y * sinvalue;
        rotated.y = rotated.x * sinvalue + rotated.y * cosvalue;
        rotated.x = tmpx;

        return rotated;
    }

    Point3Df SettingsLayer::Rotate(Point3Df point, float cosvalue, float sinvalue)
    {
        Point3Df rotated;

        // Rotate on X
        rotated.y = point.y * cosvalue - point.z * sinvalue;
        rotated.z = point.y * sinvalue + point.z * cosvalue;

        // Rotate on Y
        rotated.x = point.x * cosvalue + rotated.z * sinvalue;
        rotated.z = -point.x * sinvalue + rotated.z * cosvalue;

        // Rotate on Z
        float tmpx = rotated.x * cosvalue - rotated.y * sinvalue;
        rotated.y = rotated.x * sinvalue + rotated.y * cosvalue;
        rotated.x = tmpx;

        return rotated;
    }

    Point3Df SettingsLayer::Rotate(Point3Df point, float ax, float ay, float az)
    {
        Point3Df rotated;

        // Rotate on X
        rotated.y = point.y * cos(ax) - point.z * sin(ax);
        rotated.z = point.y * sin(ax) + point.z * cos(ax);

        // Rotate on Y
        rotated.x = point.x * cos(ay) + rotated.z * sin(ay);
        rotated.z = -point.x * sin(ay) + rotated.z * cos(ay);

        // Rotate on Z
        float tmpx = rotated.x * cos(az) - rotated.y * sin(az);
        rotated.y = rotated.x * sin(az) + rotated.y * cos(az);
        rotated.x = tmpx;

        return rotated;
    }

    Point2D SettingsLayer::Project(Point3D point, int eyedistance, int scale)
    {
        Point2D projected;

        projected.x = WIDTH / 2 + (int) ceil(scale * point.x * eyedistance / (scale * point.z + eyedistance));
        projected.y = HEIGHT / 2 + (int) ceil(scale * point.y * eyedistance / (scale * point.z + eyedistance));

        return projected;
    }

    Point2D SettingsLayer::Project(Point3Df point, int eyedistance, int scale)
    {
        Point2D projected;

        projected.x = WIDTH / 2 + (int) ceil(scale * point.x * eyedistance / (scale * point.z + eyedistance));
        projected.y = HEIGHT / 2 + (int) ceil(scale * point.y * eyedistance / (scale * point.z + eyedistance));

        return projected;
    }

    double SettingsLayer::Distance(Point3D point1, Point3D point2)
    {
        return (point1.x - point2.x) * (point1.x - point2.x) + (point1.y - point2.y) * (point1.y - point2.y) + (point1.z - point2.z) * (point1.z - point2.z);
    }

    void SettingsLayer::SpanFixedPoint(int x1, int y1, int x2, int y2, int *xstart, int *xend)
    {
        if (y2 < y1) {
            SWAP(y1, y2);
            SWAP(x1, x2);
        }

        if (y2 == y1) {
            xstart[y1] = x1;
            xend[y1] = x2;
        } else {
            int x = x1 << 16;
            int step = ((x2 - x1) << 16) / (y2 - y1);
            x += step;
            y1++;

            for (int y = y1; y <= y2; y++) {
                if ((y >= 0) && (y < HEIGHT)) {
                    if (xstart[y] == -1) {
                        xstart[y] = (x >> 16);
                    } else if ((x >> 16) < WIDTH) {
                        xend[y] = (x >> 16);
                    } else {
                        xend[y] = WIDTH-1;
                    }
                }

                x += step;
            }
        }
    }

    void SettingsLayer::DrawGlenzTriangleFixedPoint(Point2D vertice1, Point2D vertice2, Point2D vertice3, unsigned char color)
    {
        int xstart[HEIGHT];
        int xend[HEIGHT];

        for (int i = 0; i < HEIGHT; i++) {
            xstart[i] = -1;
            xend[i] = -1;
        }

        SpanFixedPoint(vertice1.x, vertice1.y, vertice2.x, vertice2.y, xstart, xend);
        SpanFixedPoint(vertice2.x, vertice2.y, vertice3.x, vertice3.y, xstart, xend);
        SpanFixedPoint(vertice3.x, vertice3.y, vertice1.x, vertice1.y, xstart, xend);

        for (int y = 0; y < HEIGHT; y++) {
            if (xstart[y] != -1) {
                int start = xstart[y];
                int end = xend[y];

                if (end == -1) {
                    //RETRO_PutPixel(start, y, color);

                    blit::screen.pen = colourpalette[int(color)];
                    blit::screen.pixel(blit::Point(start, y));

                } else {
                    if (start > end) {
                        SWAP(start, end);
                    }

                    for (int x = start + 1; x <= end; x++) {

                        // TODO fix me

                        //uint8_t* ptr = blit::screen.ptr(x, y);

                        //uint16_t newColor = (ptr + color) / 2;

                        //RETRO_PutPixel(x, y, newColor);

                        //blit::screen.pen = colourpalette[int(newColor)];
                        //blit::screen.pixel(blit::Point(x, y));
                    }
                }
            }
        }
    }

    void SettingsLayer::DrawGouraudTriangle(Point2D vertices[3], unsigned char colors[3])
    {
        int ymin = 0;
        int ymax = 0;

        for (int i = 1; i < 3; i++) {
            if (vertices[i].y < vertices[ymin].y) {
                ymin = i;
            } else if (vertices[i].y > vertices[ymax].y) {
                ymax = i;
            }
        }

        int vstart1 = ymin;
        int vstart2 = ymin;
        int vend1 = ymin + 2;
        int vend2 = ymin + 1;

        if (vend1 >= 3) {
            vend1 -= 3;
        }
        if (vend2 >= 3) {
            vend2 -= 3;
        }

        int xdiff1 = vertices[vend1].x - vertices[vstart1].x;
        int xdiff2 = vertices[vend2].x - vertices[vstart1].x;
        int ydiff1 = vertices[vend1].y - vertices[vstart1].y;
        int ydiff2 = vertices[vend2].y - vertices[vstart1].y;

        if (ydiff1 == 0) {
            ydiff1 = 1;
        }
        if (ydiff2 == 0) {
            ydiff2 = 1;
        }

        int cdiff1 = colors[vend1] - colors[vstart1];
        int cdiff2 = colors[vend2] - colors[vstart2];

        int xcalc1 = 0;
        int xcalc2 = 0;
        int ccalc1 = 0;
        int ccalc2 = 0;

        for (int y = vertices[ymin].y; y <= vertices[ymax].y; y++) {
            int x2 = vertices[vstart1].x + xcalc1 / ydiff1;
            xcalc1 += xdiff1;
            int x1 = vertices[vstart2].x + xcalc2 / ydiff2;
            xcalc2 += xdiff2;
            unsigned char cend = colors[vstart1] + ccalc1 / ydiff1;
            ccalc1 += cdiff1;
            unsigned char cstart = colors[vstart2] + ccalc2 / ydiff2;
            ccalc2 += cdiff2;

            int cincsign;
            if (cend > cstart) {
                cincsign = 1;
            } else {
                cincsign = -1;
            }

            int cdiff = abs(cstart - cend);
            int xdiff = x2 - x1;
            int cinc = cdiff - (xdiff >> 1);

            for (int x = x1; x < x2; x++) {
                RETRO_PutPixel(x, y, cstart);

                while (cinc >= 0) {
                    cstart += cincsign;
                    cinc -= xdiff;
                }

                cinc += cdiff;
            }

            if (y == vertices[vend1].y) {
                vstart1 = vend1;
                vend1 = vend2;
                xdiff1 = vertices[vend1].x - vertices[vstart1].x;
                ydiff1 = vertices[vend1].y - vertices[vstart1].y;
                cdiff1 = colors[vend1] - colors[vstart1];

                if (ydiff1 == 0) {
                    ydiff1 = 1;
                }

                xcalc1 = xdiff1;
                ccalc1 = cdiff1;
            }

            if (y == vertices[vend2].y) {
                vstart2 = vend2;
                vend2 = vend1;
                xdiff2 = vertices[vend2].x - vertices[vstart2].x;
                ydiff2 = vertices[vend2].y - vertices[vstart2].y;
                cdiff2 = colors[vend2] - colors[vstart2];

                if (ydiff2 == 0) {
                    ydiff2 = 1;
                }

                xcalc2 = xdiff2;
                ccalc2 = cdiff2;
            }
        }
    }
    */

}
