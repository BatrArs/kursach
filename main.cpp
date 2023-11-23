#include "mdfile.h"

int main(int argc, char *argv[])
{
    if(argc == 1) {
        std::cout << "Kalculator"  << std::endl;
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
    while ((opt = getopt(argc, argv, "hf:p:i:e:" ))!=-1 ) {
        switch(opt) {
        case 'h':
            std::cout << "Векторный калькулятор выполняющий действие произведения вектора"  << std::endl;
            std::cout << "-f БД пользователей -p порт -e файл ошибок" << std::endl;
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
    if(er(file_name, file_error)==12) {
        std::cout<<"Error open file"<<std::endl;
        return 1;
    }
    Server Server;

    int s = Server.self_addr(error, file_error, port);// вынес из цикла для безперебойной работы
    while(true) {
        int work_sock = Server.client_addr(s, error, file_error);

        autorized(work_sock, file_name, file_error);
        math(work_sock);
    }
    return 0;
}
