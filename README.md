# System Status Monitor

This is a light weight application which monitors the system resources of a device and pushes an aggregated set of data to a desired URL.

## Prerequisites

This built using `gcc 9.4.0` and is compatible with`gnu17`, `gnu90` and `gnu89`, other versions  may work, but I've only tested with these versions.

cURL is required to build this application and built using the libcurl4 openssl libraries, others may work, but haven't been tested. 

The dependency can be installed on debian based systems using the command `apt install libcurl4-openssl-dev`

## Building

Building is done with a `Makefile` and to build you can simply run

```bash
make clean build
```

This will build a binary and place it in `./output/app`

## Running

Running the application requires you to provide two arguments; `URL` and `INTERVAL`.
 - `URL` is the URL which the application posts the data to,
 - `INTERVAL` is the amount of time in microseconds between data transmissions.

An example of this would be:

```bash
./output/app "http://localhost" 1000
```

## Data

The data is submitted to the specified URL in this format:

```bash
curl --request POST \
     --location 'http://localhost' \
     --header 'Content-Type: application/x-www-form-urlencoded' \
     --data-urlencode 'hostname=example' \
     --data-urlencode 'disk[/dev/sdc][used]=2661788' \
     --data-urlencode 'disk[/dev/sdc][total]=263112772' \
     --data-urlencode 'disk[/dev/sde][used]=673824' \
     --data-urlencode 'disk[/dev/sde][total]=1055762868' \
     --data-urlencode 'disk[/dev/sdd][used]=61548' \
     --data-urlencode 'disk[/dev/sdd][total]=1055762868' \
     --data-urlencode 'memory[free]=30560704' \
     --data-urlencode 'memory[available]=31046300' \
     --data-urlencode 'memory[total]=32824460' \
     --data-urlencode 'cpu[cpu0][active]=14772' \
     --data-urlencode 'cpu[cpu0][total]=2025819' \
     --data-urlencode 'cpu[cpu1][active]=6754' \
     --data-urlencode 'cpu[cpu1][total]=2025437' \
     --data-urlencode 'cpu[cpu2][active]=11358' \
     --data-urlencode 'cpu[cpu2][total]=2021858' \
     --data-urlencode 'cpu[cpu3][active]=4837' \
     --data-urlencode 'cpu[cpu3][total]=2022717' \
     --data-urlencode 'cpu[cpu4][active]=15491' \
     --data-urlencode 'cpu[cpu4][total]=2021816' \
     --data-urlencode 'cpu[cpu5][active]=3182' \
     --data-urlencode 'cpu[cpu5][total]=2023573' \
     --data-urlencode 'cpu[cpu6][active]=13346' \
     --data-urlencode 'cpu[cpu6][total]=2021750' \
     --data-urlencode 'cpu[cpu7][active]=3352' \
     --data-urlencode 'cpu[cpu7][total]=2023332' \
     --data-urlencode 'cpu[cpu8][active]=11106' \
     --data-urlencode 'cpu[cpu8][total]=2017973' \
     --data-urlencode 'cpu[cpu9][active]=3475' \
     --data-urlencode 'cpu[cpu9][total]=2023799' \
     --data-urlencode 'cpu[cpu10][active]=11437' \
     --data-urlencode 'cpu[cpu10][total]=2021324' \
     --data-urlencode 'cpu[cpu11][active]=3035' \
     --data-urlencode 'cpu[cpu11][total]=2023443'
```

## Known issues

- The disk stats only report on `ext4` filesystems.

## Support

Post it in the issues tab