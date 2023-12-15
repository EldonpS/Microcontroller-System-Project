#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>

Servo servo; // mendeklrasi fungsi servo dengan nama servo

LiquidCrystal lcd(A0, A1, A2, A3, A4, A5); // rs en d4 d5 d6 mengatur pin yang digunakan untuk lcd/ mode 4 bit lcd untuk menghemat pin uno

#define panjang_pass 7 // mendefine panjang password + dengan null char

int pos = 0;    // variabel untuk posisi servo

char data[panjang_pass]; // membuat array char data sepanjang panjang_pass yang telah di define
char pass[panjang_pass] = "010203";     //password untuk program ini
int data_count = 0, pass_count = 0; // variabel counting
bool pass_benar; // variable untuk mengecek password
char input_keypad; // variabel input_keypad

int salah =0; // variabel untuk menyimpan berapa x kesalahan

const byte ROWS = 4; // membuat array untuk keypad
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
bool door = true; // untuk mengecek posisi slot pintu. 1/true untuk tertutup dan 0/false untuk terbuka

byte rowPins[ROWS] = {8, 7, 6, 5}; //untuk connect antar pin keypad dengan arduino
byte colPins[COLS] = {4, 3, 2}; 

Keypad keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS); //untuk membuat map keypad

void setup()
{
  servo.attach(9); // mengatur pin yang digunakan oleh servo
  ServoClose(); // memanggil fungsi servoclose untuk menutup servo, untuk jaga2 apabila servo keadaan awalnya hidup
  lcd.begin(16, 2); // menginisialisasi lcd dengan memanggil fungsi pada library lcd
  
}

void loop()
{
  if (door == 0)
  {
    input_keypad = keypad.getKey();

    if (input_keypad == '#')

    {
       
    lcd.clear();
      lcd.setCursor (3,0);
        lcd.print("LOCKING");
        delay (700);
      lcd.setCursor (10,0);
        lcd.print(".");
        delay (800);
      lcd.setCursor (11,0);  
        lcd.print(".");
        delay (800);
      lcd.setCursor (12,0);
        lcd.print(".");
        delay (500);
        lcd.clear();
        delay (500);
        ServoClose();
      lcd.setCursor (5,0);
      lcd.print("LOCKED");
      delay (1700);
      
      door = 1;
    }
  }

  else Open();
}

void clearData()
{
  while (data_count != 0)
  { // untuk panjang dari data
    data[data_count--] = 0; //untuk menghapus data pada array untuk membuat baru data
  }
  return;
}

void ServoOpen()
{
  for (pos = 180; pos >= 0; pos -= 5) { //posisi dari 180 sampe 0 derajat, setiap putaran sebanyak 1 derajat

    servo.write(pos);              // menulis servo agar ke posisi
    delay(15);                       // menunggu 15 ms sebelum ke posisi selanjutnya
  }
}

void ServoClose()
{
  for (pos = 0; pos <= 180; pos += 5) { //posisi dari 0 sampe 180 derajat, setiap putaran sebanyak 1 derajat
    servo.write(pos);              // menulis servo agar ke posisi
    delay(15);                       // menunggu 15 ms sebelum ke posisi selanjutnya
  }
}

void Open()
{
  lcd.setCursor(1, 0);
  lcd.print("INPUT PASSWORD");
  
  input_keypad = keypad.getKey();
  if (input_keypad) // untuk memastikan bahwa keypad di tekan
  {
    lcd.setCursor(0, 1); 
    lcd.print("******");     //untuk menyembunyikan password
    data[data_count] = input_keypad; // menyimpan input_keypad yang ditekan ke char data
    lcd.setCursor(data_count, 1); // memindahkan cursor lcd setiap data ditekan
    lcd.print(data[data_count]); //menampilkan data selama keypad ditekan
    data_count++; //mengincrement data array untuk menyimpan data selanjutnya

  }

  if (data_count == panjang_pass - 1) // untuk mengecek apakah data count sebanyak dengan panjang_pass-1
  {
    if (strcmp(data, pass) == 0) //membandingkan dua buah string, apabila password yg dimasukkan sama maka akan bernilai 0 lalu menuju ke dalam state if
    {
      lcd.clear();
      ServoOpen();
      lcd.print(" ACCESS GRANTED");
      lcd.setCursor(0,1);
      lcd.print("press # to lock");
      door = 0; // pintu terbuka
    }

    else
    {
      lcd.clear();

      lcd.setCursor(1,0); 
      lcd.print("ACCESS DENIED!");
      salah++;
      check();
      delay(1500);
      door = 1; //pintu tertutup
      
    }
    clearData();
  }

}

void check() // untuk mengecek banyak nya salah input password
{
  if(salah >= 3)
  {
    char timetxt[100];
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Password Salah");
    lcd.setCursor(0, 1);
    lcd.print("sebanyak 3x");
    delay(5000); 
    lcd.clear();
    lcd.setCursor(0, 0);
    
    for(int time = 30; time >0; time--){
        itoa(time, timetxt, 10);
        lcd.setCursor(0, 0);
        lcd.print("Tunggu");
        lcd.setCursor(7, 0);
        lcd.print(timetxt);
        lcd.setCursor(10,0);
        lcd.print("Detik");
        delay(1000);
        lcd.clear();

    }
    
    lcd.setCursor(0, 0);
    lcd.print("Silahkan Masukan");
    lcd.setCursor(0, 1);
    lcd.print("Password Lagi");
    delay(2000);
    lcd.clear();
  
    salah = 0;
    clearData();
    Open();
  }
}
