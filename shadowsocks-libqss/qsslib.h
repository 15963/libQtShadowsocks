#ifndef INCLUDE_QSSLIB_H
#define INCLUDE_QSSLIB_H

#include <iostream>
#include <vector> 

typedef struct shadow_server_tag {
     std::string ip; 
     std::string pwd; 
     unsigned short port; 
} shadow_server, *pshadow_server;

typedef struct shadow_config_tag {
    std::vector<shadow_server> server; 
    std::string local_address; 
    unsigned short local_port; 
    int time_out; 
    std::string method; 
    int http_proxy; 
    int auth; 
} shadow_config, *pshadow_config;

class Qsslib {
public:
    static Qsslib* get_instance(); 
     int init(char* config);
     int start(unsigned short port); 
     int stop(); 
     int get_status(char* res); 
     int unint(); 
private:
	Qsslib();
	Qsslib(const Qsslib&);
	Qsslib& operator=(const Qsslib&);
    std::string get_current_datetime(); 
    int parse_config(char* config);
    shadow_server m_current_server ;
    int m_status; 
    int m_errorno; 
    std::string m_error_msg; 
    shadow_config m_config; 

	static Qsslib* m_instance;
};  

#endif /*INCLUDE_QSSLIB_H*/