#include <String>
//String website = "<html><body><h1>Hello from ESP8266!</h1></body></html>";

String website = "<html>\n<head>\n  <title>Radio Station Selection</title>\n</head>\n<body>\n  <h1>Radio Station Selection</h1>\n\n  <form>\n    <label for=\"station\">Select a radio station:</label>\n    <select id=\"station\" name=\"station\">\n      <option value=\"station1\">Radio Station 1</option>\n      <option value=\"station2\">Radio Station 2</option>\n      <option value=\"station3\">Radio Station 3</option>\n      <option value=\"station4\">Radio Station 4</option>\n    </select>\n    <br>\n    <input type=\"submit\" value=\"Select\">\n  </form>\n</body>\n</html>";