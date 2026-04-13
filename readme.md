# Quick Send

## Quickly and easily transfer files

#### Usage:

##### Just compile the main.c file

```bash
gcc main.c -o quicksend
```

**server**

```bash
quicksend <port>
```

**client**

```bash
quicksend <server ip> <server port>
```

**Pipe in or out to save or write a file respectively**

Example simplex file transfer:

```bash
# receiver
quicksend 9000 > received.bin

# sender
quicksend 192.168.1.10 9000 < file.bin
```

###### Note to self *ctrl + D* in terminal signals EOF when using terminal as stdin

### Note: full file transfer is guaranteed in simplex mode. Duplex remains supported, but each side must handle EOF and shutdown carefully.

#### I used this project to practice my Tmux and Vim skills more then anything but I have found it very useful for transferring files between machines.
