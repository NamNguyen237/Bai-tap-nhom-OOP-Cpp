#include "QuanLy.h"
#include <iostream>
#include <map>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;

static map <string,int> DemMatHang;
static string RutGonNSX(const  string &nsx)
{
    if (nsx.length() <= 2)return nsx;

    const string NguyenAm = "aeiouAEIOU";//neu gap cac nguyen am
    string result = "";//bo qua nguyen am
    for(char c : nsx)
    {
        if(isalpha(c)&&NguyenAm.find(c) == string :: npos)//ktra xem co phai chu cai, va cac chu do co cac nguyen am khong
        result += (char)toupper(c);
        if (result.size() == 2) break;
    }
    return result;
}
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
    cin >> NSX;

    cout << "Nhap gia ban: ";
    cin >> GiaBan;

    while (GiaBan <= 0){
        cout << "Gia ban khong hop le!Xin vui long nhap lai:";
        cin >> GiaBan;
    }
    
    cout << "Nhap so luong trong kho : ";
    cin >> soluong;

    string prefix = RutGonNSX(NSX);
    DemMatHang[prefix]++;//dem sp cua tung hang vi du "[SN] = 1"
    int stt = DemMatHang[prefix];//so thu tu = so sp SN01,SN02,...
    string chuoiSTT = (stt < 10)?"0" + to_string(stt):to_string(stt);//them so "0" vao dang sau neu stt < 10
    maSP = prefix + chuoiSTT;
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
void DoDienTu :: Xuat()const{
    cout<<"[DO GIA DUNG]\n";
    SanPham::Xuat();
}
DoGiaDung :: ~DoGiaDung(){}
void DoGiaDung :: Nhap(){SanPham::Nhap();}
void DoGiaDung :: Xuat()const{
    cout << "[DO DIEN TU]\n";
    SanPham::Xuat();
}
