{ config, pkgs, ... }: 
let
  fa2 = pkgs.callPackage ./default.nix {};
in
{
  systemd.user.services.fractalart2 = {
    description = "Fractal Art Wallpaper";
    wantedBy = [ "graphical-session.target" ];
    partOf = [ "graphical-session.target" ];
    serviceConfig = {
      ExecStart = "${fa2}/bin/fractalart2";
      # RestartSec = 20;
      # Restart = "always";
    };
  };

  environment.systemPackages = [ fa2 ];
}
