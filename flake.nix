{
  description = "Build and devShell for OpenGL Demo";

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
        libGL
        glfw
        glew
      ];
    in
    {
      inherit self;

      # IMPORTANT: This output defines the devShell
      # To evaluate run `nix develop` or use direnv
      devShells.${system}.default = import ./shell.nix { inherit pkgs deps; };

      # IMPORTANT: This output defines the build
      # To evaluate run `nix build`
      packages.${system}.default =
        let
          OUTPUT_NAME = "demo-cpp";
          CFLAGS = "-std=c++23 -Wextra -g -Wall -lGL -lglfw -lGLEW";
        in

        pkgs.stdenv.mkDerivation {
          pname = "demo-cpp";
          version = "1.0.0";
          src = ./src;

          nativeBuildInputs = deps;

          buildInputs = with pkgs; [
            glfw
            glew
            mesa
          ];

          buildPhase = ''
            mkdir -p $out/bin
            g++  *.cpp -o ${OUTPUT_NAME} ${CFLAGS}
          '';

          installPhase = ''
            mkdir -p $out/bin
            mv ${OUTPUT_NAME} $out/bin/
            echo done
          '';
        };

    };
}
