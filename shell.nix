{
  pkgs ? import <nixpkgs> { },
  deps ? [ ],
  CFLAGS ? "",
}:
pkgs.mkShell {
  nativeBuildInputs =
    with pkgs.buildPackages;
    [
      git
      gdb
      bear
      just
    ]
    ++ deps;

  shellHook = ''
    if [ -z "$NIX_SILENT" ]; then
        export CFLAGS='${CFLAGS}'
        uname -a
    fi
  '';
}
