#include "RobotControl.h"
#include "globals.h"
#include "blackboard/Config.h"
#include "blackboard/Command.h"
#include "blackboard/State.h"

// The robot control class implements the sense-act loop.
// The sense() and act() functions are periodically called by a timer.
// The sense-act loop fetches sensor data from the state object, processes
// the data to a world model, and computes an action to be sent to the robot.
// Typically, the robot control just reads from the state and writes to state.
// The robot interface, which runs in its own thread, handles the data
// transfer between state object and robot.

double getHeadingError(AStar::Vec2i waypointGoal, AStar::Vec2i current) {
    double deltaX = waypointGoal.x - current.x;
    double deltaY = waypointGoal.y - current.y;
    double waypointHeading = atan2(deltaY, deltaX);
    double headingError = waypointHeading - state.rxPose.z;
    if (headingError > PI) {
        headingError = headingError - (2 * PI);
    }
    if (headingError < -PI) {
        headingError = headingError + (2 * PI);
    }

    return headingError;
}

RobotControl::RobotControl(QObject *parent) : QObject(parent)
{
    //initalize map, as there is no prior information, every cells starts with a occupancy value of 0.5, so its log-odds value is 0.0
    for(int i=0; i<110; i++){
        for(int j=0; j<110; j++){

            map[i][j]= 0.0;
        }
    }

    gen.setHeuristic(AStar::Heuristic::euclidean);
    gen.setWorldSize({110, 110});
    startPoint(0) = 0.0;
    startPoint(1) = 0.0;
    astar_setup = false;
    stop_flag = false;
    return_flag = false;
    finish_flag = false;
    curr_waypoint = 0;
}

RobotControl::~RobotControl()
{

}

// Initialization after construction.
void RobotControl::init()
{
    state.sampleGrid.init();
}

// Reset to an initial state.
void RobotControl::reset()
{

}

// The sense function. Computes the camera transform, the sample grid,
// and the object detection / classification.
void RobotControl::sense()
{
    // Camera transform
    state.cameraTransform_astra = config.cameraTransform_astra;
    state.cameraTransform_sr300 = config.cameraTransform_sr300;
    state.cameraTransform_d435_left = config.cameraTransform_d435_left;
    state.cameraTransform_d435_right = config.cameraTransform_d435_right;

    // Sound signal
    if(false){ // Goal has been reached
        state.txSound = true;
        if(state.rxSound){
            state.txSound = false;
            //reset Goal
        }
    }
    update_map();

}

// The act function computes an action for the robot.
// The action is expressed as a Pose the robot should take on next.
Vec2 RobotControl::act() {
    printf("x= %lf    y= %lf    or= %lf\n", state.rxPose.x, state.rxPose.y, state.rxPose.z);
    /*
    The robot will follow its right hand wall, its prioritys are the following:
    turn right > move straight > turn left > turn around
    While moving straight along the wall, it checks 2 positions on the right (rightFrontAverage and rightBackAverage)
    to align itself with the wall
    */

    //found = this->find_target();               //seek for target

    Eigen::Matrix3d rotateZRight, rotateZLeft;
    Eigen::Vector3d currentPointRightFront, currentPointRightBack, currentPointLeft;
    int currentPixelNumber;
    double rightFrontAverage = 0;   //distance to the right wall according to the right camera
    double rightBackAverage = 0;    // 2nd distance to the right wall according to the right camera, to align the robot with the wall
    double aheadAverage = 0;        //distance to the wall ahead of the robot according to the front camera
    double leftAverage = 0;         //distance to the left wall according to the left camera
    double vx = 0.0;  // linear velocity; +0 -> forward; -0 -> backward
    double vr = 0.0;  // angular velocity; +0 -> left; -0 -> right

    if(!found) {

        rotateZRight <<  cos(-1.0), -sin(-1.0), 0,      //rotates points from the right camera frame to the robot fram (only the rotation, doesnt fix the translation, as it isnt necessary)
                sin(-1.0), cos(-1.0), 0,
                0, 0, 1;

        rotateZLeft <<  cos(1.0), -sin(1.0), 0,         //rotates points from the left camera frame to the robot fram (only the rotation, doesnt fix the translation, as it isnt necessary)
                sin(1.0), cos(1.0), 0,
                0, 0, 1;

        for(int i=0; i<5; i++){             //this for loop calculates the average over 5 points, to deal with noise

            currentPixelNumber = NUMBER_OF_POINTS_D435/2; //this is the left most pixel on the middle row

            //the following code just converts the points into 3D vectores of the EIGEN-package, which made the Matrixmultiplication easier
            currentPointRightBack << state.pointBuffer_d435_right[currentPixelNumber - 100 - i].x, state.pointBuffer_d435_right[currentPixelNumber - 100 - i].y, state.pointBuffer_d435_right[currentPixelNumber - 100 - i].z;
            currentPointRightFront << state.pointBuffer_d435_right[currentPixelNumber - 150 - i].x, state.pointBuffer_d435_right[currentPixelNumber - 150 - i].y, state.pointBuffer_d435_right[currentPixelNumber -150 - i].z;
            currentPointLeft << state.pointBuffer_d435_left[currentPixelNumber + 150 + i].x, state.pointBuffer_d435_left[currentPixelNumber + 150 + i].y, state.pointBuffer_d435_left[currentPixelNumber + 150 + i].z;

            currentPointRightFront = rotateZRight * currentPointRightFront;
            currentPointRightBack = rotateZRight * currentPointRightBack;
            currentPointLeft = rotateZLeft * currentPointLeft;

            rightFrontAverage += currentPointRightFront[1];
            rightBackAverage += currentPointRightBack[1];
            leftAverage += currentPointLeft[1];

            currentPixelNumber = NUMBER_OF_POINTS_ASTRA/2 + CAMERA_ASTRA_RESOLUTION_WIDTH/2 + (i-2);
            aheadAverage += state.pointBuffer_astra[currentPixelNumber].x;
        }
        //divide by 5 to get the average of the 5 added points
        rightFrontAverage /= 5;
        rightBackAverage /= 5;
        aheadAverage /= 5;
        leftAverage /= 5;


        if (-rightFrontAverage > 0.55){          //right side free -> turn right

            vr = -1.0;
            vx = 0.1;

            if (aheadAverage < 0.4){             //got to close to the front wall, so keep turning without moving forward

                vx = 0.0;
            }

            printf("turning right: ahead = %lf, right = %lf, left= %lf \n", aheadAverage, rightFrontAverage, leftAverage);
        }

        else if (aheadAverage > 0.7){           //right blocked but free space in the front -> follow wall

            vx = 0.2;
            vr = tanh(50*(rightFrontAverage - rightBackAverage)); //makes sure we stay aligned to the wall

            printf("going straight: ahead = %lf, right = %lf, left= %lf \n", aheadAverage, rightFrontAverage, leftAverage);

            if (-rightFrontAverage < 0.15){
                vr = 0.5;                       //too close to the wall? -> turn slightly left to fix the distance
                vx= 0.1;
                printf("Too close to the right wall! turning left \n");
            }
            if (leftAverage < 0.15){

                vr = -0.5;                      //too close to the wall? -> turn slightly right to fix the distance
                vx = 0.1;
                printf("Too close to the left wall! turning right \n");
            }

        }
        else if (leftAverage > 0.5){            //neither right nor ahead are free, is left free? -> turn left

            vx = 0.1;
            vr = 1.0;

            printf("turning left: ahead = %lf, right = %lf, left= %lf \n", aheadAverage, rightFrontAverage, leftAverage);

            if (aheadAverage < 0.35){            //got to close to the front wall, so keep turning without moving forward

                vx = 0.0;
            }
        }
        else if (found) {        //if we found the target, stop
            printf("Found target, stopping... \n");

            finishPoint(0) = state.rxPose.x;
            finishPoint(1) = state.rxPose.y;

            printf("Finish x:%lf, y:%lf\n", finishPoint(0), finishPoint(1));

            vx = 0.0;
            vr = 0.0;
        }
        else{                                   //dead end, so turn around

            vx = 0.0;
            vr = 1.0;

            printf("dead end, turning around: ahead = %lf, right = %lf, left= %lf \n", aheadAverage, rightFrontAverage, leftAverage);
        }

        found = this->find_target();               //seek for target
    }
    else {
        //printf("Finish x:%lf, y:%lf\n", finishPoint(0), finishPoint(1));
        if (!astar_setup) {
            gen.loadMap("map.txt", 4);
            finishPoint(0) = state.rxPose.x;
            finishPoint(1) = state.rxPose.y;
            start_grid = convert_to_grid(startPoint);
            finish_grid = convert_to_grid(finishPoint);
            start.x = int(start_grid(0));
            start.y = int(start_grid(1));
            finish.x = int(finish_grid(0));
            finish.y = int(finish_grid(1));
            path = gen.findPath(start, finish);
            std::reverse(path.begin(), path.end());
            std::cout << "\nFound path:\n";
            for (auto& coordinate : path) {
                std::cout << coordinate.x << " " << coordinate.y << "\n";
            }

            astar_setup = true;
        }
        Eigen::Vector2d curr(state.rxPose.x, state.rxPose.y);
        Eigen::Vector2d current_loc = convert_to_grid(curr);
        AStar::Vec2i current_loc_2i;
        current_loc_2i.x = int(current_loc(0));
        current_loc_2i.y = int(current_loc(1));
        std::cout << "Grid: " << current_loc_2i.x << ' ' << current_loc_2i.y << '\n';

        if (current_loc_2i != start && !return_flag) {
            std::cout << "Found target, waiting for reset...\n";
            vx = 0.0;
            vr = 0.0;
        }
        if (abs(current_loc_2i.x - start.x) <= 2 && abs(current_loc_2i.y - start.y) <= 2 && !return_flag) {
            std::cout << "At start\n";
            vx = 0.0;
            vr = 0.0;
            return_flag = true;
        }
        if (return_flag && !finish_flag) {
            std::cout << "Going back...\n";
            //vx = 0.0;
            //vr = 0.0;

            double distanceToWaypoint = sqrt(pow(path[curr_waypoint].x - current_loc_2i.x, 2) + pow(path[curr_waypoint].y - current_loc_2i.y, 2));
            double headingError = getHeadingError(path[curr_waypoint], current_loc_2i);

            std::cout << "Next point: " << path[curr_waypoint].x << ' ' << path[curr_waypoint].y << ' ' << path.size() << ' ' << distanceToWaypoint << '\n';

            if (abs(distanceToWaypoint) >= 2.0) {
                if (abs(headingError) > 0.1) {
                    vx = 0.0;
                    vr = 0.8 * headingError;
                    if((vr < 0.2) && (vr > 0.0)) vr = 0.2;
                    else if ((vr > -0.2) && (vr < 0.0)) vr = -0.2;
                }
                else {
                    vx = 1.0 * distanceToWaypoint;
                    vr = 0.0;
                }
            }
            else {
                if (curr_waypoint == path.size()-1) {
                    vx = 0.0;
                    vr = 0.0;
                    finish_flag = true;
                    return_flag = false;
                }
                else {
                    vx = 0.0;
                    vr = 0.0;
                    curr_waypoint++;
                }
            }
        }
        if (finish_flag) {
            std::cout << "Reached target with A*\n";
            vx = 0.0;
            vr = 0.0;
        }
    }

    if (found && !stop_flag) {
        vx = 0.0;
        vr = 0.0;
        stop_flag = true;
    }
    return Vec2(vx, vr);
}

bool RobotControl::find_target() {
    cv::Mat front_im = cv::Mat(CAMERA_ASTRA_RESOLUTION_WIDTH, CAMERA_ASTRA_RESOLUTION_HEIGHT, CV_8UC3);

    for (int i = 0; i < CAMERA_ASTRA_RESOLUTION_WIDTH; ++i) {
        for (int j = 0; j < CAMERA_ASTRA_RESOLUTION_HEIGHT; ++j) {
            cv::Vec3b px(state.colorBuffer_astra[i + j * CAMERA_ASTRA_RESOLUTION_WIDTH].b,
                         state.colorBuffer_astra[i + j * CAMERA_ASTRA_RESOLUTION_WIDTH].g,
                         state.colorBuffer_astra[i + j * CAMERA_ASTRA_RESOLUTION_WIDTH].r);
            front_im.at<cv::Vec3b>(i, j) = px;
        }
    }
    cv::Mat front_im_rot;
    cv::rotate(front_im, front_im_rot, cv::ROTATE_90_CLOCKWISE);

    cv::Mat3b hsv_im;
    cv::cvtColor(front_im_rot, hsv_im, cv::COLOR_BGR2HSV);
    cv::Mat1b mask1, mask2;

    cv::inRange(hsv_im, cv::Scalar(0, 70, 50), cv::Scalar(10, 255, 255), mask1);
    cv::inRange(hsv_im, cv::Scalar(170, 70, 50), cv::Scalar(180, 255, 255), mask2);
    cv::Mat1b mask = mask1 | mask2;

    std::vector<std::vector<cv::Point>> redcont;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(mask, redcont, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    cv::Rect box;
    if(redcont.size() > 0) {
        box = cv::boundingRect(redcont[0]);
        cv::rectangle(front_im_rot, box, cv::Scalar(0, 255, 0), 5);
    }

    cv::imshow("Front", front_im_rot);

    if(box.size().width >= 0.20 * front_im_rot.size().width)
        return true;
    else
        return false;
}


//updates the map when being called in the sense function every frame.
void RobotControl::update_map() {

    //temporary maps so store which grid cells were detected as occupied/free in the current frame. 0-> no new info, 1->free/occupied
    int map_free[110][110];
    int map_occ[110][110];

    //initalize these maps
    for(int i=0; i<110; i++){
        for(int j=0; j<110; j++){

            map_free[i][j]= 0;
            map_occ[i][j] = 0;
        }
    }

    Eigen::Vector2d robot_position, current_camera_position;
    robot_position << state.rxPose.x, state.rxPose.y;
    double robot_rotation = state.rxPose.z;

    //rotation matrix for robot->world
    Eigen::Matrix2d rotate_robot_to_world;
    rotate_robot_to_world << cos(robot_rotation), -sin(robot_rotation),
                             sin(robot_rotation), cos(robot_rotation);


    //Variables to store the current endpoint in real world coordinates and grid cell coodrinates
    Eigen::Vector2d current_point_real, current_point_grid, old_point_grid;

    old_point_grid << -1,-1; //initalize with value outside the map


   //get the current camera position in world coordinates for bresenham
    current_camera_position << -0.05, 0;
    current_camera_position = rotate_robot_to_world * current_camera_position;
    current_camera_position = current_camera_position + robot_position;

    //iterate over the full width of the front camera image, only the center line is used
    for (int i=0; i<CAMERA_ASTRA_RESOLUTION_WIDTH; i++){

        //get the coordinates for the next point
        int current_point_number = CAMERA_ASTRA_RESOLUTION_HEIGHT/2 * CAMERA_ASTRA_RESOLUTION_WIDTH +i;
        current_point_real << state.pointBuffer_astra[current_point_number].x, state.pointBuffer_astra[current_point_number].y;

        //translate point from camera to robot frame
        current_point_real[0]+= -0.05;

        //rotate from robot to world frame
        current_point_real = rotate_robot_to_world * current_point_real;

        //add robot position to get world coordinates
        current_point_real = current_point_real + robot_position;

        //transfrom the point to a grid cell coordinate
        current_point_grid = convert_to_grid(current_point_real);

        //check if the current point has the same grid cell coordinate, in which case no new inforamtion is aquired and the point gets skipped
        if (current_point_grid != old_point_grid){

            old_point_grid = current_point_grid;

            //add the current point as an occupied grid cell
            map_occ[static_cast<int>(current_point_grid[1])] [static_cast<int>(current_point_grid[0])] = 1;

            //get all free points up to the current point and add them to the free list
            VectorOfPoints free = bresenham(convert_to_grid(current_camera_position), current_point_grid, 110, 110);

            for (int i=0; i<free.size()-1; i++){

                map_free[static_cast<int>(free[i][1])] [static_cast<int>(free[i][0])] = 1;
            }
        }
    }

    Eigen::Matrix2d rotateZRight, rotateZLeft;

    rotateZRight <<  cos(-1.0), -sin(-1.0),      //rotates points from the right camera frame to the robot frame
                     sin(-1.0), cos(-1.0);

    rotateZLeft <<  cos(1.0), -sin(1.0),         //rotates points from the left camera frame to the robot frame
                    sin(1.0), cos(1.0);

    old_point_grid << -1,-1; //initalize with value outside the map

    //get the current camera position in world coordinates for bresenham
     current_camera_position << 0.06, 0.145;
     current_camera_position = rotate_robot_to_world * current_camera_position;
     current_camera_position = current_camera_position + robot_position;
    //iterate over the full width of the left camera image, only the center line is used
    for (int i=0; i<CAMERA_D435_RESOLUTION_WIDTH ; i++){

        //get the coordinates for the next point
        int current_point_number = CAMERA_D435_RESOLUTION_HEIGHT/2 * CAMERA_D435_RESOLUTION_WIDTH +i;
        current_point_real << state.pointBuffer_d435_left[current_point_number].x, state.pointBuffer_d435_left[current_point_number].y;

        current_point_real= rotateZLeft * current_point_real;

        //translate point from camera to robot frame
        current_point_real[0]+= 0.06;
        current_point_real[1]+= 0.145;

        //rotate from robot to world frame
        current_point_real = rotate_robot_to_world * current_point_real;

        //add robot position to get world coordinates
        current_point_real = current_point_real + robot_position;

        //transfrom the point to a grid cell coordinate
        current_point_grid = convert_to_grid(current_point_real);

        //check if the current point as the same grid cell coordinate, in which case no new inforamtion is aquired and the point gets skipped
        if (current_point_grid != old_point_grid){

            old_point_grid = current_point_grid;

            //add the current point has an occupied grid cell
            map_occ[static_cast<int>(current_point_grid[1])] [static_cast<int>(current_point_grid[0])] = 1;

            //get all free points up to the current point and add them to the free list
            VectorOfPoints free = bresenham(convert_to_grid(current_camera_position), current_point_grid, 110, 110);

            for (int i=0; i<free.size()-1; i++){

                map_free[static_cast<int>(free[i][1])] [static_cast<int>(free[i][0])] = 1;
            }
        }
    }

    old_point_grid << -1,-1; //initalize with value outside the map

    //get the current camera position in world coordinates for bresenham
     current_camera_position << +0.06, -0.145;
     current_camera_position = rotate_robot_to_world * current_camera_position;
     current_camera_position = current_camera_position + robot_position;

    //iterate over the full width of the right camera image, only the center line is used
    for (int i=0; i<CAMERA_D435_RESOLUTION_WIDTH ; i++){

        //get the coordinates for the next point
        int current_point_number = CAMERA_D435_RESOLUTION_HEIGHT/2 * CAMERA_D435_RESOLUTION_WIDTH +i;
        current_point_real << state.pointBuffer_d435_right[current_point_number].x, state.pointBuffer_d435_right[current_point_number].y;

        current_point_real= rotateZRight * current_point_real;

        //translate point from camera to robot frame
        current_point_real[0]+= 0.06;
        current_point_real[1]+= -0.145;

        //rotate from robot to world frame
        current_point_real = rotate_robot_to_world * current_point_real;

        //add robot position to get world coordinates
        current_point_real = current_point_real + robot_position;

        //transfrom the point to a grid cell coordinate
        current_point_grid = convert_to_grid(current_point_real);

        //check if the current point has the same grid cell coordinate, in which case no new inforamtion is aquired and the point gets skipped
        if (current_point_grid != old_point_grid){

            old_point_grid = current_point_grid;

            //add the current point as an occupied grid cell
            map_occ[static_cast<int>(current_point_grid[1])] [static_cast<int>(current_point_grid[0])] = 1;

            //get all free points up to the current point and add them to the free list
            VectorOfPoints free = bresenham(convert_to_grid(current_camera_position), current_point_grid, 110, 110);

            for (int i=0; i<free.size()-1; i++){

                map_free[static_cast<int>(free[i][1])] [static_cast<int>(free[i][0])] = 1;
            }
        }
    }

    //log_odds values that get added to the map value
    double log_odds_occ = log(0.6/0.4);
    double log_odds_free = log(0.45/0.55);

    //iterate over the temporary maps of free/occupied cells and update the real map
    for(int i=0; i<110; i++){
        for(int j=0; j<110; j++){

            if (map_occ[i][j]==1) map[i][j]+= log_odds_occ;
            else if (map_free[i][j]==1) map[i][j]+= log_odds_free;
        }
    }

    //print the map to a text file. The rows are printed starting at 109 going down, as they higher numbers refer to the top of the map and need to be printed first
      FILE * pMap;
      pMap = fopen("map.txt", "w");

      //after converting the log_odds value back to the odds value, we get the believe about a cell being occupied between 0.0 and 1.0
      double odds;

      for(int i=109; i>=0; i--){
          for(int j=0; j<110; j++){

              odds = 1.0 - (1.0/(1+exp(map[i][j])));
              if (j > (55 - 5) && j < (55 + 5) && i > (55 - 5) && i < (55 + 5)) fprintf(pMap, "0");
              else if (odds >= 0.8) fprintf(pMap, "1");
              else if (odds <= 0.2) fprintf(pMap, "0");
              else fprintf(pMap, "X");

          }
          fprintf(pMap, "\n");
      }
      fprintf(pMap, "\n\n\n");
      fclose(pMap);
}


//returns the corosponding map cell for the input world coordinates
//inputs in the range -2.75m to 2.75m
//map has a 5cm grid -> 20 cells per meter
Eigen::Vector2d RobotControl::convert_to_grid(const Eigen::Vector2d& point){

    double grid_x, grid_y;
    grid_x = floor(point[0]*20)+55;
    grid_y = floor(point[1]*20)+55;

    //make sure the values are within the map boundary
    if (grid_x > 110) grid_x = 110;
    if (grid_y > 110) grid_y = 110;
    if (grid_x < 0) grid_x = 0;
    if (grid_y < 0) grid_y = 0;

    return Eigen::Vector2d(grid_x, grid_y);
}

//convert back to world coordinates
Eigen::Vector2d RobotControl::convert_to_world(const Eigen::Vector2d& point) {
    double world_x, world_y;
    world_x = (point[0] - 55) / 20;
    world_y = (point[1] - 55) / 20;

    return Eigen::Vector2d();
}

//taken from the HR lecture exercise, slightly alterd
//returns a vectorofpoints containing all the grid cells along the line between the 2 input points
VectorOfPoints RobotControl::bresenham(const Eigen::Vector2d& pointA, const Eigen::Vector2d& pointB,
        const size_t& numRows, const size_t& numCols) {

    if (pointA.x() < 0 || pointA.x() > numCols || pointA.y() < 0 || pointA.y() > numRows) {
        std::stringstream ss;
        ss << "Error in bresenham(): pointA with coordinates (" << pointA.x() << ", " << pointA.y() << ") is outside the map";
        throw std::invalid_argument(ss.str());
    }
    if (pointB.x() < 0 || pointB.x() > numCols || pointB.y() < 0 || pointB.y() > numRows) {
        std::stringstream ss;
        ss << "Error in bresenham(): pointB with coordinates (" << pointB.x() << ", " << pointB.y() << ") is outside the map";
        throw std::invalid_argument(ss.str());
    }

    VectorOfPoints pointsOnLine;

    int x0 = static_cast<int>(pointA.x());
    int y0 = static_cast<int>(pointA.y());
    const int x1 = static_cast<int>(pointB.x());
    const int y1 = static_cast<int>(pointB.y());
    const int dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
    const int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1;
    int err = dx+dy, e2; /* error value e_xy */

    Eigen::Vector2d p;
    while(true) {
        p << x0, y0;
        if (x0 >= 0 && x0 < static_cast<int>(numCols) && y0 >= 0 && y0 < static_cast<int>(numRows)) {
            pointsOnLine.push_back(p);
        }
        if (x0==x1 && y0==y1) break;
        e2 = 2*err;
        if (e2 > dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
        if (e2 < dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
    }

    return pointsOnLine;
}

