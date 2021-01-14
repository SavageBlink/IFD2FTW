#include <IRremote.h>
//#include <IRCodes.h>

int pinIR = 4;
IRrecv irrecv(pinIR);
decode_results results;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  irrecv.enableIRIn();
  Serial.println("Pret a demarrer"); 
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if(irrecv.decode() && ((log10(irrecv.results.value) > 7) && (log10(irrecv.results.value)) < 8.22)) //log10 permit us to limit the size of IRinputs
  {
    Serial.print("[");
    //irrecv.dumpPronto(&Serial);
    Serial.print(irrecv.results.value);
    Serial.println("|");
    Serial.print(log10(irrecv.results.value));
    //irrecv.printIRResultAsPronto(&Serial);
    Serial.println("]");
    //irrecv.printIRResultRawFormatted(&Serial);
    irrecv.resume();
  }
}