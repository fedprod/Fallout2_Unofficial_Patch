#!/bin/bash

set -xeu -o pipefail

extra_dir=${extra_dir:-extra}
mpack_version=${mpack_version:-4.1.9}

mpack_file="modderspack_$mpack_version.7z"
mpack_url="https://sourceforge.net/projects/sfall/files/Modders%20pack/$mpack_file/download"
compile_exe="compile.exe"
mpack_compile="ScriptEditor/resources/$compile_exe"

# directories
cache_dir="$HOME/.cache/build"
bin_dir="$(realpath extra/bin)"
mkdir -p "$cache_dir" "$bin_dir"

# compile.exe, no cache (mpack version may change)
wget -nv "$mpack_url" -O "$mpack_7z"
7zr e "$mpack_7z" "$mpack_compile"
mv -f "$compile_exe" "$bin_dir/"
# sfall headers
7zr x "$mpack_7z" "scripting_docs/headers"
mv "scripting_docs/headers" "scripts_src/sfall"
