## memgrep
*Tries to find string grepping memory of all processes (memgrepv does the same, but with progress bar)*

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
*Show all file extensions recursively*

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
