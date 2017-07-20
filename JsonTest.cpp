#include <fstream>
#include "JsonTest.h"

#define JSON_FILE "device.json"

#define RTSP_REQUEST_LENGTH_MAX    256

Json::Value Device::root;

void Device::add_object(char* id, char* username, char* password, char* ip, uint16_t port, char* URL)
{
    char entire[RTSP_REQUEST_LENGTH_MAX] = {0};
    Json::Value subobj;

    snprintf(entire, RTSP_REQUEST_LENGTH_MAX, "rtsp://%s:%s@%s:%d%s", username, password, ip, port, URL);

    subobj["Username"]  = username;
    subobj["Password"]  = password;
    subobj["HostAddr"]  = ip;
    subobj["HostPort"]  = port;
    subobj["SourceURL"] = URL;
    subobj["Entire"]    = entire;

    root[id] = subobj;
}

bool Device::write_to_file()
{
    rootstr = swriter.write(root);
    std::cout << rootstr << std::endl;

    std::ofstream out_file(JSON_FILE);
    if (!out_file.is_open())
    {
        printf("Open device.json failed!\n");
        return false;
    }
    out_file << rootstr;
    out_file.close();

    return true;
}

bool Device::read_from_file()
{
    std::ifstream in_file(JSON_FILE, std::ios::in|std::ios::binary|std::ios::ate);

    if (!in_file.is_open())
    {
        printf("Open device.json failed!\n");
        return false;
    }
    long len = in_file.tellg();
    char* buf = new char[len];
    in_file.seekg(0, std::ios::beg);
    in_file.read(buf, len);
    in_file.close();

    bool boolean = reader.parse(buf, root);

    delete [] buf;
    return boolean;
}

bool Device::find_by_id(std::string &str, std::string id)
{
    if (id == "all")
    {
        str = swriter.write(root);
        return true;
    }
    else
    {
        if (root.isMember(id))
        {
            Json::Value subobj(root[id]);
            Json::Value obj;

            //subobj["UserNname"] = root[id]["UserNname"].asString();
            //subobj["Password"]  = root[id]["Password"].asString();
            //subobj["HostAddr"]  = root[id]["HostAddr"].asString();
            //subobj["HostPort"]  = root[id]["HostPort"].asInt();
            //subobj["SourceURL"] = root[id]["SourceURL"].asString();

            obj[id] = subobj;
            str = swriter.write(obj);

            return true;
        }
        else
        {
            return false;
        }
    }
}
