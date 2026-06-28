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

        //dung de cac class khac nhu (HoaDon,KhachHang) lay du lieu
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
        void setGiaBan(int giaBan);

        void setGiaBan(const string &rawGiaBan);

        void setSoLuong(int soLuong);

        virtual void Nhap();
        virtual void Xuat()const ;
        string getGiaBanFormatted() const;

        static pair<int,string> ChuanHoaGiaBan(const string &rawGiaBan);

        virtual void LuuFile() const = 0;
        virtual string getLoai() const = 0;
        static string RutGonNSX(const string &nsx);
        static const string &DefaultFileName();
        static map<string,int> DemMatHang;
};

SanPham :: SanPham() : maSP(" "), NSX(" "), ten(" "), GiaBan(0), soluong(0) {}//tao ra du lieu trong
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

string SanPham::RutGonNSX(const string &nsx)
{
    if (nsx.length() <= 2)//neu ten NSX chi co 2 chu hoac it hon, tra ve y nguyen
        return nsx;

    static const string NguyenAm = "aeiouAEIOU";//bien loc cac nguyen am
    string result;//ket qua cuoi cung
    for (char c : nsx)//duyet tung chu tren dong san xuat
    {
        unsigned char kyTu = static_cast<unsigned char>(c);//chuyen ky tu sang unsigned char de tranh loi doi voi cac ky tu dac biet 
        if (isalpha(kyTu) && NguyenAm.find(c) == string::npos)//ktra xem co phai chu cai, va cac chu do co cac nguyen am khong
            result += static_cast<char>(toupper(kyTu));// them vao ket qua

        if (result.size() == 2)
            break;
    }
    return result;
}

pair<int,string> SanPham::ChuanHoaGiaBan(const string &rawGiaBan)//chuyen gia tu string -> int + format voi dau "."
{
    string digits;//khai bao chuoi de luu chu so
    for (char c : rawGiaBan)//duyet tung ky tu trong chuoi gia ban
    {
        if (isdigit((unsigned char)c))//gap chu so thi them vao chuoi digits
            digits.push_back(c);
    }

    if (digits.empty())//rong thi tra ve gia 0 va chuoi "0"
        return {0, "0"};

    long long value = 0;
    for (char c : digits)//duyet tung ky tu trong digits de chuyen sang so nguyen 
    {
        value = value * 10 + (c - '0');//chuyen ky tu sang so nguyen
        if (value > INT_MAX)//vuot qua gia tri int thi break
            break;
    }
    if (value > INT_MAX)
        value = INT_MAX;

    int gia = static_cast<int>(value);//chuyen tu long long sang int de tra ve gia tri
    string formatted;//tao chuoi de luu gia da duoc dinh dang voi dau "."
    string giaDaChuanHoa = to_string(gia);//chuyen gia ban sang chuoi de xu ly
    int count = 0;
    for (int i = (int)giaDaChuanHoa.size() - 1; i >= 0; --i)//duyet tu cuoi chuoi len dau
    {
        formatted.insert(formatted.begin(), giaDaChuanHoa[i]);//them tung ky tu vao dau chuoi
        if (++count == 3 && i > 0)//neu da cho 3 ky tu vao va chua den cuoi chuoi => them dau "." va reset count 
        {
            formatted.insert(formatted.begin(), '.');
            count = 0;
        }
    }

    return {gia, formatted};
}


string SanPham::getGiaBanFormatted() const//lay gia da duoc dinh dang
{
    return ChuanHoaGiaBan(to_string(GiaBan)).second;
}

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

    cout << "Nhap gia ban: ";//gia ban tho
    string giaBanRaw;
    cin >> giaBanRaw;

    pair<int,string> parsedGia = ChuanHoaGiaBan(giaBanRaw); 
    while (parsedGia.first <= 0)
    {
        cout << "Gia ban khong hop le! Xin vui long nhap lai: ";
        cin >> giaBanRaw;
        parsedGia = ChuanHoaGiaBan(giaBanRaw);
    }

    GiaBan = parsedGia.first;

    cout << "Nhap so luong trong kho : ";
    cin >> soluong;

    string prefix = RutGonNSX(NSX);
    DemMatHang[prefix]++; //dem sp cua tung hang vi du "[SN] = 1"
    int stt = DemMatHang[prefix]; //so thu tu = so sp SN01,SN02,...
    string chuoiSTT = (stt < 10)?"0" + to_string(stt):to_string(stt); //them so "0" vao dang sau neu stt < 10
    maSP = prefix + chuoiSTT;//ma san pham = 2 chu cai cua NSX + so thu tu 
    cout << "San pham co ma: "<<maSP << endl;
}
void SanPham::Xuat()const
{
    cout << "Ma san pham : "<<maSP
         <<"\nTen mat hang: " <<ten
         <<"\nNSX: "<<NSX
         <<"\nGia: "<<getGiaBanFormatted()
         <<"\nSo luong con lai trong kho: "<<soluong;
}
#endif
