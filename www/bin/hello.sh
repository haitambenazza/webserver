#!/usr/bin/env bash

echo "Content-Type: text/html"
echo ""
cat <<EOF
<html>
<head><title>CGI Test</title></head>
<body>
<h1>Hello from CGI!</h1>
<p>This is a Bash CGI script working correctly.</p>
</body>
a7777
</html>
EOF
