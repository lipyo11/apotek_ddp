#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <ctype.h>  
#include <time.h>    

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
//baru
struct Transaksi {
    int idTransaksi;
    struct ItemTransaksi items[20];
    int jumlahItems;
    int totalBelanja;
    char metodeBayar[10]; 
    int uangBayar;
    int kembalian;
    int sudahDiretur; // 0 = Belum, 1 = Sudah
};

// --- DATABASE GLOBAL (Array Sederhana) ---
struct Obat daftarObat[100];
int jumlahObat = 0;

struct Transaksi riwayatTransaksi[100];
int jumlahTransaksi = 0;

// --- FUNGSI UTILITAS (Bantuan) ---

void clearScreen() {
#ifdef _WIN32
    system("cls"); 
#else
    system("clear"); 
#endif
}

// ✅ PERBAIKAN FUNGSI PAUSE: Hanya menunggu sekali tekan Enter
void pause() {
    printf("\nTekan Enter untuk melanjutkan...");
    
    // Membersihkan buffer input (pengamanan terakhir)
    int c;
    while ((c = getchar()) != '\n' && c != EOF); 
    
    // Tunggu input Enter dari user
    getchar(); 
}

// Program Konfirmasi
int konfirmasi(char *pesan) {
    char pilihan;
    while (1) {
        printf("%s (y/n): ", pesan);
        // HANYA ambil satu karakter, gunakan spasi sebelum %c untuk skip whitespace
        if (scanf(" %c", &pilihan) != 1) {
             while (getchar() != '\n'); // Clear buffer on error
             continue; // Retry
        }
        while (getchar() != '\n'); // Clear buffer

        pilihan = tolower(pilihan); 
        if (pilihan == 'y') {
            return 1;
        } else if (pilihan == 'n') {
            return 0;
        } else {
            printf("Input tidak valid. Harap masukkan 'y' atau 'n'.\n");
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
    // Data awal untuk pengujian
    daftarObat[0] = (struct Obat){1, "Paracetamol", 5000, 100};
    daftarObat[1] = (struct Obat){2, "Bodrex", 2500, 100};
    daftarObat[2] = (struct Obat){3, "OBH Combi", 1500, 50};
    daftarObat[3] = (struct Obat){4, "Promag", 6000, 75};
    jumlahObat = 4;
}

// --- FUNGSI FITUR TAMBAHAN ---

// Rumah Pola
void rumahPola() {
    clearScreen();
    printf("=========================================\n");
    printf("             RUMAH POLA\n");
    printf("=========================================\n\n");
    printf("      /\\       \n");
    printf("     /  \\      \n");
    printf("    /____\\     \n");
    printf("   | ==== |    \n");
    printf("   | |  | |    \n");
    printf("   |______|    \n");
    printf("\n\n");
    pause();
}

// Bendera Negara (Indonesia - Menggunakan Kode Warna ANSI)
void benderaNegara() {
    clearScreen();
    printf("=========================================\n");
    printf("      BENDERA REPUBLIK INDONESIA\n");
    printf("=========================================\n\n");

    // Kode ANSI: \x1b[41m = Background Merah
    printf("\x1b[41m"); 
    printf("                                                     \n");
    printf("                                                     \n");
    printf("                                                     \n");
    printf("                                                     \n");
    printf("\x1b[0m");  // Reset warna

    // Kode ANSI: \x1b[47m = Background Putih, \x1b[30m = Teks Hitam
    printf("\x1b[47m\x1b[30m"); 
    printf("                                                     \n");
    printf("                                                     \n");
    printf("                                                     \n");
    printf("                                                     \n");
    printf("\x1b[0m"); // Reset semua warna

    printf("\n\n");
    pause();
}

// --- FUNGSI INTI APOTIK: CRUD OBAT ---

void lihatListObat() {
    clearScreen();
    printf("===================================================\n");
    printf("            LIST SEMUA OBAT\n");
    printf("===================================================\n");
    if (jumlahObat == 0) {
        printf("Belum ada data obat.\n");
    } else {
        printf("ID   | Nama Obat           | Harga        | Stok\n");
        printf("-----|---------------------|--------------|-----\n");
        for (int i = 0; i < jumlahObat; i++) {
            printf("%-4d | %-19s | Rp %-9d | %d\n",
                   daftarObat[i].id,
                   daftarObat[i].nama,
                   daftarObat[i].harga,
                   daftarObat[i].stok);
        }
    }
    printf("=====================================================\n");
}

void tambahObat() {
    clearScreen();
    printf("--- Tambah Obat Baru ---\n");
    
    struct Obat newObat;
    newObat.id = (jumlahObat == 0) ? 1 : daftarObat[jumlahObat - 1].id + 1;

    printf("Nama Obat: ");
    scanf(" %49[^\n]", newObat.nama); 
    while (getchar() != '\n'); // Membersihkan buffer

    printf("Harga: Rp ");
    if (scanf("%d", &newObat.harga) != 1) { newObat.harga = 0; }
    while (getchar() != '\n'); // Membersihkan buffer

    printf("Stok Awal: ");
    if (scanf("%d", &newObat.stok) != 1) { newObat.stok = 0; }
    while (getchar() != '\n'); // Membersihkan buffer

    if (konfirmasi("Simpan obat baru ini?")) {
        daftarObat[jumlahObat] = newObat;
        jumlahObat++;
        printf("Sukses! Obat baru telah ditambahkan dengan ID %d.\n", newObat.id);
    } else {
        printf("Penambahan obat dibatalkan.\n");
    }
    pause();
}

void editObat() {
    clearScreen();
    lihatListObat();
    printf("--- Edit Obat ---\n");
    if (jumlahObat == 0) { pause(); return; }

    int id;
    printf("Masukkan ID obat yang ingin diedit: ");
    if (scanf("%d", &id) != 1) { id = -1; }
    while (getchar() != '\n'); // Membersihkan buffer

    int index = findObatById(id);
    if (index == -1) {
        printf("Error: Obat dengan ID %d tidak ditemukan.\n", id);
    } else {
        printf("Anda mengedit: %s\n", daftarObat[index].nama);
        printf("\nMasukkan data baru (masukkan 0 jika harga/stok tidak ingin diubah/diisi):\n");

        char namaBaru[50] = "";
        int hargaBaru = 0, stokBaru = 0;

        printf("Nama Baru (%s): ", daftarObat[index].nama);
        scanf(" %49[^\n]", namaBaru); 
        while (getchar() != '\n'); // Membersihkan buffer

        printf("Harga Baru (Rp %d): Rp ", daftarObat[index].harga);
        if (scanf("%d", &hargaBaru) != 1) { hargaBaru = 0; }
        while (getchar() != '\n'); // Membersihkan buffer

        printf("Stok Baru (%d): ", daftarObat[index].stok);
        if (scanf("%d", &stokBaru) != 1) { stokBaru = 0; }
        while (getchar() != '\n'); // Membersihkan buffer

        if (konfirmasi("Simpan perubahan?")) {
            if (strlen(namaBaru) > 0 && strcmp(namaBaru, "0") != 0) {
                strcpy(daftarObat[index].nama, namaBaru);
            }
            if (hargaBaru > 0) {
                daftarObat[index].harga = hargaBaru;
            }
            if (stokBaru >= 0) { 
                daftarObat[index].stok = stokBaru;
            }
            printf("Sukses! Data obat telah diperbarui.\n");
        } else {
            printf("Perubahan dibatalkan.\n");
        }
    }
    pause();
}

void hapusObat() {
    clearScreen();
    lihatListObat();
    printf("--- Hapus Obat ---\n");
    if (jumlahObat == 0) { pause(); return; }

    int id;
    printf("Masukkan ID obat yang ingin dihapus: ");
    if (scanf("%d", &id) != 1) { id = -1; }
    while (getchar() != '\n'); // Membersihkan buffer

    int index = findObatById(id);
    if (index == -1) {
        printf("Error: Obat dengan ID %d tidak ditemukan.\n", id);
    } else {
        printf("Anda akan menghapus: %s\n", daftarObat[index].nama);
        if (konfirmasi("Apakah Anda yakin ingin menghapus obat ini?")) {
            for (int i = index; i < jumlahObat - 1; i++) {
                daftarObat[i] = daftarObat[i + 1];
            }
            jumlahObat--;
            printf("Sukses! Obat telah dihapus.\n");
        } else {
            printf("Penghapusan dibatalkan.\n");
        }
    }
    pause();
}

// --- FUNGSI INTI APOTIK: TRANSAKSI ---

void cetakStruk(struct Transaksi trx) {
    printf("\n\n--- STRUK PEMBELIAN (ID: %d) ---\n", trx.idTransaksi);
    printf("--------------------------------------\n");
    printf("Item              | Qty | Harga      | Subtotal\n");
    printf("------------------|-----|------------|----------\n");
    for (int i = 0; i < trx.jumlahItems; i++) {
        struct ItemTransaksi item = trx.items[i];
        printf("%-17s | %-3d | Rp %-7d | Rp %d\n",
               item.namaObat,
               item.jumlah,
               item.hargaSatuan,
               (item.hargaSatuan * item.jumlah));
    }
    printf("--------------------------------------\n");
    printf("Total Belanja  : Rp %d\n", trx.totalBelanja);
    printf("Metode Bayar   : %s\n", trx.metodeBayar);
    printf("Uang Bayar     : Rp %d\n", trx.uangBayar);
    printf("Kembalian      : Rp %d\n", trx.kembalian);
    printf("Status Return  : %s\n", trx.sudahDiretur ? "SUDAH DIRETRU" : "BELUM DIRETRU");
    printf("--------------------------------------\n");
    printf("Terima kasih!\n");
}

void transaksiBaru() {
    clearScreen();
    printf("=========================================\n");
    printf("            TRANSAKSI BARU\n");
    printf("=========================================\n");

    if (jumlahObat == 0) {
        printf("Belum ada obat. Transaksi tidak bisa dilakukan.\n");
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
        printf("--- Input Item ---\n");
        lihatListObat();
        
        printf("\nTotal saat ini: Rp %d\n", newTrx.totalBelanja);
        printf("Masukkan ID Obat yang dibeli (atau -1 untuk selesai): ");
        if (scanf("%d", &idObat) != 1) { idObat = 0; }
        while (getchar() != '\n'); // Membersihkan buffer

        if (idObat == -1) break;
        if (idObat == 0) continue; 

        int index = findObatById(idObat);
        if (index == -1) {
            printf("Error: Obat tidak ditemukan!\n");
            pause();
            continue;
        }

        int jumlahBeli = 0;
        printf("Jumlah %s yang dibeli (Stok: %d): ", daftarObat[index].nama, daftarObat[index].stok);
        if (scanf("%d", &jumlahBeli) != 1) { jumlahBeli = 0; }
        while (getchar() != '\n'); // Membersihkan buffer

        if (jumlahBeli > 0 && jumlahBeli <= daftarObat[index].stok) {
            
            if (newTrx.jumlahItems >= 20) {
                 printf("Error: Maksimum 20 jenis item per transaksi.\n");
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

            printf("Berhasil menambahkan %s (Qty: %d).\n", newItem.namaObat, newItem.jumlah);
            pause();
        } else {
            printf("Error: Jumlah beli tidak valid atau Stok (%d) tidak mencukupi!\n", daftarObat[index].stok);
            pause();
        }
    }

    if (newTrx.jumlahItems == 0) {
        printf("Transaksi dibatalkan karena tidak ada item.\n");
        pause();
        return;
    }

    // --- PEMBAYARAN ---
    clearScreen();
    printf("--- PROSES PEMBAYARAN ---\n");
    printf("Total Belanja Anda: Rp %d\n", newTrx.totalBelanja);
    printf("Pilih Metode Pembayaran (1. CASH / 2. QRIS): ");

    int metode;
    if (scanf("%d", &metode) != 1) { metode = 0; }
    while (getchar() != '\n'); // Membersihkan buffer

    if (metode == 2) { 
        strcpy(newTrx.metodeBayar, "QRIS");
        newTrx.uangBayar = newTrx.totalBelanja;
        newTrx.kembalian = 0;
        printf("Pembayaran QRIS berhasil. Total Rp %d. Tidak ada kembalian.\n", newTrx.totalBelanja);
    } else { 
        strcpy(newTrx.metodeBayar, "CASH");
        int uangBayar = 0;
        while (uangBayar < newTrx.totalBelanja) {
            printf("Masukkan Uang Tunai (CASH): Rp ");
            if (scanf("%d", &uangBayar) != 1) { uangBayar = 0; } 
            while (getchar() != '\n'); // Membersihkan buffer
            if (uangBayar < newTrx.totalBelanja) {
                printf("Uang tidak cukup!\n");
            }
        }
        newTrx.uangBayar = uangBayar;
        newTrx.kembalian = uangBayar - newTrx.totalBelanja;
        printf("Kembalian: Rp %d\n", newTrx.kembalian);
    }

    riwayatTransaksi[jumlahTransaksi] = newTrx;
    jumlahTransaksi++;
    cetakStruk(newTrx);
    pause();
}

void lihatRiwayatTransaksi() {
    clearScreen();
    printf("=========================================\n");
    printf("          RIWAYAT TRANSAKSI\n");
    printf("=========================================\n");
    if (jumlahTransaksi == 0) {
        printf("Belum ada riwayat transaksi.\n");
        pause();
        return;
    } 
    
    printf("ID   | Total Belanja | Metode Bayar | Status Return\n");
    printf("-----|---------------|--------------|---------------\n");
    for (int i = 0; i < jumlahTransaksi; i++) {
        printf("%-4d | Rp %-10d | %-12s | %s\n",
                 riwayatTransaksi[i].idTransaksi,
                 riwayatTransaksi[i].totalBelanja,
                 riwayatTransaksi[i].metodeBayar,
                 riwayatTransaksi[i].sudahDiretur ? "SUDAH" : "BELUM");
    }
    printf("=========================================\n");
    
    printf("Masukkan ID Transaksi untuk lihat detail (0=kembali): ");
    int id;
    if (scanf("%d", &id) != 1) { id = 0; }
    while (getchar() != '\n'); // Membersihkan buffer

    if (id != 0) {
        int index = findTransaksiById(id);
        if (index != -1) {
            cetakStruk(riwayatTransaksi[index]);
        } else {
            printf("ID Transaksi tidak ditemukan.\n");
        }
    }
    pause();
}


// --- FUNGSI INTI APOTIK: RETURN ---

void returnProduk() {
    clearScreen();
    printf("=========================================\n");
    printf("            RETURN PRODUK\n");
    printf("=========================================\n");

    if (jumlahTransaksi == 0) {
        printf("Belum ada transaksi untuk diretur.\n");
        pause();
        return;
    }

    printf("Masukkan ID Transaksi yang ingin diretur: ");
    int idTrx;
    if (scanf("%d", &idTrx) != 1) { idTrx = -1; }
    while (getchar() != '\n'); // Membersihkan buffer

    int indexTrx = findTransaksiById(idTrx);

    if (indexTrx == -1) {
        printf("Error: ID Transaksi %d tidak ditemukan.\n", idTrx);
        pause();
        return;
    }

    if (riwayatTransaksi[indexTrx].sudahDiretur) {
        printf("Error: Transaksi ini sudah pernah diproses retur.\n");
        pause();
        return;
    }

    printf("Item pada Transaksi ID %d:\n", idTrx);
    printf("ID   | Nama Obat           | Qty Dibeli\n");
    printf("-----|---------------------|-----------\n");
    for (int i = 0; i < riwayatTransaksi[indexTrx].jumlahItems; i++) {
        struct ItemTransaksi item = riwayatTransaksi[indexTrx].items[i];
        printf("%-4d | %-19s | %d\n", item.idObat, item.namaObat, item.jumlah);
    }
    printf("--------------------------------------\n");

    printf("Masukkan ID Obat yang ingin diretur (dari list di atas): ");
    int idObatRetur;
    if (scanf("%d", &idObatRetur) != 1) { idObatRetur = -1; }
    while (getchar() != '\n'); // Membersihkan buffer

    int indexItemTrx = -1;
    for (int i = 0; i < riwayatTransaksi[indexTrx].jumlahItems; i++) {
        if (riwayatTransaksi[indexTrx].items[i].idObat == idObatRetur) {
            indexItemTrx = i;
            break;
        }
    }

    if (indexItemTrx == -1) {
        printf("Error: Obat dengan ID %d tidak dibeli pada transaksi ini.\n", idObatRetur);
        pause();
        return;
    }

    int jumlahRetur;
    int qtyDibeli = riwayatTransaksi[indexTrx].items[indexItemTrx].jumlah;
    printf("Jumlah %s yang ingin diretur (Maks: %d): ", riwayatTransaksi[indexTrx].items[indexItemTrx].namaObat, qtyDibeli);
    if (scanf("%d", &jumlahRetur) != 1) { jumlahRetur = 0; }
    while (getchar() != '\n'); // Membersihkan buffer

    if (jumlahRetur <= 0 || jumlahRetur > qtyDibeli) {
        printf("Error: Jumlah retur tidak valid.\n");
        pause();
        return;
    }

    int hargaSatuan = riwayatTransaksi[indexTrx].items[indexItemTrx].hargaSatuan;
    int totalRefund = hargaSatuan * jumlahRetur;

    printf("\nTotal refund untuk %d x %s adalah Rp %d\n", jumlahRetur, riwayatTransaksi[indexTrx].items[indexItemTrx].namaObat, totalRefund);

    if (konfirmasi("Proses retur dan kembalikan uang customer?")) {
        int indexObatMaster = findObatById(idObatRetur);
        if (indexObatMaster != -1) {
            daftarObat[indexObatMaster].stok += jumlahRetur;
        } 

        riwayatTransaksi[indexTrx].sudahDiretur = 1;

        printf("✅ Sukses! Retur diproses.\n");
        printf("Stok %s dikembalikan. Harap berikan refund Rp %d ke customer.\n",
               daftarObat[indexObatMaster].nama, totalRefund);
    } else {
        printf("Retur dibatalkan.\n");
    }
    pause();
}

// --- FUNGSI MENU & LOGIN ---

// Fungsi login() dengan kesempatan mengulang (Maks 3x)
int login() {
    const char hardcodeUser[] = "admin";
    const char hardcodePass[] = "123"; 
    char inputUser[50];
    char inputPass[50];
    int attempts = 0;
    const int max_attempts = 3;

    while (attempts < max_attempts) {
        clearScreen();
        printf("=========================================\n");
        printf("            LOGIN SISTEM APOTIK\n");
        printf("=========================================\n");
        printf("Percobaan ke-%d dari %d\n", attempts + 1, max_attempts);

        printf("Username: ");
        if (scanf("%49s", inputUser) != 1) {
            while (getchar() != '\n'); 
            attempts++;
            continue;
        }
        while (getchar() != '\n'); // Membersihkan buffer

        printf("Password: ");
        if (scanf("%49s", inputPass) != 1) {
            while (getchar() != '\n');
            attempts++;
            continue;
        }
        while (getchar() != '\n'); // Membersihkan buffer

        if (strcmp(inputUser, hardcodeUser) == 0 && strcmp(inputPass, hardcodePass) == 0) {
            printf("Login Berhasil! Selamat datang.\n");
            pause();
            return 1;
        } else {
            attempts++;
            printf("❌ Username atau Password salah! ");
            if (attempts < max_attempts) {
                printf("Anda memiliki %d percobaan tersisa.\n", max_attempts - attempts);
                pause(); 
            } else {
                printf("Semua percobaan gagal. Program akan keluar.\n");
            }
        }
    }
    return 0; // Login gagal
}


void menuManajemenObat() {
    int pilihan = 0;
    while (pilihan != 5) {
        clearScreen();
        printf("=========================================\n");
        printf("            MANAJEMEN OBAT (CRUD)\n");
        printf("=========================================\n");
        printf("1. Lihat Semua Obat (Read)\n");
        printf("2. Tambah Obat Baru (Create)\n");
        printf("3. Edit Obat (Update)\n");
        printf("4. Hapus Obat (Delete)\n");
        printf("5. Kembali\n");
        printf("=========================================\n");
        printf("Pilihan Anda: ");
        
        if (scanf("%d", &pilihan) != 1) { pilihan = 0; }
        while (getchar() != '\n'); // Membersihkan buffer

        switch (pilihan) {
            case 1: lihatListObat(); pause(); break;
            case 2: tambahObat(); break;
            case 3: editObat(); break;
            case 4: hapusObat(); break;
            case 5: printf("Kembali ke Menu Apotik...\n"); break;
            default: printf("Pilihan tidak valid!\n"); pause(); break;
        }
    }
}


void menuApotik() {
    int pilihan = 0;
    while (pilihan != 5) {
        clearScreen();
        printf("=========================================\n");
        printf("          SISTEM APOTIK SEDERHANA\n");
        printf("=========================================\n");
        printf("1. Manajemen Obat\n");
        printf("2. Transaksi Baru\n");
        printf("3. Riwayat Transaksi\n");
        printf("4. Return Produk\n");
        printf("5. Logout (Kembali ke Menu Utama)\n");
        printf("=========================================\n");
        printf("Pilihan Anda: ");
        
        if (scanf("%d", &pilihan) != 1) { pilihan = 0; }
        while (getchar() != '\n'); // Membersihkan buffer

        switch (pilihan) {
            case 1: menuManajemenObat(); break;
            case 2: transaksiBaru(); break;
            case 3: lihatRiwayatTransaksi(); break;
            case 4: returnProduk(); break;
            case 5: printf("Anda telah logout.\n"); pause(); break;
            default: printf("Pilihan tidak valid!\n"); pause(); break;
        }
    }
}

// FUNGSI UTAMA (MAIN)
int main() {
    initData(); 
    int pilihan = 0;

    // JALANKAN LOGIN TERLEBIH DAHULU
    clearScreen();
    printf("*****************************************\n");
    printf("* SELAMAT DATANG DI PROGRAM UTAMA       *\n");
    printf("*****************************************\n");
    printf("anda perlu login untuk masuk ke sistem apotik.\n");
    pause(); 

    // Panggil fungsi login(). Jika gagal (return 0), program langsung berhenti.
    if (!login()) { 
        return 1; 
    }
    
    while (pilihan != 4) {
        clearScreen();
        printf("*****************************************\n");
        printf("* MENU PROGRAM UTAMA                    *\n");
        printf("*****************************************\n");
        printf("1. Sistem Apotik\n"); 
        printf("2. Rumah Pola\n");
        printf("3. Bendera Negara\n");
        printf("4. Exit\n");
        printf("*****************************************\n");
        printf("Pilihan Anda: ");

        if (scanf("%d", &pilihan) != 1) {
            pilihan = 0; 
        }
        while (getchar() != '\n'); // Membersihkan buffer

        switch (pilihan) {
            case 1:
                menuApotik(); 
                break;
            case 2:
                rumahPola(); 
                break;
            case 3:
                benderaNegara(); 
                break;
            case 4:
                if (konfirmasi("Apakah Anda yakin ingin keluar dari program?")) {
                    printf("Program diakhiri.\n");
                } else {
                    pilihan = 0;
                }
                break;
            default:
                printf("Pilihan tidak valid.\n");
                pause();
                break;
        }
    }

    return 0;
}
