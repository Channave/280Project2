#include <iostream>
#include <fstream>
#include <cstdlib>
#include "Matrix.hpp"
#include "Image.hpp"
#include "processing.hpp"

using namespace std;

static void print_usage()
{
    cout << "Usage: resize.exe IN_FILENAME OUT_FILENAME WIDTH [HEIGHT]\n"
         << "WIDTH and HEIGHT must be less than or equal to original" << endl;
}

static bool open_input(const char *ifn, ifstream &in)
{
    in.open(ifn);
    if (!in)
    {
        cout << "Error opening file: " << ifn << endl;
        return false;
    }
    return true;
}

static bool read_header_and_init(ifstream &in, Image *img)
{
    string header;
    int iW = 0, iH = 0, maxint = 0;

    if (!(in >> header >> iW >> iH >> maxint))
        return false;

    Image_init(img, iW, iH);
    return true;
}

static void load_pixels(ifstream &in, Image *img, int iW, int iH)
{
    for (int r = 0; r < iH; r++)
    {
        for (int c = 0; c < iW; c++)
        {
            int rVal, gVal, bVal;
            in >> rVal >> gVal >> bVal;
            Pixel p;
            p.r = rVal;
            p.g = gVal;
            p.b = bVal;
            Image_set_pixel(img, r, c, p);
        }
    }
}

static bool validate_dims(Image *img, int width, int &height)
{
    if (height <= 0)
        height = Image_height(img);
    int origW = Image_width(img);
    int origH = Image_height(img);
    if (width <= 0 || width > origW || height <= 0 || height > origH)
    {
        print_usage();
        return false;
    }
    return true;
}

static bool open_output(const char *ofn, ofstream &out)
{
    out.open(ofn);
    if (!out)
    {
        cout << "Error opening file: " << ofn << endl;
        return false;
    }
    return true;
}

static void write_ppm(ofstream &out, const Image *img, int width, int height)
{
    out << "P3\n";
    out << width << " " << height << "\n";
    out << 255 << "\n";
    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            Pixel px = Image_get_pixel(img, r, c);
            out << px.r << " " << px.g << " " << px.b;
            if (c + 1 < width)
                out << " ";
        }
        out << " \n";
    }
}

int main(int argc, char *argv[])
{
    if (argc != 4 && argc != 5)
    {
        print_usage();
        return 1;
    }

    const char *ifn = argv[1];
    const char *ofn = argv[2];
    int width = atoi(argv[3]);
    int height = 0;
    if (argc == 5)
        height = atoi(argv[4]);

    ifstream in;
    if (!open_input(ifn, in))
        return 1;
        
    Image img;
    if (!read_header_and_init(in, &img))
    {
        cout << "Error opening file: " << ifn << endl;
        return 1;
    }
    int iW = Image_width(&img);
    int iH = Image_height(&img);
    load_pixels(in, &img, iW, iH);
    in.close();

    if (!validate_dims(&img, width, height))
        return 1;

    seam_carve(&img, width, height);

    ofstream out;
    if (!open_output(ofn, out))
        return 1;
    write_ppm(out, &img, width, height);
    out.close();

    return 0;
}