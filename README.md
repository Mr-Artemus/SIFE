<h3 align="center" style="font-size: 250%">S.I.F.E</h3>

<p align="center" style="font-weight: bold">
	Simple and <s>Inefficient</s> File Eraser
</p>


## About the project

**S.I.F.E.** is a simple tool writen in C to erase files/directories with some methods like **AFSSI-5020** or **HMG IS5 Enhanced**. I built it for educational purpose only, so if you try to delete something from your computer before your **intelligence agency** come at your door and find something, it's not my fault 🙃



## Getting Started

#### Requirements

* CMake ( >= 3.11 )
* GCC
* A Linux system



#### Installation

```
git clone https://github.com/Mr-Artemus/SIFE.git
mkdir build && cd $_
cmake ..
make
./sife --help
```



#### Usage

````
❯ ./sife --help
S.I.F.E. version 0.1.0

USAGE: ./sife [options] file...

OPTIONS:
-h | --help         Show the help message
-m | --method       Select wiping method
--verbose           Active verbose mode
--keep              Keep file(s) after overwriting
````

```
List of all usable methods :
 1 - Simple pass of 0b00000000
 2 - Simple pass of 0b11111111
 3 - Simple pass of 0b01010101
 4 - Simple pass of pseudo-random byte
 5 - GOST-R-50739-95 first version
 6 - GOST-R-50739-95 second version
 7 - HMG IS5
 8 - HMG IS5 Enhanced
 9 - RCMP TSSIT OPS-II
10 - AFSSI-5020
11 - AR 380-19
12 - Bruce Schneier Algorithm
```



#### Examples

* `./sife --method 10 --keep backup.tar.bz2 `

- `./sife --method 6 /mnt/someMountedDisk`



## Roadmap

- [x] A working base 😎
- [x] Directories support
- [ ] Disk/Partition support
- [ ] Some optimization
- [ ] Some unit tests




