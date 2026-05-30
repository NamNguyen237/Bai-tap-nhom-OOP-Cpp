#include "QuanLy.h"
#include <iostream>
#include <map>
#include <string>
#include <iomanip>

using namespace std;

static map <string,int> DemMatHang;
SanPham :: SanPham()
{
    maSP = " ";
    ten = " ";
    GiaBan = 0;
    soluong = 0;
    NSX = " ";
}
SanPham :: ~SanPham(){}
string SanPham :: getMaSP() const {return maSP;}
int SanPham :: getGiaBan() const {return GiaBan;}
string SanPham :: getNSX() const {return NSX;}
void SanPham :: setMaSP(string ma) { this->maSP = ma;}
void SanPham :: Nhap()
{
    cout << "Nhap ten san pham: ";
    getline(cin,ten);

    cout << "Nhap nha san xuat: ";
    getline(cin,NSX);

    cout << "Nhap gia ban: ";
    cin >> GiaBan;

    while (GiaBan <= 0){
        cout << "Gia ban khong hop le!Xin vui long nhap lai:";
        cin >> GiaBan;
    }

    cout << "Nhap so luong con trong kho : ";
    cin >> soluong;

    DemMatHang[NSX]++;
    int stt = DemMatHang[NSX];
    string chuoiSTT = (stt < 10)?"0" + to_string(stt):to_string(stt);//them so "0" vao dang sau neu stt < 10
    maSP = NSX + chuoiSTT;
    cout << "San pham co ma: "<<maSP << endl;
}
void SanPham::Xuat()const
{
    cout << "Ma san pham : "<<maSP
         <<"\nTen mat hang: " <<ten
         <<"\nNSX: "<<NSX
         <<"\nGia: "<<GiaBan
         <<"\nSo luong con lai trong kho: "<<soluong;
}
void DoDienTu :: Nhap(){SanPham::Nhap();}
void DoDienTu :: Xuat()const{SanPham::Xuat();}
DoGiaDung :: ~DoGiaDung(){}
void DoGiaDung :: Nhap(){SanPham::Nhap();}
void DoGiaDung :: Xuat()const{SanPham::Xuat();}
