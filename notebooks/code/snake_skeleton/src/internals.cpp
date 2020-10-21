#include "sys/ioctl.h"
#include "termios.h"
#include "stdio.h"
#include "time.h"


namespace internal {

static const int STDIN=0;
static bool initialized = false;

// Comment ca va 

int keyEvent(){
    if( !initialized){
        termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }
    int bytesWaiting;
    //int bytesWaiting;                                                                                                                                                        
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}

void frameSleep(const int& ms){
    clock_t end;
    end = clock() + ms * CLOCKS_PER_SEC/1000;
    while( clock() < end){
        // wait 
    }
}

}