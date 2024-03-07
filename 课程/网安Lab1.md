#### 1. http://10.15.111.100/game1
step 1. view page source

step 2. view page source

step 3. capture RESONSE-packet header using Burp Suite

#### 2. http://10.15.111.100/game2
step 1. view page source

step 2. understand 302 redirection

step 3. locate redirected pages and find password

step 4. understand HTTP Referer field

step 5. capture GET-packet and rewrite Referer field using Burp Suite

step 6. capture GET-packet and rewrite Cookie field with admin privilege using Burp Suite

#### 3. http://10.214.160.13:10000/
step 1. view page source

step 2. get link from .bak file

step 3. capture RESONSE-packet header with next link included using Burp Suite

step 4. view page source and try to click the button or craft packet with button click effect