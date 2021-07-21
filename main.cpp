#include <iostream>
#include "http.h"

int main(int agrc,char *argv[]){
    Http http;
    http.HttpInit();
    http.HttpLoop();
    return 0;
}