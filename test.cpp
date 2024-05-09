#include <iostream>
#include <cmath>

float d(float x){
    return sqrt(1 - x*x);
}

#define STEPS 10

int main(){
    //

    for (int i = 0; i < 2 * STEPS + 1; i++)
    {
        float x = float(i) / STEPS;
        float space = d(x - 1);

        int left = STEPS * 2 * (1 - space);
        std::cout << left << std::endl;
        int right = left + 4 *  space * STEPS; 
        for (int j = 0; j < left; j++)
        {
            std::cout << " ";
        }
        std::cout << "*";
        for (int j = left + 1; j < right; j++)
        {
            std::cout << " ";
        }
        std::cout << "*";
        std::cout << std::endl;
    }
    std::cout << "/xbi[2J";
}