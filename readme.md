# Quick Send

## Quickly and easily transfer files    

#### Usage:

##### Just Compile the main.c file 

```bash
gcc main.c -o quicksend
```

**server** <br>


```bash
quicksend <port>
```

**client** <br>


```bash
quicksend <server ip> <server port>
```

**pipe in or out to save or write a file respectively**

### Note: File can only be guaranteed to be fully transferred when used in a simplex mode (duplex dose not guarantee full file transfer). Bi directional transfer was added for simplicity so the user can specify direction of data flow with pipes rather then with flags, client and server call the same functions to send and receive data.

#### I used this project to practice my Tmux and Vim skills more then anything but I have found it very useful for transferring files between machines.