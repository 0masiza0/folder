#include <cmath>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() {
    string num;
    vector <int> blocks = {};
    int fdig, k, sum, sign, edge, ans;
    cin >> k >> num;
    blocks.resize(251);
    sum = 0;
    fdig = 0;
    size_t i = 4;
    size_t j;
    switch (k) {
    /*case 1:
    case 5:
        cout << blocks[num.size() - 1] % k; break;
    case 2:
    case 4:
    case 8:
        if (num.size() < i) i = num.size();
        for (; i >= 0; --i) {
            fdig += blocks[num.size() - i] * pow(10, i);
        }
        cout << fdig % k; break;
    case 3:
    case 9:
        cout << sum % k; break;
    case 6:
        if (blocks[num.size() - 1] % 2 == 0) {
            switch (sum % 3) {
            case 0: cout << 0; break;
            case 1: cout << 4; break;
            case 2: cout << 2; break;
            }
        }
        else {
            switch (sum % 3) {
            case 0: cout << 3; break;
            case 1: cout << 1; break;
            case 2: cout << 5; break;
            }
        }*/
    case 7:
        sign = 1;
        sum = 0;
        i = 0;
        /*if (num.size() % 3 == 1) {
            sum = blocks[0];
            i = 1;
            sign = -1;
        }
        if (num.size() % 3 == 2) {
            sum = blocks[0] * 10 + blocks[1];
            i = 2;
            sign = -1;
        }*/
        for (; i < num.size(); i += 3) {
            edge = 0;
            for (j = 2; j >= 0; --j) {
                cout << i << " " << j << "\n";
                edge += blocks[i + 2 - j] * pow(10, j);
            }
            sum += sign * edge;
            sign = -sign;
        }
        cout << abs(sum % k); break;
    }
    cout << "\n";
    system("pause");
    return 0;
}
