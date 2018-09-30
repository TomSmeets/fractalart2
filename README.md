# Desktop Art

Reimplementation of [FractalArt](https://github.com/TomSmeets/FractalArt) in C.
Note that the wallpapers generated are not exactly the same as the original.

## Tasks
* [x] Tune to look like original
* [ ] Live code editing (X11)
  * [x] load code using `dlopen`
  * [ ] reload on build
  * [ ] rebuild on edit?
* [ ] Option to change parameters (instead of static defines)
* [ ] Export to file
* [ ] Windows port (main_win32.c)
  * [x] Cross compile with nix
  * [x] Detect resolution
  * [ ] Change wallpaper
* [ ] Android port
* [ ] Publish package to nixpkgs
  * [ ] Create a pull request
* [ ] Publish module to nixpkgs
  * [ ] Create a pull request

Want to contribute?
Pick any item from the tasks above.

## Compiling

### Linux X11
```shell
  cc -o fractalart2 -O3 -lX11 ./main_x11.c
```

## Usage

### Windows 7
TODO

Put the executable inside `%APPDATA%\Microsoft\Windows\Start Menu\Programs\Startup`
This folder can be found by navigating to: `Start Menu` -> `All Programs`

### NixOS
Inlcude the predefined module into your system configration:

```nix
{
  modules = [
    ./fractalart2/configuration.nix
  ]
}
```
