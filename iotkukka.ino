#include <EtherCard.h>

#define STATIC 0

static byte mymac[] = { 0x50,0x26,0x90,0x9a,0xd9,0xf9 };

int kosteus;

const char website[] PROGMEM = "kelokatu.hacked.jp";
//const char website[] PROGMEM = "random.ikioma.org";

uint32_t timer;
byte Ethernet::buffer[700];
Stash stash;
char buffer[25];

void setup ()
{
    Serial.begin(57600);
 
    if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) 
    Serial.println( "Failed to access Ethernet controller");
    
    if (!ether.dhcpSetup())
    Serial.println("DHCP failed");
    
    ether.printIp("IP:  ", ether.myip);
    ether.printIp("GW:  ", ether.gwip);  
    ether.printIp("DNS: ", ether.dnsip);  

    Serial.println("Please wait.  Resolving DNS...");
    if (!ether.dnsLookup(website))
    Serial.println("DNS failed");
    
    ether.printIp("DNS Resolved...Server IP: ", ether.hisip);
}

void loop () {
ether.packetLoop(ether.packetReceive());
  if (millis() > timer) 
  {
    timer = millis() + 10000;
    kosteus = analogRead(0);
    updateKosteus(kosteus);
    
    Serial.println(kosteus);
  }
}

void updateKosteus ( int kosteus )
{
 byte sd = stash.create();
 //stash.print("GET testti.php?");
 stash.print("GET /~nkuokka/hackday/firebaseTest.php?");
 //stash.print("GET /iotkukka/firebaseTest.php?");
        stash.print("humidity=");
        stash.print(kosteus);
        stash.print(" HTTP/1.0 \r\n");
 stash.println("\r\n");
 stash.save();
 Stash::prepare(PSTR("$H"), sd);
 ether.tcpSend();
}
