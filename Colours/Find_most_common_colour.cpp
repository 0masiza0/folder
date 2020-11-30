#include <clocale>
#include <iostream>
#include "CImg.h"           //  библиотека для работы с изображениями
#include <string>
#include <tuple>
#include <unordered_map>

using namespace cimg_library;

//  В переменной типа Colour хранятся данные об интенсивности цветовых каналов (RGB) отдельного пикселя
struct Colour {
    unsigned char
            r = 0,
            g = 0,
            b = 0;
};

//  Чтобы использовать Colour в качестве ключа в unordered_map, определим хэш-функцию.
//  В свою очередь хэш-функция требует определения оператора ==
//  (lhs - left-hand side, rhs - right-hand side)
bool operator== (const Colour& lhs, const Colour& rhs) {
    return std::tie(lhs.r, lhs.g, lhs.b) ==
           std::tie(rhs.r, rhs.g, rhs.b);
}

//  Используем полиномиальный хэш вида x1 + x2 * p + ... + xn * p ^ (n - 1).
//  К p предъявлено следующее требование: p > самого большого кода цвета (> 255);
//  Максимальное значение хэша при p = 256 равно 255 + 255 * 256 + 255 * 256 ^ 2 = 16'777'215,
//  что умещается в переменную типа uint64_t, а значит, переполнения не случится и
//  брать в качестве хэша остаток от деления на "размер" переменной необязательно
struct HashFunction {
    uint64_t operator() (const Colour &in_key) const {
        return in_key.r +
               in_key.g * coef +
               in_key.b * coef * coef;
    }
    const uint64_t coef = 256;
};

//  Псевдоним для часто употребляемого типа
using u_map_colour = std::unordered_map<Colour, size_t, HashFunction>;


//  Обернём методы для работы с окном вывода (дисплеем) в отдельный класс
class Display {
protected:

    //  Тип CImg<T> представляет изображение в виде сетки пикселей со значениями типа T (здесь unsigned char)
    //  Сетка имеет размерность 4: ширина, высота, глубина самого изображения и число цветовых каналов пикселей
    //  (= 3 для RGB).
    CImg<unsigned char> display;

public:
    explicit Display(size_t width, size_t height, size_t colour_dim) {
        //  Значение глубины изображения задаётся как 1, потому что работа осуществляется с "плоским" 2D изображением.
        //  (Глубина 0 соответствует пустому изображению).
        display = CImg<unsigned char>(width, height, 1, colour_dim);
    }

    //  Заполнить дисплей цветом colour
    CImg<unsigned char> Fill(const Colour& colour) {
        return display.fill(colour.r, colour.g, colour.b);
    }

    //  Вывести дисплей с названием name
    void Print(const char*& name) {
        display.display(name);
    }
};


//  Обернём методы для работы с входным изображением в отдельный класс
class Image {
private:

    //  Устройство типа CImg<T> см. в секции private класса Display
    CImg<unsigned char> image;

public:

    //  Создание объекта типа CImg<unsigned char> из изображения с адресом input_path
    explicit Image(const char*& input_path) {
        image = CImg<unsigned char>(input_path);
    }

    //  Функция сохраняет в u_map_colour freqs данные о том, сколько раз встретилась та или иная комбинация RGB
    [[nodiscard]]
    u_map_colour CountFrequencies() const {
        u_map_colour freqs;
        unsigned char red, green, blue;

        for (size_t i = 0; i < image.width(); ++i) {
            for (size_t j = 0; j < image.height(); ++j) {
                //  В image(i, j, 0, k) хранится интенсивность k-го цветового канала пикселя с координатами i, j.
                //  (k = 0, 1, 2 для красного, зелёного и синего каналов соответственно)
                std::tie(red, green, blue) = std::tie(image(i, j, 0, 0),
                                                      image(i, j, 0, 1),
                                                      image(i, j, 0, 2));
                ++freqs[{red, green, blue}];
            }
        }
        return freqs;
    }
};


//  Функция ищет пиксель, встречающийся наиболее часто.
//  Данные о RGB составляющих пикселя схраняются в max_freq_colour (max frequent colour);
//  Частотность этого пикселя сохраняется в max_freq
Colour FindMostPopularColour(const u_map_colour& freqs) {
    size_t max_freq = 0;
    Colour max_freq_colour;
    for (const auto& [pixel, freq] : freqs) {
        if (freq > max_freq) {
            max_freq_colour = pixel;
            max_freq = freq;
        }
    }
    return max_freq_colour;
}


//  Создадим объект типа Image (внутри хранится CImg массив) по изображению с адресным путём input_path.
//  Аргументы конструкторов CImg массивов требуют в качестве названия строку типа const char*,
//  поэтому input_path преобразуем к этому виду.
//  Создадим дисплей Display (внутри также хранится CImg массив) с названием title,
//  размерами 1 х 1 пиксель и палитрой RGB.
//  Далее:
//      Посчитаем частотность встречаемостей различных пикселей изображения (image.CountFrequencies());
//      Найдём самый частый пиксель (FindMostPopularColour);
//      "Зальём" этим цветом дисплей (display.Fill);
//  Выведем результат на экран
int main() {
    std::setlocale(LC_ALL, "Russian");

    std::cout << "Введите путь к входному файлу:\n";

    std::string input_path;
    std::cin >> input_path;

    const char* title = "Самый распространённый цвет";
    const char* input_path_c = input_path.c_str();

    Image image(input_path_c);
    Display display(1, 1, 3);

    display.Fill(
            FindMostPopularColour(
                    image.CountFrequencies()
            )
    );
    display.Print(title);

    return 0;
}
