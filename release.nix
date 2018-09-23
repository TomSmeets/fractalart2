{ pkgs ? import <nixpkgs> {} }: with pkgs; 
let
  base = callPackage ./default.nix {};
in
  stdenv.mkDerivation rec {
  name = "${base.name}-full";

  buildCommand = ''
    mkdir $out
    cd $out

    cp ${callPackage ./default.nix {}}/bin/*                    ./${base.name}-x11
    cp ${pkgsCross.mingw32.callPackage  ./default.nix {}}/bin/* ./${base.name}-w32.exe
    cp ${pkgsCross.mingwW64.callPackage ./default.nix {}}/bin/* ./${base.name}-w64.exe

    tar -czf "./${base.name}-src.tar.gz" ${./.}
  '';
}
