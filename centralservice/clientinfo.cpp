#include "clientinfo.h"

ClientInfo::ClientInfo()
{

}

ClientInfo::~ClientInfo() {

}

int ClientInfo::getAppId() {
    return _appid;
}
int ClientInfo::getType() {
    return _type;
}
QString ClientInfo::getDesc() {
    return _desc;
}
void ClientInfo::setAppId(int value) {
    this->_appid = value;
}
void ClientInfo::setType(int value) {
    this->_type = value;
}
void ClientInfo::setDesc(const QString& value) {
    this->_desc = value;
}





