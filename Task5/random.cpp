#include <iostream>
#include <cmath>
#include <string>
#include <ctime>
#include <fstream>


int main(int argc, char *argv[])
{
    srand(time(0));
    std::string rand[128];
    int y = 0;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 8; ++k) {
                rand[y++] = std::to_string(i) + std::to_string(j) + std::to_string(k); 
            }
        }
    }
    std::string temp;
    int k;
    for (int i = 0 ; i < 128; ++i) {
        k = std::rand() % 128;
        std::swap(rand[i], rand[k]);
    }
    for (int i = 0 ; i < 128; ++i) {
        k = std::rand() % 128;
        std::swap(rand[i], rand[k]);
    }
    for (int i = 0 ; i < 128; ++i) {
        k = std::rand() % 128;
        std::swap(rand[i], rand[k]);
    }
    std::ofstream fout("my.map");
    for (int i = 0 ; i < 125; ++i) {
        fout << rand[i][0] << ' ' << rand[i][1] << ' ' << rand[i][2] << ' ' << 0 << std::endl;
    }
    fout.close();

}
