{
  description = "cpp env";

  inputs.flake-utils.url = "github:numtide/flake-utils";

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem
      (system:
        let pkgs = nixpkgs.legacyPackages.${system}; in
        {
          devShells.default = pkgs.mkShell {
            buildInputs = with pkgs; [
              clang-tools
              pkgsCross.avr.buildPackages.gcc
              pkgsCross.avr.buildPackages.binutils
              arduino-language-server
              arduino
              arduino-core
              screen
              # pkgs.pkgsCross.avr.buildPackages.gcc
              # pkgs.pkgsCross.avr.buildPackages.gcc.libc
              # pkgsi686Linux.glibc
              # avrdude
            ];
            shellHook = ''
            '';
          };
        }
      );
}
