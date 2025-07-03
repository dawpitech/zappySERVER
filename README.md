# Zappy server C++ implementation

This project is a server for the game Zappy.

> The minimal size of the map is 8x8 !

You can find below the necessary flag to launch it

```bash
$ ./zappy_server -p 1234 -x 8 -y 8 -n Fire Ice -c 8 -f 1
$ ./zappy_server -h
USAGE: ./zappy_server -p port -x width -y height -n name1 name2 ... -c clientsNb -f freq
port		is the port number
width		is the width of the world (min 8)
height		is the height of the world (min 8)
nameX		is the name of the team X
clientsNb	is the number of authorized clients per team (min 1)
freq		is the reciprocal of time unit for execution of actions (min 0.1)
```
