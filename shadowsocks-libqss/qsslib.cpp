#include "qsslib.h"
#include <signal.h>
#include <iostream>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QDateTime>
#include <boost/bind.hpp>

using namespace QSS;
using namespace std; 
Qsslib* Qsslib::m_instance = NULL; 

Qsslib::Qsslib() {
}

Qsslib::Qsslib(const Qsslib&) {
}

Qsslib& Qsslib::operator=(const Qsslib&) {
}

int Qsslib::parse_config(char* config) {  
    std::string param(config); 
    QJsonParseError json_error;
    QByteArray jsonstr(config); 
    QJsonDocument doc = QJsonDocument::fromJson(jsonstr,&json_error); 
    if (!doc.isNull() && (json_error.error == QJsonParseError::NoError)) {
       if (doc.isObject()) {
           QJsonObject object = doc.object(); 
           if (object.contains("server")) {
               QJsonValue value = object.value("server");  
               if (value.isArray()) {
                   QJsonArray array = value.toArray();       
                   int nsize = array.size();
                   for (int i=0; i< nsize; i++) {
                        QString svr_ip; 
                        QString svr_pwd; 
                        QString svr_port;                         
                        QJsonObject item_obj = array.at(i).toObject(); 
                        if (item_obj.contains("ip")) {
                             QJsonValue svr_ip_value = item_obj.value("ip");
                             if (svr_ip_value.isString()) {
                                 svr_ip = svr_ip_value.toString(); 
                             }
                        }
                        if (item_obj.contains("port")) {
                             QJsonValue svr_port_value = item_obj.value("port");
                             if (svr_port_value.isString()) {
                                 svr_port = svr_port_value.toString(); 
                             }
                        }
                        if (item_obj.contains("password")) {
                             QJsonValue svr_pwd_value = item_obj.value("password");
                             if (svr_pwd_value.isString()) {
                                 svr_pwd = svr_pwd_value.toString(); 
                             }
                        }
                        shadow_server svr_base_tag;
                        std::string server_ip(svr_ip.toUtf8().constData()); 
                        svr_base_tag.ip=(server_ip); 
                        std::string server_port(svr_port.toUtf8().constData()); 
                        svr_base_tag.port=(server_port); 
                        std::string server_pwd(svr_pwd.toUtf8().constData()); 
                        svr_base_tag.pwd=(server_pwd); 
                        m_config.server.push_back(svr_base_tag); 
                   }
               }                           
           }
           if (object.contains("local_address")) {
               QJsonValue lo_addr_value = object.value("server");  
               if (lo_addr_value.isString()) {
                   m_config.local_address = lo_addr_value.toString().toUtf8().constData(); 
               }
           }
           if (object.contains("local_port")) {
               QJsonValue lo_port_value = object.value("local_port");  
               if (lo_port_value.isString()) {
                   m_config.local_port = lo_port_value.toString().toUtf8().constData(); 
               }
           }
           if (object.contains("timeout")) {
               QJsonValue lo_timeout_value = object.value("timeout");  
               if (lo_timeout_value.isString()) {
                   m_config.time_out = lo_timeout_value.toString().toUtf8().constData(); 
               }
           }
           if (object.contains("method")) {
               QJsonValue lo_method_value = object.value("method");  
               if (lo_method_value.isString()) {
                   m_config.method = lo_method_value.toString().toUtf8().constData(); 
               }
           }
           if (object.contains("http_proxy")) {
               QJsonValue lo_http_value = object.value("http_proxy");  
               if (lo_http_value.isBool()) {
                   m_config.http_proxy = (lo_http_value.toVariant().toBool() == true) ? 1: 0;
               }
           }
           if (object.contains("auth")) {
               QJsonValue lo_auth_value = object.value("auth");  
               if (lo_auth_value.isBool()) {
                   m_config.auth = (lo_auth_value.toVariant().toBool() == true) ? 1: 0;
               }
           }
       }
    } else {
       m_error_no = CONFIG_PARSE_ERROR; 
       m_error_msg = "json data parse error, data:" + param; 
       return CONFIG_PARSE_ERROR;
    }
    return SUCCEED; 
}

int Qsslib::init(char* config) {
    m_status = FAILED;
    memset(&m_current_server,0,sizeof(m_current_server));
    m_error_no = 0; 
    m_thread_run = 1; 
    m_error_msg = ""; 
    return parse_config(config);  
}


void Qsslib::shadow_connect_thread(void *pdata) {  
  
  bool used_http_proxy = false; 
  int  try_connect_count = 0; 
  Qsslib *pself = (Qsslib *)pdata; 
  boost::mutex::scoped_lock lock(pself->get_thread_lock()); 
  shadow_config ssconfig = pself->get_shadow_config();    
  if (ssconfig.http_proxy) {
    used_http_proxy = true; 
  }
  pself->set_server_status(PENDING);    
  while(pself->get_thread_isrun()) {
  int ncount = ssconfig.server.size(); 
    if( ncount > 0 && (pself->get_server_status())== FAILED ) {
      try_connect_count = ncount; 
      for (int i = 0; i < ncount; i++) {
            shadow_server srv = ssconfig.server[i]; 
            pself->get_shadows_client().setup(QString::fromLocal8Bit(srv.ip.c_str()),
                QString::fromLocal8Bit(srv.port.c_str()),
                QString::fromLocal8Bit(ssconfig.local_address.c_str()),
                QString::fromLocal8Bit(ssconfig.local_port.c_str()),
                QString::fromLocal8Bit(srv.pwd.c_str()),
                QString::fromLocal8Bit(ssconfig.method.c_str()),
                QString::fromLocal8Bit(ssconfig.time_out.c_str()),
                used_http_proxy);
            pself->get_shadows_client().setAutoBan(false);
            if (used_http_proxy)
                pself->get_shadows_client().setHttpMode(true);   
            if ( pself->get_shadows_client().start(false)) {
                pself->set_server_status(SUCCEED);    
                break;  
            } else {
                try_connect_count --; 
            }
        }
        if (try_connect_count == 0) {
            pself->set_server_status(FAILED);    
        }
    }    
    BOOST_THREAD_SLEEP(1000);
   }
}

int Qsslib::start(unsigned short port){   
   if (m_status == SUCCEED)
       return SUCCEED; 
   else if (m_status == PENDING) {
       return PENDING; 
   } 
   m_thread_run = 1;     
   boost::thread thr(boost::bind(&Qsslib::shadow_connect_thread,this));
   //boost::thread thr(&Qsslib::shadow_connect_thread,this); 
   thr.join(); 
   return PENDING; 
}

int Qsslib::stop() {
    m_thread_run = 0; 
    m_shadows_client.stop();
    return SUCCEED; 
}

int Qsslib::get_status(char* res) {
    if (m_status == SUCCEED) {
       std::string result;
       std::stringstream sstr(result);
       sstr<< "server:" << m_current_server.ip;
       sstr<< "port:" << m_current_server.port;
       sstr<< "password:" << m_current_server.pwd; 
       res = (char*) sstr.str().c_str();
    }
    return m_status; 
}

int Qsslib::uninit() {
    memset(&m_current_server,0,sizeof(m_current_server));
    m_error_no = 0; 
    m_error_msg = ""; 
    if (m_status == SUCCEED) {
        m_status = FAILED;   
        return stop(); 
    }
    return SUCCEED; 
}

int Qsslib::get_thread_isrun() {
    return m_thread_run; 
 }
     
void Qsslib::set_current_server(shadow_server srv) {
   m_current_server.ip = srv.ip; 
   m_current_server.pwd = srv.pwd; 
   m_current_server.port = srv.port; 
}
     
shadow_config Qsslib::get_shadow_config() {
    return m_config; 
} 

void Qsslib::set_server_status(int status) {
    m_status = status; 
}

const std::string Qsslib::get_error_message() const {
    return m_error_msg; 
}

Client& Qsslib::get_shadows_client() {
    return m_shadows_client; 
}

int  Qsslib::get_server_status() {
    return m_status; 
}

boost::mutex& Qsslib::get_thread_lock() {
    return m_thread_lock; 
} 

Qsslib* Qsslib::get_instance() {
    if (m_instance == NULL) {
        if (m_instance == NULL) {  
            m_instance = new Qsslib(); 
       }     
    }
    return m_instance; 
}


void Qsslib::destroy_instance() {
    if (m_instance != NULL) {
        delete m_instance; 
        m_instance = NULL;
    }
}
