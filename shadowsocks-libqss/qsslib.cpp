#include "qsslib.h"
#include <signal.h>
#include <iostream>
#include "client.h"
#include "utils.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDateTime>
#include "error.h"


using namespace QSS;

Qsslib::Qsslib() {
}

Qsslib::Qsslib(const Qsslib&) {
}

Qsslib& Qsslib::operator=(const Qsslib&) {
}

std::string Qsslib::get_current_datetime() {
    QDateTime local(QDateTime::currentDateTime());
    QString localtime = local.toString(yyyy:MM:dd:hh:mm:ss);
    std::string strtime = localTime.toUtf8().constData(); 
    return strtime; 
}


int Qsslib::parse_config(char* config) {
    
    std::string param(config); 
    QJsonParseError jsonError;
    QByteArray jsonstr(config); 
    QJsonDocument doc = QJsonDocument::fromJson(jsonstr,&jsonError); 
    if (!doc.isNull() && (jsonError.error == QJsonParseError::NoError)) {
       if (doc.isObject()) {
           QJsonObject object = doc.object(); 
           if (object.contains("server")) {
               QJsonValue value = object.value("server");
               
           }
       }
    } else {
       m_errorno = CONFIG_PARSE_ERROR; 
       m_errormsg = "json data parse error, data:" + param; 
       return CONFIG_PARSE_ERROR;
    }

    return SUCCEED; 
}

int Qsslib::init(char* config) {
    m_queuestart.clear();
    m_queuestop.clear(); 
    m_status = ""; 
    m_connected = FAILED; 
    error_no = 0; 
    m_errormsg = ""; 
    return parse_config(config);  
}

int Qsslib::start(unsigned short port){
    //prefix with start queue R_: start,P_: pending, T_: stop;
   if (m_status == SUCCEED)
       return SUCCEED; 
   else {
        std::string startMark="R_"+get_current_datetime(); 
        queue_start.push_back(startMark); 
        m_status = PENDING; 
   }
       
   //TODO: start thread 

   return PENDING; 
}

int Qsslib::stop() {
   return SUCCEED; 
}

int Qsslib::get_status(char* res) {
    if (m_status == SUCCEED) {
      res = (char*) m_current_server; 
    }

    return m_status; 
}

int Qsslib::unint() {

    return SUCCEED; 
}

Qsslib* Qsslib::get_instance() {
    if (m_instance == NULL) {
         //TODO : lock();
       if (m_instance == NULL) {
            m_instance = new Qsslib(); 
       }
       //TODO : unlock();

    }
    return m_instance; 
}

