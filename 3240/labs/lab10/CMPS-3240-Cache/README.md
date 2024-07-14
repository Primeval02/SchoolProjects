# CMPS-3240-Cache

Lab 10 - Caches

## Requirements

This lab *must* be completed on the department's `odin.cs.csubak.edu`. It is only guaranteed to work this way. If you try to get it to work on your own local machine (linux, mac, win) you are on your own.

## Objectives

* Implement code that reduces the effectiveness of the cache.
* Collect experimental results to investigate the performance of the cache.

## Required files

As a necessary first step, clone this repository:

```shell
$ https://github.com/DrAlbertCruz/CMPS-3240-Cache.git
...
$ cd CMPS-3240-Cache
```
To compile all of the programs for this lab, give the command:

```shell
$ make all
```

which should run without any errors or warnings.

# Approach

## Part 1 - Cache info

The `cache_info.out` program prints out information about the cache on the current system. On Linux systems, this information can also be found in the `/sys` subsystem in the directories:

```
/sys/devices/system/cpu/cpu*/cache/index*/
```

Run the code with the command:

```shell
$ ./cache_info.out
```

If it worked you should see something like the following:

```
Parameters for the L1 cache (Data):

        line_size=       64
             sets=       64
  pages per block=        1 (pagesize=     4096)
    associativity=        8-way set associative
       cache_size=       32K bytes total

Parameters for the L1 cache (Instruction):

        line_size=       64
             sets=       64
  pages per block=        1 (pagesize=     4096)
    associativity=        8-way set associative
       cache_size=       32K bytes total

Parameters for the L2 cache (Unified):

        line_size=       64
             sets=      512
  pages per block=        8 (pagesize=     4096)
    associativity=        8-way set associative
       cache_size=      256K bytes total

Parameters for the L3 cache (Unified):

        line_size=       64
             sets=    20480
  pages per block=      320 (pagesize=     4096)
    associativity=       20-way set associative
       cache_size=    25600K bytes total
``` 

and so on. Note these values because they will be important for the next part of the lab.

### Sidebar - sleipnir.cs.csubak.edu

This lab was designed for odin which has an L3 cache. If you're on your own device and it is old enough, it is possible that it does not have an L3 cache. In which case, you may get the following:

```shell
 *** fatal error: no such file: /sys/devices/system/cpu/cpu0/cache/index3/level...
 ```
 
There is no validation to prevent the outermost for loop in `cache_info.c` to stop. If you got this error then it is not really an error, you just don't have an L3 cache. If you want to see this for yourself, try to run this program on sleipnir. You will also get the following when attempting to run it:

```shell
Parameters for the L1 cache (Data):

        line_size=       64
             sets=      512
  pages per block=        8 (pagesize=     4096)
    associativity=        2-way set associative
       cache_size=       64K bytes total

Parameters for the L1 cache (Instruction):

        line_size=       64
             sets=      512
  pages per block=        8 (pagesize=     4096)
    associativity=        2-way set associative
       cache_size=       64K bytes total

Parameters for the L2 cache (Unified):

        line_size=       64
             sets=     1024
  pages per block=       16 (pagesize=     4096)
    associativity=       16-way set associative
       cache_size=     1024K bytes total

 *** fatal error: no such file: /sys/devices/system/cpu/cpu0/cache/index3/level
 ```
 
Other details to note are the differences between the older CPU in sleipnir vs. odin. Note that odin's cache is smaller and has less sets per line. However, odin has more 'ways' of associativity. For example. odin's L1 data cache is 8-way set associative, whereas sleipnir's is only 2-way. Also consider that odin has an L3 cache and sleipnir does not.

## Part 2 - Cache off

The `cache_off.out` program defeats the L2 cache on the server to give really poor memory bandwidth. Before you run this program, however, we need to modify it. It was originally designed for the department's obsolete server, sleipnir. Specifically, pay attention the readme starting on line 2. The data it gives here is for sleipnir, and you should use the new data from odin. For example, odin's L2 has a `line_size` of 64 and only 512 sets thus `block_size` should be reduced to 32768. Run the program with:

```shell
$ ./cache_off.out
```

Note the poor performance, as this might be useful when evaluating the next program, which is the bulk of this lab.

# Part 3 - Cache

`cache.out` program allocates a very large array and then accesses indexes using a skip amount specified on standard in to the program. For example, if you specify a skip amount of 127, it will access j, then j+127, and so on. For this portion of the lab, we will investigate the effects of giving different skip values (the line_offset variable in the program). To run this program, you will give the following shell command:

```shell
while true; do echo "127" | ./cache.out | grep Bandwidth; sleep 10; done
```
where 127 is the skip value. Let this loop run for about a dozen iterations before hitting CTRL-C, so that you can gather an observed average (removing any outliers that may have been caused by other running programs). The results of the last iteration will be stored in the logfile cache_<skipValue>.log.

Use the following skip values when running the program (note that these are pairs of similar numbers where the second number is a power of 2 and the first number is a prime close to the second number):

127 128 251 256 509 512 1021 1024 2039 2048 4093 4096

# Check Off

Create a table of the number of runs, and the maximum, minimum, average, and standard deviation of the memory bandwidth for each of the runs of the cache program that was requested above. You may want to present the results as a figure/plot instead. Note any trends between the pairs of values (e.g. between 127 and 128 or between 1021 and 1024) you saw when compiling this table. Be prepared to explain these trends. 

M. Thomas and M. Danforth, revised by A. Cruz
