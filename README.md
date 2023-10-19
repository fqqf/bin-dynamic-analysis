## watchstring
*Find string in memory write. You can use it to determine which instruction interacts with your string*

### Usage:
```bash
$ ps -ef | grep optmgr
root      5605  3011  0 Oct17 ?        00:01:49 /usr/local/bin/optmgr

$ ./pin -pid 5605 -t watchstring.so -s "PAYLOADSTRING" -f "/tmp/output.log"

$ cat /tmp/output.log
Memory write containing 'PAYLOADSTRING' detected at IP: 0x7f32ff

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
*Warp binary with logger*

### Usage:
```bash
$ logme /bin/rhttpd /var/log/rhttpd.log
Binary modified successfully. Restart the process to apply changes.
$ rhttpd 80 

$ cat /var/log/rhttpd.log
Listening server at :80
```
