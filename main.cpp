#include <iostream>
#include <fstream>
#include <windows.h>

using namespace std;

int main(){

    BITMAPFILEHEADER header;
    ifstream image(R"(image.bmp)", ifstream::binary);
    if (!image){
        cerr << "File opening failed!" << endl;
        return 1;}
    image.read(reinterpret_cast<char *>(&header), sizeof(header));
    if (header.bfType != 0x4d42) {
        cout << "Wrong file format!" << endl;
        return 11;
    }

    image.read(reinterpret_cast<char*>(&header), sizeof(header));
    unsigned long weight = header.bfSize;
    cout << "The size of memory being allocated: " << weight << " bytes" << endl;


    image.seekg(sizeof(BITMAPFILEHEADER));
    BITMAPINFOHEADER info;
    image.read(reinterpret_cast<char*>(&info), sizeof(BITMAPINFOHEADER));
    int height = info.biHeight;
    int width = info.biWidth;
    int size = width * height;

//    int pixel = info.biBitCount / 8;
    char* buff = new char[3 * size];
    unsigned long start = header.bfOffBits;

    image.seekg(start);
    image.read(buff, size*3);
    info.biWidth = height;
    info.biHeight = width;




// вправо
    char* rightRot = new char[3 * size];
    for (int i = 0; i<width; ++i){
        for (int j = 0; j<height; ++j){
            int newIndexR = ((width-i-1) * height +j) *3;
            int oldIndexR = (j*width +i) *3;

            for (int k = 0; k<3; ++k)
                rightRot[newIndexR + k] = buff[oldIndexR + k];}}

    ofstream rightRotatedImage("Right.bmp", ofstream::binary);
    if (!rightRotatedImage){
        delete[] buff;
        delete[] rightRot;
        image.close();
        cerr << "Error" << endl;
        return 1;
    }
    image.seekg(0, std::ifstream::beg);
    rightRotatedImage << image.rdbuf();
    rightRotatedImage.seekp(sizeof(BITMAPFILEHEADER));
    rightRotatedImage.write(reinterpret_cast<char*>(&info), sizeof(BITMAPINFOHEADER));
    rightRotatedImage.seekp(start);
    rightRotatedImage.write(rightRot, size*3);
    rightRotatedImage.close();

// влево
    char* leftRot = new char[3 * size];
    for (int i = 0; i < height; ++i){
        for (int j = 0; j < width; ++j){
            int newIL = ((height - i - 1) + j*height) * 3;
            int oldIL = (i * width + j) * 3;

            for (int k = 0; k < 3; ++k)
                leftRot[newIL + k] = buff[oldIL + k];
        }
    }

    ofstream leftRotatedImage("Left.bmp", ofstream::binary);
    if (!leftRotatedImage){
        delete[] buff;
        delete[] leftRot;
        image.close();
        cerr << "Error" << endl;
        return 1;
    }
    image.seekg(0, std::ifstream::beg);
    leftRotatedImage << image.rdbuf();
    image.close();
    leftRotatedImage.seekp(sizeof(BITMAPFILEHEADER));
    leftRotatedImage.write(reinterpret_cast<char*>(&info), sizeof(BITMAPINFOHEADER));
    leftRotatedImage.seekp(start);
    leftRotatedImage.write(leftRot, size*3);
    leftRotatedImage.close();



    delete[] buff;
    delete[] rightRot;
    delete[] leftRot;


    return 0;
}
