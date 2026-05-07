{
  pkgs ? import <nixpkgs> { },
  deps ? [ ],
}:
pkgs.mkShell {
  nativeBuildInputs =
    with pkgs.buildPackages;
    [
      git
      gdb
    ]
    ++ deps;

  shellHook = ''
    if [ -z "$NIX_SILENT" ]; then
        uname -a
    fi
  '';
}
