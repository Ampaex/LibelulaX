#pragma once
#include "freertos/FreeRTOS.h"
#include <vector>

#define EARTH_RADIUS 6372.795477598

/**
 * @brief Simple conversion, minute / meter. (Mean between lat and long).
 *          Only for small distances.
 */
#define gps_meterToMinuteLat(meter) meter / 18490.526 
#define gps_meterToMinuteLon(meter) meter / 14630.04

using namespace std;

// GPS coordinate
struct GPS_coordinate
{
    double latitude, longitude, altitude;

    // @brief Construct a new empty gps coordinate.
    GPS_coordinate();

    /**
     * @brief Construct a new gps coordinate.
     *
     * @param latitude Coordinate latitude.
     * @param longitude Coordinate longitude.
     * @param altitude Altitude. If left empty then altitude will not be changed.
     */
    GPS_coordinate(double latitude, double longitude, double altitude = 0.);
    
    //Convert units into radians
    GPS_coordinate toRadians();

    //Converts units into degrees
    GPS_coordinate toDegrees();

    //Check if this coordinate has not been initialized
    operator bool() const {return latitude != 0. || longitude != 0. || altitude != 0.;};
};

class GPS
{
private:
    // List of coordinates for path.
    vector<GPS_coordinate> path = {};

    GPS_coordinate currentPosition;
    GPS_coordinate home;
    float currentCompass;

public:
    /**
     * @brief Construct a new GPS object giving a path
     *
     * @param coords Vector of coordinates
     */
    GPS(vector<GPS_coordinate> coords);
    // Construct a new empty GPS object
    GPS();
    ~GPS();

    //################## PATH CONTROL ##################

    // Add new coordinate to the end of path
    void appendCoordinate(GPS_coordinate coord);

    // Append a new path to the old one
    void appendPath(vector<GPS_coordinate> coords);

    // Delete the old path and start a new one
    void newPath(vector<GPS_coordinate> coords);

    size_t getPathSize();

    //@return Return the next coordinate and erase it
    GPS_coordinate popCoord();

    GPS_coordinate getCoordAt(int i);

    // Clear every coordinate in path
    void clearPath(void);

    /**
     * @brief Clear the path and orbit in circles around designated point.
     * 
     * @param center Coordinate center of the circle.
     * @param radius Radius of the circle. (In meters)
     * @param nPoints Number of points of the circumference.
     */
    void orbitCoord(GPS_coordinate center, float radius, int nPoints);

    //################## DIRECTION CONTROL ##################

    void setCurrentPosition(GPS_coordinate coord);

    GPS_coordinate getCurrentPosition();

    void setHome(GPS_coordinate coord){home = coord;};

    GPS_coordinate getHome(){return home;};

    void setCompass(float compass){currentCompass = compass;};
    float getCompass(){return currentCompass;};

    /**
     * @brief Compass angle from current position towards desired coordinate.
     * 
     * @param coord Destination point.
     * @return Compass angle in clockwise sexagesimal degrees.
     */
    float compassToCoord(GPS_coordinate coord);

    /**
     * @brief Distance between current position and the desired coordinate.
     * The calculation could have a distance error of 0.3%, especially at the polar extremes and for long distances through various parallels.
     * 
     * @param coord Destination point.
     * @return Distance in Km.
     */
    double distanceToCoord(GPS_coordinate coord);

    /**
     * @brief Compass angle from A towards B coordinates.
     * 
     * @param coord_A Point A.
     * @param coord_B Point B.
     * @return Compass angle in clockwise sexagesimal degrees. 
     */
    float compassBetweenCoord(GPS_coordinate coord_A, GPS_coordinate coord_B);

    /**
     * @brief Distance between A and B coordinates.
     * The calculation could have a distance error of 0.3%, especially at the polar extremes, and for long distances through various parallels.
     * 
     * @param coord_A Point A.
     * @param coord_B Point B.
     * @return Distance in Km.
     */
    double distanceBetweenCoord(GPS_coordinate coord_A, GPS_coordinate coord_B);
};

