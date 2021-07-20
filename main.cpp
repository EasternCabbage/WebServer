#include <iostream>
#include "http.h"

void main(int agrc,char *argv[]){
    Http http;
    http.HttpInit();
    http.HttpLoop();
}