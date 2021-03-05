// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <iostream>
#include <fonts/barcadebrawl.h>
#include "Gear.hpp"
#include "SettingsLayer.h"
#include "Layer.hpp"
#include "Imagesheet.hpp"

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
    }

    void SettingsLayer::render(const dang::Gear& gear)
    {

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

        // blobs here
        // Draw blobs
        /*
        for (int i = 0; i < NUM_BLOBS; i++) {
            if (Blobs[i].x > 0 && Blobs[i].x < WIDTH - BLOB_DRADIUS && Blobs[i].y > 0 && Blobs[i].y < HEIGHT - BLOB_DRADIUS) {
                for (int y = 0; y < BLOB_DRADIUS; y++) {
                    for (int x = 0; x < BLOB_DRADIUS; x++) {

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
