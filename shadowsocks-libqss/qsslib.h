#ifndef INCLUDE_QSSLIB_H
#define INCLUDE_QSSLIB_H

#include "error.h"
#include "client.h"
#include <iostream>
#include <vector> 
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>


#define BOOST_THREAD_SLEEP(n) boost::thread::sleep(boost::get_system_time() + boost::posix_time::millisec(n))

typedef struct shadow_server_tag {
     std::string ip; 
     std::string pwd; 
     std::string port; 
} shadow_server, *pshadow_server;

typedef struct shadow_config_tag {
    std::vector<shadow_server> server; 
    std::string local_address; 
    std::string local_port; 
    std::string time_out; 
    std::string method; 
    int http_proxy; 
    int auth; 
} shadow_config, *pshadow_config;

class Qsslib {
public:
     int  init(char* config);
     int  start(unsigned short port); 
     int  stop(); 
     int  get_status(char* res); 
     int  uninit(); 
     int  get_thread_isrun();
     void set_current_server(shadow_server srv);
     void set_server_status(int status);
     int  get_server_status();  
     shadow_config get_shadow_config(); 
     boost::mutex&  get_thread_lock(); 
     static Qsslib* get_instance(); 
     static void destroy_instance(); 
     static void shadow_connect_thread(void *pdata); 
     const std::string get_error_message() const; 
     Client& get_shadows_client();    
private:
	 Qsslib();
	 Qsslib(const Qsslib&);
	 Qsslib& operator=(const Qsslib&);
     int parse_config(char* config);
     shadow_server m_current_server ;
     int m_status; 
     int m_error_no; 
     int m_thread_run;
     std::string    m_error_msg; 
     shadow_config  m_config; 
     boost::mutex   m_thread_lock; 
 	 static Qsslib* m_instance;
     Client         m_shadows_client; 
};  

#endif /*INCLUDE_QSSLIB_H*/
