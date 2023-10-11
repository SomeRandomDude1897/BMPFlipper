#include <iostream>
#include <fstream>
#include <vector>
#include "BMP.h"

using namespace std;

int main()
{
    BMP* bmp = new BMP();

    bmp -> read_file("berry.bmp");

    bmp -> print_file_size();

    bmp -> flip_right_and_save();

    bmp -> flip_left_and_save();

    bmp -> apply_gauss_filter();

    delete bmp;

    return 0;
}