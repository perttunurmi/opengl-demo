default:
    @just make_compile_commands

make_compile_commands:
    @printf "CFLAGS: $CFLAGS"
    bear -- g++ src/*.cpp $CFLAGS -I src/lib -o /dev/null
