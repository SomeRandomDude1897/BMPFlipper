#include <iostream>
#include <fstream>
#include <vector>

#pragma warning(disable:4996)
#define min(a,b) ((a)<(b)?(a):(b))

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
struct pixel {
    uint32_t b;
    uint32_t g;
    uint32_t r;
};
int main()
{
    FILE* file = fopen("sea.bmp", "rb");

    header_data headdata;
    unsigned char info[14];

    fread(info, 1, 14, file);
    fread((char*)&headdata, 1, 40, file);
    cout << "File size: " << *(int*)&info[2] / 1024 << " bytes" << endl;

    unsigned char* extra_header_data = new unsigned char[headdata.dib - 40];

    fread(extra_header_data, 1, headdata.dib - 40, file);

    int size = headdata.width * headdata.height * 3;

    unsigned char* data = new unsigned char[size];

    fread(data, 1, size, file);

    fclose(file);

    header_data headdata2 = headdata;
    headdata2.width = headdata.height;
    headdata2.height = headdata.width;

    FILE* outfile1 = fopen("flipped right.bmp", "wb");
    fwrite((const char*)&info, 1, 14, outfile1);
    fwrite((const char*)&headdata2, 1, 40, outfile1);
    fwrite(extra_header_data, 1, headdata.dib - 40, outfile1);
    int extraheight = headdata.height % 4;
    int extrawidth = headdata.width % 4;

    for (int y = 0; y < headdata.width; y++)
    {
        unsigned char* towriteline = new unsigned char[headdata.height * 3 + extraheight];
        for (int x = 0; x < headdata.height; x++)
        {
            int b = data[(x * headdata.width + (headdata.width - y - 1)) * 3];
            int g = data[(x * headdata.width + (headdata.width - y - 1)) * 3 + 1];
            int r = data[(x * headdata.width + (headdata.width - y - 1)) * 3 + 2];
            towriteline[x * 3] = b;
            towriteline[x * 3 + 1] = g;
            towriteline[x * 3 + 2] = r;
        }
        for (int i = 0; i < extraheight; ++i)
        {
            towriteline[headdata.height * 3 + i * 3] = 0;
            towriteline[headdata.height * 3 + i * 3 + 1] = 0;
            towriteline[headdata.height * 3 + i * 3 + 2] = 0;
        }
        fwrite(towriteline, 1, headdata.height * 3 + extraheight, outfile1);
    }
    for (int i = 0; i < extrawidth; ++i)
    {
        unsigned char* towriteline = new unsigned char[headdata.height * 3 + extraheight];
        for (int x = 0; x < headdata.height + extraheight; x++)
        {
            towriteline[x * 3] = 0;
            towriteline[x * 3 + 1] = 0;
            towriteline[x * 3 + 2] = 0;
        }
        fwrite(towriteline, 1, headdata.height * 3 + extraheight, outfile1);
    }

    fclose(outfile1);

    FILE* outfile2 = fopen("flipped left.bmp", "wb");
    fwrite((const char*)&info, 1, 14, outfile2);
    fwrite((const char*)&headdata2, 1, 40, outfile2);
    fwrite(extra_header_data, 1, headdata.dib - 40, outfile2);
    for (int y = 0; y < headdata.width; y++)
    {
        unsigned char* towriteline = new unsigned char[headdata.height * 3 + extraheight];
        for (int x = headdata.height - 1; x >= 0; x--)
        {
            int b = data[(x * headdata.width + (headdata.width - y - 1)) * 3];
            int g = data[(x * headdata.width + (headdata.width - y - 1)) * 3 + 1];
            int r = data[(x * headdata.width + (headdata.width - y - 1)) * 3 + 2];
            towriteline[(headdata.height - x) * 3] = b;
            towriteline[(headdata.height - x) * 3 + 1] = g;
            towriteline[(headdata.height - x) * 3 + 2] = r;
        }
        for (int i = 0; i < extraheight; ++i)
        {
            towriteline[headdata.height * 3 + i * 3] = 0;
            towriteline[headdata.height * 3 + i * 3 + 1] = 0;
            towriteline[headdata.height * 3 + i * 3 + 2] = 0;
        }
        fwrite(towriteline, 1, headdata.height * 3 + extraheight, outfile2);
    }
    for (int i = 0; i < extrawidth; ++i)
    {
        unsigned char* towriteline = new unsigned char[headdata.height * 3 + extraheight];
        for (int x = 0; x < headdata.height + extraheight; x++)
        {
            towriteline[x * 3] = 0;
            towriteline[x * 3 + 1] = 0;
            towriteline[x * 3 + 2] = 0;
        }
        fwrite(towriteline, 1, headdata.height * 3 + extraheight, outfile2);
    }
    double pi = 3.14;
    int r = 2;
    double sigma = 3;
    vector<vector<double>> gauss;
    double summ = 0;
    for (int h = 0; h < 2 * r; h++) {
        gauss.push_back(vector<double>());
        for (int w = 0; w < 2 * r; w++) {
            gauss[h].push_back(exp(-((h - r) * (h - r) + (w - r) * (w - r)) / (2 * (sigma * sigma))) / (2 * pi * sigma * sigma));
            summ += gauss[h][w];
        }
    }
    FILE* outfile3 = fopen("with gaussian filter.bmp", "wb");
    fwrite((const char*)&info, 1, 14, outfile3);
    fwrite((const char*)&headdata, 1, 40, outfile3);
    fwrite(extra_header_data, 1, headdata.dib - 40, outfile3);
    for (int y = 0; y < headdata.height; y++)
    {
        unsigned char* towriteline = new unsigned char[headdata.width * 3 + extrawidth];
        for (int x = 0; x < headdata.width; x++)
        {
            double gaussumb = 0;
            double gaussumg = 0;
            double gaussumr = 0;
            for (int h = 0; h < 2 * r; h++) {
                for (int w = 0; w < 2 * r; w++) {
                    if (y + h - r >= 0 && x + w - r >= 0 && y + h - r < headdata.height && x + w - r < headdata.width)
                    {
                        gaussumb += gauss[h][w] / summ * data[((y + h - r) * headdata.width + x - r + w) * 3];
                        gaussumg += gauss[h][w] / summ * data[((y + h - r) * headdata.width + x - r + w) * 3 + 1];
                        gaussumr += gauss[h][w] / summ * data[((y + h - r) * headdata.width + x - r + w) * 3 + 2];
                    }
                }
            }
            int b = gaussumb;
            int g = gaussumg;
            int r = gaussumr;
            data[((y)*headdata.width + x) * 3] = b;
            data[((y)*headdata.width + x) * 3 + 1] = g;
            data[((y)*headdata.width + x) * 3 + 2] = r;
            towriteline[x * 3] = b;
            towriteline[x * 3 + 1] = g;
            towriteline[x * 3 + 2] = r;
        }
        for (int i = 0; i < extrawidth; ++i)
        {
            towriteline[headdata.width * 3 + i * 3] = 0;
            towriteline[headdata.width * 3 + i * 3 + 1] = 0;
            towriteline[headdata.width * 3 + i * 3 + 2] = 0;
        }
        fwrite(towriteline, 1, headdata.width * 3 + extrawidth, outfile3);
    }
    for (int i = 0; i < extraheight; ++i)
    {
        unsigned char* towriteline = new unsigned char[headdata.height * 3 + extraheight];
        for (int x = 0; x < headdata.width + extraheight; x++)
        {
            towriteline[x * 3] = 0;
            towriteline[x * 3 + 1] = 0;
            towriteline[x * 3 + 2] = 0;
        }
        fwrite(towriteline, 1, headdata.width * 3 + extraheight, outfile3);
    }
    fclose(outfile2);


}