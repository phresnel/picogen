
#define STANDALONE 1

#if STANDALONE
# include <SDL/SDL.h>
# include <SDL/SDL_opengl.h>
#else
# include <GL/gl.h>
# include <GL/glu.h>
#endif

#include <cmath>


#include "main.hh"


namespace {
        

float heightFunction (float u, float v) {
        return 0.1f * sinf(u*3.0f) * sinf (v*3.0f);
}


void draw () {
        glClearColor (0.25f, 0.25f, 0.25f, 1.0f);
        glClearDepth (1.0f);
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        

        glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        
        
        glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)800/(GLfloat)600,0.1f,500.0f);


        glMatrixMode(GL_MODELVIEW);        
        glLoadIdentity();
        gluLookAt(0,1,-10, 0,0,0, 0,1,0);
        
        
        const float noAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
        const float whiteDiffuse[] = {1.0f, 0.75f, 0.5f, 1.0f};
        const float position[] = {1.0f, 1.0f, 0.0f, 0.0f};
        glLightfv(GL_LIGHT0, GL_AMBIENT, noAmbient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteDiffuse);
        glLightfv(GL_LIGHT0, GL_POSITION, position);
        glEnable (GL_LIGHT0);
        glEnable (GL_LIGHTING);
        glShadeModel(GL_FLAT);

        
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
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glBegin (GL_QUADS);
        for (int v=0; v<height; ++v) {
                const float 
                        vf0 = scale * (static_cast<float>(v) * rheightf - 0.5f),
                        vf1 = scale * (static_cast<float>(1+v) * rheightf - 0.5f)
                ;
                for (int u=0; u<width; ++u) {
                        const float 
                                uf0 = scale * (static_cast<float>(u) * rwidthf - 0.5f),
                                uf1 = scale * (static_cast<float>(1+u) * rwidthf - 0.5f)
                        ;
                        
                        const float
                                h00 = heightFunction (uf0, vf0),
                                h01 = heightFunction (uf0, vf1),
                                h10 = heightFunction (uf1, vf0),
                                h11 = heightFunction (uf1, vf1)
                        ;
                        
                        glVertex3f (uf0, h00, vf0);
                        glVertex3f (uf1, h10, vf0);
                        glVertex3f (uf1, h11, vf1);
                        glVertex3f (uf0, h01, vf1);
                }
        }
        glEnd ();
        
        glFlush();
}


} // namespace {

        
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
                draw ();
                SDL_GL_SwapBuffers();                
        }
        return 0;
}
} // namespace {
#endif



int main () {
        return standalone ();
}
