# reverse-tools

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
