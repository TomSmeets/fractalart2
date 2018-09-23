{ pkgs ? import <nixpkgs> {} }: with pkgs;
let
  base = { input, output, ... }@args: stdenv.mkDerivation (rec {
    version = "1.0.1";
    name = "fractalart2-${version}";
    src = ./.;

    buildCommand = ''
      mkdir -p $out/bin
      echo cc=$CC

      cd $src
      $CC -O2 -Werror $CFLAGS -o "$out/bin/${output}" "${input}"
    '';
  } // args);

  x11 = base {
    buildInputs = with xorg; [ libX11 ];
    CFLAGS="-lX11";
    input = "main_x11.c";
    output = "fractalart2";
  };

  win32 = base {
    input = "main_win32.c";
    output = "fractalart.exe";
  };
in
  if stdenv.targetPlatform.isWindows then win32
  else x11
