# System Status Monitor

This is a light weight application which monitors the system resources of a server and pushes an aggregated view to a UDP server.

## Prerequisites

This built using `gcc 9.4.0` and is compatible with`gnu17`, `gnu90` and `gnu89`, other versions  may work, but its only tested with these versions.

## Building

Building is done with a `Makefile` and to build you can simply run

```bash
make clean build
```

This will build a binary and place it in `./output/app`

## Running

Running the application requires you to provide three arguments; `HOST`, `PORT` and `INTERVAL`.
 - `HOST` is the host UDP server of the collector service recieving the packets,
 - `PORT` is the port of the UDP server collector service recieving the packets,
 - `INTERVAL` is the amount of time in seconds between data transmissions.

An example of this would be:

```bash
./output/app localhost 55554 1
```

## Data

Data is transmitted over UDP in the query string format e.g.

```
hostname=example&disk[/dev/sdc][used]=1746352&disk[/dev/sdc][total]=1055762868&memory[free]=7067948&memory[available]=7325056&memory[total]=8082584&cpu[cpu0][active]=14244&cpu[cpu0][total]=1796810&cpu[cpu1][active]=8758&cpu[cpu1][total]=1800125&cpu[cpu2][active]=12982&cpu[cpu2][total]=1794211&cpu[cpu3][active]=4006&cpu[cpu3][total]=1796249&cpu[cpu4][active]=10711&cpu[cpu4][total]=1793444&cpu[cpu5][active]=5642&cpu[cpu5][total]=1794146&cpu[cpu6][active]=11997&cpu[cpu6][total]=1794547&cpu[cpu7][active]=5055&cpu[cpu7][total]=1796055&cpu[cpu8][active]=11689&cpu[cpu8][total]=1794631&cpu[cpu9][active]=3959&cpu[cpu9][total]=1795607&cpu[cpu10][active]=12434&cpu[cpu10][total]=1789304&cpu[cpu11][active]=3722&cpu[cpu11][total]=1795655
```

## Example Server

Here's a crude example of UDP socket server written in PHP capable of receiving the packets.

```php
<?php

$port = 55554;
$host = '0.0.0.0';

$socket = socket_create(AF_INET, SOCK_DGRAM, SOL_UDP);

if (!$socket) {
     $socketError = socket_strerror(socket_last_error());
     throw new Exception('Socket failed to create due to: ' . $socketError);
}

socket_set_nonblock($socket);
socket_set_option($socket, SOL_SOCKET, SO_BROADCAST, true);
socket_set_option($socket, SOL_SOCKET, SO_REUSEADDR, true);

if (defined('SO_REUSEPORT')) {
    socket_set_option($socket, SOL_SOCKET, SO_REUSEPORT, true);
}

if(!socket_bind($socket, $host, $port)) {
     $socketError = socket_strerror(socket_last_error());
     throw new Exception('Socket failed to bind due to: ' . $socketError);
}

$read = [$socket]; 
$write = NULL; 
$except = NULL;

while(socket_select($read, $write, $except, NULL)) {
   while(is_string($data = socket_read($socket, 2048))) {
        parse_str($data, $output);
        var_dump($output);
    }
}

```

## Known issues

- The disk stats only report on `ext4` filesystems.

## Support

Post it in the issues tab