#include <UnitTest++/UnitTest++.h>
#include "mdfile.h"
using namespace std;

struct Cons_fix {
	ErrorHandler * p;
	Cons_fix() {
		p = new ErrorHandler();
	}
	~Cons_fix() {
		delete p;
	}
};

SUITE(Connect_to_server)
{
	TEST_FIXTURE(Cons_fix, NormalTestCase){
		p->errors("error2","error.txt");
		CHECK(true);	
	}

	TEST_FIXTURE(Cons_fix, NoNormalTestCase){
		CHECK_THROW(p->errors("error2","error.xt"),error_server);
			
	}
	
}
 
struct Cons_fixx {
    Server* q;
    Cons_fixx() {
        ErrorHandler handler;
        q = new Server(handler);
    }
    ~Cons_fixx() {
        delete q;
    }
};
SUITE(Conn)
{

	TEST_FIXTURE(Cons_fixx, InvalidServerAddress){
		CHECK_THROW(q->client_addr(-1,"2130706433", "33333"), error_server);	
	}
}

struct Cons_fixxx {
    ClientHandler* r;
    Cons_fixxx() {
        ErrorHandler handler;
        r = new ClientHandler(handler);
    }
    ~Cons_fixxx() {
        delete r;
    }
};
SUITE(Connt)
{

	TEST_FIXTURE(Cons_fixxx, 2){
    CHECK_THROW(r->autorized(-1, "2130707717", "33333"), error_server); 
	}

	TEST_FIXTURE(Cons_fixxx, 3){
    CHECK_THROW(r->autorized(10, "base.txt", "error.txt"), error_server); 
	}

	TEST_FIXTURE(Cons_fixxx, 4){
    CHECK_THROW(r->autorized(10, "base.txt", "33333"), error_server); 
	}


}


int main(int argc, char **argv)
{
	return UnitTest::RunAllTests();
}
