---
config:
  layout: elk
---
classDiagram
    class SanPham {
        <<Abstract>>
        - string maSP
        - string NSX
        - string ten
        - int GiaBan
        - int soluong
        + static map~string,int~ DemMatHang
        + SanPham()
        + virtual ~SanPham()
        + getMaSP() string
        + getTen() string
        + getGiaBan() int
        + getNSX() string
        + getSoLuong() int
        + CheckSoLuongKho(int SoLuongMua) void
        + setMaSP(string ma) void
        + setTen(string ten) void
        + setNSX(string nsx) void
        + setGiaBan(int giaBan) void
        + setGiaBan(string rawGiaBan) void
        + setSoLuong(int soLuong) void
        + virtual Nhap() void
        + virtual Xuat() void const
        + getGiaBanFormatted() string const
        + static ChuanHoaGiaBan(string rawGiaBan) pair~int,string~
        + virtual LuuFile() void const
        + virtual getLoai() string const
        + static RutGonNSX(string nsx) string
        + static DefaultFileName() string
    }
    class DoDienTu {
        + DoDienTu()
        + ~DoDienTu() override
        + Nhap() override
        + Xuat() void const override
        + LuuFile() void const override
        + getLoai() string const override
    }
    class DoGiaDung {
        + DoGiaDung()
        + ~DoGiaDung() override
        + Nhap() override
        + Xuat() void const override
        + LuuFile() void const override
        + getLoai() string const override
    }
    class ChiTietHoaDon {
        <<struct>>
        + SanPham* sp
        + int SoLuongMua
        + string maSP
        + string tenSP
        + string nsx
        + int giaBan
        + int soLuongMua
    }
    class HoaDon {
        - string MaHD
        - string TenKH
        - vector~ChiTietHoaDon~ DanhSachSP
        - long long tong
        + HoaDon()
        + ~HoaDon()
        + setTenKH(string TenKH) void
        + themSanPham(SanPham* sp, int soLuongMua) bool
        + operator+(SanPham* sp) HoaDon&
        + operator-() HoaDon&
        + TongTien() int
        + getTongTien() int const
        + inHoaDon() void
        + LuuFile(string TenFile) void const
    }
    DoDienTu --|> SanPham : Class Dẫn xuất
    DoGiaDung --|> SanPham : Class Dẫn xuất
    HoaDon *-- ChiTietHoaDon : Liên kết qua mảng động vector
    ChiTietHoaDon o--> SanPham : Pointer liên kết tham chiếu