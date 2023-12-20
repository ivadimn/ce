#ifndef _CAR_H_06_
#define _CAR_H_06_


typedef struct {
    char name[32];
    double speed;
    double fuel;

} car_t;

void car_construct(car_t*, const char* );
void car_destruct(car_t*);
void car_accelerate(car_t*);
void car_brake(car_t*);
void car_refuel(car_t*);

#endif