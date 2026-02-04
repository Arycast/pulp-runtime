# Arduino Port of pulp-runtime

All codes inside this directory only needed for packaging for Arduino library.
When use pulp-runtime library without Arduino, this directory can be ignored.

## Packaging the library (for generate release by maintainer)

Create `tools` directory (`tools` directory is ignored by version control).

Put your tools archive (compiler, debugger, jtag server/openocd) inside `tools` directory

Check (and edit if necessary) `package_*_index.json.in` in the same directory with this README file, check if tools url and archiveFileName already correct

Run `create_package` script. `package.tar.gz` file (or file defined by `PACKAGE_FILENAME` in `create_package`) will be generated
after release package generated, create package index with: `create_package_index` script

## notes for maintainer

make sure `PACKAGE_FILENAME` (from `create_package`) included in `.gitignore`
make sure tar excluded list `TAR_EXCLUDE_LIST` (from `create_package`) includes `create_package`, `create_package_index` and `TAR_EXCLUDE_LIST` itself
make sure `package_*_index.json` and `package_*_index.json.in` is excluded, some entry in platform need to be set by `PACKAGE_FILENAME` size and sha256


## Installing library in Arduino (by arduino user)

Run localhost server in this repository root, check host and port in `package_*_index.json.in`:
```bash
python -m "http.server" "8081"
```

Add board file with url that refer to generated index:
e.g. `http://localhost:8081/package_icdec_index.json` (example with `package_icdec_index.json`)
