#!/usr/bin/env python3

print("Content-Type: text/html\r")
print("\r")
print("<html>")
print("<head><title>CGI Test</title></head>")
print("<body>")
print("<h1>Hello from CGI!</h1>")
print("<p>This is a Python CGI script working correctly.</p>")
print("</body>")
print("</html>")
for i in range(0, 500000):
    print('a7777', end="\n")
