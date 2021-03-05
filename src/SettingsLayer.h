// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework
#pragma once

#include <list>

#include "Layer.hpp"
#include "32blit.hpp"

#define CLAMP255(n) (n < 0 ? 0 : (n > 255 ? 255 : n))
#define SWAP(x, y) x = x + y; y = x - y; x = x - y;
#define COLOURS 256
#define WIDTH 320
#define HEIGHT 240

#define EYEDISTANCE 256
#define SCALE 50
#define VERTICES 8
#define FACES 12

#define NUM_BLOBS 160
#define BLOB_RADIUS 20
#define BLOB_DRADIUS (BLOB_RADIUS * 2)
#define BLOB_SRADIUS (BLOB_RADIUS * BLOB_RADIUS)


# define M_PI		3.14159265358979323846	/* pi */

namespace pnk
{
    struct Point3Df
    {
        float x, y, z;
    };

    struct Point3D
    {
        int x, y, z;
    };

    struct Point2Df {
        float x, y;
    };

    struct Point2D {
        int x, y;
    };

    //Point3Df Shape[VERTICES] = { {1, 1, 1}, {1, -1, 1}, {-1, -1, 1}, {-1, 1, 1}, {1, 1, -1}, {1, -1, -1}, {-1, -1, -1}, {-1, 1, -1} };
    //int Faces[FACES][4] = { {4, 0, 1, 1}, {4, 5, 1, 1}, {0, 3, 2, 2}, {0, 1, 2, 2}, {3, 7, 6, 6}, {3, 2, 6, 6}, {7, 4, 5, 5}, {7, 6, 5, 5}, {5, 1, 2, 2}, {5, 6, 2, 2}, {7, 3, 0, 0}, {7, 4, 0, 0} };
    //int FaceColor[FACES] = { 40, 45, 20, 25, 30, 35, 40, 45, 50, 55, 20, 25 };

    class SettingsLayer : public dang::Layer
    {
    public:
        SettingsLayer();

        ~SettingsLayer() override = default;

        void update(uint32_t dt, const dang::Gear &gear) override;

        void render(const dang::Gear &gear) override;

    protected:
        explicit SettingsLayer(Layer::E_TYPE type) : Layer(type){};

        #define NUM_BALLS 4
        #define SINE_VALUES 360

        struct MetaBall {
            float x, y, r;
        } Balls[NUM_BALLS];

        float SinTable[SINE_VALUES];
        float CosTable[SINE_VALUES];

        blit::Pen colourpalette[COLOURS];

        blit::Pen backgroundColour;
        blit::Pen foregroundColour;
        blit::Pen highlightColour;

        blit::Rect displayRect;

        unsigned char Blob[BLOB_DRADIUS * BLOB_DRADIUS];
        Point2D Blobs[NUM_BLOBS];

        /*
        Point3D Rotate(Point3D point, float cosvalue, float sinvalue);

        Point3Df Rotate(Point3Df point, float cosvalue, float sinvalue);

        Point3Df Rotate(Point3Df point, float ax, float ay, float az);

        Point2D Project(Point3D point, int eyedistance, int scale);

        Point2D Project(Point3Df point, int eyedistance, int scale);

        double Distance(Point3D point1, Point3D point2);

        void SpanFixedPoint(int x1, int y1, int x2, int y2, int *xstart, int *xend);
        void DrawGlenzTriangleFixedPoint(Point2D vertice1, Point2D vertice2, Point2D vertice3, unsigned char color);
        void DrawGouraudTriangle(Point2D vertices[3], unsigned char colors[3]);
         */

    };
}

