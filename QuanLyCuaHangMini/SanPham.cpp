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
    GiaBan = 0.0;
    soluong = 0;
    NSX = " ";
}
SanPham :: ~SanPham()
{

}
string SanPham :: getMaSP() const {return maSP;}
double SanPham :: getGiaBan() const {return GiaBan;}
string SanPham :: getNSX() const {return NSX;}
void SanPham :: setMaSP(string maSP) {maSP  = maSP;}
void SanPham :: Nhap()
{
    cout << "Nhap ten san pham: ";
    cin.ignore();
    getline(cin,ten);

    cout << "Nhap nha san xuat: ";
    getline(cin,NSX);

    cout << "Nhap gia ban: ";
    cin >> GiaBan;

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
DoDienTu :: DoDienTu() : SanPham()//chon DoDienTu() tu lop con DoDienTu ben trong SanPham()
{
    NSX = " ";
}
DoDienTu :: ~DoDienTu()
{

}
void DoDienTu :: Nhap()
{
    SanPham :: Nhap();//tinh thua ke

    cout << "Nhap nha san xuat : ";
    cin >> NSX;
}
void DoDienTu :: Xuat()const
{
    SanPham :: Xuat();

    cout <<"San xuat boi "<<NSX<<endl;
}
DoGiaDung :: DoGiaDung() : SanPham()
{
    NSX = " ";
}
DoGiaDung :: ~DoGiaDung()
{

}
void DoGiaDung :: Nhap()
{
    SanPham :: Nhap();
    cout << "Nhap nha san xuat : ";
    cin >> NSX;
}
void DoGiaDung :: Xuat()const
{
    SanPham :: Xuat();

    cout <<"San xuat boi "<<NSX<<endl;
}
