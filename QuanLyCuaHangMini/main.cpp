#include <iostream>
#include "Menu.h"

using namespace std;

// Dinh nghia bien tinh statics cho lop SanPham
map<string,int> SanPham::DemMatHang;

int main()
{
    // Vong lap menu giao dien
    while (true)
    {
        hienThiMenu();

        int luaChon = 0;
        if (!(cin >> luaChon))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (luaChon == 9)
            break;

        xuLyLuaChon(luaChon);
    }

    cout << "Ket thuc chuong trinh.\n";
    return 0;
};
