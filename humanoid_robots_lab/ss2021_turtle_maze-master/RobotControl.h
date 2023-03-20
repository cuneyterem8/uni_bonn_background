#ifndef ROBOTCONTROL_H
#define ROBOTCONTROL_H
#include <QObject>
#include "util/Vec2.h"

#include <opencv2/opencv.hpp>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/StdVector>

#include "astar.hpp"

typedef std::vector<Eigen::Vector2d, Eigen::aligned_allocator<Eigen::Vector2d> > VectorOfPoints;

class RobotControl : public QObject
{
    Q_OBJECT

public:


    RobotControl(QObject *parent = 0);
    ~RobotControl();

    //the world as a 5cm grid map, first value being the rows/x-axis, second value the columns/y-axis
    double map[110][110];

    bool found;

    Eigen::Vector2d startPoint;
    Eigen::Vector2d finishPoint;
    AStar::Generator gen;
    AStar::coordVec path;
    Eigen::Vector2d start_grid;
    Eigen::Vector2d finish_grid;
    AStar::Vec2i start, finish;
    bool astar_setup, stop_flag, return_flag, finish_flag;
    int curr_waypoint;

    void init();
    void reset();

    void step(){sense(); act();}
    void sense();
    Vec2 act();

    bool find_target();

    void update_map();
    Eigen::Vector2d convert_to_grid(const Eigen::Vector2d& point);
    Eigen::Vector2d convert_to_world(const Eigen::Vector2d& point);
    VectorOfPoints bresenham(const Eigen::Vector2d& pointA, const Eigen::Vector2d& pointB, const size_t& numRows, const size_t& numCols);

signals:
    void messageOut(QString);

public slots:
    void configChangedIn(){}

};


#endif
