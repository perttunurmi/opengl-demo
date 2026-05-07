{
  description = "Build and devShell for TIEA3112";

  inputs.nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";

  outputs =
    {
      self,
      nixpkgs,
    }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
    in
    let
      deps = with pkgs; [
        gcc
        glfw
      ];
    in
    {
      inherit self;

      # IMPORTANT: This output defines the devShell
      # To evalute run `nix develop` or use direnv
      devShells.${system}.default = import ./shell.nix { inherit pkgs deps; };

      # IMPORTANT: This output defines the build
      # To evaluate run `nix build`
      packages.${system}.default =
        let
          OUTPUT_NAME = "demo-cpp";
          CFLAGS = "-Wall -lGL -lglfw";
          MAIN = src/main.cpp;
        in

        pkgs.stdenv.mkDerivation {
          pname = "demo-cpp";
          version = "1.0.0";
          src = ./src;

          nativeBuildInputs = deps;

          buildPhase = ''
            mkdir -p $out/bin
            mkdir -p $out/frames
            export FRAME_DIR=$out/frames
            g++ ${MAIN} -o ${OUTPUT_NAME} ${CFLAGS}
          '';

          installPhase = ''
            mkdir -p $out/bin
            mv ${OUTPUT_NAME} $out/bin/
            echo done
          '';
        };

    };
}
