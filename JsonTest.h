#include <iostream>
#include "json.h"
#include "json-forwards.h"

class Device
{
public:
    Device(){};
    ~Device(){};

    void add_object(char* id, char* username, char* password, char* ip, uint16_t port, char* URL);
    bool delete_object(std::string id);
    bool update_object(std::string id, std::string data);

    bool write_to_file();
    bool read_from_file();

    bool find_by_id(std::string &str, std::string id);

private:
    static Json::Value root;
    std::string rootstr;

    Json::Reader       reader;
    Json::FastWriter   fwriter;
    Json::StyledWriter swriter;
};
