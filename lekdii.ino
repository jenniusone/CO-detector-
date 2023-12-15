//ChatBot
#include "CTBot.h";
CTBot mybot;

//Set WiFi
String ssid = "lekdy";      //Ubah SSID WiFi
String pass = "teloletom";   //Ubah Passwod WiFi
String token = "5872708952:AAHk7WHs1QAOdT1ZqF9te7JPaYh4YNp7Ejs";
const int id = 1109292261;

//Deklarasi
int pinMQ = D1;                //Pin D1 (Bisa diubah)
int setMQ = 80;                //Atur nilai MQ kotor
int setwaktu = 100;       //Waktu inteval (dalam milisekon)
int analogMQ = 0;
int lock = 0;
bool state = false;
void setup() {
  //Serial
  Serial.begin(115200);
  Serial.println("Memulai telegram bot");

  //Konek ke Telegram
  mybot.wifiConnect(ssid, pass);
  mybot.setTelegramToken(token);
  if(mybot.testConnection()){
    Serial.println("Koneksi berhasil");
  } else {
    Serial.println("Koneksi Gagal ");
  }

  delay(2000);
}

void loop() {
  //Deklarasi
  TBMessage msg;
  
  if (mybot.getNewMessage(msg)) {
    Serial.println("Pesan Masuk ");
  if(msg.text.equalsIgnoreCase("/start")){
    state = true;
  }
  if(msg.text.equalsIgnoreCase("/stop")){
    state = false;
  }
    Serial.println(state);
  }
  if (state) {
  
  //Baca MQ
  analogMQ = analogRead(pinMQ);
  //Cek MQ sesuai set waktu
  if(lock == 0){
    //Print hasil
    Serial.print("Nilai MQ = ");
    Serial.println(analogMQ);
    
    //Jika analogMQ melebihi set
    if(analogMQ >= setMQ){
      //Print
      Serial.println("Toilet Bau! - Silahkan bersihkan toilet");
      Serial.println("");
      mybot.sendMessage(msg.sender.id, "[MQ Bot] Toilet Bau - Silahkan bersihkan toilet");
      lock = 1;
    } else {
      //Print
      Serial.print("Toilet Bersih! - Menunggu waktu interval terpenuhi");
      Serial.println("");
      mybot.sendMessage(msg.sender.id, "[MQ Bot] Toilet Bersih - Menunggu waktu interval terpenuhi");
      lock = 2;
    }
  }

  //Jika Toilet Bau
  if(lock == 1){
    //Print
    if(msg.text.equalsIgnoreCase("dibersihkan")){
      //Print
      Serial.println("Sedang dibersihkan. Menunggu intruksi sudah dibersihkan");
      Serial.println("");
      mybot.sendMessage(msg.sender.id, "[MQ Bot] Balas 'sudah' jika telah dibersihkan");
    } else if (msg.text.equalsIgnoreCase("sudah")){
      //Print
      Serial.println("Sudah dibersihkan. Melakukan sensing ulang");
      Serial.println("");
      mybot.sendMessage(msg.sender.id, "[MQ Bot] Melakukan sensing ulang");
      lock = 0;
    }
  }

  //Jika toilet bersih
  if(lock == 2){
    //Print
    delay(setwaktu);
   lock = 0;
  }
  }else{
  if (mybot.getNewMessage(msg)) {
    Serial.print("new message");
    Serial.println(msg.text);
  }
  }
}
