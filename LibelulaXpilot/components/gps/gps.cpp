#include "gps.hpp"
#include <stdio.h>
#include <math.h>

//################ GPS COORDINATE ###############

GPS_coordinate::GPS_coordinate() : latitude(0.),
                                   longitude(0.),
                                   altitude(0.)
{
}

GPS_coordinate::GPS_coordinate(double latitude, double longitude, double altitude) : latitude(latitude),
                                                                                  longitude(longitude),
                                                                                  altitude(altitude)
{
}

GPS_coordinate GPS_coordinate::toRadians()
{
    GPS_coordinate ret(latitude, longitude, altitude);
    ret.latitude = (ret.latitude * M_PI) / 180.;
    ret.longitude = (ret.longitude * M_PI) / 180.;

    return ret;
};

GPS_coordinate GPS_coordinate::toDegrees()
{
    GPS_coordinate ret(latitude, longitude, altitude);
    ret.latitude = (ret.latitude * 180.) / M_PI;
    ret.longitude = (ret.longitude * 180.) / M_PI;

    return ret;
}

//################## GPS ########################

GPS::GPS(vector<GPS_coordinate> coords)
{
    // Copy contents from coords vector to class vector path.
    path.insert(path.end(), coords.begin(), coords.end());
}

GPS::GPS()
{
}

GPS::~GPS()
{
}

void GPS::appendCoordinate(GPS_coordinate coord)
{
    path.emplace_back(coord);
}

void GPS::appendPath(vector<GPS_coordinate> coords)
{
    path.insert(path.end(), coords.begin(), coords.end());
}

void GPS::newPath(vector<GPS_coordinate> coords)
{
    path.clear();
    appendPath(coords);
}

size_t GPS::getPathSize()
{
    return path.size();
}

GPS_coordinate GPS::popCoord()
{
    GPS_coordinate ret = path.front();
    path.erase(path.begin());
    return ret;
}

void GPS::clearPath(void)
{
    path.clear();
}

void GPS::setCurrentPosition(GPS_coordinate coord)
{
    currentPosition = coord;
}

GPS_coordinate GPS::getCurrentPosition()
{
    return currentPosition;
}

float GPS::compassToCoord(GPS_coordinate coord)
{
    return compassBetweenCoord(currentPosition, coord);
}

double GPS::distanceToCoord(GPS_coordinate coord)
{
    return distanceBetweenCoord(currentPosition, coord);
}

float radians(float inp) { return (inp * M_PI) / 180.; };
float degrees(float inp) { return (inp * 180.) / M_PI; };

float GPS::compassBetweenCoord(GPS_coordinate coord_A, GPS_coordinate coord_B)
{
    GPS_coordinate _A = coord_A.toRadians();
    GPS_coordinate _B = coord_B.toRadians();
    /*double latGradient = log(tan(_B.latitude / 2. + M_PI_4) / tan(_A.latitude / 2. + M_PI_4));
    double longGradient = fabs(_A.longitude - _B.longitude);
    if(longGradient>180) longGradient = fmod(longGradient, 180);
    float bearing = atan2(longGradient, latGradient);*/

    double y = sin(_B.longitude-_A.longitude) * cos(_B.latitude);
    double x = cos(_A.latitude)*sin(_B.latitude) -
            sin(_A.latitude)*cos(_B.latitude)*cos(_B.longitude-_A.longitude);
    float bearing = atan2(y, x);

    // Return the radian angle in form of sexagesimal degree
    return fmod((degrees(bearing)+360),360);
}

double GPS::distanceBetweenCoord(GPS_coordinate coord_A, GPS_coordinate coord_B)
{
    GPS_coordinate _A = coord_A.toRadians();
    GPS_coordinate _B = coord_B.toRadians();

    //return EARTH_RADIUS * acos(sin(_A.latitude) * sin(_B.latitude) + cos(_A.latitude) * cos(_B.latitude) * cos(_A.longitude - _B.longitude));

    // Haversine Formula
    double dlong = _B.longitude - _A.longitude;
    double dlat = _B.latitude - _A.latitude;
 
    double ans = pow(sin(dlat / 2), 2) +
                          cos(_A.latitude) * cos(_B.latitude) *
                          pow(sin(dlong / 2), 2);
 
    ans = 2 * asin(sqrt(ans));
 
    // Radius of Earth in
    // Kilometers, R = 6371
    // Use R = 3956 for miles
    long double R = 6371;
     
    // Calculate the result
    ans = ans * R;

    return ans;
}