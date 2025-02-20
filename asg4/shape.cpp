// $Id: shape.cpp,v 1.1 2015-07-16 16:47:51-07 - - $

#include <typeinfo>
#include <unordered_map>
using namespace std;

#include "shape.h"
#include "util.h"
#include "graphics.h"

static unordered_map<void*,string> fontname {
   {GLUT_BITMAP_8_BY_13       , "Fixed-8x13"    },
   {GLUT_BITMAP_9_BY_15       , "Fixed-9x15"    },
   {GLUT_BITMAP_HELVETICA_10  , "Helvetica-10"  },
   {GLUT_BITMAP_HELVETICA_12  , "Helvetica-12"  },
   {GLUT_BITMAP_HELVETICA_18  , "Helvetica-18"  },
   {GLUT_BITMAP_TIMES_ROMAN_10, "Times-Roman-10"},
   {GLUT_BITMAP_TIMES_ROMAN_24, "Times-Roman-24"},
};

static unordered_map<string,void*> fontcode {
   {"Fixed-8x13"    , GLUT_BITMAP_8_BY_13       },
   {"Fixed-9x15"    , GLUT_BITMAP_9_BY_15       },
   {"Helvetica-10"  , GLUT_BITMAP_HELVETICA_10  },
   {"Helvetica-12"  , GLUT_BITMAP_HELVETICA_12  },
   {"Helvetica-18"  , GLUT_BITMAP_HELVETICA_18  },
   {"Times-Roman-10", GLUT_BITMAP_TIMES_ROMAN_10},
   {"Times-Roman-24", GLUT_BITMAP_TIMES_ROMAN_24},
};

ostream& operator<< (ostream& out, const vertex& where) {
   out << "(" << where.xpos << "," << where.ypos << ")";
   return out;
}

shape::shape() {
   DEBUGF ('c', this);
}

text::text (void* glut_bitmap_font, const string& textdata):
      glut_bitmap_font(glut_bitmap_font), textdata(textdata) {
   DEBUGF ('c', this);
}

ellipse::ellipse (GLfloat width, GLfloat height):
dimension ({width, height}) {
   DEBUGF ('c', this);
}

circle::circle (GLfloat diameter): ellipse (diameter, diameter) {
   DEBUGF ('c', this);
}


polygon::polygon (const vertex_list& vertices): vertices(vertices) {
   
   DEBUGF ('c', this);
}

rectangle::rectangle (GLfloat width, GLfloat height):
       polygon({{0, 0}, {width, 0}, {width, height}, {0, height}}) {
   DEBUGF ('c', this << "(" << width << "," << height << ")");
}

square::square (GLfloat width): rectangle (width, width) {
   DEBUGF ('c', this);
}

diamond::diamond (GLfloat width, GLfloat height): 
        polygon({{0-width/2, 0}, {0, 0+height/2},
                 {0+width/2, 0}, {0, 0-height/2} }){
   DEBUGF ('c', this);
}

triangle::triangle (const vertex_list& vertices): polygon(vertices) {

  DEBUGF ('c', this);
}





equilateral::equilateral(GLfloat width): 
    triangle({{-width/2, 0}, {0 ,width/2*sqrtf(3)},
            {width/2,0}}){
 
  DEBUGF ('c', this);

 }


void text::draw (const vertex& center, const rgbcolor& color) const {
 DEBUGF ('d', this << "(" << center << "," << color << ")");
   auto text = reinterpret_cast<const GLubyte*> (textdata.c_str());
   glColor3ubv (color.ubvec);
   glRasterPos2f (center.xpos, center.ypos);
   glutBitmapString (glut_bitmap_font, text);
   glutSwapBuffers();
}

void ellipse::draw (const vertex& center, const rgbcolor& color) const {
  DEBUGF ('d', this << "(" << center << "," << color << ")");
   glBegin (GL_POLYGON);
   glEnable (GL_LINE_SMOOTH);
   glColor3ubv (color.ubvec);

   const float i = 2 * M_PI / 32;
   float width = dimension.xpos;
   float height = dimension.ypos;

   for (float theta = 0; theta < 2 * M_PI; theta += i) {
      float x = width * cos (theta) + center.xpos;
      float y = height * sin (theta) + center.ypos;
      glVertex2f (x, y);
   }
   glEnd();
   cout << "end\n";
}

void polygon::draw (const vertex& center, const rgbcolor& color) const {
   DEBUGF ('d', this << "(" << center << "," << color << ")");

   if (window::draw_border) {
      glLineWidth(window::thickness);
      glBegin(GL_LINE_LOOP);
      glColor3ubv(rgbcolor(window::border_color).ubvec);  
      
      for (vertex v : vertices) {
        float x = center.xpos + v.xpos;
        float y = center.ypos + v.ypos;
        glVertex2f(x,y);
      }
      glEnd();
   }

   glBegin (GL_POLYGON);
   glColor3ubv (color.ubvec);
   for (vertex v : vertices) {
      float x = center.xpos + v.xpos;
      float y = center.ypos + v.ypos;
      glVertex2f(x,y);
   }
  
   glEnd();
}

void shape::show (ostream& out) const {
   out << this << "->" << demangle (*this) << ": ";
}

void text::show (ostream& out) const {
   shape::show (out);
   out << glut_bitmap_font << "(" << fontname[glut_bitmap_font]
       << ") \"" << textdata << "\"";
}

void ellipse::show (ostream& out) const {
   shape::show (out);
   out << "{" << dimension << "}";
}

void polygon::show (ostream& out) const {
   shape::show (out);
   out << "{" << vertices << "}";
}

ostream& operator<< (ostream& out, const shape& obj) {
   obj.show (out);
   return out;
}

