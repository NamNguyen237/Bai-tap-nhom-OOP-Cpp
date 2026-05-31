#ifndef HOADON_H
#define HOADON_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;


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
        int SoLuongMua;
        cout << "Nhap so luong mua: ";
        cin >> SoLuongMua;
        if (SoLuongMua > sp->getSoLuong()){
            cout << "Trong kho khong du!Con lai "<<sp->getSoLuong()<<" san pham";
            return *this;// khong them vao hoa don
        }
    sp -> CheckSoLuongKho(SoLuongMua);
    DanhSachSP.push_back(sp);
    cout << "Da them ["<<sp->getMaSP()<<"] "<<"x"<<SoLuongMua<<" vao hoa don"<<MaHD<<endl;
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
#endif
