#include "gps.hpp"
#include <stdio.h>
#include <math.h>

//################ GPS COORDINATE ###############

GPS_coordinate::GPS_coordinate() : latitude(0.),
                                   longitude(0.),
                                   altitude(0.)
{
}

GPS_coordinate::GPS_coordinate(float latitude, float longitude, float altitude) : latitude(latitude),
                                                                                  longitude(longitude),
                                                                                  altitude(altitude)
{
}

GPS_coordinate GPS_coordinate::toRadians()
{
    GPS_coordinate ret(latitude, longitude, altitude);
    ret.latitude = (ret.latitude * M_PI) / 180;
    ret.longitude = (ret.longitude * M_PI) / 180;

    return ret;
};

GPS_coordinate GPS_coordinate::toDegrees()
{
    GPS_coordinate ret(latitude, longitude, altitude);
    ret.latitude = (ret.latitude * 180) / M_PI;
    ret.longitude = (ret.longitude * 180) / M_PI;

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

float GPS::distanceToCoord(GPS_coordinate coord)
{
    return distanceBetweenCoord(currentPosition, coord);
}

float GPS::compassBetweenCoord(GPS_coordinate coord_A, GPS_coordinate coord_B)
{
    GPS_coordinate _A = coord_A.toRadians();
    GPS_coordinate _B = coord_B.toRadians();
    float latGradient = log(tan(_B.latitude / 2 + M_PI_4) / tan(_A.latitude / 2 + M_PI_4));
    float longGradient = fmod(abs(_A.longitude - _B.longitude), 180);
    float radianAngle = atan2(longGradient, latGradient);

    // Return the radian angle in form of sexagesimal degree
    return (radianAngle * 180) / M_PI;
}

float GPS::distanceBetweenCoord(GPS_coordinate coord_A, GPS_coordinate coord_B)
{
    GPS_coordinate _A = coord_A.toRadians();
    GPS_coordinate _B = coord_B.toRadians();

    return EARTH_RADIUS * acos(sin(_A.latitude) * sin(_B.latitude) + cos(_A.latitude) * cos(_B.latitude) * cos(_A.longitude - _B.longitude));
}