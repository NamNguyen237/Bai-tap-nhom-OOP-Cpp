#ifndef QUANLY_H
#define QUANLY_H
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class SanPham
{
    private:
        string maSP,NSX,ten;
        int GiaBan;
        int soluong;
    protected:
        virtual string LayTienToMa() const;
    public:
        SanPham();
        virtual ~SanPham();//tranh ro ri bo nho -_-
        string getMaSP() const;
        int getGiaBan() const;
        string getNSX() const;
        void setMaSP(string ma);
        virtual void Nhap();
        virtual void Xuat()const ;
        //17-19:dung de cac class khac (HoaDon,KhachHang) lay du lieu
};

class DoDienTu : public SanPham
{
    private:
        int baohanh;
        int congSuat;
    protected:
        string LayTienToMa() const override;
    public:
        DoDienTu();
        ~DoDienTu() override; //override: ghi de len class SanPham,giup compiler kiem tra thong tin
        void Nhap() override;
        void Xuat() const override;
};

class DoGiaDung : public SanPham
{
    private:
        string chatLieu;
        string congDung;
    protected:
        string LayTienToMa() const override;
    public:
        DoGiaDung();
        ~DoGiaDung() override;
        void Nhap() override;
        void Xuat() const override;
};

class HoaDon
{
    private:
        string MaHD;
        vector <SanPham*> DanhSachSP;
    public:
        HoaDon();
        ~HoaDon();
        int TongTien();
        void inHoaDon() const;
        HoaDon& operator+(SanPham* sp);//cong don hoa don
        int getTongTien() const;
        void LuuFile(const string &tenFile) const;
};

class KhachHang
{
    private:
        string TenKH;
        vector<HoaDon*> LichSuMuaHang;
    public:
        KhachHang();
        KhachHang(string ten);
        ~KhachHang();
        void ThemHoaDonDaMua(HoaDon* hd);
        void ThongTinKhachHang() const;
};
#endif
