#ifndef SANPHAMCON_H
#define SANPHAMCON_H
#include "SanPham.h"
#include <iostream>
#include <map>
#include <string>
#include <iomanip>
#include <fstream>
class DoDienTu : public SanPham
{
    public:
        DoDienTu();
        ~DoDienTu() override; //override: ghi de len class SanPham,giup compiler kiem tra thong tin
        void Nhap() override;
        void Xuat() const override;
        void LuuFile() const override;
        string getLoai() const override;
};

class DoGiaDung : public SanPham
{
    public:
        DoGiaDung();
        ~DoGiaDung() override;
        void Nhap() override;
        void Xuat() const override;
        void LuuFile() const override;
        string getLoai() const override;
};

DoDienTu::DoDienTu() {}
DoGiaDung::DoGiaDung() {}

void DoDienTu :: Nhap(){SanPham::Nhap();}
void DoDienTu :: Xuat()const{
    cout<<"[DO DIEN TU]\n";
    SanPham::Xuat();
}
void DoDienTu::LuuFile() const
{
    ofstream out(SanPham::DefaultFileName(), ios::app);
    if (!out)
    {
        cerr << "Khong the mo file " << SanPham::DefaultFileName() << " de luu san pham." << endl;
        return;
    }
    out << getLoai() << "|" << getMaSP() << "|" << getTen() << "|" << getNSX()
        << "|" << getGiaBan() << "|" << getSoLuong() << "\n";
}
string DoDienTu::getLoai() const
{
    return "DDT";
}
DoDienTu :: ~DoDienTu(){}
DoGiaDung :: ~DoGiaDung(){}
void DoGiaDung :: Nhap(){SanPham::Nhap();}
void DoGiaDung :: Xuat()const{
    cout << "[DO GIA DUNG]\n";
    SanPham::Xuat();
}
void DoGiaDung::LuuFile() const
{
    ofstream out(SanPham::DefaultFileName(), ios::app);
    if (!out)
    {
        cerr << "Khong the mo file " << SanPham::DefaultFileName() << " de luu san pham." << endl;
        return;
    }
    out << getLoai() << "|" << getMaSP() << "|" << getTen() << "|" << getNSX()
        << "|" << getGiaBan() << "|" << getSoLuong() << "\n";
}
string DoGiaDung::getLoai() const
{
    return "DGD";
}
#endif