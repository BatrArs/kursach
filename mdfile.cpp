/**
* @file mdfile.cpp
 */
#include "mdfile.h"
#include <random>
#include <string>
/**
 * @brief Class for error handling
 */
ErrorHandler::ErrorHandler()
{

}
	/**
* @brief Method for writing errors to a file
* @param error Error text
 * @param name File name for recording errors
 */
void ErrorHandler::errors(std::string error, std::string name)
{
    std::ofstream file;
    file.open(name, std::ios::app);
    if(file.is_open()) {
        time_t seconds = time(NULL);
        tm* timeinfo = localtime(&seconds);
        file<<error<<':'<<asctime(timeinfo)<<std::endl;
        std::cout << "error: " << error << std::endl;
        file.close();
    }
}
	/**
* @brief Method for handling errors when opening a file
*
* @param file_name File name
 * @param file_error File name for recording errors
 */
int ErrorHandler::er(std::string file_name, std::string file_error)
{
    std::fstream file;
    file.exceptions(std::ifstream::badbit | std::ifstream::failbit);
    try {
        file.open(file_name);
        return 1;
    } catch(const std::exception & ex) {
        std::string error = "error open file";
        errors(error, file_error);
        return 12;
    }
}
/**
 * @brief Class for the server
 * * @param handler handler
* @throw for the server
*/
Server::Server(ErrorHandler handler)
{
    m_errorHandler = handler;
}
	/**
* @brief Method for configuring the server address
*
* @param error Error text
 * @param file_error File name for recording errors
 * @param port Server port
 * @throw self_addr 
 */
int Server::self_addr(string error, string file_error, int port)
{
    int s = socket(AF_INET, SOCK_STREAM, 0);
    int option = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)); //норм сокет
    sockaddr_in * self_addr = new (sockaddr_in);
    self_addr->sin_family = AF_INET;
    self_addr->sin_port = htons(port);
    self_addr->sin_addr.s_addr = inet_addr("127.0.0.1");
    std::cout << "Wait for connect client...\n";
    int b = bind(s,(const sockaddr*) self_addr,sizeof(sockaddr_in));
    struct timeval timeout {55, 0}; //тайм-аут
    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    if(b == -1) {
        std::cout << "Binding error\n";
        error = "error binding";
        m_errorHandler.errors(error, file_error);
        return 1;
    }
    listen(s, SOMAXCONN);
    return s;
}
	/**
* @brief Method for setting up the client's address
*
* @param s Server socket descriptor
 * @param error Error text
 * @param file_error File name for recording errors
 * @throw client_addr
 */
int Server::client_addr(int s, string error, string file_error)
{
    sockaddr_in * client_addr = new sockaddr_in;
    socklen_t len = sizeof (sockaddr_in);
    int work_sock = accept(s, (sockaddr*)(client_addr), &len);
    if(work_sock == -1) {
        std::cout << "Error #2\n";
        error = "error #2";
        m_errorHandler.errors(error, file_error);
        return 1;
    } else {
        //Successful connection to the server
        std::cout << "Successfull client connection!\n";
        //std::cout << work_sock << endl;"\n";
        return work_sock;
    }
}
	/**
* @brief Constructor of the ClientHandler class.
 *@param handler Error handler.
  * @throw ClientHandler
  */
ClientHandler::ClientHandler(ErrorHandler handler)
{
    m_errorHandler = handler;
}

	/**
*@brief Client authorization method.
 *@param work_sock is a socket for working with the client.
 *@param file_name is the name of the file with the user's data.
 *@param file_error is the name of the file for recording errors.
 * @throw autorized
 */
int ClientHandler::autorized(int work_sock, string file_name, string file_error)
{
    std::string ok = "OK";
    std::string err = "ERR";
    std::string error;
    char msg[255];
    bzero(msg, sizeof (msg));

    //Authorization
    recv(work_sock, &msg, sizeof(msg), 0);
    std::string message = msg;
    std::string login, hashq;
    std::fstream file;
    file.open(file_name);
    getline (file, login, ':');
    getline (file, hashq);
    if(message != login){
        msgsend(work_sock,  err);
        error = "Login error";
        m_errorHandler.errors(error, file_error);
        close(work_sock);
        return 1;
    }else{

        
//salt challenge
        std::string salt = generate_salt(16);

        //salt sent to the client
        msgsend(work_sock,  salt);
        bzero(msg, sizeof (msg));
        recv(work_sock, msg, sizeof(msg), 0);
        std::string sah = salt + hashq;
        std::string digest;
        digest = MD(sah);

        //cout << digest << " " << hashq << " " << msg << endl;

        //password verification
        if(digest != msg){
            cout << digest << endl;
            cout << msg << endl;
            msgsend(work_sock,  err);
            error = "Password error";
            m_errorHandler.errors(error, file_error);
            close(work_sock);
            return 1;
        }else{
            msgsend(work_sock,  ok);
        }
    }
    return 1;
}
	/**
*@brief is a method for calculating mathematical operations.
 *@param work_sock is a socket for working with the client.
  * @throw math
 */
int ClientHandler::math(int work_sock)
{
    int32_t kolvo;
    int32_t numb;
    int32_t vect;
    recv(work_sock, &kolvo, sizeof(kolvo), 0);
    //std::cout << "kolvo" << kolvo <<std::endl;
    //cycle of vectors
    for(int j=0; j<kolvo; j++) {
        recv(work_sock, &numb, sizeof(numb), 0);//taking the length for the first vector
//std::cout << "numb" << numb <<std::endl;
        int64_t sum = 1;
        //value cycle
        for(int i=0; i<numb; i++) {
            recv(work_sock, &vect, sizeof(vect), 0);
            //std::cout << "vect" << vect <<std::endl;
            sum = sum*vect;
            //std::cout << "sum " << sum <<std::endl;
        }
        int32_t mfc;
        if (sum > 2147483647)
            mfc = 2147483647;
        else if (sum < -2147483648)
            mfc = -2147483648;
        else
            mfc = sum;
        send(work_sock, &mfc, sizeof(mfc), 0);
    }

    std::cout << "Program finish!" <<std::endl;
    close(work_sock);
    return 1;
}
	/**
*@brief Salt generation method.
 *@param length is the length of the salt.
  * @throw generate_salt
 */
std::string ClientHandler::generate_salt(std::size_t length) 
{
    const std::string CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> distribution(0, CHARACTERS.size() - 1);

    std::string random_string;

    for (std::size_t i = 0; i < length; ++i) {
        random_string += CHARACTERS[distribution(generator)];
    }

    return random_string;
}
	/**
*@brief is the method of sending the message.
 *@param work_sock is a socket for working with the client.
 *@param mess Message to send.
 * @throw msgsend
 */
void ClientHandler::msgsend(int work_sock, string mess)
{
    char *buffer = new char[4096];
    std::strcpy(buffer, mess.c_str());
    send(work_sock, buffer, mess.length(), 0);
    std::memset(buffer, 0, 4096); 
    delete[] buffer; 
}
	/**
*@brief Hash calculation method.
 *@param sah Hash string.
 * @throw MD
 */
std::string ClientHandler::MD(std::string sah)
{
    Weak::MD5 hash;
    std::string digest;
    StringSource(sah, true,  new HashFilter(hash, new HexEncoder(new StringSink(digest)))); // строка-приемник
    return digest;
}
