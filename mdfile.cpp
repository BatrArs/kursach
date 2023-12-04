#include "mdfile.h"
void msgsend(int work_sock, string mess)
{
    char *buffer = new char[4096];
    std::strcpy(buffer, mess.c_str());
    send(work_sock, buffer, mess.length(), 0);
}


std::string MD(std::string sah)
{
    Weak::MD5 hash;
    std::string digest;
    StringSource(sah, true,  new HashFilter(hash, new HexEncoder(new StringSink(digest))));  // строка-приемник
    return digest;
}
void errors(std::string error, std::string name)
{
    std::ofstream file;
    file.open(name, std::ios::app);
    if(file.is_open()) {
        time_t seconds = time(NULL);
        tm* timeinfo = localtime(&seconds);
        file<<error<<':'<<asctime(timeinfo)<<std::endl;
        std::cout << "error: " << error << std::endl;
    }
}
int er(std::string file_name, std::string file_error)
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

int Server::self_addr(string error, string file_error, int port)
{
    int s = socket(AF_INET, SOCK_STREAM, 0);
    int option = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));//норм сокет
    sockaddr_in * self_addr = new (sockaddr_in);
    self_addr->sin_family = AF_INET;
    self_addr->sin_port = htons(port);
    self_addr->sin_addr.s_addr = inet_addr("127.0.0.1");
    std::cout << "Wait for connect client...\n";
    int b = bind(s,(const sockaddr*) self_addr,sizeof(sockaddr_in));
    struct timeval timeout {55, 0};//тайм-аут
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    if(b == -1) {
        std::cout << "Binding error\n";
        error = "error binding";
        errors(error, file_error);
        return 1;
    }
    listen(s, SOMAXCONN);
    return s;
}

int Server::client_addr(int s, string error, string file_error)
{
    sockaddr_in * client_addr = new sockaddr_in;
    socklen_t len = sizeof (sockaddr_in);
    int work_sock = accept(s, (sockaddr*)(client_addr), &len);
    if(work_sock == -1) {
        std::cout << "Error #2\n";
        error = "error #2";
        errors(error, file_error);
        return 1;
    } else {
        //Успешное подключение к серверу
        std::cout << "Successfull client connection!\n";
        return work_sock;
    }
}

int autorized(int work_sock, string file_name, string file_error)
{

    std::string ok = "OK";
    std::string salt = "2D2D2D2D2D2D2D22";
    std::string err = "ERR";
    std::string error;
    char msg[255];
    bzero(msg, sizeof (msg));

    //Авторизация
    recv(work_sock, &msg, sizeof(msg), 0);
    std::string message = msg;
    std::string login, hashq;
    std::fstream file;
    file.open(file_name);
    getline (file, login, ':');
    getline (file, hashq);

    //cout << message << " " << login << endl;

    //Сверка логина
    if(message != login){
        msgsend(work_sock,  err);
        error = "Ошибка логина";
        errors(error, file_error);
        close(work_sock);
        return 1;
    }else{


        //соль отправленная клиенту
        msgsend(work_sock,  salt);
        bzero(msg, sizeof (msg));
        recv(work_sock, msg, sizeof(msg), 0);
        std::string sah = salt + hashq;
        std::string digest;
        digest = MD(sah);

        //cout << digest << " " << hashq << " " << msg << endl;

        //Сверка пароля
        if(digest != msg){
            cout << digest << endl;
            cout << msg << endl;
            msgsend(work_sock,  err);
            error = "Ошибка пароля";
            errors(error, file_error);
            close(work_sock);
            return 1;
        }else{
            msgsend(work_sock,  ok);
        }
    }
    return 1;
}




int math(int work_sock)
{
    int32_t kolvo;
    int32_t numb;
    int32_t vect;
    recv(work_sock, &kolvo, sizeof(kolvo), 0);
    //std::cout << "kolvo" << kolvo <<std::endl;
    //цикл векторов
    for(int j=0; j<kolvo; j++) {
        recv(work_sock, &numb, sizeof(numb), 0);//прием длинны для первого вектора
        //std::cout << "numb" << numb <<std::endl;
        int64_t sum = 1;
        //цикл значений
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

