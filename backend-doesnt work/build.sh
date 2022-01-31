cd ~/Prog/cheatsite/backend
echo `/opt/emsdk/upstream/emscripten/em++ --bind ${PWD}/funcs.cpp -O3 -s WASM=1 -o library.js` || exit 128
rm bin/* -R
mv library* bin/
