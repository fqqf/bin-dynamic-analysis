## watchstring
*Detect which part of code interacts with certain string*

### Usage:
```bash
$ ps -ef | grep optmgr
root      5605  3011  0 Oct17 ?        00:01:49 /usr/local/bin/optmgr

$ ./pin -pid 5605 -t watchstring.so -s "PAYLOADSTRING" -f "/tmp/output.log"

$ cat /tmp/output.log
Memory write containing 'PAYLOADSTRING' detected at IP: 0x7f32ff
Memory reаd containing 'PAYLOADSTRING' detected at IP: 0x7f7fab
```
## memgrep
*Find string in memory of all processes (fast-memgrep does the same, but with multithreading)*

### Usage:
```bash
$ memgrep PAYLOADSTRING
l2mgr:
snmpd:
mmmoed:
nginx 5855 contains string 'PAYLOADSTRING'
httpd 6174 contains string 'PAYLOADSTRING'
httpd 6176 contains string 'PAYLOADSTRING'
systemd:
```

## r3
*Get info about directory files' extensions*

### Usage:
```bash
$ r3 /etc/nginx
conf
html
num
pem
tmpl
types
```

## logme
*Intercepts calls to binary by warping it with logger*

### Usage:
```bash
$ logme /bin/rhttpd /var/log/rhttpd.log
Binary modified successfully. Restart the process to apply changes.
$ rhttpd 80 

$ cat /var/log/rhttpd.log
Thu Nov  9 05:19:05 PST 2023 : [bash] : /bin/rhttpd 80
Listening server at :80
```
