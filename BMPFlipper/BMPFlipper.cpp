#include <iostream>
#include <fstream>
#include <vector>

#pragma warning(disable:4996)

using namespace std;

struct header_data
{
    uint32_t dib{ 0 };
    uint32_t width{ 0 };
    uint32_t height{ 0 };
    uint16_t plane{ 1 };
    uint16_t bpp{ 0 };
    uint32_t bi_rgb{ 0 };
    uint32_t raw_size{ 0 };
    uint32_t res_hor{ 0 };
    uint32_t res_vert{ 0 };
    uint32_t colornum{ 0 };
    uint32_t impcolors{ 0 };
};
/*
struct file_data
{
    uint16_t id;
    uint32_t file_size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t padding
};
*/
int main()
{
    FILE* file = fopen("field.bmp", "rb");

    header_data head_data;
    unsigned char info[14]; // Я не понимаю почему, но по какой-то причине у меня не считывается через структуру размер файла, поэтому единственное, что я придумал это считать данные всырую

    fread(info, 1, 14, file);
    fread((char*)&head_data, 1, 40, file);
    cout << "File size: " << *(int*)&info[2] / 1024 << " kbytes" << endl;

    unsigned char* extra_header_data = new unsigned char[head_data.dib - 40];

    fread(extra_header_data, 1, head_data.dib - 40, file);

    int size = head_data.width * head_data.height * 3;

    unsigned char* data = new unsigned char[size];

    fread(data, 1, size, file);

    fclose(file);

    header_data head_data2 = head_data;
    head_data2.width = head_data.height;
    head_data2.height = head_data.width;

    FILE* outfile1 = fopen("flipped right.bmp", "wb");
    fwrite((const char*)&info, 1, 14, outfile1);
    fwrite((const char*)&head_data2, 1, 40, outfile1);
    fwrite(extra_header_data, 1, head_data.dib - 40, outfile1);
    int extraheight = head_data.height % 4;
    int extrawidth = head_data.width % 4;

    int b = 0;
    int g = 0;
    int r = 0;

    unsigned char* towriteline = new unsigned char[head_data.height * 3 + extraheight];

    for (int y = 0; y < head_data.width; y++)
    {
        
        for (int x = 0; x < head_data.height; x++)
        {
            b = data[(x * head_data.width + (head_data.width - y - 1)) * 3];
            g = data[(x * head_data.width + (head_data.width - y - 1)) * 3 + 1];
            r = data[(x * head_data.width + (head_data.width - y - 1)) * 3 + 2];
            towriteline[x * 3] = b;
            towriteline[x * 3 + 1] = g;
            towriteline[x * 3 + 2] = r;
        }
        for (int i = 0; i < extraheight; ++i)
        {
            towriteline[head_data.height * 3 + i * 3] = 0;
            towriteline[head_data.height * 3 + i * 3 + 1] = 0;
            towriteline[head_data.height * 3 + i * 3 + 2] = 0;
        }
        fwrite(towriteline, 1, head_data.height * 3 + extraheight, outfile1);
        
    }
    
    for (int i = 0; i < extrawidth; ++i)
    {
        for (int x = 0; x < head_data.height + extraheight; x++)
        {
            towriteline[x * 3] = 0;
            towriteline[x * 3 + 1] = 0;
            towriteline[x * 3 + 2] = 0;
        }
        fwrite(towriteline, 1, head_data.height * 3 + extraheight, outfile1);
    }

    fclose(outfile1);

    FILE* outfile2 = fopen("flipped left.bmp", "wb");
    fwrite((const char*)&info, 1, 14, outfile2);
    fwrite((const char*)&head_data2, 1, 40, outfile2);
    fwrite(extra_header_data, 1, head_data.dib - 40, outfile2);
    for (int y = 0; y < head_data.width; y++)
    {
        for (int x = head_data.height - 1; x >= 0; x--)
        {
            b = data[(x * head_data.width + (head_data.width - y - 1)) * 3];
            g = data[(x * head_data.width + (head_data.width - y - 1)) * 3 + 1];
            r = data[(x * head_data.width + (head_data.width - y - 1)) * 3 + 2];
            towriteline[(head_data.height - x) * 3] = b;
            towriteline[(head_data.height - x) * 3 + 1] = g;
            towriteline[(head_data.height - x) * 3 + 2] = r;
        }
        for (int i = 0; i < extraheight; ++i)
        {
            towriteline[head_data.height * 3 + i * 3] = 0;
            towriteline[head_data.height * 3 + i * 3 + 1] = 0;
            towriteline[head_data.height * 3 + i * 3 + 2] = 0;
        }

        fwrite(towriteline, 1, head_data.height * 3 + extraheight, outfile2);
    }
    for (int i = 0; i < extrawidth; ++i)
    {
        for (int x = 0; x < head_data.height + extraheight; x++)
        {
            towriteline[x * 3] = 0;
            towriteline[x * 3 + 1] = 0;
            towriteline[x * 3 + 2] = 0;
        }
    }

    fclose(outfile2);


    double pi = 3.14;
    int radius = 5;
    double sigma = 3;
    vector<vector<double>> gauss;
    double summ = 0;
    for (int h = 0; h < 2 * radius; h++) {
        gauss.push_back(vector<double>());
        for (int w = 0; w < 2 * radius; w++) {
            gauss[h].push_back(exp(-((h - radius) * (h - radius) + (w - radius) * (w - radius)) / (2 * (sigma * sigma))) / (2 * pi * sigma * sigma));
            summ += gauss[h][w];
        }
    }
    FILE* outfile3 = fopen("with gaussian filter.bmp", "wb");
    fwrite((const char*)&info, 1, 14, outfile3);
    fwrite((const char*)&head_data, 1, 40, outfile3);
    fwrite(extra_header_data, 1, head_data.dib - 40, outfile3);
    unsigned char* towriteline2 = new unsigned char[head_data.width * 3 + extrawidth];
    for (int y = 0; y < head_data.height; y++)
    {
        
        for (int x = 0; x < head_data.width; x++)
        {
            double gaussumb = 0;
            double gaussumg = 0;
            double gaussumr = 0;
            for (int h = 0; h < 2 * radius; h++) {
                for (int w = 0; w < 2 * radius; w++) {
                    if (y + h - radius >= 0 && x + w - radius >= 0 && y + h - radius < head_data.height && x + w - radius < head_data.width)
                    {
                        gaussumb += gauss[h][w] / summ * data[((y + h - radius) * head_data.width + x - radius + w) * 3];
                        gaussumg += gauss[h][w] / summ * data[((y + h - radius) * head_data.width + x - radius + w) * 3 + 1];
                        gaussumr += gauss[h][w] / summ * data[((y + h - radius) * head_data.width + x - radius + w) * 3 + 2];
                    }
                }
            }
            b = gaussumb;
            g = gaussumg;
            r = gaussumr;
            data[((y)*head_data.width + x) * 3] = b;
            data[((y)*head_data.width + x) * 3 + 1] = g;
            data[((y)*head_data.width + x) * 3 + 2] = r;
            towriteline2[x * 3] = b;
            towriteline2[x * 3 + 1] = g;
            towriteline2[x * 3 + 2] = r;
        }
        for (int i = 0; i < extrawidth; ++i)
        {
            towriteline2[head_data.width * 3 + i * 3] = 0;
            towriteline2[head_data.width * 3 + i * 3 + 1] = 0;
            towriteline2[head_data.width * 3 + i * 3 + 2] = 0;
        }
        fwrite(towriteline2, 1, head_data.width * 3 + extrawidth, outfile3);
    }
    for (int i = 0; i < extraheight; ++i)
    {
        for (int x = 0; x < head_data.width + extraheight; x++)
        {
            towriteline2[x * 3] = 0;
            towriteline2[x * 3 + 1] = 0;
            towriteline2[x * 3 + 2] = 0;
        }
        fwrite(towriteline2, 1, head_data.width * 3 + extraheight, outfile3);
    }


    delete[] data;

    return 0;


}