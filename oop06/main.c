#include <stdio.h>
#include "car.h"


int main(int argc, char** argv) {

    car_t car;
    car_construct(&car, "Renault");
    
    car_refuel(&car, 100.0);
    printf("Car is refueled, the correct fuel level is %f\n", car.fuel);
    while (car.fuel > 0) {
        printf("Car fuel level: %f\n", car.fuel);
    }
    

    return 0;
}
