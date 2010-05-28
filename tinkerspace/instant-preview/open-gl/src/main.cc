//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2009  Sebastian Mach (*1983)
// * mail: phresnel/at/gmail/dot/com
// * http://phresnel.org
// * http://picogen.org
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//#define STANDALONE 1

#if STANDALONE
# include <SDL/SDL.h>
# include <SDL/SDL_opengl.h>
#else
# include <GL/gl.h>
# include <GL/glu.h>
#endif

#include <cmath>
#include <ctime>


#include "main.hh"


namespace {


float heightFunction (float u, float v) {
        return 1.0f * sinf(u*1.0f) * sinf (v*1.0f);
}



void setupQuality () {
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        glHint (GL_POLYGON_SMOOTH_HINT, GL_NICEST);
        glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
        glEnable (GL_LINE_SMOOTH);
        glEnable(GL_POLYGON_SMOOTH);
}



void setupLighting () {
        const float noAmbient[] = {0.3f, 0.3f, 0.4f, 1.0f};
        const float f=1.0f, whiteDiffuse[] = {f*1.0f, f*1.0f, f*1.0f, 1.0f};
        const float position[] = {1.0f, -1.0f, 0.5f, 0.0f};
        glLightfv(GL_LIGHT0, GL_AMBIENT, noAmbient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteDiffuse);
        glLightfv(GL_LIGHT0, GL_POSITION, position);
        glEnable (GL_LIGHT0);
        glEnable (GL_LIGHTING);
        glShadeModel(GL_SMOOTH);
}



void setupCamera () {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(70.0f,(GLfloat)800/(GLfloat)600,0.1f,500.0f);


        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0,5.0,15.0f, 0.0,0.0,0.0, 0,1,0);
        glScalef (1.0f,1.0f,-1.0f);

        //glTranslatef(-0.0f,-5.0f,-16.0f);
        /*const float
                c = 0.0f,//static_cast<float>(clock ()) / static_cast<float>(CLOCKS_PER_SEC),
                r = 10.0f + 5.0f * sinf (c)
        ;
        gluLookAt(r * sinf(c), 5.0f * sinf(c*0.25f), r * cosf(c), 0.0,0.0,0.0, 0,1,0);*/
}



void setupFillmode () {
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        //glEnable(GL_CULL_FACE);
        //glCullFace (GL_BACK);
}

} // namespace {


namespace instant_preview {
void draw (const HeightFunction &heightFunction) {
        glClearColor (0.5f, 0.5f, 0.5f, 1.0f);
        glClearDepth (1.0f);
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        setupQuality();
        setupLighting();
        setupCamera();

        const int width = 128, height = width;
        const float
                widthf = static_cast<float>(width),
                heightf = static_cast<float>(height),
                rwidthf = 1.0f / widthf,
                rheightf = 1.0f / heightf
        ;

        const float
                scale = 10.0f
        ;

        glColor3f(1.0f, 1.0f, 1.0f);

        for (int v=0; v<height; ++v) {
                const float
                        dv0 = static_cast<float>(v)   * rheightf,
                        dv1 = static_cast<float>(v+1)   * rheightf,
                        dv2 = static_cast<float>(v+2)   * rheightf,
                        dv3 = static_cast<float>(v+3)   * rheightf,
                        v0 = scale * (dv0 - 0.5f),
                        v1 = scale * (dv1 - 0.5f),
                        v2 = scale * (dv2 - 0.5f),
                        v3 = scale * (dv3 - 0.5f)
                ;

                glBegin (GL_QUAD_STRIP);
                for (int u=0; u<=width; ++u) {
                        const float
                                du0 = static_cast<float>(u)   * rwidthf,
                                du1 = static_cast<float>(u+1)   * rwidthf,
                                du2 = static_cast<float>(u+2)   * rwidthf,
                                u0 = scale * (du0 - 0.5f),
                                u1 = scale * (du1 - 0.5f),
                                u2 = scale * (du2 - 0.5f)
                        ;

                        // Invoke height function and pump into vertices.
                        const float
                                h01[3] = {u0, heightFunction (du0, dv1), v1},
                                h02[3] = {u0, heightFunction (du0, dv2), v2},

                                h10[3] = {u1, heightFunction (du1, dv0), v0},
                                h11[3] = {u1, heightFunction (du1, dv1), v1},
                                h12[3] = {u1, heightFunction (du1, dv2), v2},
                                h13[3] = {u1, heightFunction (du1, dv3), v3},

                                h21[3] = {u2, heightFunction (du2, dv1), v1},
                                h22[3] = {u2, heightFunction (du2, dv2), v2}
                        ;

                        // Grab u/v vectors.
                        const float
                                u11_[3] = {h21[0]-h01[0], h21[1]-h01[1], h21[2]-h01[2]},
                                u12_[3] = {h22[0]-h02[0], h22[1]-h02[1], h22[2]-h02[2]},
                                v11_[3] = {h12[0]-h10[0], h12[1]-h10[1], h12[2]-h10[2]},
                                v12_[3] = {h13[0]-h11[0], h13[1]-h11[1], h13[2]-h11[2]}
                        ;

                        // Normalise.
                        const float
                                l11 = 1.0f / sqrtf (u11_[0]*u11_[0] + u11_[1]*u11_[1] + u11_[2]*u11_[2]),
                                l12 = 1.0f / sqrtf (u12_[0]*u12_[0] + u12_[1]*u12_[1] + u12_[2]*u12_[2]),

                                u11[3] = {l11*u11_[0], l11*u11_[1], l11*u11_[2]},
                                u12[3] = {l12*u12_[0], l12*u12_[1], l12*u12_[2]},

                                v11[3] = {l11*v11_[0], l11*v11_[1], l11*v11_[2]},
                                v12[3] = {l12*v12_[0], l12*v12_[1], l12*v12_[2]}
                        ;

                        // Get cross products from normalised u/v -> our vertex normals.
                        const float
                                cross11[3] = {
                                        u11[1]*v11[2] - u11[2]*v11[1],
                                        u11[2]*v11[0] - u11[0]*v11[2],
                                        u11[0]*v11[1] - u11[1]*v11[0]
                                },
                                cross12[3] = {
                                        u12[1]*v12[2] - u12[2]*v12[1],
                                        u12[2]*v12[0] - u12[0]*v12[2],
                                        u12[0]*v12[1] - u12[1]*v12[0]
                                }
                        ;

                        // Draw the stuff.
                        glNormal3fv (cross11);
                        glVertex3fv (h11);
                        glNormal3fv (cross12);
                        glVertex3fv (h12);
                }
                glEnd ();
        }

        glFlush();
}
}


#if STANDALONE

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

namespace {
int standalone () {
        if (0 != SDL_Init (SDL_INIT_VIDEO)) {
                return 1;
        }

        SDL_WM_SetCaption ("picogen - instant open-gl preview", "picogen");
        SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
        SDL_SetVideoMode (800, 600, 32, SDL_OPENGL);

        struct fun : public instant_preview::HeightFunction {
                float operator () (float u, float v) const {
                        return sinf(u) * cosf (v);
                }
        };

        bool done = false;
        while (!done) {
                SDL_Event event;
                while (SDL_PollEvent(&event)) {
                        switch(event.type) {
                        case SDL_QUIT:
                                done = true;
                                break;
                        case SDL_KEYDOWN:
                                if (event.key.keysym.sym == SDLK_ESCAPE)
                                        done = true;
                                break;
                        default:
                                break;
                        }
                }
                draw (fun());
                SDL_GL_SwapBuffers();
        }
        return 0;
}
} // namespace {


int main (int, char*[]) {
        return standalone ();
}

#endif
