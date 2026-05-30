#include "QuanLy.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

using namespace std;

HoaDon::HoaDon()
{
    MaHD = "Ma HD " + to_string(rand()%9000+1000);//random so ma HD
}
HoaDon::~HoaDon()
{
    DanhSachSP.clear();//xoa danh sach
}
HoaDon &HoaDon::operator+(SanPham* sp)
{
if (sp != NULL)
    {
        DanhSachSP.push_back(sp);
        cout <<"Da them san pham ["<<sp->getMaSP()<<"] vao hoa don!"
             <<"Ma HD: "<<MaHD;
    }
    return *this;//tro lai hoa don hien tai de tiep tuc mua tiep
}
int HoaDon::TongTien()
{
    int tong = 0;
    for (SanPham *sp:DanhSachSP){
        tong += sp->getGiaBan();//lay gia ban cua tung loai san pham
    }
    return tong;
}
int HoaDon::getTongTien() const//tong so tien cac san pham 
{
    int tong = 0;
    for (SanPham *sp : DanhSachSP){//chay 1 vong kiem tra cac san pham trong hoa don
    tong += sp -> getGiaBan();
    }
    return tong;
}
void HoaDon::inHoaDon() const 
{
    cout << "====HOA DON MAT HANG===="<<endl;
    if (DanhSachSP.empty())
    {
        cout << "Khong co san pham nao!"<<endl;
    }else{
        for (size_t i = 0; i < DanhSachSP.size();i++){//size_t:kich thuoc cua danh sach
            cout << " " << i+1;//So thu tu
            DanhSachSP[i] ->Xuat();//hien thi don hang
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
        outFile << " "<<i+1<<".MaSP: "<<DanhSachSP[i]->getMaSP()
                << "NSX: " << DanhSachSP[i] ->getNSX()
                <<"Gia: " << DanhSachSP[i]->getGiaBan()<<" VND\n";
    }
    outFile << "--------------------------------------------------------\n";
    outFile << " TONG TIEN: " << getTongTien() << " VND\n";
    outFile << "========================================================\n\n";
    outFile.close();
    cout << "Da luu hoa don "<<MaHD<<"vao file "<<TenFile<<endl;
}