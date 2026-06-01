#ifndef HOADON_H
#define HOADON_H
#include "SanPham.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;
struct ChiTietHoaDon
{
    SanPham *sp;
    int SoLuongMua;
    string maSP;
    string tenSP;
    string nsx;
    int giaBan;
    int soLuongMua;
};
class HoaDon
{
    private:
        string MaHD;
        string TenKH;
        vector <ChiTietHoaDon> DanhSachSP;
    public:
        HoaDon();
        ~HoaDon();
        int TongTien();
        void inHoaDon() const;
        bool themSanPham(SanPham* sp, int soLuongMua);
        HoaDon& operator+(SanPham* sp);//cong don hoa don
        void setTenKH(const string &TenKH);
        int getTongTien() const;
        void LuuFile(const string &tenFile) const;
};
bool HoaDon::themSanPham(SanPham* sp, int soLuongMua)
{
    if (sp == NULL)
        return false;

    if (soLuongMua <= 0)
    {
        cout << "So luong mua khong hop le.\n";
        return false;
    }

    if (soLuongMua > sp->getSoLuong()){
        cout << "Trong kho khong du! Con lai "<<sp->getSoLuong()<<" san pham\n";
        return false;// khong them vao hoa don
    }

    sp->CheckSoLuongKho(soLuongMua);

    ChiTietHoaDon chiTiet;
    chiTiet.maSP = sp->getMaSP();
    chiTiet.tenSP = sp->getTen();
    chiTiet.nsx = sp->getNSX();
    chiTiet.giaBan = sp->getGiaBan();
    chiTiet.soLuongMua = soLuongMua;
    DanhSachSP.push_back(chiTiet);

    cout << "Da them ["<<sp->getMaSP()<<"] x"<<soLuongMua<<" vao hoa don "<<MaHD<<endl;
    return true;
}
HoaDon::HoaDon()
{
    MaHD = "Ma HD " + to_string(rand()%9000+1000);//random so ma HD
    TenKH = "";
}
HoaDon::~HoaDon()
{
    DanhSachSP.clear();//xoa danh sach
}
HoaDon &HoaDon::operator+(SanPham* sp)
{
    if (sp == NULL)
    {
        cout << "San pham khong hop le.\n";
        return *this;
    }
    int SoLuongMua = 0;
    cout << "Nhap so luong mua: ";
    cin >> SoLuongMua;
    sp -> CheckSoLuongKho(SoLuongMua);
    ChiTietHoaDon chiTiet;
    chiTiet.sp = sp;
    chiTiet.SoLuongMua = SoLuongMua;
    DanhSachSP.push_back(chiTiet);
    cout << "Da them ["<<sp->getMaSP()<<"] "<<"x"<<SoLuongMua<<" vao hoa don"<<MaHD<<endl;
        
    return *this;//tro lai hoa don hien tai de tiep tuc mua tiep
}
int HoaDon::TongTien()
{
    int tong = 0;
    for (const auto& chiTiet : DanhSachSP){
        tong += chiTiet.sp->getGiaBan() * chiTiet.SoLuongMua;//lay gia ban cua tung loai san pham
    }
    return tong;
}
int HoaDon::getTongTien() const//tong so tien cac san pham 
{
    int tong = 0;
    for (const auto& chiTiet : DanhSachSP){//chay 1 vong kiem tra cac san pham trong hoa don
        tong += chiTiet.sp->getGiaBan() * chiTiet.SoLuongMua;
    }
    return tong;
}
void HoaDon::inHoaDon() const 
{
    cout << "====HOA DON MAT HANG===="<<endl;
    cout << "Ma HD: "<<MaHD<<endl;
    cout << "Ten KH: "<<TenKH<<endl;
    if (DanhSachSP.empty())
    {
        cout << "Khong co san pham nao!"<<endl;
    }else{
        for (size_t i = 0; i < DanhSachSP.size();i++){//size_t:kich thuoc cua danh sach
        const ChiTietHoaDon &sp = DanhSachSP[i];
                    cout << i + 1 << ". Ma SP: " << sp.maSP
                         << " | Ten: " << sp.tenSP
                         << " | NSX: " << sp.nsx
                         << " | Don gia: " << sp.giaBan
                         << " | SL: " << sp.soLuongMua
                         << " | Thanh tien: " << sp.giaBan * sp.soLuongMua << " VND\n";
        }
    }
    cout <<"\nTONG TIEN THANH TOAN: "<< getTongTien()<<"VND"<<endl;
}
void HoaDon :: LuuFile(const string &TenFile)const 
{
    ofstream outFile(TenFile,ios::app);//open file to continue write chu khong xoa di noi dung cu
    if (!outFile)
    {
        cerr << "Loi!Khong the mo file "<<TenFile<<"!"<<endl;
        return;
    }
    outFile << "========================================================\n";
    outFile << " HOA DON BAN HANG: " << MaHD << "\n";
    outFile << "--------------------------------------------------------\n";
    for (size_t i = 0; i < DanhSachSP.size();i++)
    {
       const ChiTietHoaDon &sp = DanhSachSP[i];
               outFile << " "<<i+1<<". MaSP: "<<sp.maSP
                       << " | Ten: " << sp.tenSP
                       << " | NSX: " << sp.nsx
                       << " | Don gia: " << sp.giaBan
                       << " | SL: " << sp.soLuongMua
                       << " | Thanh tien: " << sp.giaBan * sp.soLuongMua << " VND\n";
    }
    outFile << "--------------------------------------------------------\n";
    outFile << " TONG TIEN: " << getTongTien() << " VND\n";
    outFile << "========================================================\n\n";
    outFile.close();
    cout << "Da luu hoa don "<<MaHD<<"vao file "<<TenFile<<endl;
}
#endif
