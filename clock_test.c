#include "headers.h"
int main (){
    int pid = fork();
    if(pid==0){
        execv("./clk.out",NULL);
    }else
    {

    }
    initClk();
    while(1){
        printf("%d\n",getClk());
        sleep(2);

    }
    destroyClk(true);
    return 0;
}