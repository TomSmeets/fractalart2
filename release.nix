{ pkgs ? import <nixpkgs> {} }: with pkgs; stdenv.mkDerivation {
  name = "fractalart2-full";

  buildCommand = ''
    mkdir -p $out/bin
    cd $out/bin
    cp ${callPackage ./default.nix {}}/bin/*                    fractal-art-x11
    cp ${pkgsCross.mingw32.callPackage  ./default.nix {}}/bin/* fractal-art-w32.exe
    cp ${pkgsCross.mingwW64.callPackage ./default.nix {}}/bin/* fractal-art-w64.exe
  '';
}
