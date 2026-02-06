# Arduino Port of pulp-runtime

All codes inside this directory only needed for packaging for Arduino library.
When use pulp-runtime library without Arduino, this directory can be ignored.


## Packaging the library (for generate release by maintainer)

Create `tools/riscv-toolchain-openocd` directory.

```bash
mkdir --verbose "<project root>/arduino/tools/riscv-toolchain-openocd"
```
(replace `<project root>` with root directory of the project, that contain `.git`)

Put your tools archive (`chroot.tar.xz` which is contain compiler, debugger, jtag server/openocd) inside `tools/riscv-toolchain-openocd` directory (all files with various tar extension inside `tools` directory is ignored by version control).
Toolchain is available to download at (https://drive.google.com/file/d/1mzljznFbGxnvn5V_P4foVcp7WK74oBoJ/view?usp=sharing).

Check (and edit if necessary) `package_*_index.json.in` in the same directory with this README file, check if tools url and archiveFileName is already correct.

Run `create_package` script in `tools/openocd` directory to generate tools package for openocd script.
```bash
pushd "<project root>/arduino/tools/openocd"
./create_package
popd
```

Run `create_package` script in project's `arduino` directory. `package.tar.gz` file (or file defined by `PACKAGE_FILENAME` in `create_package`) will be generated
after release package generated, create package index with: `create_package_index` script. if you don't set python3 as python, then just call `create_package_index` with python3 (command are given below):
```bash
pushd "<project root>"
./create_package
./create_package_index
popd
```
in case you don't set python3 as python:
```bash
pushd "<project root>"
./create_package
python3 create_package_index
popd
```


## Notes for maintainer

make sure `PACKAGE_FILENAME` (from `create_package`) included in `.gitignore`
make sure tar excluded list `TAR_EXCLUDE_LIST` (from `create_package`) includes `create_package`, `create_package_index` and `TAR_EXCLUDE_LIST` itself
make sure `package_*_index.json` and `package_*_index.json.in` is excluded, some entry in platform need to be set by `PACKAGE_FILENAME` size and sha256


## Installing library in Arduino (by arduino user)

Run localhost server in this `arduino` directory, check host and port in `package_*_index.json.in`:
```bash
cd "<project root>/arduino"
python -m "http.server" "8081"
```
in case you don't set python3 as python:
```bash
cd "<project root>/arduino"
python3 -m "http.server" "8081"
```

Python HTTP server command will be blocking the terminal. You probably want to use `screen` to run it:
```bash
cd "<project root>/arduino"
screen -S "httpserver" python -m "http.server" "8081"
```

Add board file with url that refer to generated index:
e.g. `http://localhost:8081/package_icdec_index.json` (example with `package_icdec_index.json`)
