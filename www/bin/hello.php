<?php
#!/usr/bin/env php
header("Content-Type: text/html");

echo "<html>\n";
echo "<head><title>CGI Test</title></head>\n";
echo "<body>\n";
echo "<h1>Hello from CGI!</h1>\n";
echo "<p>This is a PHP CGI script working correctly.</p>\n";

echo "<h2>For Loop Output:</h2>\n";
echo "<ul>\n";
for ($i = 1; $i <= 5000000; $i++) {
    echo "<li>Loop iteration: $i</li>\n";
}
echo "</ul>\n";

echo "<h2>Dynamic Content:</h2>\n";
echo "<p>Current timestamp: " . date('Y-m-d H:i:s') . "</p>\n";

echo "</body>\n";
echo "a7777\n";
echo "</html>\n";
?>
