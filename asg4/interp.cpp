// $Id: interp.cpp,v 1.2 2015-07-16 16:57:30-07 - - $

#include <memory>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

#include <GL/freeglut.h>

#include "debug.h"
#include "interp.h"
#include "shape.h"
#include "util.h"

unordered_map<string,interpreter::interpreterfn>
interpreter::interp_map {
   {"define" , &interpreter::do_define },
   {"draw"   , &interpreter::do_draw   },
   {"border" , &interpreter::do_border },
   {"moveby" , &interpreter::do_moveby }
};

unordered_map<string,interpreter::factoryfn>
interpreter::factory_map {
   {"text"           , &interpreter::make_text           },
   {"ellipse"        , &interpreter::make_ellipse        },
   {"circle"         , &interpreter::make_circle         },
   {"polygon"        , &interpreter::make_polygon        },
   {"rectangle"      , &interpreter::make_rectangle      },
   {"square"         , &interpreter::make_square         },
   {"triangle"       , &interpreter::make_triangle       },
   {"equilateral"    , &interpreter::make_equilateral    },
   {"diamond"        , &interpreter::make_diamond        }
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

interpreter::shape_map interpreter::objmap;

//default code

interpreter::~interpreter() {
   for (const auto& itor: objmap) {
      cout << "objmap[" << itor.first << "] = "
           << *itor.second << endl;
   }
}

void interpreter::interpret (const parameters& params) {
   DEBUGF ('i', params);
   param begin = params.cbegin();
   string command = *begin;
   auto itor = interp_map.find (command);
   if (itor == interp_map.end()) throw runtime_error ("syntax error");
   interpreterfn func = itor->second;
   func (++begin, params.cend());
}

void interpreter::do_define (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   string name = *begin;
   objmap.emplace (name, make_shape (++begin, end));
}


void interpreter::do_draw (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   cout << end - begin << endl;
   
   if (end - begin != 4) throw runtime_error ("syntax error");
   string name = begin[1];
   shape_map::const_iterator itor = objmap.find (name);
   if (itor == objmap.end()) {
      throw runtime_error (name + ": no such shape");
   }
   vertex where {from_string<GLfloat> (begin[2]),
                 from_string<GLfloat> (begin[3])};
   rgbcolor color {begin[0]};
   window::push_back(object(itor->second, where, color));
}

void interpreter::do_border(param begin, param end) {
   DEBUGF ('f', range (begin, end));
   string color = *begin;
   *begin++;
   int t =  from_string<int> (*begin);
   window::border_color = color;
   window::thickness = t;
}


void interpreter::do_moveby (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   window::delta = stof(*begin);
}

shape_ptr interpreter::make_shape (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   string type = *begin++;
   auto itor = factory_map.find(type);
   if (itor == factory_map.end()) {
      throw runtime_error (type + ": no such shape");
   }
   factoryfn func = itor->second;
   return func (begin, end);
}

shape_ptr interpreter::make_text (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   //Calls text ctor and makes shared_ptr to text object created.
   string words;
   auto font = shape::fontcode.find(*begin);
   if (font == shape::fontcode.end()){
      throw runtime_error (*begin + ": no such font");
   }
   ++begin;
   while (begin != end){
      words += *begin++ + " ";
   }
   return make_shared<text> (font->second, words);
}

shape_ptr interpreter::make_ellipse (param begin, param end) {
   DEBUGF ('f', range (begin, end));

   return make_shared<ellipse> (GLfloat(stof(begin[0])), 
                                GLfloat(stof(begin[1])));
}

shape_ptr interpreter::make_circle (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   return make_shared<circle> (GLfloat(stof(begin[0])));
}

shape_ptr interpreter::make_polygon (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   vertex_list vertices;
   while (begin != end) {
      vertices.push_back({GLfloat(stof(begin[0])), 
                          GLfloat(stof(begin[1]))});
      begin+=2;
   }
   return make_shared<polygon> (vertices);
}

shape_ptr interpreter::make_rectangle (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   return make_shared<rectangle> (GLfloat(stof(begin[0])),
                                  GLfloat(stof(begin[1])));
}


shape_ptr interpreter::make_square (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   return make_shared<square> (GLfloat(stof(begin[0])));
}

shape_ptr interpreter::make_diamond(param begin, param end) {
   DEBUGF ('f', range (begin, end));
   return make_shared<diamond> (GLfloat(stof(begin[0])), 
                                GLfloat(stof(begin[1])));
}

shape_ptr interpreter::make_triangle (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   vertex_list vertices;
   while (begin != end) {
      vertices.push_back({GLfloat(stof(begin[0])), 
                          GLfloat(stof(begin[1]))});
      begin+=2;
   }
   return make_shared<triangle> (vertices);
}



shape_ptr interpreter::make_equilateral (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   return make_shared<equilateral> (GLfloat(stof(begin[0])));
}



