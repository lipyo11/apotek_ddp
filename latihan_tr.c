#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <stdarg.h>

// Jika menggunakan sistem non-Windows, sertakan unistd.h untuk usleep
#ifndef _WIN32
#include <unistd.h>
// Untuk Windows, gunakan windows.h dan Sleep
#else
#include <windows.h>
#endif

// --- KODE WARNA ANSI ---
#define RESET "\x1b[0m"
#define MERAH_TEKS "\x1b[31m"
#define HIJAU_TEKS "\x1b[32m"
#define KUNING_TEKS "\x1b[33m"
#define BIRU_TEKS "\x1b[34m"
#define MAGENTA_TEKS "\x1b[35m"
#define CYAN_TEKS "\x1b[36m"
#define PUTIH_TEKS "\x1b[37m"

#define MERAH_BG "\x1b[41m"
#define PUTIH_BG "\x1b[47m"
#define BIRU_BG "\x1b[44m"
#define HITAM_TEKS "\x1b[30m"

// --- STRUKTUR DATA ---

struct Obat {
    int id;
    char nama[50];
    int harga;
    int stok;
};

struct ItemTransaksi {
    int idObat;
    char namaObat[50];
    int hargaSatuan;
    int jumlah;
};

struct Transaksi {
    int idTransaksi;
    struct ItemTransaksi items[20];
    int jumlahItems;
    int totalBelanja;
    char metodeBayar[10];
    int uangBayar;
    int kembalian;
    int sudahDiretur;
};

// --- DATABASE GLOBAL (Array Sederhana) ---
struct Obat daftarObat[100];
int jumlahObat = 0;

struct Transaksi riwayatTransaksi[100];
int jumlahTransaksi = 0;

// --- FUNGSI UTILITAS (Bantuan & Perbaikan Input) ---

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// FUNGSI FLUSH INPUT PALING STABIL
void flush_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ⭐ FUNGSI UTAMA UNTUK RATA TENGAH HEADER
void printCentered(const char *text) {
    int terminal_width = 80; 
    int text_length = strlen(text);
    
    // Perhitungan kasar untuk mengabaikan kode ANSI dalam panjang string
    int real_length = 0;
    for (int i = 0; i < text_length; i++) {
        if (text[i] == '\x1b') {
            while (i < text_length && text[i] != 'm') i++;
        } else {
            real_length++;
        }
    }

    int padding = (terminal_width - real_length) / 2;

    if (padding < 0) padding = 0;

    for (int i = 0; i < padding; i++) {
        printf(" ");
    }
    printf("%s\n", text);
}

// ⭐ FUNGSI BANTUAN UNTUK PADDING INPUT (Variadic Argument)
void printLeftPadded(const char *format, ...) {
    va_list args;
    va_start(args, format);

    const int padding = 20; // Padding agar output terlihat terpusat

    for (int i = 0; i < padding; i++) {
        printf(" ");
    }
    vprintf(format, args);
    va_end(args);
}

// FUNGSI PAUSE
void pause() {
    printf("\n");
    printCentered(KUNING_TEKS "Tekan Enter untuk melanjutkan..." RESET);
    flush_input(); 
}

void loading(const char *pesan) {
    printLeftPadded(CYAN_TEKS "%s" RESET, pesan);
    fflush(stdout);
    for (int i = 0; i < 5; i++) {
        printf(CYAN_TEKS "." RESET);
        fflush(stdout);
        #ifdef _WIN32
            Sleep(200);
        #else
            usleep(200000);
        #endif
    }
    printf("\n");
}

// Program Konfirmasi
int konfirmasi(char *pesan) {
    char pilihan;
    while (1) {
        printLeftPadded(MAGENTA_TEKS "%s" RESET " (y/n): ", pesan);
        if (scanf(" %c", &pilihan) != 1) { 
             flush_input(); 
             continue;
        }
        flush_input(); 

        pilihan = tolower(pilihan);
        if (pilihan == 'y') {
            return 1;
        } else if (pilihan == 'n') {
            return 0;
        } else {
            printLeftPadded(MERAH_TEKS "Input tidak valid. Harap masukkan 'y' atau 'n'.\n" RESET);
        }
    }
}

int findObatById(int id) {
    for (int i = 0; i < jumlahObat; i++) {
        if (daftarObat[i].id == id) {
            return i;
        }
    }
    return -1;
}

int findTransaksiById(int id) {
    for (int i = 0; i < jumlahTransaksi; i++) {
        if (riwayatTransaksi[i].idTransaksi == id) {
            return i;
        }
    }
    return -1;
}

void initData() {
    daftarObat[0] = (struct Obat){1, "Paracetamol", 5000, 100};
    daftarObat[1] = (struct Obat){2, "Bodrex", 2500, 100};
    daftarObat[2] = (struct Obat){3, "OBH Combi", 1500, 50};
    daftarObat[3] = (struct Obat){4, "Promag", 6000, 75};
    jumlahObat = 4;
}


// --- UPDATE: RUMAH KOMPLEKS (Segitiga/Persegi + Trapesium/Persegi Panjang) ---
// --- FUNGSI BANTUAN UNTUK DELAY (ANIMASI) ---
// Taruh fungsi ini SEBELUM void rumahPola()
void delay_ms(int ms) {
    #ifdef _WIN32
        Sleep(ms); // Windows pakai milidetik
    #else
        usleep(ms * 1000); // Linux/Mac pakai mikrodearik
    #endif
}
void rumahPola() {
    clearScreen();
    printCentered(BIRU_TEKS "=========================================" RESET);
    printCentered(CYAN_TEKS "   RUMAH SKETSA (SERBA PROPORSIONAL)" RESET);
    printCentered(BIRU_TEKS "=========================================" RESET);
    printf("\n");

    int n = 0; 
    char bahan = '#';

    // --- INPUT SKALA (n) ---
    // Angka ini akan menjadi tinggi untuk SEMUA bagian (Cerobong, Atap, Body)
    printLeftPadded("Masukkan " HIJAU_TEKS "Skala Bangunan" RESET " (Min 4): ");
    if (scanf("%d", &n) != 1) {
        flush_input();
        printLeftPadded(MERAH_TEKS "Input tidak valid. Default ke 6.\n" RESET);
        n = 6;
    }
    flush_input();

    if (n < 4) {
        n = 4;
        printLeftPadded(KUNING_TEKS "Skala diatur ke 4 agar proporsional.\n" RESET);
    }

    // --- INPUT BAHAN ---
    printLeftPadded("Masukkan " HIJAU_TEKS "Karakter Bahan" RESET " (1 karakter): ");
    if (scanf(" %c", &bahan) != 1) {
        flush_input();
        printLeftPadded(MERAH_TEKS "Input tidak valid. Default ke '#'.\n" RESET);
        bahan = '#';
    }
    flush_input();

    loading("Membangun Sesuai Ukuran...");
    printf("\n");

    // --- PERHITUNGAN DIMENSI ---
    
    // 1. Tinggi Komponen (SEMUA SAMA DENGAN n)
    int t_cerobong = n; // <--- INI PERUBAHANNYA (Sesuai permintaan)
    int t_atap     = n;
    int t_body     = n;

    // 2. Lebar Komponen
    int l_alas_segitiga = (2 * n) - 1; 
    int l_persegi       = l_alas_segitiga;    
    int l_kanan         = n * 2; 

    // Lebar cerobong tetap kita buat agak ramping (setengah n) 
    // supaya terlihat seperti cerobong, bukan lantai tambahan.
    int l_cerobong = (n < 6) ? 3 : n / 2; 

    // 3. Posisi (Centering)
    int total_width = l_persegi + l_kanan;
    int offset_x = (80 - total_width) / 2;
    if (offset_x < 0) offset_x = 0;
    
    // Posisi Horizontal Cerobong (Indentasi)
    int indent_cerobong = offset_x + n + (l_kanan / 4);

    int speed = 15; 
    int row_delay = 50; 

    // ==========================================
    // --- FASE 0: CEROBONG ASAP ---
    // ==========================================
    // Mencetak cerobong setinggi 'n' baris
    
    printf(KUNING_TEKS); 
    for (int i = 0; i < t_cerobong; i++) {
        // Spasi margin kiri sampai posisi cerobong
        for (int s = 0; s < indent_cerobong; s++) printf(" ");

        // Cetak blok cerobong
        for (int k = 0; k < l_cerobong; k++) {
            printf("%c", bahan); 
            fflush(stdout); delay_ms(speed);
        }
        printf("\n");
        delay_ms(row_delay);
    }
    printf(RESET);

    // ==========================================
    // --- FASE 1: ATAP ---
    // ==========================================
    // Mencetak atap setinggi 'n' baris (looping i <= n)
    
    for (int i = 1; i <= n; i++) {
        for (int s = 0; s < offset_x; s++) printf(" ");

        // SEGITIGA (HIJAU)
        for (int s = 0; s < n - i; s++) printf(" "); // Spasi pembentuk segitiga
        printf(HIJAU_TEKS); 
        for (int k = 0; k < (2 * i) - 1; k++) {
            printf("%c", bahan); fflush(stdout); delay_ms(speed);
        }

        // ATAP DATAR (MERAH)
        printf(MERAH_TEKS); 
        for (int k = 0; k < l_kanan; k++) {
            printf("%c", bahan); fflush(stdout); delay_ms(speed);
        }
        printf(RESET "\n"); delay_ms(row_delay);
    }

    // ==========================================
    // --- FASE 2: BODY RUMAH ---
    // ==========================================
    // Mencetak body setinggi 'n' baris (looping i <= n)

    for (int i = 1; i <= n; i++) {
        for (int s = 0; s < offset_x; s++) printf(" ");

        // PERSEGI KIRI (UNGU)
        printf(MAGENTA_TEKS); 
        for (int k = 0; k < l_persegi; k++) {
            printf("%c", bahan); fflush(stdout); delay_ms(speed);
        }

        // PERSEGI PANJANG KANAN (BIRU)
        printf(BIRU_TEKS); 
        for (int k = 0; k < l_kanan; k++) {
            printf("%c", bahan); fflush(stdout); delay_ms(speed);
        }
        printf(RESET "\n"); delay_ms(row_delay);
    }

    printf("\n");
    printCentered(CYAN_TEKS "Rumah Selesai!" RESET);
    printf("\n");
    pause();
}


void benderaNegara() {
    clearScreen();
    printCentered(BIRU_TEKS "=========================================" RESET);
    printCentered(CYAN_TEKS "      BENDERA REPUBLIK INDONESIA" RESET);
    printCentered(BIRU_TEKS "=========================================" RESET);
    printf("\n");

    int tinggi = 0;
    // Input Tinggi
    printLeftPadded("Masukkan " HIJAU_TEKS "Tinggi" RESET " Bendera (Min 4, kelipatan 2): ");
    if (scanf("%d", &tinggi) != 1 || tinggi < 4) {
        flush_input();
        printLeftPadded(MERAH_TEKS "Input Tinggi tidak valid. Default ke 4.\n" RESET);
        tinggi = 4;
    }
    flush_input();

    // Validasi Kelipatan 2
    if (tinggi % 2 != 0) {
        tinggi++;
        printLeftPadded(KUNING_TEKS "Tinggi diatur menjadi %d (Kelipatan 2 terdekat).\n" RESET, tinggi);
    }

    int lebar = tinggi * 2;
    int tinggiSetengah = tinggi / 2;

    loading("Menjahit Bendera Merah Putih");
    printf("\n");

    int total_width = lebar; 
    int center_offset = (80 - total_width) / 2;
    if (center_offset < 0) center_offset = 0;

    // Kecepatan animasi (semakin kecil semakin cepat)
    int speed = 10; 

    // --- BAGIAN MERAH ---
    for (int i = 0; i < tinggiSetengah; i++) {
        // 1. Cetak Margin Kiri (Posisi Awal)
        for (int j = 0; j < center_offset; j++) printf(" ");
        
        // 2. Set Warna Background Merah
        printf(MERAH_BG);

        // 3. Loop Cetak Per Karakter (Efek Mengetik)
        for (int j = 0; j < lebar; j++) {
            printf(" ");            // Cetak 1 blok merah
            fflush(stdout);         // Paksa tampil di layar segera
            delay_ms(speed);        // Jeda sebentar
        }
        
        // 4. Reset Warna & Pindah Baris
        printf(RESET "\n");
    }

    // --- BAGIAN PUTIH ---
    for (int i = 0; i < tinggiSetengah; i++) {
        // 1. Cetak Margin Kiri
        for (int j = 0; j < center_offset; j++) printf(" ");

        // 2. Set Warna Background Putih (Teks Hitam agar aman)
        printf(PUTIH_BG HITAM_TEKS);

        // 3. Loop Cetak Per Karakter (Efek Mengetik)
        for (int j = 0; j < lebar; j++) {
            printf(" ");            // Cetak 1 blok putih
            fflush(stdout);         // Paksa tampil di layar segera
            delay_ms(speed);        // Jeda sebentar
        }
        
        // 4. Reset Warna & Pindah Baris
        printf(RESET "\n");
    }

    printf("\n");
    printCentered(CYAN_TEKS "MERDEKA!" RESET);
    printf("\n\n");
    pause();
}
// --- FUNGSI INTI APOTIK: CRUD OBAT ---

void lihatListObat() {
    clearScreen();
    printCentered(BIRU_TEKS "===================================================" RESET);
    printCentered(CYAN_TEKS "             LIST SEMUA OBAT" RESET);
    printCentered(BIRU_TEKS "===================================================" RESET);
    
    const int table_width = 41;
    int padding = (80 - table_width) / 2;

    if (jumlahObat == 0) {
        printCentered(KUNING_TEKS "Belum ada data obat." RESET);
    } else {
        for (int i = 0; i < padding; i++) printf(" ");
        printf(HIJAU_TEKS "ID | Nama Obat         | Harga      | Stok\n" RESET);
        for (int i = 0; i < padding; i++) printf(" ");
        printf(HIJAU_TEKS "---|-------------------|------------|-----\n" RESET);
        
        for (int i = 0; i < jumlahObat; i++) {
            for (int j = 0; j < padding; j++) printf(" ");
            printf("%-2d | %-17s | " MERAH_TEKS "Rp %-7d" RESET " | %d\n",
                   daftarObat[i].id,
                   daftarObat[i].nama,
                   daftarObat[i].harga,
                   daftarObat[i].stok);
        }
    }
    printCentered(BIRU_TEKS "=====================================================" RESET);
}

void tambahObat() {
    clearScreen();
    printCentered(MAGENTA_TEKS "--- Tambah Obat Baru ---" RESET);
    printf("\n");

    struct Obat newObat;
    newObat.id = (jumlahObat == 0) ? 1 : daftarObat[jumlahObat - 1].id + 1;

    printLeftPadded("Nama Obat: ");
    if (fgets(newObat.nama, sizeof(newObat.nama), stdin) == NULL) {
        printLeftPadded(MERAH_TEKS "Error membaca input nama.\n" RESET);
        return;
    }
    newObat.nama[strcspn(newObat.nama, "\n")] = 0;

    printLeftPadded("Harga (Rp): ");
    if (scanf("%d", &newObat.harga) != 1) { newObat.harga = 0; }
    flush_input(); 

    printLeftPadded("Stok Awal: ");
    if (scanf("%d", &newObat.stok) != 1) { newObat.stok = 0; }
    flush_input(); 
    printf("\n");

    if (konfirmasi("Simpan obat baru ini?")) {
        loading("Menyimpan data");
        daftarObat[jumlahObat] = newObat;
        jumlahObat++;
        printLeftPadded(HIJAU_TEKS "Sukses! Obat baru telah ditambahkan dengan ID %d.\n" RESET, newObat.id);
    } else {
        printLeftPadded(KUNING_TEKS "Penambahan obat dibatalkan.\n" RESET);
    }
    pause();
}

void editObat() {
    clearScreen();
    lihatListObat();
    printCentered(MAGENTA_TEKS "--- Edit Obat ---" RESET);
    if (jumlahObat == 0) { pause(); return; }

    int id;
    printLeftPadded("Masukkan ID obat yang ingin diedit: ");
    if (scanf("%d", &id) != 1) { id = -1; }
    flush_input(); 

    int index = findObatById(id);
    if (index == -1) {
        printLeftPadded(MERAH_TEKS "Error: Obat dengan ID %d tidak ditemukan.\n" RESET, id);
    } else {
        printLeftPadded("Anda mengedit: " KUNING_TEKS "%s\n" RESET, daftarObat[index].nama);
        printLeftPadded("\nMasukkan data baru (masukkan 0 jika harga/stok tidak ingin diubah/diisi, atau ENTER/0 untuk nama):\n");

        char namaBaru[50];
        namaBaru[0] = '\0';
        int hargaBaru = -1, stokBaru = -1;

        printLeftPadded("Nama Baru (" CYAN_TEKS "%s" RESET "): ", daftarObat[index].nama);
        if (fgets(namaBaru, sizeof(namaBaru), stdin) != NULL) {
             namaBaru[strcspn(namaBaru, "\n")] = 0;
        }

        printLeftPadded("Harga Baru (" CYAN_TEKS "Rp %d" RESET "): Rp ", daftarObat[index].harga);
        if (scanf("%d", &hargaBaru) != 1) { hargaBaru = -1; }
        flush_input();

        printLeftPadded("Stok Baru (" CYAN_TEKS "%d" RESET "): ", daftarObat[index].stok);
        if (scanf("%d", &stokBaru) != 1) { stokBaru = -1; }
        flush_input();

        printf("\n");
        if (konfirmasi("Simpan perubahan?")) {
            loading("Memperbarui data");
            if (strlen(namaBaru) > 0 && strcmp(namaBaru, "0") != 0) {
                strcpy(daftarObat[index].nama, namaBaru);
            }
            if (hargaBaru > 0) {
                daftarObat[index].harga = hargaBaru;
            }
            if (stokBaru >= 0) {
                daftarObat[index].stok = stokBaru;
            }
            printLeftPadded(HIJAU_TEKS "Sukses! Data obat telah diperbarui.\n" RESET);
        } else {
            printLeftPadded(KUNING_TEKS "Perubahan dibatalkan.\n" RESET);
        }
    }
    pause();
}

void hapusObat() {
    clearScreen();
    lihatListObat();
    printCentered(MAGENTA_TEKS "--- Hapus Obat ---" RESET);
    if (jumlahObat == 0) { pause(); return; }

    int id;
    printLeftPadded("Masukkan ID obat yang ingin dihapus: ");
    if (scanf("%d", &id) != 1) { id = -1; }
    flush_input(); 

    int index = findObatById(id);
    if (index == -1) {
        printLeftPadded(MERAH_TEKS "Error: Obat dengan ID %d tidak ditemukan.\n" RESET, id);
    } else {
        printLeftPadded("Anda akan menghapus: " MERAH_TEKS "%s\n" RESET, daftarObat[index].nama);
        if (konfirmasi("Apakah Anda yakin ingin menghapus obat ini?")) {
            loading("Menghapus data");
            for (int i = index; i < jumlahObat - 1; i++) {
                daftarObat[i] = daftarObat[i + 1];
            }
            jumlahObat--;
            printLeftPadded(HIJAU_TEKS "Sukses! Obat telah dihapus.\n" RESET);
        } else {
            printLeftPadded(KUNING_TEKS "Penghapusan dibatalkan.\n" RESET);
        }
    }
    pause();
}

// --- FUNGSI INTI APOTIK: TRANSAKSI ---

void cetakStruk(struct Transaksi trx) {
    const int line_width = 38;
    int padding = (80 - line_width) / 2;
    for (int i = 0; i < padding; i++) printf(" ");
    printf(CYAN_TEKS "\n\n--- STRUK PEMBELIAN (ID: %d) -----\n" RESET, trx.idTransaksi);
    for (int i = 0; i < padding; i++) printf(" ");
    printf("------------------------------------------------\n");
    for (int i = 0; i < padding; i++) printf(" ");
    printf(HIJAU_TEKS "Item              | Qty | Harga      | Subtotal\n" RESET);
    for (int i = 0; i < padding; i++) printf(" ");
    printf("------------------|-----|------------|----------\n");
    for (int i = 0; i < trx.jumlahItems; i++) {
        struct ItemTransaksi item = trx.items[i];
        for (int j = 0; j < padding; j++) printf(" ");
        printf("%-17s | %-3d | " MAGENTA_TEKS "Rp %-7d" RESET " | " BIRU_TEKS "Rp %d" RESET "\n",
               item.namaObat,
               item.jumlah,
               item.hargaSatuan,
               (item.hargaSatuan * item.jumlah));
    }
    for (int i = 0; i < padding; i++) printf(" ");
    printf("-------------------------------------------------\n");
    for (int i = 0; i < padding; i++) printf(" ");
    printf(KUNING_TEKS "Total Belanja " RESET "  : " MERAH_TEKS "Rp %d\n" RESET, trx.totalBelanja);
    for (int i = 0; i < padding; i++) printf(" ");
    printf("Metode Bayar     : %s\n", trx.metodeBayar);
    for (int i = 0; i < padding; i++) printf(" ");
    printf("Uang Bayar       : Rp %d\n", trx.uangBayar);
    for (int i = 0; i < padding; i++) printf(" ");
    printf("Kembalian        : " HIJAU_TEKS "Rp %d\n" RESET, trx.kembalian);
    for (int i = 0; i < padding; i++) printf(" ");
    printf("Status Return    : %s\n", trx.sudahDiretur ? MERAH_TEKS "SUDAH DIRETRU" RESET : HIJAU_TEKS "BELUM DIRETRU" RESET);
    for (int i = 0; i < padding; i++) printf(" ");
    printf("-----------------------------------------------\n");
    for (int i = 0; i < padding; i++) printf(" ");
    printf(CYAN_TEKS "Terima kasih!\n" RESET);
}

void transaksiBaru() {
    clearScreen();
    printCentered(BIRU_TEKS "=========================================" RESET);
    printCentered(CYAN_TEKS "             TRANSAKSI BARU" RESET);
    printCentered(BIRU_TEKS "=========================================" RESET);

    if (jumlahObat == 0) {
        printLeftPadded(MERAH_TEKS "Belum ada obat. Transaksi tidak bisa dilakukan.\n" RESET);
        pause();
        return;
    }

    struct Transaksi newTrx;
    newTrx.idTransaksi = jumlahTransaksi + 1;
    newTrx.jumlahItems = 0;
    newTrx.totalBelanja = 0;
    newTrx.sudahDiretur = 0;

    int idObat = 0;
    while (idObat != -1) {
        clearScreen();
        printCentered(MAGENTA_TEKS "--- Input Item ---" RESET);
        lihatListObat();

        printLeftPadded(HIJAU_TEKS "\nTotal saat ini: Rp %d\n" RESET, newTrx.totalBelanja);
        printLeftPadded("Masukkan ID Obat yang dibeli (atau -1 untuk selesai): ");
        if (scanf("%d", &idObat) != 1) { idObat = 0; }
        flush_input(); 

        if (idObat == -1) break;
        if (idObat == 0) continue;

        int index = findObatById(idObat);
        if (index == -1) {
            printLeftPadded(MERAH_TEKS "Error: Obat tidak ditemukan!\n" RESET);
            pause();
            continue;
        }

        int jumlahBeli = 0;
        printLeftPadded("Jumlah " KUNING_TEKS "%s" RESET " yang dibeli (Stok: %d): ", daftarObat[index].nama, daftarObat[index].stok);
        if (scanf("%d", &jumlahBeli) != 1) { jumlahBeli = 0; }
        flush_input(); 

        if (jumlahBeli > 0 && jumlahBeli <= daftarObat[index].stok) {

            if (newTrx.jumlahItems >= 20) {
                 printLeftPadded(MERAH_TEKS "Error: Maksimum 20 jenis item per transaksi.\n" RESET);
                 pause();
                 continue;
            }

            struct ItemTransaksi newItem;
            newItem.idObat = daftarObat[index].id;
            strcpy(newItem.namaObat, daftarObat[index].nama);
            newItem.hargaSatuan = daftarObat[index].harga;
            newItem.jumlah = jumlahBeli;

            newTrx.items[newTrx.jumlahItems] = newItem;
            newTrx.jumlahItems++;
            newTrx.totalBelanja += (newItem.hargaSatuan * newItem.jumlah);

            daftarObat[index].stok -= jumlahBeli;

            printLeftPadded(HIJAU_TEKS "Berhasil menambahkan %s (Qty: %d).\n" RESET, newItem.namaObat, newItem.jumlah);
            pause();
        } else {
            printLeftPadded(MERAH_TEKS "Error: Jumlah beli tidak valid atau Stok (%d) tidak mencukupi!\n" RESET, daftarObat[index].stok);
            pause();
        }
    }

    if (newTrx.jumlahItems == 0) {
        printLeftPadded(KUNING_TEKS "Transaksi dibatalkan karena tidak ada item.\n" RESET);
        pause();
        return;
    }

    // --- PEMBAYARAN ---
    clearScreen();
    printCentered(MAGENTA_TEKS "--- PROSES PEMBAYARAN ---" RESET);
    printLeftPadded("Total Belanja Anda: " MERAH_TEKS "Rp %d\n" RESET, newTrx.totalBelanja);
    printLeftPadded("Pilih Metode Pembayaran (1. CASH / 2. QRIS): ");

    int metode;
    if (scanf("%d", &metode) != 1) { metode = 0; }
    flush_input(); 

    loading("Memproses pembayaran");

    if (metode == 2) {
        strcpy(newTrx.metodeBayar, "QRIS");
        newTrx.uangBayar = newTrx.totalBelanja;
        newTrx.kembalian = 0;
        printLeftPadded(HIJAU_TEKS "Pembayaran QRIS berhasil. Total Rp %d. Tidak ada kembalian.\n" RESET, newTrx.totalBelanja);
    } else {
        strcpy(newTrx.metodeBayar, "CASH");
        int uangBayar = 0;
        while (uangBayar < newTrx.totalBelanja) {
            printLeftPadded("Masukkan Uang Tunai (CASH): Rp ");
            if (scanf("%d", &uangBayar) != 1) { uangBayar = 0; }
            flush_input(); 
            if (uangBayar < newTrx.totalBelanja) {
                printLeftPadded(MERAH_TEKS "Uang tidak cukup!\n" RESET);
            }
        }
        newTrx.uangBayar = uangBayar;
        newTrx.kembalian = uangBayar - newTrx.totalBelanja;
        printLeftPadded("Kembalian: " HIJAU_TEKS "Rp %d\n" RESET, newTrx.kembalian);
    }

    riwayatTransaksi[jumlahTransaksi] = newTrx;
    jumlahTransaksi++;
    cetakStruk(newTrx);
    pause();
}

void lihatRiwayatTransaksi() {
    clearScreen();
    printCentered(BIRU_TEKS "=========================================" RESET);
    printCentered(CYAN_TEKS "          RIWAYAT TRANSAKSI" RESET);
    printCentered(BIRU_TEKS "=========================================" RESET);

    if (jumlahTransaksi == 0) {
        printCentered(KUNING_TEKS "Belum ada riwayat transaksi." RESET);
        pause();
        return;
    }

    const int table_width = 42;
    int padding = (80 - table_width) / 2;

    for (int i = 0; i < padding; i++) printf(" ");
    printf(HIJAU_TEKS "ID | Total Belanja | Metode Bayar | Status Return\n" RESET);
    for (int i = 0; i < padding; i++) printf(" ");
    printf(HIJAU_TEKS "---|---------------|--------------|---------------\n" RESET);
    for (int i = 0; i < jumlahTransaksi; i++) {
        for (int j = 0; j < padding; j++) printf(" ");
        printf("%-2d | " MERAH_TEKS "Rp %-10d" RESET " | %-12s | %s\n",
                 riwayatTransaksi[i].idTransaksi,
                 riwayatTransaksi[i].totalBelanja,
                 riwayatTransaksi[i].metodeBayar,
                 riwayatTransaksi[i].sudahDiretur ? MERAH_TEKS "SUDAH" RESET : HIJAU_TEKS "BELUM" RESET);
    }
    printCentered(BIRU_TEKS "=========================================" RESET);

    printLeftPadded("Masukkan ID Transaksi untuk lihat detail (0=kembali): ");
    int id;
    if (scanf("%d", &id) != 1) { id = 0; }
    flush_input(); 

    if (id != 0) {
        int index = findTransaksiById(id);
        if (index != -1) {
            loading("Mengambil detail transaksi");
            cetakStruk(riwayatTransaksi[index]);
        } else {
            printLeftPadded(MERAH_TEKS "ID Transaksi tidak ditemukan.\n" RESET);
        }
    }
    pause();
}


// --- FUNGSI INTI APOTIK: RETURN ---

void returnProduk() {
    clearScreen();
    printCentered(BIRU_TEKS "=========================================" RESET);
    printCentered(CYAN_TEKS "            RETURN PRODUK" RESET);
    printCentered(BIRU_TEKS "=========================================" RESET);

    if (jumlahTransaksi == 0) {
        printLeftPadded(KUNING_TEKS "Belum ada transaksi untuk diretur.\n" RESET);
        pause();
        return;
    }

    printLeftPadded("Masukkan ID Transaksi yang ingin diretur: ");
    int idTrx;
    if (scanf("%d", &idTrx) != 1) { idTrx = -1; }
    flush_input(); 

    int indexTrx = findTransaksiById(idTrx);

    if (indexTrx == -1) {
        printLeftPadded(MERAH_TEKS "Error: ID Transaksi %d tidak ditemukan.\n" RESET, idTrx);
        pause();
        return;
    }

    if (riwayatTransaksi[indexTrx].sudahDiretur) {
        printLeftPadded(MERAH_TEKS "Error: Transaksi ini sudah pernah diproses retur.\n" RESET);
        pause();
        return;
    }

    printLeftPadded("Item pada Transaksi ID " KUNING_TEKS "%d" RESET ":\n", idTrx);
    
    const int table_width = 35;
    int padding = (80 - table_width) / 2;
    for (int i = 0; i < padding; i++) printf(" ");
    printf(HIJAU_TEKS "ID | Nama Obat         | Qty Dibeli\n" RESET);
    for (int i = 0; i < padding; i++) printf(" ");
    printf(HIJAU_TEKS "---|-------------------|-----------\n" RESET);
    for (int i = 0; i < riwayatTransaksi[indexTrx].jumlahItems; i++) {
        struct ItemTransaksi item = riwayatTransaksi[indexTrx].items[i];
        for (int j = 0; j < padding; j++) printf(" ");
        printf("%-2d | %-17s | %d\n", item.idObat, item.namaObat, item.jumlah);
    }
    for (int i = 0; i < padding; i++) printf(" ");
    printf("--------------------------------------\n");

    printLeftPadded("Masukkan ID Obat yang ingin diretur (dari list di atas): ");
    int idObatRetur;
    if (scanf("%d", &idObatRetur) != 1) { idObatRetur = -1; }
    flush_input(); 

    int indexItemTrx = -1;
    for (int i = 0; i < riwayatTransaksi[indexTrx].jumlahItems; i++) {
        if (riwayatTransaksi[indexTrx].items[i].idObat == idObatRetur) {
            indexItemTrx = i;
            break;
        }
    }

    if (indexItemTrx == -1) {
        printLeftPadded(MERAH_TEKS "Error: Obat dengan ID %d tidak dibeli pada transaksi ini.\n" RESET, idObatRetur);
        pause();
        return;
    }

    int jumlahRetur;
    int qtyDibeli = riwayatTransaksi[indexTrx].items[indexItemTrx].jumlah;
    printLeftPadded("Jumlah " KUNING_TEKS "%s" RESET " yang ingin diretur (Maks: %d): ", riwayatTransaksi[indexTrx].items[indexItemTrx].namaObat, qtyDibeli);
    if (scanf("%d", &jumlahRetur) != 1) { jumlahRetur = 0; }
    flush_input(); 

    if (jumlahRetur <= 0 || jumlahRetur > qtyDibeli) {
        printLeftPadded(MERAH_TEKS "Error: Jumlah retur tidak valid.\n" RESET);
        pause();
        return;
    }

    int hargaSatuan = riwayatTransaksi[indexTrx].items[indexItemTrx].hargaSatuan;
    int totalRefund = hargaSatuan * jumlahRetur;

    printLeftPadded("\nTotal refund untuk %d x %s adalah " MERAH_TEKS "Rp %d\n" RESET, jumlahRetur, riwayatTransaksi[indexTrx].items[indexItemTrx].namaObat, totalRefund);

    if (konfirmasi("Proses retur dan kembalikan uang customer?")) {
        loading("Memproses retur");
        int indexObatMaster = findObatById(idObatRetur);
        if (indexObatMaster != -1) {
            daftarObat[indexObatMaster].stok += jumlahRetur;
        }

        riwayatTransaksi[indexTrx].sudahDiretur = 1;

        printLeftPadded(HIJAU_TEKS "✅ Sukses! Retur diproses.\n" RESET);
        printLeftPadded("Stok " KUNING_TEKS "%s" RESET " dikembalikan. Harap berikan refund " MERAH_TEKS "Rp %d" RESET " ke customer.\n",
               daftarObat[indexObatMaster].nama, totalRefund);
    } else {
        printLeftPadded(KUNING_TEKS "Retur dibatalkan.\n" RESET);
    }
    pause();
}

// --- FUNGSI MENU & LOGIN ---

int login() {
    const char hardcodeUser[] = "admin";
    const char hardcodePass[] = "123";
    char inputUser[50];
    char inputPass[50];
    int attempts = 0;
    const int max_attempts = 3;

    while (attempts < max_attempts) {
        clearScreen();
        printCentered(BIRU_TEKS "=========================================" RESET);
        printCentered(CYAN_TEKS "          LOGIN SISTEM APOTIK" RESET);
        printCentered(BIRU_TEKS "=========================================" RESET);
        
        printLeftPadded("Percobaan ke-" KUNING_TEKS "%d" RESET " dari %d\n", attempts + 1, max_attempts);

        printLeftPadded("Username: ");
        // Perbaikan: Batasi input 49 karakter untuk mencegah buffer overflow
        if (scanf("%49s", inputUser) != 1) {
            flush_input();
            attempts++;
            continue;
        }
        flush_input(); 

        printLeftPadded("Password: ");
        // Perbaikan: Batasi input 49 karakter untuk mencegah buffer overflow
        if (scanf("%49s", inputPass) != 1) {
            flush_input();
            attempts++;
            continue;
        }
        flush_input(); 

        if (strcmp(inputUser, hardcodeUser) == 0 && strcmp(inputPass, hardcodePass) == 0) {
            loading("Memeriksa kredensial");
            printLeftPadded(HIJAU_TEKS "Login Berhasil! Selamat datang.\n" RESET);
            pause();
            return 1;
        } else {
            attempts++;
            printLeftPadded(MERAH_TEKS "❌ Username atau Password salah! " RESET);
            if (attempts < max_attempts) {
                printLeftPadded("Anda memiliki %d percobaan tersisa.\n", max_attempts - attempts);
                pause();
            } else {
                printLeftPadded("Semua percobaan gagal. Program akan keluar.\n");
            }
        }
    }
    return 0; // Login gagal
}

void menuManajemenObat() {
    int pilihan = 0;
    while (pilihan != 5) {
        clearScreen();
        printCentered(BIRU_TEKS "=========================================" RESET);
        printCentered(CYAN_TEKS "          MANAJEMEN OBAT (CRUD)" RESET);
        printCentered(BIRU_TEKS "=========================================" RESET);
        
        printLeftPadded("1. Lihat Semua Obat " HIJAU_TEKS "(Read)\n" RESET);
        printLeftPadded("2. Tambah Obat Baru " HIJAU_TEKS "(Create)\n" RESET);
        printLeftPadded("3. Edit Obat " HIJAU_TEKS "(Update)\n" RESET);
        printLeftPadded("4. Hapus Obat " HIJAU_TEKS "(Delete)\n" RESET);
        printLeftPadded("5. " KUNING_TEKS "Kembali\n" RESET);
        
        printCentered(BIRU_TEKS "=========================================" RESET);
        printLeftPadded("Pilihan Anda: ");

        if (scanf("%d", &pilihan) != 1) { pilihan = 0; }
        flush_input(); 

        switch (pilihan) {
            case 1: loading("Mengambil data obat"); lihatListObat(); pause(); break;
            case 2: tambahObat(); break; 
            case 3: editObat(); break;
            case 4: hapusObat(); break;
            case 5: printLeftPadded(KUNING_TEKS "Kembali ke Menu Apotik...\n" RESET); pause(); break;
            default: printLeftPadded(MERAH_TEKS "Pilihan tidak valid!\n" RESET); pause(); break;
        }
    }
}


void menuApotik() {
    int pilihan = 0;
    while (pilihan != 5) {
        clearScreen();
        printCentered(BIRU_TEKS "=========================================" RESET);
        printCentered(CYAN_TEKS "      SISTEM APOTIK SEDERHANA" RESET);
        printCentered(BIRU_TEKS "=========================================" RESET);
        
        printLeftPadded("1. Manajemen Obat\n");
        printLeftPadded("2. Transaksi Baru\n");
        printLeftPadded("3. Riwayat Transaksi\n");
        printLeftPadded("4. Return Produk\n");
        printLeftPadded("5. " KUNING_TEKS "Logout (Kembali ke Menu Utama)\n" RESET);
        
        printCentered(BIRU_TEKS "=========================================" RESET);
        printLeftPadded("Pilihan Anda: ");

        if (scanf("%d", &pilihan) != 1) { pilihan = 0; }
        flush_input(); 

        switch (pilihan) {
            case 1: loading("Masuk menu manajemen"); menuManajemenObat(); break;
            case 2: loading("Memulai transaksi baru"); transaksiBaru(); break;
            case 3: lihatRiwayatTransaksi(); break;
            case 4: loading("Masuk menu retur produk"); returnProduk(); break;
            case 5: printLeftPadded(KUNING_TEKS "Anda telah logout.\n" RESET); pause(); break;
            default: printLeftPadded(MERAH_TEKS "Pilihan tidak valid!\n" RESET); pause(); break;
        }
    }
}

// FUNGSI UTAMA (MAIN)
int main() {
    initData();
    int pilihan = 0;

    if (!login()) {
        return 1;
    }

    // --- MENU UTAMA SETELAH LOGIN SUKSES ---
    while (pilihan != 4) {
        clearScreen();
        printCentered(BIRU_TEKS "=========================================" RESET);
        printCentered(CYAN_TEKS "          MENU PROGRAM UTAMA" RESET);
        printCentered(BIRU_TEKS "=========================================" RESET);

        printLeftPadded("1. Sistem Apotik\n");
        printLeftPadded("2. Rumah Pola\n");
        printLeftPadded("3. Bendera Negara\n");
        printLeftPadded("4. " MERAH_TEKS "Exit\n" RESET);

        printCentered(BIRU_TEKS "=========================================" RESET);
        printLeftPadded("Pilihan Anda: ");

        if (scanf("%d", &pilihan) != 1) {
            pilihan = 0;
        }
        flush_input(); 

        switch (pilihan) {
            case 1:
                loading("Masuk ke Sistem Apotik");
                menuApotik();
                break;
            case 2:
                loading("Masuk ke Rumah Pola");
                rumahPola();
                break;
            case 3:
                loading("Masuk ke Bendera Negara");
                benderaNegara();
                break;
            case 4:
                if (konfirmasi("Apakah Anda yakin ingin keluar dari program?")) {
                    printLeftPadded(MERAH_TEKS "Program diakhiri.\n" RESET);
                } else {
                    pilihan = 0;
                }
                break;
            default:
                printLeftPadded(MERAH_TEKS "Pilihan tidak valid.\n" RESET);
                pause();
                break;
        }
    }

    return 0;
}