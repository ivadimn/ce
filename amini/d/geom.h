#ifndef _GEOM_H_
#define _GEOM_H_

#define PI 3.14159265359

typedef struct {
    double x;
    double y;
} cart_pos_2d_t;

typedef struct {
    double length;
    double theta;
} polar_pos_2d_t;

typedef struct {
    double x;
    double y;
    double z;
} cart_pos_3d_t;

typedef struct {
    double length;
    double theta;
    double phi;
} polar_pos_3d_t;


double to_radian(double deg);
double to_degree(double rad);

double cos_deg(double deg);
double acos_deg(double deg);

double sin_deg(double deg);
double asin_deg(double deg);

cart_pos_2d_t convert_to_2d_cart_pos(const polar_pos_2d_t* polar_pos);
polar_pos_2d_t convert_to_2d_polar_pos(const cart_pos_2d_t* cart_pos);

cart_pos_3d_t convert_to_3d_cart_pos(const polar_pos_3d_t* polar_pos);
polar_pos_3d_t convert_to_3d_polar_pos(const cart_pos_3d_t* cart_pos);

#endif