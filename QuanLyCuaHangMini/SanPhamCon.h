#ifndef SANPHAMCON_H
#define SANPHAMCON_H
#include "SanPham.h"
#include <iostream>
#include <map>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;
static void luuSanPhamRaFile(const SanPham& sanPham)
{
    ofstream out(SanPham::DefaultFileName(), ios::app);
    if (!out)
    {
        cerr << "Khong the mo file " << SanPham::DefaultFileName() << " de luu san pham." << endl;
        return;
    }

    out << sanPham.getLoai() << "|" << sanPham.getMaSP() << "|" << sanPham.getTen() << "|" << sanPham.getNSX()
        << "|" << sanPham.getGiaBan() << "|" << sanPham.getSoLuong() << "\n";
}

class DoDienTu : public SanPham
{
    public:
        DoDienTu();
        //override: ghi de len class SanPham,giup compiler kiem tra thong tin
        ~DoDienTu() override;
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

DoDienTu::DoDienTu() = default;
DoGiaDung::DoGiaDung() = default;

void DoDienTu :: Nhap(){SanPham::Nhap();}
void DoDienTu :: Xuat()const{
    cout<<"[DO DIEN TU]\n";
    SanPham::Xuat();
}
void DoDienTu::LuuFile() const
{
    luuSanPhamRaFile(*this);
}
string DoDienTu::getLoai() const
{
    return "DDT";
}
DoDienTu :: ~DoDienTu() = default;
DoGiaDung :: ~DoGiaDung() = default;
void DoGiaDung :: Nhap(){SanPham::Nhap();}
void DoGiaDung :: Xuat()const{
    cout << "[DO GIA DUNG]\n";
    SanPham::Xuat();
}
void DoGiaDung::LuuFile() const
{
    luuSanPhamRaFile(*this);
}
string DoGiaDung::getLoai() const
{
    return "DGD";
}
#endif
