#ifndef HOADON_H
#define HOADON_H
#include "SanPham.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <limits>

using namespace std;
struct ChiTietHoaDon
{
    SanPham *sp = nullptr;
    int SoLuongMua = 0;
    string maSP;
    string tenSP;
    string nsx;
    int giaBan = 0;
    int soLuongMua = 0;
};
class HoaDon
{
    private:
        string MaHD;
        string TenKH;
        vector <ChiTietHoaDon> DanhSachSP;
        long long tong = 0;
    public:
        HoaDon();
        ~HoaDon();
        int TongTien();
        void inHoaDon();
        bool themSanPham(SanPham* sp, int soLuongMua);
        bool operator+(SanPham* sp);//cong don hoa don
        HoaDon& operator-();//xoa sp khi khach hang doi y
        void setTenKH(const string &TenKH);
        int getTongTien() const;
        void LuuFile(const string &tenFile) const;
};
void HoaDon :: setTenKH(const string &TenKH)//ghi ten khach hang
{
    this->TenKH = TenKH;
}

HoaDon::HoaDon()// ham khoi tao
{
    MaHD = "Ma HD " + to_string(rand()%9000+1000);//random so ma HD
    TenKH = "";
}
HoaDon::~HoaDon()//ham huy
{
    DanhSachSP.clear();//xoa danh sach
}
bool HoaDon::themSanPham(SanPham* sp, int soLuongMua)//them hang vao hoa don theo so luong tuy y
{
    if (sp == nullptr)//kiem tra san pham co hop le khong
        return false;

    if (soLuongMua <= 0)//kiem tra neu so luong mua <= 0
    {
        cout << "So luong mua khong hop le.\n";
        return false;
    }

    if (soLuongMua > sp->getSoLuong())//kiem tra neu so luong mua vuot qua so luong trong kho
    {
        cout << "Trong kho khong du! Con lai "<<sp->getSoLuong()<<" san pham\n";
        return false;// khong them vao hoa don
    }

    sp->CheckSoLuongKho(soLuongMua);//giam so luong kho sau khi mua thanh cong

    ChiTietHoaDon chiTiet;
    chiTiet.sp = sp;
    chiTiet.SoLuongMua = soLuongMua;
    chiTiet.maSP = sp->getMaSP();
    chiTiet.tenSP = sp->getTen();
    chiTiet.nsx = sp->getNSX();
    chiTiet.giaBan = sp->getGiaBan();
    chiTiet.soLuongMua = soLuongMua;
    DanhSachSP.push_back(chiTiet);

    cout << "Da them ["<<sp->getMaSP()<<"] x"<<soLuongMua<<" vao hoa don "<<MaHD<<endl;
    return true;
}
bool HoaDon::operator+(SanPham* sp){//them san pham theo yeu cau so luong cua nguoi mua
    if (sp == nullptr)
    {
        cout << "San pham khong hop le.\n";
        return false;
    }

    int SoLuongMua = 0;
    cout << "Nhap so luong mua: ";
    if (!(cin >> SoLuongMua))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');//huy ki tu thua sau khi nhap so luong mua
        cout << "So luong mua khong hop le.\n";
        return false;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    return themSanPham(sp, SoLuongMua);
}

HoaDon &HoaDon::operator-()//xoa san pham moi nhat ban vua mua
{
    if (DanhSachSP.empty())
    {
        cout << "Khong co san pham nao de xoa trong hoa don.\n";
        return *this;
    }

    ChiTietHoaDon chiTiet = DanhSachSP.back();
    DanhSachSP.pop_back();

    if (chiTiet.sp)
    {
        chiTiet.sp->setSoLuong(chiTiet.sp->getSoLuong() + chiTiet.soLuongMua);
    }

    cout << "Da xoa san pham gan nhat: " << chiTiet.maSP
         << " x" << chiTiet.soLuongMua << "\n";
    return *this;
}

int HoaDon::TongTien()
{
    return getTongTien();
}
int HoaDon::getTongTien() const // tinh hoac cap nhat tong tien hoa don
{
    int tongTien = 0;
    for (const auto& chiTiet : DanhSachSP)//duyet tung dong hoa don
        tongTien += chiTiet.giaBan * chiTiet.soLuongMua;//lay gia ban cua tung loai san pham

    return tongTien;
}
void HoaDon::inHoaDon()
{
    cout << "====HOA DON MAT HANG===="<<endl;
    cout << "Ma HD: "<<MaHD<<endl;
    cout << "Ten KH: "<<TenKH<<endl;
    if (DanhSachSP.empty())
    {
        cout << "Khong co san pham nao!"<<endl;
    }
    else
    {
        for (size_t i = 0; i < DanhSachSP.size(); i++)//size_t:kich thuoc cua danh sach
        {
            const ChiTietHoaDon &sp = DanhSachSP[i];
            int thanhTien = sp.giaBan * sp.soLuongMua;
            cout << i + 1 << ". Ma SP: " << sp.maSP
                 << " | Ten: " << sp.tenSP
                 << " | NSX: " << sp.nsx
                 << " | Don gia: " << SanPham::ChuanHoaGiaBan(to_string(sp.giaBan)).second << " VND"
                 << " | SL: " << sp.soLuongMua
                 << " | Thanh tien: " << SanPham::ChuanHoaGiaBan(to_string(thanhTien)).second << " VND\n";
        }
    }
    tong = getTongTien();
    cout <<"\nTONG TIEN THANH TOAN: "<< SanPham::ChuanHoaGiaBan(to_string(tong)).second <<" VND"<<endl;
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
    long long tongHoaDon = 0;
    for (size_t i = 0; i < DanhSachSP.size(); i++)
    {
       const ChiTietHoaDon &sp = DanhSachSP[i];
       int thanhTien = sp.giaBan * sp.soLuongMua;
       tongHoaDon += thanhTien;
       outFile << " "<<i+1<<". MaSP: "<<sp.maSP
               << " | Ten: " << sp.tenSP
               << " | NSX: " << sp.nsx
               << " | Don gia: " << SanPham::ChuanHoaGiaBan(to_string(sp.giaBan)).second << " VND"
               << " | SL: " << sp.soLuongMua
               << " | Thanh tien: " << SanPham::ChuanHoaGiaBan(to_string(thanhTien)).second << " VND\n";
    }
    outFile << "--------------------------------------------------------\n";
    outFile << " TONG TIEN: " << SanPham::ChuanHoaGiaBan(to_string(tongHoaDon)).second << " VND\n";
    outFile << "========================================================\n\n";
    outFile.close();
    cout << "Da luu hoa don "<<MaHD<<" vao file "<<TenFile<<endl;
}
#endif
