#ifndef OBJECTINFO_H
#define OBJECTINFO_H

#include <opencv2/opencv.hpp>

class ObjectInfo
{
public:
    ObjectInfo();
    ObjectInfo(cv::Mat icon,
               std::string name,
               std::string URL,
               int frameNumber,
               cv::Rect location);
    ~ObjectInfo();

    //getters
    cv::Mat icon();
    std::string name();
    std::string URL();
    int frameNumber();
    cv::Rect location();

    //setters
    void icon(cv::Mat);
    void name(std::string);
    void URL(std::string);
    void frameNumber(int);
    void location(cv::Rect);

    //stream operators
    friend cv::FileStorage& operator << (cv::FileStorage& fs, const ObjectInfo oi);
    friend cv::FileNode& operator >> (cv::FileNode& fn, ObjectInfo oi);
private:
    cv::Mat _icon;
    std::string _name;
    std::string _URL;
    int _frameNumber;
    cv::Rect _location;
};


class ObjectInfoHandler
{
public:
    ObjectInfoHandler();
    ObjectInfoHandler(std::string filename);

    //getters
    std::vector<ObjectInfo> objectList();

    //state checkers
    bool isOpened();

    //IO
    void loadObjectsFromFile(std::string filename);
    void writeObjectsToFile();
    void writeObjectsToFile(std::string filename);
private:
    void loadObjectInfo();

    bool _open;
    std::string _filename;
    std::vector<ObjectInfo> _objectList;
};

#endif // OBJECTINFO_H
