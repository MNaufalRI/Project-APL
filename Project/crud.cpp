#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <vector> 
#include <limits> 
using std::vector;  
using namespace std;

// ===================== STRUCT ===================== 
// Struct untuk menyimpan data karakter
struct Karakter {
    string nama;
    int health;
    int defense;
    int attack;
    string gambar;
};

struct Enemy {
    string nama;
    int health;
    int defense;
    int attack;
    string gambar;
};

// Struct untuk menyimpan data user
struct User {
    string username;
    string password;
    bool is_admin;
};

// Const
const int maks_karakter = 10;
const int maks_musuh = 10;
const int maks_user = 10;
const int maks_attempt = 3;

// Global variables
Karakter karakter[maks_karakter];
Enemy musuh[maks_musuh];
User users[maks_user];
int jumlahKarakter = 0;
int jumlahMusuh = 0;
int jumlahUser = 0;
bool is_login = false;
bool is_admin = false;
string userSekarang;

void enter(bool tampilkan_enter = true) {
    if (tampilkan_enter) {
        cout << "\nTekan Enter untuk kembali ke menu...";
    }
    cin.ignore(1000, '\n');
    cin.get();
}

void bersihkanBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// ===================== CSV FUNCTION ===================== 
// Fungsi untuk menyimpan data ke CSV
void simpanKeCSV() {
    ofstream file("characters.csv");
    try {
        // Cek apakah file bisa dibuka
        if (!file.is_open()) {
            throw runtime_error("Gagal membuka file characters.csv untuk ditulis!");
        }

        // Header CSV
        file << "Nama,Attack,Health,Defense,Gambar\n";

        // Tulis data
        for (int i = 0; i < jumlahKarakter; i++) {
            file << karakter[i].nama << ","
                 << karakter[i].attack << ","
                 << karakter[i].health << ","
                 << karakter[i].defense << ","
                 << karakter[i].gambar << "\n";
        }

        // Cek apakah penulisan berhasil
        if (file.fail()) {
            throw runtime_error("Gagal menulis data ke file!");
        }

        cout << "Data karakter berhasil disimpan ke characters.csv\n";
    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
    }
    file.close(); 
}

void simpanMusuhKeCSV() {
    ofstream file("enemy.csv");
    try {
        if (!file.is_open()) {
            throw runtime_error("Gagal membuka file enemy.csv untuk ditulis!");
        }

        file << "Nama,Attack,Health,Defense,Gambar\n";
        
        for (int i = 0; i < jumlahMusuh; i++) {
            file << musuh[i].nama << ","
                 << musuh[i].attack << ","
                 << musuh[i].health << ","
                 << musuh[i].defense << ","
                 << musuh[i].gambar << "\n";

            if (file.fail()) {
                throw runtime_error("Gagal menulis data musuh ke file!");
            }
        }
        cout << "Data musuh berhasil disimpan ke enemy.csv (" 
             << jumlahMusuh << " data)\n";
    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
    }
    if (file.is_open()) {
        file.close();
    }
}

void muatKarakterDariCSV() {
    ifstream file("characters.csv");
    try {
        // Cek apakah file ada
        if (!file.is_open()) {
            throw runtime_error("File characters.csv tidak ditemukan!");
        }

        string line;
        getline(file, line); // Skip header

        // Reset jumlahKarakter sebelum memuat ulang
        jumlahKarakter = 0;

        while (getline(file, line)) {
            if (line.empty()) continue; // Skip baris kosong

            vector<string> row;
            stringstream ss(line);
            string cell;

            // Split baris CSV
            while (getline(ss, cell, ',')) {
                row.push_back(cell);
            }

            // Validasi format CSV (harus 5 kolom)
            if (row.size() != 5) {
                throw runtime_error("Format file CSV tidak valid (kolom tidak sesuai)!");
            }

            // Cek kapasitas array
            if (jumlahKarakter >= maks_karakter) {
                throw runtime_error("Kapasitas karakter penuh, tidak bisa memuat lebih banyak data.");
            }

            // Simpan data ke struct
            karakter[jumlahKarakter].nama = row[0];
            
            try {
                karakter[jumlahKarakter].attack = stoi(row[1]);
                karakter[jumlahKarakter].health = stoi(row[2]);
                karakter[jumlahKarakter].defense = stoi(row[3]);
            } catch (const invalid_argument& e) {
                throw runtime_error("Data attack/health/defense harus angka!");
            } catch (const out_of_range& e) {
                throw runtime_error("Data attack/health/defense terlalu besar!");
            }

            karakter[jumlahKarakter].gambar = row[4];
            jumlahKarakter++;
        }

        cout << "Data karakter berhasil dimuat (" << jumlahKarakter << " entri)\n";
    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
        jumlahKarakter = 0; // Reset data jika error
    }
    file.close();
}

void muatMusuhDariCSV() {
    ifstream file("enemy.csv");
    try {
        if (!file.is_open()) {
            throw runtime_error("File enemy.csv tidak ditemukan!");
        }

        string line;
        getline(file, line); // Skip header
        jumlahMusuh = 0;

        while (getline(file, line)) {
            if (line.empty()) continue;

            vector<string> row;
            stringstream ss(line);
            string cell;

            while (getline(ss, cell, ',')) {
                row.push_back(cell);
            }

            if (row.size() != 5) {
                throw runtime_error("Format file enemy.csv tidak valid!");
            }

            if (jumlahMusuh >= maks_musuh) {
                throw runtime_error("Kapasitas musuh penuh!");
            }

            musuh[jumlahMusuh].nama = row[0];
            
            try {
                musuh[jumlahMusuh].attack = stoi(row[1]);
                musuh[jumlahMusuh].health = stoi(row[2]);
                musuh[jumlahMusuh].defense = stoi(row[3]);
            } catch (...) {
                throw runtime_error("Data stat musuh harus angka!");
            }

            musuh[jumlahMusuh].gambar = row[4];
            jumlahMusuh++;
        }

        cout << "Data musuh berhasil dimuat (" << jumlahMusuh << " entri)\n";
    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
        jumlahMusuh = 0;
    }
    file.close();
}

// ===================== MUSUH FUNCTIONS ===================== 
void tambahMusuh() {
    try {
        if (jumlahMusuh >= maks_musuh) {
            throw runtime_error("Data musuh penuh! Maksimal: " + to_string(maks_musuh));
        }

        Enemy newEnemy;
        cout << "Nama Musuh: ";
        cin >> newEnemy.nama;

        // Cek duplikasi nama
        for (int i = 0; i < jumlahMusuh; i++) {
            if (musuh[i].nama == newEnemy.nama) {
                throw runtime_error("Nama musuh sudah digunakan!");
            }
        }

        cout << "Attack: ";
        if (!(cin >> newEnemy.attack) || newEnemy.attack < 0) {
            throw runtime_error("Attack harus angka >= 0!");
        }

        // Validasi health dan defense
        cout << "Health: ";
        if (!(cin >> newEnemy.health) || newEnemy.health < 0) {
            throw runtime_error("Health harus angka >= 0!");
        }

        cout << "Defense: ";
        if (!(cin >> newEnemy.defense) || newEnemy.defense < 0) {
            throw runtime_error("Defense harus angka >= 0!");
        }

        cout << "Link Gambar: ";
        cin >> newEnemy.gambar;
        bersihkanBuffer();

        musuh[jumlahMusuh++] = newEnemy;
        simpanMusuhKeCSV();
        cout << "Musuh berhasil ditambahkan!\n";

    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

void tampilkanMusuh(){
    cout << "\n==================== Daftar Musuh ====================\n"<< endl;
        cout << left << setw(5) << "No" 
             << setw(15) << "Nama"
             << setw(10) << "Attack"
             << setw(10) << "Health"
             << setw(10) << "Defense" << endl;
        cout << "------------------------------------------------------------\n";

        for (int i = 0; i < jumlahMusuh; i++) {
            cout << left << setw(5) << i+1
                 << setw(15) << musuh[i].nama
                 << setw(10) << musuh[i].attack 
                 << setw(10) << musuh[i].health
                 << setw(10) << musuh[i].defense << endl;
        }
        cout << "------------------------------------------------------------\n";
}

void ubahMusuh(int startIndex = 0) {
    try {
        if (jumlahMusuh == 0) {
            cout << "Tidak ada karakter yang tersimpan.\n";
            enter();
            return;
        } 

        // Tampilkan daftar karakter
        tampilkanMusuh();

        int index;
        cout << "Masukkan nomor musuh yang ingin diubah: ";
        if (!(cin >> index) || index < startIndex + 1 || index > jumlahMusuh) {
            throw runtime_error("Nomor tidak valid!");
        }
        index--;

        // Input data baru
        string namaBaru;
        cout << "Nama Baru: ";
        cin >> namaBaru;

        // Cek duplikasi nama (kecuali untuk dirinya sendiri)
        for (int i = 0; i < jumlahMusuh; i++) {
            if (i != index && musuh[i].nama == namaBaru) {
                throw runtime_error("Nama sudah digunakan oleh musuh lain!");
            }
        }

        musuh[index].nama = namaBaru;

        cout << "Attack Baru: ";
        if (!(cin >> musuh[index].attack) || musuh[index].attack < 0) {
            throw runtime_error("Attack harus angka >= 0!");
        }

        // Validasi health dan defense
        cout << "Health Baru: ";
        if (!(cin >> musuh[index].health) || musuh[index].health < 0) {
            throw runtime_error("Health harus angka >= 0!");
        }

        cout << "Defense Baru: ";
        if (!(cin >> musuh[index].defense) || musuh[index].defense < 0) {
            throw runtime_error("Defense harus angka >= 0!");
        }

        cout << "Link Gambar Baru: ";
        cin >> musuh[index].gambar;

        simpanMusuhKeCSV();
        cout << "Musuh berhasil diubah!\n";

        bersihkanBuffer();

    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

void hapusMusuh(bool confirm = false, int index = -1) {
    if (jumlahMusuh == 0) {
        cout << "Tidak ada musuh yang tersimpan.\n";
        enter();
        return;
    } 
    
    try {
        if (!confirm) {
            tampilkanMusuh();
            
            cout << "Masukkan nomor musuh yang ingin dihapus: ";
            if (!(cin >> index) || index < 1 || index > jumlahMusuh) {
                throw runtime_error("Nomor tidak valid!");
            }
            index--;

            cout << "Apakah Anda yakin ingin menghapus " << musuh[index].nama << "? (y/n): ";
            char choice;
            cin >> choice;
            bersihkanBuffer();

            if (choice == 'y' || choice == 'Y') {
                hapusMusuh(true, index);
            } else {
                cout << "Penghapusan dibatalkan.\n";
                return;
            }
        } else {
            // Proses penghapusan
            for (int i = index; i < jumlahMusuh - 1; i++) {
                musuh[i] = musuh[i + 1];
            }
            jumlahMusuh--;
            simpanMusuhKeCSV();
            cout << "Musuh berhasil dihapus!\n";
        }
    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

// ===================== IMPLEMENTASI SORT DAN POINTER ===================== 
// Fungsi dengan parameter dereference
void printKarakterDetail(const Karakter *karakterPtr) {
    cout << "\nDetail Karakter:" << endl;
    cout << "Nama: " << karakterPtr->nama << endl;
    cout << "Attack: " << karakterPtr->attack << endl;
    cout << "Health: " << karakterPtr->health << endl;
    cout << "Defense: " << karakterPtr->defense << endl;
    cout << "Gambar: " << karakterPtr->gambar << endl;
}

// Fungsi dengan parameter address-of
void updateHealth(Karakter &karakterRef, int newHealth) {
    try {
        if (newHealth < 0) throw runtime_error("Health tidak boleh negatif!");
        karakterRef.health = newHealth;
    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
    }
}

// Fungsi untuk menyalin array karakter
void salinKarakter(Karakter dest[], const Karakter src[], int jumlah) {
    for (int i = 0; i < jumlah; i++) {
        dest[i] = src[i];
    }
}

// 1. Bubble Sort untuk Nama (Ascending)
void bubbleSortKarakter(Karakter karakter[], int jumlah) {
    try {
        if (jumlah <= 0) throw runtime_error("Data karakter kosong!");
        
        for (int i = 0; i < jumlah-1; i++) {
            for (int j = 0; j < jumlah-i-1; j++) {
                if (karakter[j].nama > karakter[j+1].nama) {
                    swap(karakter[j], karakter[j+1]);
                }
            }
        }
    } catch (const exception& e) {
        cerr << "[ERROR] Sorting gagal: " << e.what() << endl;
    }
}

// 2. Merge Sort untuk Attack (Descending)
void merge(Karakter karakter[], int kiri, int tengah, int kanan) {
    try {
        // Validasi parameter
        if (kiri < 0 || tengah < kiri || kanan < tengah) {
            throw runtime_error("Indeks tidak valid (kiri=" + to_string(kiri) + 
                              ", tengah=" + to_string(tengah) + 
                              ", kanan=" + to_string(kanan) + ")");
        }

        int ukuranKiri = tengah - kiri + 1;
        int ukuranKanan = kanan - tengah;

        // Validasi ukuran array
        if (ukuranKiri <= 0 || ukuranKanan <= 0) {
            throw runtime_error("Ukuran sub-array tidak valid!");
        }

        Karakter arrayKiri[ukuranKiri], arrayKanan[ukuranKanan];

        // Salin data ke sub-array kiri
        for (int i = 0; i < ukuranKiri; i++) {
            arrayKiri[i] = karakter[kiri + i];
        }

        // Salin data ke sub-array kanan
        for (int j = 0; j < ukuranKanan; j++) {
            arrayKanan[j] = karakter[tengah + 1 + j];
        }

        // Proses merge
        int i = 0, j = 0, k = kiri;
        while (i < ukuranKiri && j < ukuranKanan) {
            if (arrayKiri[i].attack >= arrayKanan[j].attack) {
                karakter[k] = arrayKiri[i];
                i++;
            } else {
                karakter[k] = arrayKanan[j];
                j++;
            }
            k++;
        }

        // Salin sisa elemen jika ada
        while (i < ukuranKiri) {
            karakter[k] = arrayKiri[i];
            i++;
            k++;
        }

        while (j < ukuranKanan) {
            karakter[k] = arrayKanan[j];
            j++;
            k++;
        }

    } catch (const exception& e) {
        cerr << "[ERROR] Merge gagal: " << e.what() << endl;
    }
}

void mergeSortKarakter(Karakter karakter[], int kiri, int kanan) {
    try {
        // Validasi parameter
        if (kiri < 0 || kanan < 0 || kiri >= kanan) {
            throw runtime_error("Indeks tidak valid (kiri=" + to_string(kiri) + 
                              ", kanan=" + to_string(kanan) + ")");
        }

        if (jumlahKarakter == 0) {
            throw runtime_error("Array karakter kosong!");
        }

        // Proses rekursif
        if (kiri < kanan) {
            int tengah = kiri + (kanan - kiri) / 2;
            mergeSortKarakter(karakter, kiri, tengah);
            mergeSortKarakter(karakter, tengah + 1, kanan);
            merge(karakter, kiri, tengah, kanan);
        }

    } catch (const exception& e) {
        cerr << "[ERROR] MergeSort gagal: " << e.what() << endl;
    }
}


// ===================== MAIN MENU FUNCTION ===================== 
void login(int maks_percobaan = maks_attempt) {
    int attempt = 0;
    while (attempt < maks_percobaan) {
        try {
            string username, password;
            cout << "\n=== LOGIN ===" << endl;
            cout << "Percobaan ke-" << attempt + 1 << " dari " << maks_percobaan << endl;
            
            cout << "Username: ";
            if (!(cin >> username)) throw runtime_error("Input username tidak valid!");
            
            cout << "Password: ";
            if (!(cin >> password)) throw runtime_error("Input password tidak valid!");

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            bool found = false;
            for (int i = 0; i < jumlahUser; i++) {
                if (users[i].username == username && users[i].password == password) {
                    found = true;
                    is_login = true;
                    userSekarang = username;
                    is_admin = users[i].is_admin;
                    cout << "Login berhasil!\n";
                    return;
                }
            }
            if (!found) throw runtime_error("Username/password salah!");
            
        } catch (const exception& e) {
            cerr << "[ERROR] " << e.what() << endl;
            if (++attempt >= maks_percobaan) {
                cerr << "Anda telah gagal login " << maks_percobaan << " kali. Program berhenti.\n";
                exit(1);
            }
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
}


void registrasi(int maxUsers = maks_user) {
    try {
        if (jumlahUser >= maxUsers) {
            throw runtime_error("Kapasitas user penuh! Tidak bisa mendaftar.");
        }

        string username, password;
        cout << "\n=== REGISTER ===" << endl;
        cout << "Username: ";
        if (!(cin >> username) || username.empty()) {
            throw runtime_error("Username tidak boleh kosong!");
        }

        for (int i = 0; i < jumlahUser; i++) {
            if (users[i].username == username) {
                throw runtime_error("Username sudah digunakan!");
            }
        }

        cout << "Password: ";
        if (!(cin >> password) || password.empty()) {
            throw runtime_error("Password tidak boleh kosong!");
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        users[jumlahUser].username = username;
        users[jumlahUser].password = password;
        users[jumlahUser].is_admin = false; // Default: user biasa
        jumlahUser++;

        cout << "Registrasi berhasil! Silakan login.\n";

    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
        cin.clear(); 
        cin.ignore(1000, '\n'); 
    }
}

// ===================== CHARACTER FUNCTION ===================== 
void tambahKarakter(int maxCharacters = maks_karakter) {
    try {
        if (jumlahKarakter >= maxCharacters) {
            throw runtime_error("Data karakter penuh! Maksimal: " + to_string(maxCharacters));
        }

        Karakter newKarakter;
        cout << "Nama Karakter: ";
        cin >> newKarakter.nama;

        // Cek duplikasi nama
        for (int i = 0; i < jumlahKarakter; i++) {
            if (karakter[i].nama == newKarakter.nama) {
                throw runtime_error("Nama karakter sudah digunakan!");
            }
        }

        cout << "Attack: ";
        if (!(cin >> newKarakter.attack) || newKarakter.attack < 0) {
            throw runtime_error("Attack harus angka >= 0!");
        }

        cout << "Health: ";
        if (!(cin >> newKarakter.health) || newKarakter.health < 0) {
            throw runtime_error("Health harus angka >= 0!");
        }

        cout << "Defense: ";
        if (!(cin >> newKarakter.defense) || newKarakter.defense < 0) {
            throw runtime_error("Defense harus angka >= 0!");
        }

        cout << "Link Gambar: ";
        cin >> newKarakter.gambar;
        bersihkanBuffer();

        karakter[jumlahKarakter++] = newKarakter;
        simpanKeCSV();
        cout << "Karakter berhasil ditambahkan!\n";

    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

void tampilkanKarakterCRUD(){
    cout << "\n==================== Daftar Karakter ====================\n"<< endl;
        cout << left << setw(5) << "No" 
             << setw(15) << "Nama"
             << setw(10) << "Attack"
             << setw(10) << "Health"
             << setw(10) << "Defense" << endl;
        cout << "------------------------------------------------------------\n";

        for (int i = 0; i < jumlahKarakter; i++) {
            cout << left << setw(5) << i+1
                 << setw(15) << karakter[i].nama
                 << setw(10) << karakter[i].attack 
                 << setw(10) << karakter[i].health
                 << setw(10) << karakter[i].defense << endl;
        }
        cout << "------------------------------------------------------------\n";
}

void tampilkanKarakter(bool detailed = false) {
    if (jumlahKarakter == 0) {
        cout << endl << "Tidak ada karakter yang tersimpan.\n";
        enter();
        return;
    }

    if (!detailed) {
        tampilkanKarakterCRUD();
    } else {
        cout << "\n==================== Daftar Karakter (Detailed) ====================\n"<< endl;
        cout << left << setw(5) << "No" 
             << setw(20) << "Nama"
             << setw(15) << "Attack"
             << setw(15) << "Health"
             << setw(15) << "Defense"
             << setw(25) << "Gambar" << endl;
        cout << "----------------------------------------------------------------------------\n";

        for (int i = 0; i < jumlahKarakter; i++) {
            cout << left << setw(5) << i+1
                 << setw(20) << karakter[i].nama
                 << setw(15) << karakter[i].attack 
                 << setw(15) << karakter[i].health
                 << setw(15) << karakter[i].defense
                 << setw(25) << karakter[i].gambar << endl;
        }
        cout << "----------------------------------------------------------------------------\n";
    }

    // sort menu 
    int pilihan;
    do {
        cout << "\n=== SORT MENU ===" << endl;
        cout << "1. Urutkan berdasarkan Nama (Ascending)" << endl;
        cout << "2. Urutkan berdasarkan Attack (Descending)" << endl;
        cout << "3. Kembali ke Menu Utama" << endl;
        cout << "Pilihan: ";
        cin >> pilihan;
        bersihkanBuffer();

        if (pilihan >= 1 && pilihan <= 2) {
            Karakter sortedKarakter[maks_karakter];
            salinKarakter(sortedKarakter, karakter, jumlahKarakter);
            
            if (pilihan == 1) {
                bubbleSortKarakter(sortedKarakter, jumlahKarakter);
                cout << "\nKarakter setelah diurutkan (Nama Ascending):\n";
            } else if (pilihan == 2) {
                mergeSortKarakter(sortedKarakter, 0, jumlahKarakter-1);
                cout << "\nKarakter setelah diurutkan (Attack Descending):\n";
            }
            
            cout << "\n==================== Daftar Karakter (Sorted) ====================\n"<< endl;
            cout << left << setw(5) << "No" 
                 << setw(15) << "Nama"
                 << setw(10) << "Attack"
                 << setw(10) << "Health"
                 << setw(10) << "Defense" << endl;
            cout << "------------------------------------------------------------\n";

            for (int i = 0; i < jumlahKarakter; i++) {
                cout << left << setw(5) << i+1
                     << setw(15) << sortedKarakter[i].nama
                     << setw(10) << sortedKarakter[i].attack 
                     << setw(10) << sortedKarakter[i].health
                     << setw(10) << sortedKarakter[i].defense << endl;
            }
            cout << "------------------------------------------------------------\n";
        } else if (pilihan != 3) {
            cout << "Pilihan tidak valid!" << endl;
        }
    } while (pilihan != 3);
}

void ubahKarakter(int startIndex = 0) {
    try {
        if (jumlahKarakter == 0) {
            cout << "Tidak ada karakter yang tersimpan.\n";
            enter();
            return;
        } 

        // Tampilkan daftar karakter
        tampilkanKarakterCRUD();

        int index;
        cout << "Masukkan nomor karakter yang ingin diubah: ";
        if (!(cin >> index) || index < startIndex + 1 || index > jumlahKarakter) {
            throw runtime_error("Nomor tidak valid! ");
        }
        index--;

        // Input data baru
        string namaBaru;
        cout << "Nama Baru: ";
        cin >> namaBaru;

        // Cek duplikasi nama (kecuali untuk dirinya sendiri)
        for (int i = 0; i < jumlahKarakter; i++) {
            if (i != index && karakter[i].nama == namaBaru) {
                throw runtime_error("Nama sudah digunakan oleh karakter lain!");
            }
        }

        karakter[index].nama = namaBaru;

        cout << "Attack Baru: ";
        if (!(cin >> karakter[index].attack) || karakter[index].attack < 0) {
            throw runtime_error("Attack harus angka >= 0!");
        }

        // Validasi health dan defense
        cout << "Health Baru: ";
        if (!(cin >> karakter[index].health) || karakter[index].health < 0) {
            throw runtime_error("Health harus angka >= 0!");
        }

        cout << "Defense Baru: ";
        if (!(cin >> karakter[index].defense) || karakter[index].defense < 0) {
            throw runtime_error("Defense harus angka >= 0!");
        }

        cout << "Link Gambar Baru: ";
        cin >> karakter[index].gambar;
        bersihkanBuffer();

        simpanKeCSV();
        cout << "Karakter berhasil diubah!\n";

    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

void hapusKarakter(bool confirm = false, int index = -1) {
    if (jumlahKarakter == 0) {
        cout << "Tidak ada karakter yang tersimpan.\n";
        enter();
        return;
    } 
    
    try {
        if (!confirm) {
            tampilkanKarakterCRUD();
            
            cout << "Masukkan nomor karakter yang ingin dihapus: ";
            if (!(cin >> index) || index < 1 || index > jumlahKarakter) {
                throw runtime_error("Nomor tidak valid!");
            }
            index--;

            cout << "Apakah Anda yakin ingin menghapus " << karakter[index].nama << "? (y/n): ";
            char choice;
            cin >> choice;
            bersihkanBuffer();

            if (choice == 'y' || choice == 'Y') {
                hapusKarakter(true, index);
            } else {
                cout << "Penghapusan dibatalkan.\n";
                return;
            }
        } else {
            // Proses penghapusan
            for (int i = index; i < jumlahKarakter - 1; i++) {
                karakter[i] = karakter[i + 1];
            }
            jumlahKarakter--;
            simpanKeCSV();
            cout << "Karakter berhasil dihapus!\n";
        }
    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

// ===================== ERROR HANDLING ===================== 

bool ScanInput(const string& input, int& output) {
    stringstream ss(input);
    ss >> output;

    // Cek apakah input valid (ada angka dan tidak gagal)
    if (ss.fail()) {
        return false;
    }

    return true;
}

// ===================== SUB MENU FUNCTION ===================== 

void adminMenu(bool adminsmenu = true) {
    int pilihan;
    string input;
    while (true) { 
        cout << "\n=== MENU ADMIN (" << userSekarang << ") ===" << endl;
        cout << "1. Tambah Karakter\n";
        cout << "2. Tambah Musuh\n";
        cout << "3. Tampilkan Karakter\n";
        cout << "4. Tampilkan Musuh\n";
        cout << "5. Ubah Karakter\n";
        cout << "6. Hapus Karakter\n";
        cout << "7. Ubah Musuh\n";
        cout << "8. Hapus Musuh\n";
        cout << "9. Logout\n" << endl;
        cout << "Pilih menu: ";
        getline(cin, input);
        // cin >> pilihan;

        if (!ScanInput(input, pilihan)) {
            cout << "Input tidak valid! Masukkan angka." << endl;
            continue;
        }
        
        if (pilihan == 9) {
            is_login = false;
            is_admin = false;
            cout << "Logout berhasil.\n";
            break;
        }
        
        switch (pilihan) {
            case 1: tambahKarakter(); break;
            case 2: tambahMusuh(); break;
            case 3: tampilkanKarakter(); break;
            case 4: tampilkanMusuh(); break;
            case 5: ubahKarakter(); break;
            case 6: hapusKarakter(); break;
            case 7: ubahMusuh(); break;
            case 8: hapusMusuh(); break;
            default:
                cout << "Pilihan tidak valid!\n";
        }
    }
}

void userMenu(bool adminsmenu = true) {
    int pilihan;
    string input;
    if (adminsmenu) {
        cout << "\n=== MENU USER (" << userSekarang << ") ===" << endl;
    }
    cout << "1. Lihat Karakter" << endl;
    cout << "2. Lihat Karakter (Detailed)" << endl;
    cout << "3. Logout" << endl;
    cout << "Pilihan: ";
    getline(cin, input);
    // cin >> pilihan;

    if (!ScanInput(input, pilihan)) {
        cout << "Input tidak valid! Masukkan angka." << endl;
        return;
    }

    switch (pilihan) {
        case 1: 
            tampilkanKarakter();
            break;
        case 2:
            tampilkanKarakter(true);
            break;
        case 3:
            is_login = false;
            cout << "Logout berhasil." << endl;
            break;
        default:
            cout << "Pilihan tidak valid!" << endl;
    }
}

// ===================== MAIN PROGRAM ===================== 
int main(int argc, char* argv[]) {
    muatKarakterDariCSV();
    muatMusuhDariCSV();
    users[jumlahUser++] = {"rizal", "039", true};  // User admin default

    int menu_utama;
    string input;

    while (true) {
        if (!is_login) {
            cout << "\n=== MENU UTAMA ===" << endl;
            cout << "1. Login" << endl;
            cout << "2. Register" << endl;
            cout << "3. Keluar" << endl;
            cout << "Pilihan: ";
            getline(cin, input);
            // cin >> menu_utama;

            if (!ScanInput(input, menu_utama)) {
                cout << "Input tidak valid! Masukkan angka." << endl;
                continue;
            }

            switch (menu_utama) {
                case 1: login(); break;
                case 2: registrasi(); break;
                case 3: 
                    cout << "Keluar dari program." << endl;
                    return 0;
                default:
                    cout << "Pilihan tidak valid!" << endl;
            }
        }
        else {
            if (is_admin) {
                adminMenu();
            }
            else {
                userMenu();
            }
        }
    }

    return 0;
}