#include "BMP.h"
#include <iostream>
#include <fstream>
#include <vector>

#pragma warning(disable:4996)



BMP::BMP()
{
	std::cout << "created BMP structure" << '\n';
    extraheight = head_data.height % 4;
    extrawidth = head_data.width % 4;
}

BMP::~BMP()
{
    std::cout << "destroyed BMP structure" << '\n';
}

void BMP::print_file_size()
{
    std::cout << "File size: " << *(int*)&info[2] / 1024 << " kbytes" << '\n';
}

void BMP::apply_gauss_filter()
{
    int radius = 2;
    double sigma = 3;
    std::vector<std::vector<double>> gauss;
    double summ = 0;
    for (int h = 0; h < 2 * radius; h++) {
        gauss.push_back(std::vector<double>());
        for (int w = 0; w < 2 * radius; w++) {
            gauss[h].push_back(exp(-((h - radius) * (h - radius) + (w - radius) * (w - radius)) / (2 * (sigma * sigma))) / (2 * pi * sigma * sigma));
            summ += gauss[h][w];
        }
    }
    FILE* outfile = fopen("with gaussian filter.bmp", "wb");
    fwrite((const char*)&info, 1, 14, outfile);
    fwrite((const char*)&head_data, 1, 40, outfile);
    pixel* line_to_write = new pixel[head_data.width + extrawidth];
    double gaussum[3];
    for (int y = 0; y < head_data.height; y++)
    {

        for (int x = 0; x < head_data.width; x++)
        {
            gaussum[0] = 0;
            gaussum[1] = 0;
            gaussum[2] = 0;
            for (int h = 0; h < 2 * radius; h++) {
                for (int w = 0; w < 2 * radius; w++) {
                    if (y + h - radius >= 0 && x + w - radius >= 0 && y + h - radius < head_data.height && x + w - radius < head_data.width)
                    {
                        gaussum[0] += gauss[h][w] / summ * data[((y + h - radius) * head_data.width + x - radius + w)].b;
                        gaussum[1] += gauss[h][w] / summ * data[((y + h - radius) * head_data.width + x - radius + w)].g;
                        gaussum[2] += gauss[h][w] / summ * data[((y + h - radius) * head_data.width + x - radius + w)].r;
                    }
                }
            }
            data[((y)*head_data.width + x)].b = (uint8_t) gaussum[0];
            data[((y)*head_data.width + x)].g = (uint8_t) gaussum[1];
            data[((y)*head_data.width + x)].r = (uint8_t) gaussum[2];
            line_to_write[x] = data[((y)*head_data.width + x)];
        }
        for (int i = 0; i < extrawidth; ++i)
        {
            line_to_write[head_data.width + i] = pixel();
        }
        fwrite(line_to_write, 1, head_data.width * 3 + extrawidth, outfile);
    }
    for (int i = 0; i < extraheight; ++i)
    {
        for (int x = 0; x < head_data.width + extraheight; x++)
        {
            line_to_write[x] = pixel();
        }
        fwrite(line_to_write, 1, head_data.width * 3 + extraheight, outfile);
    }
    delete[] line_to_write;
    fclose(outfile);

    std::cout << "Apllied gaussian filter" << '\n';
}

void BMP::flip_left_and_save()
{
    header_data head_data_temp = head_data;
    head_data_temp.width = head_data.height;
    head_data_temp.height = head_data.width;

    FILE* outfile = fopen("flipped left.bmp", "wb");
    fwrite((const char*)&info, 1, 14, outfile);
    fwrite((const char*)&head_data_temp, 1, 40, outfile);


    pixel* line_to_write = new pixel[head_data.height + extraheight];

    for (int y = 0; y < head_data.width; y++)
    {

        for (int x = head_data.height-1; x > 0 ; x--)
        {
            line_to_write[(head_data.height - x)] = data[(x * head_data.width + (head_data.width - y - 1))];
        }
        for (int i = 0; i < extraheight; ++i)
        {
            line_to_write[head_data.height + i] = pixel();
        }
        fwrite(line_to_write, 1, head_data.height * 3 + extraheight, outfile);

    }

    for (int i = 0; i < extrawidth; ++i)
    {
        for (int x = 0; x < head_data.height + extraheight; x++)
        {
            line_to_write[head_data.height * x + i] = pixel();
        }
        fwrite(line_to_write, 1, head_data.height * 3 + extraheight, outfile);
    }
    delete[] line_to_write;
    fclose(outfile);

    std::cout << "Flipped left" << '\n';
}

void BMP::flip_right_and_save()
{
    header_data head_data_temp = head_data;
    head_data_temp.width = head_data.height;
    head_data_temp.height = head_data.width;

    FILE* outfile = fopen("flipped right.bmp", "wb");
    fwrite((const char*)&info, 1, 14, outfile);
    fwrite((const char*)&head_data_temp, 1, 40, outfile);

    pixel* line_to_write = new pixel[head_data.height + extraheight];

    for (int y = 0; y < head_data.width; y++)
    {

        for (int x = 0; x < head_data.height; x++)
        {
            line_to_write[x] = data[(x * head_data.width + (head_data.width - y - 1))];
        }
        for (int i = 0; i < extraheight; ++i)
        {
            line_to_write[head_data.height + i] = pixel();
        }
        fwrite(line_to_write, 1, head_data.height * 3 + extraheight, outfile);

    }

    for (int i = 0; i < extrawidth; ++i)
    {
        for (int x = 0; x < head_data.height + extraheight; x++)
        {
            line_to_write[head_data.height * x + i] = pixel();
        }
        fwrite(line_to_write, 1, head_data.height * 3 + extraheight, outfile);
    }
    delete[] line_to_write;
    fclose(outfile);
    

    std::cout << "Flipped right" << '\n';
}

void BMP::read_file(const char* file_name)
{
    FILE* file = fopen(file_name, "rb");

    fread(info, 1, 14, file);
    fread((char*)&head_data, 1, 40, file);

    int size = head_data.width * head_data.height * 3;

    unsigned char* data_temp = new unsigned char[size];
    fread(data_temp, 1, size, file);

    pixel px;

    for (int h = 0; h < head_data.height; h++)
    {
        for (int w = 0; w < head_data.width; w++)
        {
            px.b = data_temp[(h * head_data.width + w) * 3];
            px.g = data_temp[(h * head_data.width + w) * 3 + 1];
            px.r = data_temp[(h * head_data.width + w) * 3 + 2];
            data.push_back(px);
        }
    }
    delete[] data_temp;
    
    fclose(file);
}