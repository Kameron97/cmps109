// $Id: cix.cpp,v 1.1 2017-08-15 16:31:59-07 - - $

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <cstring>
using namespace std;

#include <libgen.h>
#include <sys/types.h>
#include <unistd.h>

#include "protocol.h"
#include "logstream.h"
#include "sockets.h"

logstream log (cout);
struct cix_exit: public exception {};

unordered_map<string,cix_command> command_map {
   {"exit", cix_command::EXIT},
   {"help", cix_command::HELP},
   {"ls"  , cix_command::LS  },
   {"get" , cix_command::GET },
   {"rm" , cix_command::RM },
   {"put" , cix_command::PUT },
};
void cix_help() {
   static const vector<string> help = {
      "exit         - Exit the program.  Equivalent to EOF.",
      "get filename - Copy remote file to local host.",
      "help         - Print help summary.",
      "ls           - List names of files on remote server.",
      "put filename - Copy local file to remote host.",
      "rm filename  - Remove file from remote server.",
   };
   for (const auto& line: help) cout << line << endl;
}

void cix_ls (client_socket& server) {
   cix_header header;
   header.command = cix_command::LS;
   log << "sending header " << header << endl;
   send_packet (server, &header, sizeof header);
   recv_packet (server, &header, sizeof header);
   log << "received header " << header << endl;
   if (header.command != cix_command::LSOUT) {
      log << "sent LS, server did not return LSOUT" << endl;
      log << "server returned " << header << endl;
   }else {
      char buffer[header.nbytes + 1];
      recv_packet (server, buffer, header.nbytes);
      log << "received " << header.nbytes << " bytes" << endl;
      buffer[header.nbytes] = '\0';
      cout << buffer;
   }
}

void cix_get(client_socket& server, string filename) {
    cix_header header;
    header.command = cix_command::GET;
   filename.copy (header.filename, filename.size());

    log << "sending header " << header << endl;
    send_packet(server, &header, sizeof header);
    recv_packet(server, &header, sizeof header);
    log << "received header " << header << endl;


    if (header.command != cix_command::FILE) {
        log << "Error: No such file: " << header.filename << endl;
        return;
    }
    else{
      char buffer[header.nbytes + 1];
      recv_packet(server, buffer, header.nbytes);
      log << "received " << header.nbytes << " bytes" << endl;
      buffer[header.nbytes] = '\0';
      ofstream outfile (filename, ofstream::out);
      outfile.write(buffer, sizeof buffer);
      outfile.close();
    }
    
}

void cix_rm(client_socket& server, string filename){
   cix_header header;
   header.command = cix_command::RM;
   filename.copy (header.filename, filename.size());


   log << "sending header " << header << endl;
   send_packet (server, &header, sizeof header);
   recv_packet (server, &header, sizeof header);
   log << "received header " << header << endl;

   if (header.command != cix_command::ACK) {
      log << "Error: No such file: " << header.filename << endl;
      return;
   }
   else{ 
       log << header.filename << ": file succesfully removed." << endl;
    }
}

void cix_put (client_socket& server, string& filename) {
   cix_header header;
   header.command = cix_command::PUT;
   filename.copy (header.filename, filename.size());

   string getFile;
   char buffer;

   ifstream file (filename);

   if (file.fail()) {
      log << "Error: No such file: " << header.filename << endl;
    return;
   }
   else{
       while(file.good()){
      file.get(buffer);
      getFile.push_back(buffer);
    }
   }

   
   file.close();
   header.nbytes = getFile.size();

   log << "sending header " << header << endl;
   send_packet (server, &header, sizeof header);
   send_packet (server, getFile.c_str(), getFile.size());
   log << "sent " << getFile.size() << " bytes" << endl;
   recv_packet (server, &header, sizeof header);
   log << "received header " << header << endl;

   if (header.command != cix_command::ACK) {
      log << "sent CIX_PUT, server did not return CIX_ACK" << endl;
      log << "server returned " << header << endl;
      log << "put: " << filename << ": " 
          << strerror(header.nbytes) << endl;
   }
    else{
     log << "put: " << filename << ": OK" << endl;
  }
}




void usage() {
   cerr << "Usage: " << log.execname() << " [host] [port]" << endl;
   throw cix_exit();
}

int main (int argc, char** argv) {
   log.execname (basename (argv[0]));
   log << "starting" << endl;
   vector<string> args (&argv[1], &argv[argc]);
   string host;
   in_port_t port;
   if (args.size() > 2) usage();
   if (args.size() == 1) {
        host = get_cix_server_host(args, 1);//for ignoring args
        port = get_cix_server_port(args, 0);
   }else{
      host = get_cix_server_host (args, 0);
      port = get_cix_server_port (args, 1);
    }
   log << to_string (hostinfo()) << endl;
   try {
      log << "connecting to " << host << " port " << port << endl;
      client_socket server (host, port);
      log << "connected to " << to_string (server) << endl;
      for (;;) {
         string line;
         getline (cin, line);
         if (cin.eof()) throw cix_exit();
         log << "command: " << line << endl; //entire command
         size_t fst_space_pos = line.find(" ");
         string cmd_str = line.substr(0, fst_space_pos);
         string args = (fst_space_pos != string::npos
               ? line.substr(fst_space_pos+1, line.size())
               : "");
         
         const auto& itor = command_map.find (cmd_str);
         cix_command cmd = itor == command_map.end()
                         ? cix_command::ERROR : itor->second;
         switch (cmd) {
            case cix_command::EXIT:
               throw cix_exit();
               break;
            case cix_command::HELP:
               cix_help();
               break;
            case cix_command::LS:
               cix_ls (server);
               break;
            case cix_command::GET:
               cix_get (server, args);
               break;
            case cix_command::RM:
               cix_rm (server, args);
               break;
           case cix_command::PUT:
              cix_put (server, args);
               break;
            default:
               log << cmd_str << ": invalid command" << endl;
               break;
         }
      }
   }catch (socket_error& error) {
      log << error.what() << endl;
   }catch (cix_exit& error) {
      log << "caught cix_exit" << endl;
   }
   log << "finishing" << endl;
   return 0;
}
