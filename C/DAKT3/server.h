/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  
*********/

// Load Wi-Fi library
#include <WiFi.h>

static  double analogCurrent = 0, analogVoltage = 0, temp = 0, temp2 = 0;
static float iMax = 2;
static float uMax = 230;
static int digitalCurrent = 0, oldC = 0;
static int digitalVoltage = 0, digitalVoltage1 = 0, oldV = 0;
static int Dumax = 0;
static int Dimax = 0;
static int climbhill = 0;
int count2 = 0;
//static float osg_u[3]= {0,0,0};
//static float osg_qu[3]= {0,0,0};
//static float u[3]= {0,0,0}

// Replace with your network credentials
const char *ssid_2 = "hnb";
const char *password = "huy12345678";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output32State = "off";
String output27State = "off";

// Assign output variables to GPIO pins
const int output32 = 32;
const int output27 = 27;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 20;

void configServer()
{
  // Initialize the output variables as outputs
  pinMode(output32, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output32, LOW);

  // Connect to Wi-Fi network with ssid_2 and password
  WiFi.begin(ssid_2, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  server.begin();
}

void ComunicateToServer()
{
  WiFiClient client = server.available(); // Listen for incoming clients

  if (client)
  { // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    String currentLine = ""; // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime)
    { // loop while the client's connected
      currentTime = millis();
      if (client.available())
      {                         // if there's bytes to read from the client,
        char c = client.read(); // read a byte, then
        header += c;
        if (c == '\n')
        { // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0)
          {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // turns the GPIOs on and off
            if (header.indexOf("GET /26/on") >= 0)
            {
              output32State = "on";
              digitalWrite(output32, HIGH);
            }
            else if (header.indexOf("GET /26/off") >= 0)
            {
              output32State = "off";
              digitalWrite(output32, LOW);
            }
            else if (header.indexOf("GET /27/on") >= 0)
            {
              output27State = "on";
              digitalWrite(output27, HIGH);
            }
            else if (header.indexOf("GET /27/off") >= 0)
            {
              output27State = "off";
              digitalWrite(output27, LOW);
            }
            // Set iMax and uMax
            else if (header.indexOf("GET /imax/plus") >= 0)
            {
              iMax = iMax + 0.2;
              Serial.println("iMax = " + String(iMax));
            }
            else if (header.indexOf("GET /imax/minus") >= 0)
            {
              iMax = iMax - 0.2;
              Serial.println("iMax = " + String(iMax));
            }
            else if (header.indexOf("GET /umax/plus") >= 0)
            {
              uMax = uMax + 2;
              Serial.println("uMax = " + String(uMax));
            }
            else if (header.indexOf("GET /umax/minus") >= 0)
            {
              uMax = uMax - 2;
              Serial.println("uMax = " + String(uMax));
            }

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 10px 20px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}");
            client.println(".button3 {background-color: #006600; font-size: 20px; color: white}</style></head>");            

            // Web Page Heading
            client.println("<body><h2>ESP32 Web Server</h2>");
            client.println("<h3>I = " + String(analogCurrent)+" (A)</h3>");
            client.println("<h3>U = " + String(analogVoltage)+" (V)</h3>");
            // Display current state, and ON/OFF buttons for GPIO 26
            client.println("<p>GPIO Relay " + output32State + "</p>");
            // If the output32State is off, it displays the ON button
            if (output32State == "off")
            {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
            }
            else
            {
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.print("<p><a href=\"/imax/plus\"><button class=\"button3\">+</button></a></p>");
            client.print("<p> Imax = " + String(iMax) + " (A) </p>");
            client.println("<p><a href=\"/imax/minus\"><button class=\"button3\">-</button></a></p>");
            client.println("<br>");
            client.print("<p><a href=\"/umax/plus\"><button class=\"button3\">+</button></a></p>");
            client.print("<p> Umax = " + String(uMax) + " (V)</p>");
            client.println("<p><a href=\"/umax/minus\"><button class=\"button3\">-</button></a></p>");
            
            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          }
          else
          { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        }
        else if (c != '\r')
        {                   // if you got anything else but a carriage return character,
          currentLine += c; // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
  }
}
