#ifndef INCLUDE_QSSLIB_H
#define INCLUDE_QSSLIB_H

#include <iostream>
#include <vector> 

#define SUCCEED  0 
#define FAILED  -1
#define PENDING -2
#define CONFIG_PARSE_ERROR -3; 

struct shadowserver {
     std::string ip; 
     std::string pwd; 
     short port; 
};

struct shadowconfig {
    std::vector<shadowserver> server; 
    std::string local_address; 
    short local_port; 
    int time_out; 
    std::string method; 
    int http_proxy; 
    int auth; 
};

class Qsslib {
public:
    static Qsslib* getInstance(); 
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

    std::vector<std::string> m_queuestart; 
    std::vector<std::string> m_queuestop;
    std::string m_status;
    int m_connected; 
    int m_errorno; 
    std::string m_errormsg; 
    shadowconfig m_config; 

	static Qsslib* instance;
};  

#endif /*INCLUDE_QSSLIB_H*/