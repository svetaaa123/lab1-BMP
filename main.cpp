#include <iostream>
#include <fstream>
using namespace std;
#pragma pack(push, 1)

struct BMPHeader {
    char signature[2];   // Сигнатура файла ('B' и 'M')
    int fileSize;        // Размер файла в байтах
    short reserved1;     // Зарезервированное значение
    short reserved2;     // Зарезервированное значение
    int dataOffset;      // Смещение данных пикселей от начала файла

    // Дополнительные поля для заголовка BMP
    int headerSize;      // Размер структуры заголовка BMP
    int width;           // Ширина изображения в пикселях
    int height;          // Высота изображения в пикселях
    short colorPlanes;   // Количество плоскостей (должно быть 1)
    short bitsPerPixel;  // Глубина цвета в битах
    int compression;     // Метод сжатия (обычно без сжатия = 0)
    int imageSize;       // Размер изображения в байтах (ширина * высота * глубина цвета)
    int horizontalRes;   // Горизонтальное разрешение в пикселях на метр
    int verticalRes;     // Вертикальное разрешение в пикселях на метр
    int numColors;       // Количество использованных цветов (0 - все)
    int importantColors; // Количество важных цветов (0 - все)
};

#pragma pack(pop)

void right(const unsigned char* oldFile, unsigned char* newFile, int width, int height) {
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            int newIndex = ((width-i-1) * height +j) *3;
            int oldIndex = (j*width +i) *3;
            for (int k = 0; k<3; ++k)
                newFile[newIndex + k] = oldFile[oldIndex + k];
        }
    }
}

void left(const unsigned char* oldFile, unsigned char* newFile, int width, int height) {
    for (int i = 0; i < height; ++i){
        for (int j = 0; j < width; ++j){
            int newIndex = ((height - i - 1) + j*height) * 3;
            int oldIndex = (i * width + j) * 3;

            for (int k = 0; k < 3; ++k)
                newFile[newIndex + k] = oldFile[oldIndex + k];
        }
    }
}

int main() {
    ifstream bmpImage("2.bmp", ios::binary);

    if (!bmpImage) {
        cout << "Failed to open the input file!" << endl;
        return 1;
    }

    BMPHeader bmpHeader{};
    bmpImage.read(reinterpret_cast<char*>(&bmpHeader), sizeof(BMPHeader));

    int weight = bmpHeader.fileSize;
    cout << "The size of memory being allocated: " << weight << " bytes" << endl;
    int width = bmpHeader.width;
    int height = bmpHeader.height;
    int size = width * height * 3;
    cout << width << endl;
    cout << height << endl;
    cout << size << endl;
    unsigned char* buff = new unsigned char[size];
    bmpImage.read(reinterpret_cast<char*>(buff), size);

    bmpImage.close();

    unsigned char* rotatedImageR = new unsigned char[size]; // для поворота по часовой стрелке
    unsigned char* rotatedImageL = new unsigned char[size]; // для поворота против часовой стрелки

    right(buff, rotatedImageR, width, height);
    left(buff, rotatedImageL, width, height);


    bmpHeader.dataOffset = sizeof(BMPHeader);
    bmpHeader.width = height;
    bmpHeader.height = width;

    ofstream clockwiseImage("image_rotated_R.bmp", ios::binary);
    ofstream counterClockwiseImage("image_rotated_L.bmp", ios::binary);

    clockwiseImage.write(reinterpret_cast<const char*>(&bmpHeader), sizeof(BMPHeader));
    clockwiseImage.write(reinterpret_cast<const char*>(rotatedImageR), size);

    counterClockwiseImage.write(reinterpret_cast<const char*>(&bmpHeader), sizeof(BMPHeader));
    counterClockwiseImage.write(reinterpret_cast<const char*>(rotatedImageL), size);

    clockwiseImage.close();
    counterClockwiseImage.close();

    delete[] buff;
    delete[] rotatedImageR;
    delete[] rotatedImageL;

    return 0;
}
