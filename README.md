<div align="center"> 
  <h1> Quadtree: Image Compressor </h1>
  <h3> Tugas Kecil 2 IF2211 Strategi Algoritma </h3>

![C++](https://img.shields.io/badge/C%2B%2B-17-blue?logo=c%2B%2B&logoColor=white)
![OpenCV](https://img.shields.io/badge/OpenCV-4.x-green?logo=opencv&logoColor=white)
![Magick++](https://img.shields.io/badge/Magick%2B%2B-ImageMagick-orange?logo=imagemagick&logoColor=white)
![WSL](https://img.shields.io/badge/WSL-Supported-blueviolet?logo=windows&logoColor=white)

![HMIF GIF](./doc/HMIF.gif)

</div>

## Program Description

Program ini adalah **kompresor gambar berbasis Quadtree** yang melakukan compress terhadap gambar dengan membagi area menjadi blok-blok homogen. Kompresi dilakukan secara adaptif: area gambar yang lebih bervariasi akan dibagi lebih detail, sementara area yang seragam akan direpresentasikan dengan blok besar.

#### Fitur utama:

- Mendukung berbagai metode error untuk menentukan homogenitas:
  - Variance
  - Mean Absolute Deviation (MAD)
  - Max Pixel Difference
  - Entropy
  - Structural Similarity Index (SSIM)
- Program juga dapat menerima **target rasio kompresi** (misalnya 0.8 = 80%) dan secara otomatis menyesuaikan nilai threshold agar mencapai target tersebut melalui pencarian adaptif.
- Dengan ini, program akan menghasilkan output berupa:
  - Menyimpan **gambar hasil kompresi** dalam format `.jpg`
  - Menyimpan **GIF animasi proses kompresi** yang memperlihatkan bagaimana Quadtree dibentuk secara bertahap (dari blok besar menjadi blok kecil)

---

## Getting Started: Requirement dan Instalasi

#### 1. Wajib Menggunakan WSL untuk Pengguna Windows

Pengguna **Linux** dan **MacOS** silakan lewati bagian ini. Bagi pengguna Windows, **program ini wajib dijalankan di dalam WSL (Windows Subsystem for Linux)** karena:

- Instalasi dependensi seperti OpenCV dan Magick++ **jauh lebih mudah** dilakukan di lingkungan Linux.
- Proses kompilasi menggunakan Makefile telah disesuaikan untuk sistem Linux.
- Penanganan path file input/output (misal `/mnt/c/Users/...`) akan lebih stabil dan konsisten di WSL.

Jika belum memiliki WSL, silakan ikuti panduan resmi Microsoft:  
[https://learn.microsoft.com/en-us/windows/wsl/install](https://learn.microsoft.com/en-us/windows/wsl/install)

---

#### 2. Dependensi yang Diperlukan

Program ini membutuhkan library berikut:

- [`OpenCV`](https://opencv.org/) untuk pemrosesan gambar
- [`Magick++`](https://imagemagick.org/Magick++/) (C++ API dari ImageMagick) untuk ekspor animasi GIF

Instalasi dapat dilakukan dengan perintah berikut (pada WSL/Ubuntu/Debian):

```bash
sudo apt update
sudo apt install build-essential pkg-config libopencv-dev libmagick++-dev
```

---

#### 3.Clone Repository GitHub

Clone repositori ini terlebih dahulu:

```bash
git clone https://github.com/0xNathaniel/Tucil2_13523003_13523013.git
cd Tucil2_13523003_13523013
```

---

#### 4. Kompilasi Program

Program ini dilengkapi dengan Makefile yang sudah otomatis mengatur semua file sumber dan dependensi. Untuk compile program, cukup jalankan:

```bash
make
```

Perintah ini akan menghasilkan binary di `bin/main`

---

#### 5. Menjalankan Program

Setelah berhasil dikompilasi, jalankan program dengan:

```bash
make run
```

---

## Authors

| Nama                     | NIM      |
| ------------------------ | -------- |
| Dave Daniell Yanni       | 13523003 |
| Nathaniel Jonathan Rusli | 13523013 |
