
#include "mdfile.h"
	/**
 * @file main.cpp
 * @brief Main program file
* @param argc Number of command line arguments
* @param argv Array of command line arguments
 */
int main(int argc, char *argv[])
{
    if(argc == 1) {
 std::cout << "Kalculator" << std::endl;
 std::cout << "-h info" << std::endl;
 std::cout << "-f file name" << std::endl;
 std::cout << "-p port" << std::endl;
 std::cout << "-e error file" << std::endl;
 }
 int opt;
 int port = 33333;
 std::string file_name = "/ect/vcalc.conf";
 std::string file_error = "/var/log/vcalc.log";
 std::string error;
 while ((opt = getopt(argc, argv, "hf:p:i:e:" ) )!=-1 ) {
switch(opt) {
case 'h':
std::cout << "Vector calculator performing the action of the product of a vector" << std::endl;
 std::cout << "-f user database -p port -e error file" << std::endl;
 std::cout << "-h info" << std::endl;
 std::cout << "-f file name" << std::endl;
 std::cout << "-p port" << std::endl;
 std::cout << "-e error file" << std::endl;
            return 1;
            break;
        case 'f': {
            file_name = std::string(optarg);
        };
            break;
        case 'p': {
            port = stoi(std::string(optarg));
        }
        case 'e': {
            file_error = std::string(optarg);
        };
            break;
        }
    }
    ErrorHandler errorHandler;
    if(errorHandler.er(file_name, file_error)==12) {
        std::cout<<"Error open file"<<std::endl;
        return 1;
    }
    Server Server(errorHandler);
    ClientHandler clientHandler(errorHandler);
    int s = Server.self_addr(error, file_error, port);//removed from the loop for trouble-free operation
 while(true) {
        int work_sock = Server.client_addr(s, error, file_error);
 clientHandler.autorized(work_sock, file_name, file_error);
 clientHandler.math(work_sock);
 }
 return 0;
}


