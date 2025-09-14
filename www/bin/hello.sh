#!/usr/bin/env bash

printf "Content-Type: text/html\r\n\r\n"
cat <<EOF
<html>
<head><title>CGI Test</title></head>
<body>
<h1>Hello from CGI!</h1>
<p>This is a Bash CGI script working correctly.</p>
</body>
</html>
EOF