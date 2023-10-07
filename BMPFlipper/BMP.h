#include <iostream> // ��� iostream �����, ��� ����������� ������������, ������ ���
#include <vector>
#pragma once

struct header_data
{
    uint32_t dib;
    uint32_t width;
    uint32_t height;
    uint16_t plane;
    uint16_t bpp;
    uint32_t bi_rgb;
    uint32_t raw_size;
    uint32_t res_hor;
    uint32_t res_vert;
    uint32_t colornum;
    uint32_t impcolors;
};
struct pixel
{
    uint8_t b{ 0 };
    uint8_t g{ 0 };
    uint8_t r{ 0 };
};
const double pi = 355/113;

class BMP
{
public:
	BMP();
    ~BMP();
    void read_file(const char* file_name);
    void print_file_size();
    void flip_right_and_save();
    void flip_left_and_save();
    void apply_gauss_filter();
private:
    unsigned char info[14]; // � ������ ����� �� ������� ��������� ����� ���������, �� �� �����-�� ���������� ��� ������� � �� ���� ������ ������ ����� ���� ��� �����, �� ������ ������� 14 ������ ���� (>_<)
    header_data head_data;
    std::vector<pixel> data;
    int extraheight;
    int extrawidth;
};

