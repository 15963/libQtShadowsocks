#include "qsslib.h"
#include <stdlib.h>
#include <string>
#include <iostream>
#include <assert.h>


void test_gsslib_init_json_right(std::string str) {
     char *p = new char[ str.size() + 1];
     std::copy(str.begin(), str.end(), p);
     p[str.size()] = '\0'; 
     int ret = Qsslib::get_instance()->init(p); 
     assert(ret == 0); 
     printf("ret:%d,parser succeed!\n",ret); 
     delete[] p; 
}

void test_gsslib_init_json_mistake(std::string str) {
     str += "mistake"; 
     char *p = new char[ str.size() + 1];
     std::copy(str.begin(), str.end(), p);
     p[str.size()] = '\0'; 
     int ret = Qsslib::get_instance()->init(p); 
     assert(ret == -3); 
     printf("ret:%d,parser failed!\n",ret);
     delete[] p; 

}

void test_gsslib_start() {
    printf("gsslib start!\n"); 
    int ret = Qsslib::get_instance()->start(18081); 
    printf("start ret:%d\n",ret); 
}

void test_gsslib_stop() {
   int ret = Qsslib::get_instance()->stop(); 
   printf("stop ret:%d\n",ret); 
}

void test_gsslib_get_status() {
   char buff[256]={0};
   printf("gss get status\n"); 
   Qsslib::get_instance()->get_status(buff); 
   std::cout<<"current server:" << buff <<std::endl;
}

void test_gsslib_uninit() {
  int ret = Qsslib::get_instance()->uninit(); 
}

int main(int argc, char** argv ) {
   
    std::string config_format_data ="{\"server\":[{\"ip\":\"192.168.1.11\",\"port\":447,\"password\":\"123445\"}],\"local_address\":\"127.0.0.1\",\"local_port\":1080,\"timeout\":600,\"method\":\"aes-256-cfb\",\"http_proxy\":false,\"auth\": false}";   
   
    test_gsslib_init_json_right(config_format_data); 
    test_gsslib_init_json_mistake(config_format_data); 
    test_gsslib_start(); 
    test_gsslib_get_status();
    //test_gsslib_stop(); 
    //test_gsslib_get_status(); 
    //test_gsslib_uninit();

    
    return 0; 

}