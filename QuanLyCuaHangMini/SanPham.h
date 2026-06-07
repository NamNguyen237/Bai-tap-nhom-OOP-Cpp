#ifndef SANPHAM_H
#define SANPHAM_H
#include <iostream>
#include <map>
#include <string>
#include <iomanip>
#include <fstream>
#include <cctype>
#include <climits>
#include <utility>

using namespace std;


class SanPham
{
    private:
        string maSP,NSX,ten;
        int GiaBan;
        int soluong;
    public:
        SanPham();
        //tranh ro ri bo nho
        virtual ~SanPham();

        //dung de cac class khac (HoaDon,KhachHang) lay du lieu
        string getMaSP() const;

        string getTen() const;
        int getGiaBan() const;
        string getNSX() const;

        //Khach hang mua bao nhieu
        int getSoLuong() const {return soluong;}

        //check soluong trong kho
        void CheckSoLuongKho(int SoLuongMua){soluong -= SoLuongMua;}

        void setMaSP(const string &ma);
        void setTen(const string &ten);
        void setNSX(const string &nsx);
        // Gán giá bán từ giá trị số nguyên cho mục đích tính toán
        void setGiaBan(int giaBan);

        // Dùng khi nhập giá từ chuỗi có thể có dấu "." hoặc ký tự khác
        void setGiaBan(const string &rawGiaBan);

        void setSoLuong(int soLuong);

        virtual void Nhap();
        virtual void Xuat()const ;
        // Trả về chuỗi giá có định dạng dấu chấm để in ra màn hình
        string getGiaBanFormatted() const;

        // Chuẩn hóa đầu vào giá tiền
        static pair<int,string> ChuanHoaGiaBan(const string &rawGiaBan);

        virtual void LuuFile() const = 0;
        virtual string getLoai() const = 0;
        static string RutGonNSX(const string &nsx);
        static const string &DefaultFileName();
        static map<string,int> DemMatHang;
};


string SanPham::RutGonNSX(const string &nsx)
{
    if (nsx.length() <= 2)
        return nsx;

    static const string NguyenAm = "aeiouAEIOU";//neu gap cac nguyen am
    string result;//bo qua nguyen am
    for (char c : nsx)
    {
        unsigned char kyTu = static_cast<unsigned char>(c);
        if (isalpha(kyTu) && NguyenAm.find(c) == string::npos)//ktra xem co phai chu cai, va cac chu do co cac nguyen am khong
            result += static_cast<char>(toupper(kyTu));

        if (result.size() == 2)
            break;
    }
    return result;
}
SanPham :: SanPham() : maSP(" "), NSX(" "), ten(" "), GiaBan(0), soluong(0) {}
SanPham :: ~SanPham() = default;
string SanPham :: getMaSP() const {return maSP;}
string SanPham :: getTen() const {return ten;}
int SanPham :: getGiaBan() const {return GiaBan;}
string SanPham :: getNSX() const {return NSX;}
void SanPham :: setMaSP(const string &ma) { this->maSP = ma;}
void SanPham :: setTen(const string &ten) { this->ten = ten; }
void SanPham :: setNSX(const string &nsx) { this->NSX = nsx; }
void SanPham :: setGiaBan(int giaBan) { this->GiaBan = giaBan; }
void SanPham :: setSoLuong(int soLuong) { this->soluong = soLuong; }
const string &SanPham::DefaultFileName() { static const string fileName = "sanpham.txt"; return fileName; }

// Chuyển đổi chuỗi nhập giá bán thành giá số nguyên và chuỗi định dạng in ra.
// - Loại bỏ tất cả ký tự không phải chữ số.
// - Nếu chuỗi sau khi loại bỏ rỗng thì trả về giá 0 và chuỗi "0".
// - Nếu giá vượt quá INT_MAX thì giới hạn lại để tránh tràn số.
// - Trả về pair<int,string> trong đó first là giá thực tế dùng tính toán,
//   second là chuỗi có dấu chấm mỗi 3 chữ số để dễ đọc.
pair<int,string> SanPham::ChuanHoaGiaBan(const string &rawGiaBan)
{
    string digits;
    for (char c : rawGiaBan)
    {
        if (isdigit((unsigned char)c))
            digits.push_back(c);
    }

    if (digits.empty())
        return {0, "0"};

    long long value = 0;
    for (char c : digits)
    {
        value = value * 10 + (c - '0');
        if (value > INT_MAX)
            break;
    }
    if (value > INT_MAX)
        value = INT_MAX;

    int gia = static_cast<int>(value);
    string formatted;
    string giaDaChuanHoa = to_string(gia);
    int count = 0;
    for (int i = (int)giaDaChuanHoa.size() - 1; i >= 0; --i)
    {
        formatted.insert(formatted.begin(), giaDaChuanHoa[i]);
        if (++count == 3 && i > 0)
        {
            formatted.insert(formatted.begin(), '.');
            count = 0;
        }
    }

    return {gia, formatted};
}

// Trả về chuỗi giá bán đã định dạng từ giá số nguyên hiện tại.
string SanPham::getGiaBanFormatted() const
{
    return ChuanHoaGiaBan(to_string(GiaBan)).second;
}

// Gán giá bán từ chuỗi nhập liệu. Chuỗi có thể chứa dấu chấm hoặc ký tự khác,
// hàm sẽ chuẩn hóa và chỉ giữ lại phần số.
void SanPham::setGiaBan(const string &rawGiaBan)
{
    GiaBan = ChuanHoaGiaBan(rawGiaBan).first;
}

void SanPham :: Nhap()
{
    cout << "Nhap ten san pham: ";
    getline(cin,ten);

    cout << "Nhap nha san xuat: ";
    cin >> NSX;

    cout << "Nhap gia ban: ";
    string giaBanRaw;
    cin >> giaBanRaw;

    // Chuẩn hóa đầu vào giá, cho phép người dùng nhập có hoặc không có dấu chấm.
    pair<int,string> parsedGia = ChuanHoaGiaBan(giaBanRaw);
    while (parsedGia.first <= 0)
    {
        cout << "Gia ban khong hop le! Xin vui long nhap lai: ";
        cin >> giaBanRaw;
        parsedGia = ChuanHoaGiaBan(giaBanRaw);
    }

    // Lưu giá đã chuẩn hóa vào biến số để dùng cho tính toán.
    GiaBan = parsedGia.first;

    cout << "Nhap so luong trong kho : ";
    cin >> soluong;

    //Thuat toan tao ma san pham: lay 2 ky tu dau tien cua NSX (sau khi bo qua nguyen am)
    // + so thu tu cua mat hang do trong kho
    string prefix = RutGonNSX(NSX);
    DemMatHang[prefix]++;//dem sp cua tung hang vi du "[SN] = 1"
    int stt = DemMatHang[prefix];//so thu tu = so sp SN01,SN02,...
    string chuoiSTT = (stt < 10)?"0" + to_string(stt):to_string(stt);//them so "0" vao dang sau neu stt < 10
    maSP = prefix + chuoiSTT;
    cout << "San pham co ma: "<<maSP << endl;
}
void SanPham::Xuat()const
{
    cout << "Ma san pham : "<<maSP
         <<"\nTen mat hang: " <<ten
         <<"\nNSX: "<<NSX
         <<"\nGia: "<<getGiaBanFormatted()   // In giá đã định dạng với dấu chấm để dễ đọc
         <<"\nSo luong con lai trong kho: "<<soluong;
}
#endif
