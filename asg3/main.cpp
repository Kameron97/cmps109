

#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <unistd.h>
#include <fstream>

using namespace std;

#include "listmap.h"
#include "xpair.h"
#include "util.h"

using str_str_map = listmap<string,string>;
using str_str_pair = str_str_map::value_type;

bool equals;


//giver function
void scan_options (int argc, char** argv) {
   opterr = 0;
   for (;;) {
      int option = getopt (argc, argv, "@:");
      if (option == EOF) break;
      switch (option) {
         case '@':
            traceflags::setflags (optarg);
            break;
         default:
            complain() << "-" << (char) optopt << ": invalid option"
               << endl;
            break;
      }
   }
}


//trim()
//used in order to "trim" strim args
string trim(const string& input) {
   size_t first = input.find_first_not_of(" \t");
   size_t last = input.find_last_not_of(" \t");
   if (first == string::npos){
      return "";
   }
   return input.substr(first, last+1-first);
}

//concatenate()
//seperates eky and value inputs
str_str_pair concatenate(const string& dis){
   string Key = "";
   string Value = "";
   size_t pos = dis.find_first_of("=");
   //if diss[0] is a number then....
   if(dis[0] != '#' && dis.size() != 0){
      //key+value is given an input
      Key += trim(dis.substr (0, pos));
      Value += trim(dis.substr (pos+1));
      if( pos == string::npos ){
         equals = false;
         Value = "";
      }
   }
   else {
      equals = false;
   }
   str_str_pair pair(Key, Value);
   return pair;
}

//checks to see what type of storage we are dealing with
int checkCase(str_str_pair pair){
   int type = 0;
   //if emtpy hadd 100 to the value
   if(pair.first != ""){
      type += 100;
   }
   if(equals) {
      type += 10;
   }
   if(pair.second != "") {
      type += 1;
   }
   return type;

}


//evaluate()
void printSol(str_str_map& test, string& filename, istream& in){
   string line = "";
   if(filename.compare("-")){
      ifstream in(filename);
      cin.rdbuf(in.rdbuf());
   }
   for (size_t i = 0; getline(in, line); ++i){
      equals = true;
      cout << filename << ": " << i+1 << ": " << line << endl;
      str_str_pair pair = concatenate(trim(line));

      //checks various cases given by checkcase(). helps determine what
      //to be done by each scenario
      switch (checkCase(pair)) {
         case 100:  test.find(pair.first); break;
         case 110:  test.remove(pair.first); break;
         case 111: test.insert(pair); cout << pair << endl; break;
         case 10:  test.print(); break;
         case 11:  test.printVal(pair.second); break;
         default:  break;
      }
   }
}

int main (int argc, char** argv) {

   //sys_info::set_execname (argv[0]);
   string name = string(argv[0]);
   size_t slashpos = name.find_last_of ('/') + 1;
   name = name.substr (slashpos);
   scan_options (argc, argv);
   str_str_map test;
   string filename = "-";
   for (int i = 0; i < argc; i++) {
      if(i==0) { i++; }
      if(argc >= 2) {
         filename = string(argv[i]);
         ifstream in(filename);
         if(!in.good()){
            cerr << name << ": " << filename <<
               ": No such file or directory" << endl;
         }
         else {
            printSol(test, filename, in);
         }
      }
      else {
         printSol(test, filename, cin);
      }
   }
   return EXIT_SUCCESS;
}
