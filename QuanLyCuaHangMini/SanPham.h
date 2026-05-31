#ifndef SANPHAM_H
#define SANPHAM_H
#include <iostream>
#include <map>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;


class SanPham
{
    private:
        string maSP,NSX,ten;
        int GiaBan;
        int soluong;
    public:
        SanPham();
        virtual ~SanPham();//tranh ro ri bo nho -_-
        //dung de cac class khac (HoaDon,KhachHang) lay du lieu
        string getMaSP() const;
        string getTen() const;
        int getGiaBan() const;
        string getNSX() const;

        int getSoLuong() const {return soluong;}//Khach hang mua bao nhieu
        void CheckSoLuongKho(int SoLuongMua){soluong -= SoLuongMua;}//check soluong trong kho

        void setMaSP(const string &ma);
        void setTen(const string &ten);
        void setNSX(const string &nsx);
        void setGiaBan(int giaBan);
        void setSoLuong(int soLuong);

        virtual void Nhap();
        virtual void Xuat()const ;
        virtual void LuuFile() const = 0;
        virtual string getLoai() const = 0;
        static string RutGonNSX(const string &nsx);
        static const string &DefaultFileName();
        static map<string,int> DemMatHang;
};


string SanPham::RutGonNSX(const string &nsx)
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
string SanPham :: getTen() const {return ten;}
int SanPham :: getGiaBan() const {return GiaBan;}
string SanPham :: getNSX() const {return NSX;}
void SanPham :: setMaSP(const string &ma) { this->maSP = ma;}
void SanPham :: setTen(const string &ten) { this->ten = ten; }
void SanPham :: setNSX(const string &nsx) { this->NSX = nsx; }
void SanPham :: setGiaBan(int giaBan) { this->GiaBan = giaBan; }
void SanPham :: setSoLuong(int soLuong) { this->soluong = soLuong; }
const string &SanPham::DefaultFileName() { static const string fileName = "sanpham.txt"; return fileName; }
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
#endif