struct meas{
  double bocznikF = 0;
  double bocznikS = 0;

  double bocznik = 0;

  double zasilanie = 0;

  double ampers = 0;
  double wats = 0;
  
  double lastAvgBocznik = 0;
  double lastAvgZasilanie = 0;
  double lastAvgAmpers = 0;
  double lastAvgWats = 0;

  double avgBocznik = 0;
  double avgZasilanie = 0;
  double avgAmpers = 0;
  double avgWats = 0;
};
  unsigned long int zasiteration = 1;
  unsigned long int iteration = 1;
  int lastIteration = 1;

  String outStr = "";
  int sectionQuantity = 3;


void setup() {
  // put your setup code here, to run once:
  pinMode(22, OUTPUT);
  Serial.begin(9600);
  String info = "time iteration ";
  for (int section = 0; section < sectionQuantity; section++)
  {
    info = info + "bocznikF bocznikS bocznik lastAvgZasilanie lastAvgAmpers lastAvgWats  avgZasilanie avgAmpers avgWats ";
  }
  Serial.println(info);
}


  struct meas m[3];

void loop() {

  if(lastIteration <= 20)
  {
    digitalWrite(22,LOW);
    delay(200);
    for (int section = 0; section < sectionQuantity; section++)
    {
      m[section].zasilanie = float(analogRead(A2 + (section * sectionQuantity)) * 25) / 1024;
      m[section].lastAvgZasilanie = m[section].lastAvgZasilanie + (m[section].zasilanie - m[section].lastAvgZasilanie ) / float(lastIteration);
      m[section].avgZasilanie = m[section].avgZasilanie + (m[section].zasilanie - m[section].avgZasilanie ) / double(zasiteration);
    }
    lastIteration++;
    zasiteration++;
  }
  else
  {
    digitalWrite(22,HIGH);
    delay(200);
    for (int section = 0; section < sectionQuantity; section++)
    {
      m[section].bocznikF = double(analogRead(A1 + (section * sectionQuantity)) * 5) / 1024;
      m[section].bocznikS = double(analogRead(A0 + (section * sectionQuantity)) * 5) / 1024;



      m[section].bocznik = (m[section].bocznikF - m[section].bocznikS);
      
      m[section].ampers = m[section].bocznik / 0.22;
      m[section].wats = m[section].ampers * m[section].lastAvgZasilanie;


      m[section].lastAvgBocznik = m[section].lastAvgBocznik + (m[section].bocznik - m[section].lastAvgBocznik ) / float(lastIteration-20);
      m[section].lastAvgAmpers = m[section].lastAvgAmpers + (m[section].ampers - m[section].lastAvgAmpers ) / float(lastIteration-20);
      m[section].lastAvgWats = m[section].lastAvgWats + (m[section].wats - m[section].lastAvgWats ) / float(lastIteration-20);

      m[section].avgBocznik = m[section].avgBocznik + (m[section].bocznik - m[section].avgBocznik ) / float(iteration);
      m[section].avgAmpers = m[section].avgAmpers + (m[section].ampers - m[section].avgAmpers ) / double(iteration);
      m[section].avgWats = m[section].avgWats + (m[section].wats - m[section].avgWats ) / double(iteration);
    }
    lastIteration++;
    iteration++;
  }


  //if(lastIteration >= 60){
    outStr = String(float(millis() / float(1000))) + " " + String(iteration) + " ";

    for (int section = 0; section < sectionQuantity; section++)
    {
      outStr = outStr + String(m[section].bocznik,5) + " " + String(m[section].bocznikF,5) + " " + String(m[section].bocznikS,5) + " ";

      outStr = outStr + String(m[section].lastAvgZasilanie,5) + " " + String(m[section].avgZasilanie,5) + " ";
      outStr = outStr + String(m[section].lastAvgAmpers,5) + " " +  String(m[section].avgAmpers,5) + " ";
      outStr = outStr + String(m[section].lastAvgWats,5) + " " + String(m[section].avgWats,5) + "    ";
    }
    Serial.println(outStr);

    outStr = "";

      if(lastIteration >= 60){
    lastIteration = 1;
    }

  delay(200);
}






