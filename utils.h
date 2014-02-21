#ifndef UTILS_H
#define UTILS_H

#include <QTime>
#include <opencv2/opencv.hpp>

namespace ul {
//used to format the time of the labels
//interface is fatter than it needs to be
//but I thought the extra getters/setters
//would be useful
class FormatTime
{
public:
    FormatTime();
    FormatTime(int cur, int total);

    //getters
    std::string getFormattedTime();
    std::string getFormattedTime(int currentSeconds);

    //setters
    void totalTime(int totalSeconds);
    void currentTime(int currentSeconds);
private:
    //helper functions
    std::string convertTime(int seconds);

    int _currentTime;
    int _totalTime;
};

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
    cv::Mat icon() const;
    std::string name() const;
    std::string URL() const;
    int frameNumber() const;
    cv::Rect location() const;

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
    std::vector<ObjectInfo> objectList() const;

    //state checkers
    bool isOpened() const;

    //modifiers
    void addObject(ObjectInfo);

    //IO
    void loadObjectsFromFile(std::string filename);
    void writeObjectsToFile();
    void writeObjectsToFile(std::string filename);

    //Misc
    std::vector<ObjectInfo> getObjectsIn(int frameNumber);
private:
    void loadObjectInfo();
    bool isObjectInList(std::vector<ObjectInfo>,ObjectInfo);

    bool _open;
    std::string _filename;
    std::vector<ObjectInfo> _objectList;
};

std::string getFileNameWithoutExtension(std::string filename);
std::vector<std::string> split(const std::string &s, char delim);
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
}

#endif // UTILS_H
