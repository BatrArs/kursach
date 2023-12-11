/**
* @file mdfile.h
* @brief Description of the ErrorHandler, Server, ClientHandler and error_server classes
 * @author Arseniy Batrakov
 * @version 1.0
 * @date 10.12.2023
* @brief Header file for Connect
 */

#include <netinet/in.h>
#include <iostream>
#include <cassert>
#include <arpa/inet.h>
#include <cstdlib>
#include <unistd.h>
#include <ctime>
#include <fstream>
#include <sstream>
#include <string>
#include <cryptopp/cryptlib.h>
#include <iostream>
#include <vector>
#include <getopt.h>
#include <cryptopp/hex.h> // HexEncoder
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
using namespace CryptoPP;
using namespace std;
#include <cryptopp/md5.h> 

/**
 * @brief Class for error handling
 * @details for error handling
 */

class ErrorHandler
{
public:
    ErrorHandler();
    static void errors(std::string error, std::string name);

    static int er(std::string file_name, std::string file_error);
};
/**
 * @brief Class for the server
 * @details for the server
 */
class Server
{
public:
    Server(ErrorHandler handler);

    int self_addr(string error, string file_error, int port);

    int client_addr(int s, string error, string file_error);

private:

    ErrorHandler m_errorHandler;
};
/**
 * @brief Class for client processing
 *  @details for client processing
 */

class ClientHandler
{
public:
    ClientHandler(ErrorHandler handler);
    int autorized(int work_sock, string file_name, string file_error);
    int math(int work_sock);

private:
    std::string generate_salt(std::size_t length);
   
    void msgsend(int work_sock, string mess);
    
    std::string MD(std::string sah);

    ErrorHandler m_errorHandler;
};

/** @brief Class error
* @details Output error message
*/
class error: public invalid_argument
{
public:
	explicit error (const std::string& what_arg):
		std::invalid_argument(what_arg) {}
	explicit error (const char* what_arg):
		std::invalid_argument(what_arg) {}
};

