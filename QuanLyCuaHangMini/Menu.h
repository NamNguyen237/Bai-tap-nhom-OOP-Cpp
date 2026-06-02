#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>
#include "SanPham.h"
#include "SanPhamCon.h"
#include "HoaDon.h"

using namespace std;

// Ten file du lieu co so
static const string TEN_FILE_SANPHAM = "sanpham.txt";
static const string TEN_FILE_HOADON = "hoadon.txt";

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

        int giaBan = stoi(giaBanStr);
        int soLuong = stoi(soLuongStr);
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

    return NULL;;
}
void inSanPhamTheoMa(const string& tenFile, const string& maSP)
{
    vector<SanPham*> danhSach = docTatCaSanPham(tenFile);
    SanPham* sanPham = timSanPhamTheoMa(danhSach, maSP);

    if (!sanPham)
    {
        cout << "Khong tim thay ma san pham: " << maSP << "\n";
        danhSach.clear();
        return;
    }

    cout << "=== THONG TIN SAN PHAM ===\n";
    sanPham->Xuat();
    cout << "\n";
    
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

    for (SanPham* sanPham : danhSach)
        delete sanPham;
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
        for (SanPham* sanPham : danhSach)
            delete sanPham;
        return false;
    }

    bool ketQua = luuDanhSachSanPham(danhSach, tenFile);
    for (SanPham* sanPham : danhSach)
        delete sanPham;
    return ketQua;
}

// Sua thong tin san pham dua tren maSP va cap nhat file
bool suaSanPham(const string& tenFile, const string& maSP)
{
    vector<SanPham*> danhSach = docTatCaSanPham(tenFile);
    SanPham* sanPhamChinhSua = nullptr;

    for (SanPham* sanPham : danhSach)
    {
        if (sanPham->getMaSP() == maSP)
        {
            sanPhamChinhSua = sanPham;
            break;
        }
    }

    if (!sanPhamChinhSua)
    {
        for (SanPham* sanPham : danhSach)
            delete sanPham;
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
        int giaBan = stoi(duLieu);
        if (giaBan > 0)
            sanPhamChinhSua->setGiaBan(giaBan);
    }

    cout << "So luong moi: ";
    getline(cin, duLieu);
    if (!duLieu.empty())
    {
        int soLuong = stoi(duLieu);
        if (soLuong >= 0)
            sanPhamChinhSua->setSoLuong(soLuong);
    }

    bool ketQua = luuDanhSachSanPham(danhSach, tenFile);
    for (SanPham* sanPham : danhSach)
        delete sanPham;
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
        cout << "Nhap ma san pham can mua (0 de ket thuc): ";
        string maSP;
        getline(cin, maSP);

        if (maSP == "0")
            break;

        SanPham* sanPham = timSanPhamTheoMa(danhSach, maSP);
        if (!sanPham)
        {
            cout << "Khong tim thay ma san pham: " << maSP << "\n";
            continue;
        }

        cout << "Thong tin san pham:\n";
        sanPham->Xuat();
        cout << "\nNhap so luong mua: ";

        int soLuongMua = 0;
        if (!(cin >> soLuongMua))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "So luong mua khong hop le.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (hoaDon.themSanPham(sanPham, soLuongMua))
            daCoSanPham = true;
    }

    if (!daCoSanPham)
    {
        cout << "Hoa don khong co san pham, khong luu file.\n";
            for (SanPham* sanPham : danhSach)
                delete sanPham;
        return;
    }

    cout << "\n";
    hoaDon.inHoaDon();
    hoaDon.LuuFile(tenFileHoaDon);

    if (luuDanhSachSanPham(danhSach, tenFileSanPham))
        cout << "Da cap nhat so luong ton kho trong file " << tenFileSanPham << "\n";
    else
        cout << "Khong the cap nhat file san pham " << tenFileSanPham << "\n";

}
// Hien thi menu chinh len man hinh
void hienThiMenu()
{
    cout << "\n=== QUAN LY SAN PHAM TU FILE TXT ===\n";
        cout << "1. Them san pham moi va luu file\n";
        cout << "2. In tat ca san pham tu file\n";
        cout << "3. In thong tin san pham theo ma\n";
        cout << "4. Xoa san pham theo ma\n";
        cout << "5. Sua san pham theo ma\n";
        cout << "6. Lap hoa don ban hang va tinh tong tien\n";
        cout << "7. Thoat\n";
        cout << "Lua chon: ";
}

// Xu ly lua chon nguoi dung da chon tu menu
void xuLyLuaChon(int luaChon)
{
    if (luaChon == 1)
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
        }
        else if (luaChon == 2)
        {
            inTatCaSanPhamTuFile(TEN_FILE_SANPHAM);
        }
        else if (luaChon == 3)
        {
            cout << "Nhap ma san pham can in: ";
            string maSP;
            getline(cin, maSP);
            inSanPhamTheoMa(TEN_FILE_SANPHAM, maSP);
        }
        else if (luaChon == 4)
        {
            cout << "Nhap ma san pham can xoa: ";
            string maSP;
            getline(cin, maSP);
            if (xoaSanPham(TEN_FILE_SANPHAM, maSP))
                cout << "Da xoa san pham " << maSP << " tu file.\n";
            else
                cout << "Khong tim thay ma san pham: " << maSP << "\n";
        }
        else if (luaChon == 5)
        {
            cout << "Nhap ma san pham can sua: ";
            string maSP;
            getline(cin, maSP);
            if (suaSanPham(TEN_FILE_SANPHAM, maSP))
                cout << "Da cap nhat thong tin san pham " << maSP << " trong file.\n";
            else
                cout << "Khong tim thay ma san pham: " << maSP << "\n";
        }
        else if (luaChon == 6)
        {
            lapHoaDonBanHang(TEN_FILE_SANPHAM, TEN_FILE_HOADON);
        }
        else
        {
            cout << "Lua chon khong hop le. Vui long thu lai.\n";
        }
}

#endif // MENU_H
