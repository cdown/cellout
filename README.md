`cellout` is a simple battery status printer.

Some versions of `awk` interpret `-g` as an option to the interpreter rather than
the script. To avoid this, notify your awk interpreter that there are no more
interpreter options with `--`, or your interpreter's equivalent.
