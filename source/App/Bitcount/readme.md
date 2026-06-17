
bitcount - Bit counting tools
====================================================

This tool allows to determine bit counts per access unit.

What it does
------------

For each access unit in the bitstream, the tool prints out a POC value and a
bit count (comma separated).


Usage
-----

```
bitcount <str.bin>
```

where `<str.bin>` is a raw bitstream (See Annex B of the VVC specification).


Limitations
-----------

Currently tested only on CfP 2026 bitstreams.

