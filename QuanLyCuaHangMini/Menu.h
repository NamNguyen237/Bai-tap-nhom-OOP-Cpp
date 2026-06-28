// Controller chính: Kết nối UI Console với các Entity (SanPham, HoaDon).
// Chứa toàn bộ logic xử lý menu và thao tác I/O với file txt.
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

// Phải loop qua để delete từng phần tử do dùng con trỏ (cấp phát động).
// Không giải phóng chỗ này là bị memory leak ngay. Xong xuôi thì clear() vector.
void giaiPhongDanhSachSanPham(vector<SanPham*>& danhSach)
{
    for (SanPham* sanPham : danhSach)
        delete sanPham;

    danhSach.clear();
}

// Factory method pattern đơn giản: parse dòng txt và dựa vào flag "DDT"/"DGD" 
// để init đúng class con (DoDienTu/DoGiaDung).
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

    // Tách chuỗi để lấy phần số (ví dụ: DDT12 -> lấy số 12).
    // Sync lại ID lớn nhất vào biến static DemMatHang để auto-increment lúc tạo mới không bị lặp mã.
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

// Load toàn bộ DB từ txt lên memory (vector).
// CRUD trên memory trước rồi mới flush (ghi đè) lại xuống file để tối ưu I/O.
vector<SanPham*> docTatCaSanPham(const string& tenFile)
{
    vector<SanPham*> danhSach;
    ifstream input(tenFile);
    if (!input)
        return danhSach; // file khong ton tai hoac khong mo duoc

    string dong;
    // Đọc từng dòng, bỏ qua dòng trống hoặc dòng bị dính carriage return (\r) do khác biệt Win/Linux
    while (getline(input, dong))
    {
        if (dong.empty())
            continue;

        if (!dong.empty() && dong.back() == '\r')
            dong.pop_back();

        // Split chuỗi bằng dấu '|'
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
            // Catch-all block: Nếu data sai format làm lỗi parse số (stoi) thì skip dòng này, chặn crash app
            continue;
        }

        SanPham* sanPham = taoSanPhamTuDong(loai, maSP, ten, nsx, giaBan, soLuong);
        if (sanPham)
            danhSach.push_back(sanPham);
    }

    return danhSach;
}
// Dùng linear search duyệt vector tìm mã. Trả về pointer, cẩn thận check nullptr khi xài.
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
    // Note: Do load full list lên object list tạm, 
    // phải đảm bảo mọi return path đều đã gọi giải phóng RAM.
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
// Flush vector xuống file. Dùng mode ios::trunc để overwrite trắng file cũ.
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

// Note: Chỗ này nhờ Polymorphism (đa hình) + hàm virtual Xuat() ở class base
// nên lúc runtime gọi danhSach[i]->Xuat() nó sẽ tự trigger đúng override method của class con.
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

// Phải dùng Iterator để truyền vào hàm erase. 
// Nhớ delete object giải phóng RAM trước khi erase khỏi vector.
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

// Update field. Có trick là check if (!duLieu.empty()) để nếu user enter bỏ qua thì giữ nguyên data cũ, đỡ phải gõ lại.
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
// Lập hóa đơn. Số lượng tồn kho sẽ auto giảm đi 1 ngay bên trong logic overload operator '+' (hoaDon + sanPham).
// Xử lý xong trên memory chỉ việc call luuDanhSachSanPham để đồng bộ file.
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

        // Gọi overload unary operator '-' để rollback/xóa SP gần nhất nếu add nhầm (đồng thời cộng bù lại kho).
        // cho mấy ông không biết, unary operator là toán tử đơn ngôi, khác với toán tử bình thường là nhị ngôi 
        // (ví dụ: a+b là nhị ngôi, -a là đơn ngôi)
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

        if (hoaDon + sanPham) //nhị ngôi thêm sản phẩm nhưng return dạng boolean để check hóa đơn đã có sản phẩm này hay chưa
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
// Parser hóa đơn. Do txt format mỗi khối nhiều dòng, nên parse dựa vào flag string đường kẻ "=====" để ngắt khối.
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
// Hóa đơn cũ nhất luôn ở index 0, chặt đầu vector (erase begin) rồi lưu lại.
bool xoaHoaDonCuNhat(const string& tenFile)
{
    vector<string> danhSachHoaDon = docTatCaHoaDon(tenFile);
    if (danhSachHoaDon.empty())
        return false;

    danhSachHoaDon.erase(danhSachHoaDon.begin()); //chặt đầu vector xóa đi hóa đơn đầu tiên (cũ nhất)

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

// UI Helper: Căn giữa text tự động tính toán padding khoảng trắng 2 bên
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

// Main event loop switch-case xử lý menu
// Note: upcasting con trỏ SanPham* = new DoDienTu() để dùng chung interface.
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
            // Clear buffer: Xóa sạch ký tự '\n' còn sót lại trong stdin để không bị trôi lệnh getline() ở dưới
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
    
            // Nhờ hàm Nhap() là virtual function nên trình biên dịch sẽ gọi đúng override method của class con.
            sanPham->Nhap(); // (Ông Đức bớt sửa nhầm thành Xuat() đi nha)
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
