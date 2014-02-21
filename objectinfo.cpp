#include "objectinfo.h"

ObjectInfo::ObjectInfo()
{
    _icon = cv::Mat();
    _name = "";
    _URL = "";
    //the frame number can never
    //naturally be less than zero
    _frameNumber = -1;
    _location = cv::Rect();
}

ObjectInfo::ObjectInfo(cv::Mat icon,
                       std::string name,
                       std::string URL,
                       int frameNumber,
                       cv::Rect location)
{
    _icon = icon;
    _name = name;
    _URL = URL;
    _frameNumber = frameNumber;
    _location = location;
}

ObjectInfo::~ObjectInfo()
{}

cv::Mat ObjectInfo::icon()
{return _icon;}

std::string ObjectInfo::name()
{return _name;}

std::string ObjectInfo::URL()
{return _URL;}

int ObjectInfo::frameNumber()
{return _frameNumber;}

cv::Rect ObjectInfo::location()
{return _location;}

void ObjectInfo::icon(cv::Mat i)
{_icon = i;}

void ObjectInfo::name(std::string n)
{_name = n;}

void ObjectInfo::URL(std::string u)
{_URL = u;}

void ObjectInfo::frameNumber(int f)
{_frameNumber = f;}

void ObjectInfo::location(cv::Rect l)
{_location = l;}

cv::FileStorage& operator << (cv::FileStorage& fs, const ObjectInfo oi)
{
    fs << "name" << oi._name;
    fs << "icon" << oi._icon;
    fs << "URL" << oi._URL;
    fs << "frameNumber" << oi._frameNumber;
    fs << "location" << oi._location;

    return fs;
}

cv::FileNode& operator >> (cv::FileNode& fn, ObjectInfo oi)
{
    cv::Mat ico;
    fn["icon"] >> ico;
    oi.icon(ico);

    oi.name((std::string)fn["name"]);

    oi.URL((std::string)fn["URL"]);

    oi.frameNumber((int)fn["frameNumber"]);

    std::vector<int> r;
    fn["location"] >> r;
    cv::Rect rl(r[0],r[1],r[2],r[3]);
    oi.location(rl);

    return fn;
}

ObjectInfoHandler::ObjectInfoHandler()
{
    _filename = "";
    _open = false;
}

ObjectInfoHandler::ObjectInfoHandler(std::string filename)
{
    loadObjectsFromFile(filename);
}

std::vector<ObjectInfo> ObjectInfoHandler::objectList()
{return _objectList;}

bool ObjectInfoHandler::isOpened()
{return _open;}

void ObjectInfoHandler::loadObjectsFromFile(std::string filename)
{
    _filename = filename;
    loadObjectInfo();
}

void ObjectInfoHandler::writeObjectsToFile()
{
    //TODO
}

void ObjectInfoHandler::writeObjectsToFile(std::string filename)
{
    //TODO
}

void ObjectInfoHandler::loadObjectInfo()
{
    cv::FileStorage fs(_filename, cv::FileStorage::READ);
    if(!fs.isOpened())
    {
        _open = false;
        return;
    }
    std::vector<ObjectInfo> toReturn;
    cv::FileNode objects = fs["Objects"];
    cv::FileNodeIterator it = objects.begin(), end = objects.end();
    for(; it!= end; ++it)
    {
        cv::FileNode node = objects[((cv::FileNode)*it).name()];

        ObjectInfo t;
        node >> t;
        toReturn.push_back(t);
    }
    _objectList = toReturn;
}
