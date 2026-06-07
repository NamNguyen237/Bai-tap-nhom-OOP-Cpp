#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include "SanPham.h"
#include "SanPhamCon.h"
#include "HoaDon.h"

using namespace std;

// Ten file du lieu co so
static const string TEN_FILE_SANPHAM = "sanpham.txt";
static const string TEN_FILE_HOADON = "hoadon.txt";

void giaiPhongDanhSachSanPham(vector<SanPham*>& danhSach)
{
    for (SanPham* sanPham : danhSach)
        delete sanPham;

    danhSach.clear();
}

// Tao mot doi tuong SanPham tu mot dong ban ghi co san trong file
SanPham* taoSanPhamTuDong(const string& loai,const string& maSP,const string& ten,const string& nsx,int giaBan,int soLuong)
{
    SanPham* sanPham = nullptr;
    if (loai == "DDT")
        sanPham = new DoDienTu();
    else if (loai == "DGD")
        sanPham = new DoGiaDung();

    if (!sanPham)
        return nullptr;

    sanPham->setMaSP(maSP);
    sanPham->setTen(ten);
    sanPham->setNSX(nsx);
    sanPham->setGiaBan(giaBan);
    sanPham->setSoLuong(soLuong);

    // Cap nhat dem san pham da ton tai de tranh trung ma khi them moi
    size_t viTri = maSP.size();
    while (viTri > 0 && isdigit(static_cast<unsigned char>(maSP[viTri - 1])))
        --viTri;

    if (viTri < maSP.size())
    {
        string tienTo = maSP.substr(0, viTri);
        int stt = stoi(maSP.substr(viTri));
        SanPham::DemMatHang[tienTo] = max(SanPham::DemMatHang[tienTo], stt);
    }

    return sanPham;
}

// Doc tat ca san pham tu file vao bo nho
vector<SanPham*> docTatCaSanPham(const string& tenFile)
{
    vector<SanPham*> danhSach;
    ifstream input(tenFile);
    if (!input)
        return danhSach; // file khong ton tai hoac khong mo duoc

    string dong;
    while (getline(input, dong))
    {
        if (dong.empty())
            continue;

        if (!dong.empty() && dong.back() == '\r')
            dong.pop_back();

        stringstream phanTach(dong);
        string loai, maSP, ten, nsx, giaBanStr, soLuongStr;

        if (!getline(phanTach, loai, '|') ||
            !getline(phanTach, maSP, '|') ||
            !getline(phanTach, ten, '|') ||
            !getline(phanTach, nsx, '|') ||
            !getline(phanTach, giaBanStr, '|') ||
            !getline(phanTach, soLuongStr, '|'))
        {
            continue; // bo qua dong khong hop le
        }

        int giaBan = 0;
        int soLuong = 0;
        try
        {
            giaBan = stoi(giaBanStr);
            soLuong = stoi(soLuongStr);
        }
        catch (...)
        {
            continue;
        }

        SanPham* sanPham = taoSanPhamTuDong(loai, maSP, ten, nsx, giaBan, soLuong);
        if (sanPham)
            danhSach.push_back(sanPham);
    }

    return danhSach;
}
SanPham* timSanPhamTheoMa(vector<SanPham*>& danhSach, const string& maSP)
{
    for (SanPham* sanPham : danhSach)
    {
        if (sanPham->getMaSP() == maSP)
            return sanPham;
    }

    return nullptr;
}
void inSanPhamTheoMa(const string& tenFile, const string& maSP)
{
    vector<SanPham*> danhSach = docTatCaSanPham(tenFile);
    SanPham* sanPham = timSanPhamTheoMa(danhSach, maSP);

    if (!sanPham)
    {
        cout << "Khong tim thay ma san pham: " << maSP << "\n";
        giaiPhongDanhSachSanPham(danhSach);
        return;
    }

    cout << "=== THONG TIN SAN PHAM ===\n";
    sanPham->Xuat();
    cout << "\n";
    giaiPhongDanhSachSanPham(danhSach);
}
// Luu danh sach san pham vao file, ghi de toan bo noi dung cu
bool luuDanhSachSanPham(const vector<SanPham*>& danhSach, const string& tenFile)
{
    ofstream output(tenFile, ios::trunc);
    if (!output)
        return false;

    for (const SanPham* sanPham : danhSach)
    {
        output << sanPham->getLoai() << "|"
               << sanPham->getMaSP() << "|"
               << sanPham->getTen() << "|"
               << sanPham->getNSX() << "|"
               << sanPham->getGiaBan() << "|"
               << sanPham->getSoLuong() << "\n";
    }

    return true;
}

// In toan bo san pham tren file ra man hinh
void inTatCaSanPhamTuFile(const string& tenFile)
{
    vector<SanPham*> danhSach = docTatCaSanPham(tenFile);
    if (danhSach.empty())
    {
        cout << "Khong co san pham nao trong file " << tenFile << "\n";
        return;
    }

    cout << "=== DANH SACH SAN PHAM TRONG FILE ===\n";
    for (size_t i = 0; i < danhSach.size(); ++i)
    {
        cout << "[" << i + 1 << "]\n";
        danhSach[i]->Xuat();
        cout << "\n------------------------------------\n";
    }

    giaiPhongDanhSachSanPham(danhSach);
}

// Xoa san pham co ma maSP va cap nhat lai file
bool xoaSanPham(const string& tenFile, const string& maSP)
{
    vector<SanPham*> danhSach = docTatCaSanPham(tenFile);
    bool daXoa = false;

    for (auto it = danhSach.begin(); it != danhSach.end(); ++it)
    {
        if ((*it)->getMaSP() == maSP)
        {
            delete *it;
            danhSach.erase(it);
            daXoa = true;
            break;
        }
    }

    if (!daXoa)
    {
        giaiPhongDanhSachSanPham(danhSach);
        return false;
    }

    bool ketQua = luuDanhSachSanPham(danhSach, tenFile);
    giaiPhongDanhSachSanPham(danhSach);
    return ketQua;
}

// Sua thong tin san pham dua tren maSP va cap nhat file
bool suaSanPham(const string& tenFile, const string& maSP)
{
    vector<SanPham*> danhSach = docTatCaSanPham(tenFile);
    SanPham* sanPhamChinhSua = timSanPhamTheoMa(danhSach, maSP);

    if (!sanPhamChinhSua)
    {
        giaiPhongDanhSachSanPham(danhSach);
        return false;
    }

    cout << "Thong tin hien tai:\n";
    sanPhamChinhSua->Xuat();
    cout << "\n--- Nhap thong tin moi (de trong neu khong doi) ---\n";

    string duLieu;
    cout << "Ten moi: ";
    getline(cin, duLieu);
    if (!duLieu.empty())
        sanPhamChinhSua->setTen(duLieu);

    cout << "NSX moi: ";
    getline(cin, duLieu);
    if (!duLieu.empty())
        sanPhamChinhSua->setNSX(duLieu);

    cout << "Gia ban moi: ";
    getline(cin, duLieu);
    if (!duLieu.empty())
    {
        int giaBan = SanPham::ChuanHoaGiaBan(duLieu).first;
        if (giaBan > 0)
            sanPhamChinhSua->setGiaBan(giaBan);
    }

    cout << "So luong moi: ";
    getline(cin, duLieu);
    if (!duLieu.empty())
    {
        try
        {
            int soLuong = stoi(duLieu);
            if (soLuong >= 0)
                sanPhamChinhSua->setSoLuong(soLuong);
        }
        catch (...)
        {
            cout << "So luong khong hop le, giu nguyen gia tri cu.\n";
        }
    }

    bool ketQua = luuDanhSachSanPham(danhSach, tenFile);
    giaiPhongDanhSachSanPham(danhSach);
    return ketQua;
}
void lapHoaDonBanHang(const string& tenFileSanPham, const string& tenFileHoaDon)
{
    vector<SanPham*> danhSach = docTatCaSanPham(tenFileSanPham);
    if (danhSach.empty())
    {
        cout << "Khong co san pham nao de ban.\n";
        return;
    }

    HoaDon hoaDon;
    string tenKhachHang;
    cout << "Nhap ten khach hang (bo trong neu la khach le): ";
    getline(cin, tenKhachHang);
    hoaDon.setTenKH(tenKhachHang);

    bool daCoSanPham = false;
    while (true)
    {
        cout << "Nhap ma san pham can mua (2 de xoa san pham gan nhat, 0 de ket thuc): ";
        string maSP;
        getline(cin, maSP);

        if (maSP == "0")
            break;

        if (maSP == "2")
        {
            -hoaDon;
            continue;
        }

        SanPham* sanPham = timSanPhamTheoMa(danhSach, maSP);
        if (!sanPham)
        {
            cout << "Khong tim thay ma san pham: " << maSP << "\n";
            continue;
        }

        cout << "Thong tin san pham:\n";
        sanPham->Xuat();

        if (hoaDon + sanPham)
            daCoSanPham = true;
    }

    if (!daCoSanPham)
    {
        cout << "Hoa don khong co san pham, khong luu file.\n";
        giaiPhongDanhSachSanPham(danhSach);
        return;
    }

    cout << "\n";
    hoaDon.inHoaDon();
    hoaDon.LuuFile(tenFileHoaDon);

    if (luuDanhSachSanPham(danhSach, tenFileSanPham))
        cout << "Da cap nhat so luong ton kho trong file " << tenFileSanPham << "\n";
    else
        cout << "Khong the cap nhat file san pham " << tenFileSanPham << "\n";

    giaiPhongDanhSachSanPham(danhSach);
}
vector<string> docTatCaHoaDon(const string& tenFile)
{
    vector<string> danhSachHoaDon;
    ifstream input(tenFile);
    if (!input)
        return danhSachHoaDon;

    const string duongKe = "========================================================";
    string dong;
    string hoaDon;
    bool dangDocHoaDon = false;

    while (getline(input, dong))
    {
        if (!dong.empty() && dong.back() == '\r')
            dong.pop_back();

        if (dong == duongKe)
        {
            if (!dangDocHoaDon)
            {
                dangDocHoaDon = true;
                hoaDon.clear();
                hoaDon += dong + "\n";
            }
            else
            {
                hoaDon += dong + "\n";
                danhSachHoaDon.push_back(hoaDon);
                hoaDon.clear();
                dangDocHoaDon = false;
            }
            continue;
        }

        if (dangDocHoaDon)
            hoaDon += dong + "\n";
    }

    if (dangDocHoaDon && !hoaDon.empty())
        danhSachHoaDon.push_back(hoaDon);

    return danhSachHoaDon;
}
void inHoaDonMoiNhatTuFile(const string& tenFile)
{
    vector<string> danhSachHoaDon = docTatCaHoaDon(tenFile);
    if (danhSachHoaDon.empty())
    {
        cout << "Chua co hoa don nao trong file " << tenFile << "\n";
        return;
    }

    cout << "=== HOA DON MOI NHAT ===\n";
    cout << danhSachHoaDon.back();
}
bool xoaHoaDonCuNhat(const string& tenFile)
{
    vector<string> danhSachHoaDon = docTatCaHoaDon(tenFile);
    if (danhSachHoaDon.empty())
        return false;

    danhSachHoaDon.erase(danhSachHoaDon.begin());

    ofstream output(tenFile, ios::trunc);
    if (!output)
        return false;

    for (size_t i = 0; i < danhSachHoaDon.size(); ++i)
    {
        output << danhSachHoaDon[i];
        if (i + 1 < danhSachHoaDon.size())
            output << "\n";
    }

    return true;
}

const int DO_RONG_MENU = 62;

string canGiua(const string& noiDung, int doRong)
{
    if ((int)noiDung.size() >= doRong)
        return noiDung.substr(0, doRong);

    int khoangTrongTrai = (doRong - (int)noiDung.size()) / 2;
    int khoangTrongPhai = doRong - (int)noiDung.size() - khoangTrongTrai;
    return string(khoangTrongTrai, ' ') + noiDung + string(khoangTrongPhai, ' ');
}

void inDuongKeMenu(char kyTu)
{
    std::cout << "+" << string(DO_RONG_MENU - 2, kyTu) << "+\n";
}

void inDongCanGiuaMenu(const string& noiDung)
{
    std::cout << "|" << canGiua(noiDung, DO_RONG_MENU - 2) << "|\n";
}

void inDongMenu(int soThuTu, const string& noiDung)
{
    stringstream dong;
    dong << soThuTu << ". " << noiDung;
    std::cout << "| " << std::left << std::setw(DO_RONG_MENU - 4)
              << dong.str().substr(0, DO_RONG_MENU - 4) << " |\n";
}

// Hien thi menu chinh len man hinh
void hienThiMenu()
{
    std::cout << "\n";
    inDuongKeMenu('=');
    inDongCanGiuaMenu("QUAN LY CUA HANG MINI");
    inDongCanGiuaMenu("Du lieu TXT: san pham - hoa don");
    inDuongKeMenu('-');
    inDongMenu(1, "Them san pham moi va luu file");
    inDongMenu(2, "In tat ca san pham tu file");
    inDongMenu(3, "In thong tin san pham theo ma");
    inDongMenu(4, "Xoa san pham theo ma");
    inDongMenu(5, "Sua san pham theo ma");
    inDongMenu(6, "Lap hoa don ban hang va tinh tong tien");
    inDongMenu(7, "In hoa don moi nhat");
    inDongMenu(8, "Xoa hoa don cu nhat");
    inDongMenu(9, "Thoat chuong trinh");
    inDuongKeMenu('=');
    std::cout << "Nhap lua chon [1-9]: ";
}

// Xu ly lua chon nguoi dung da chon tu menu
void xuLyLuaChon(int luaChon)
{
    switch (luaChon)
    {
        case 1:
        {
            cout << "Chon loai san pham: 1. Do dien tu  2. Do gia dung\n";
            cout << "Lua chon: ";
            int luaChonLoai = 0;
            cin >> luaChonLoai;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
            SanPham* sanPham = nullptr;
            if (luaChonLoai == 1)
                sanPham = new DoDienTu();
            else if (luaChonLoai == 2)
                sanPham = new DoGiaDung();
            else
            {
                cout << "Lua chon loai san pham khong hop le.\n";
                return;
            }
    
            sanPham->Nhap(); //nhập đang đúng tự nhiên ông đức sửa thành xuất gây lỗi, nên sửa lại thành nhập
            sanPham->LuuFile();
            delete sanPham;
            cout << "Da luu san pham vao file " << TEN_FILE_SANPHAM << "\n";
            break;
        }

        case 2:
        {
            inTatCaSanPhamTuFile(TEN_FILE_SANPHAM);
            break;
        }

        case 3:
        {
            cout << "Nhap ma san pham can in: ";
            string maSP;
            getline(cin, maSP);
            inSanPhamTheoMa(TEN_FILE_SANPHAM, maSP);
            break;
        }

        case 4:
        {
            cout << "Nhap ma san pham can xoa: ";
            string maSP;
            getline(cin, maSP);
            if (xoaSanPham(TEN_FILE_SANPHAM, maSP))
                cout << "Da xoa san pham " << maSP << " tu file.\n";
            else
                cout << "Khong tim thay ma san pham: " << maSP << "\n";
            break;
        }

        case 5:
        {
            std::cout << "Nhap ma san pham can sua: ";
            std::string maSP;
            std::getline(std::cin, maSP);
            if (suaSanPham(TEN_FILE_SANPHAM, maSP))
                std::cout << "Da cap nhat thong tin san pham " << maSP << " trong file.\n";
            else
                std::cout << "Khong tim thay ma san pham: " << maSP << "\n";
            break;
        }

        case 6:
        {
            lapHoaDonBanHang(TEN_FILE_SANPHAM, TEN_FILE_HOADON);
            break;
        }

        case 7:
        {
            inHoaDonMoiNhatTuFile(TEN_FILE_HOADON);
            break;
        }
        case 8:
        {
            if (xoaHoaDonCuNhat(TEN_FILE_HOADON))
                cout << "Da xoa hoa don cu nhat tu file " << TEN_FILE_HOADON << "\n";
            else
                cout << "Khong co hoa don nao de xoa trong file " << TEN_FILE_HOADON << "\n";
            break;
        }
        case 9:
        {
            cout << "Dang thoat chuong trinh...\n";
            exit(0);
        }

        default:
        {
            cout << "Lua chon khong hop le. Vui long thu lai.\n";
            break;
        }
    }
}

#endif // MENU_H
