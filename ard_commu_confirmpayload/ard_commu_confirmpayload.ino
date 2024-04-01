String str_rcv; // string receive
String str_buffer; // string buffer
void setup() {
  Serial.begin(115200);
  Serial.setTimeout(1);
  str_rcv = "";
  str_buffer = "";
}
void loop() {
  while (!Serial.available())
    ;
  // x = Serial.readString().toInt();
  str_rcv = Serial.readString();
  Serial.print("data rcv = ");
  Serial.print(str_rcv);
  Serial.print("---");
  if (str_rcv[str_rcv.length()-1] == 'z') { // string terminator at last position; check if end with z
    str_buffer += str_rcv;
    Serial.print(str_buffer);
    str_buffer = "";
  } else {
    str_buffer += str_rcv;
  }
}
