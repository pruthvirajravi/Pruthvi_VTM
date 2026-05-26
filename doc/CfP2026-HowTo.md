# Guidelines on usage of VTM and ancillary tools for 2026 CfP

This document provides guidelines on the usage of VTM and ancillary tools for
the preparation of anchors, additional encodings, and associated data.

[VTM encoder settings](#vtm-encoder-settings) describes VTM encoder
configurations.

[VTM decoder usage](#vtm-decoder-settings) describes VTM decoder usage.

[PYUV conversion](#pyuv-conversion-tool) describes a tool to convert between
YUV and PYUV formats. The PYUV format is used for distribution of decoded
sequences (see Annex A of JVET-AP2026).


## VTM encoder settings

This section describes the command-line parameters to be provided to the VTM
encoder to produce the VTM anchors and additional VTM references to be used in
the 2026 Call for Proposals on Video Compression with Capability beyond VVC.

VTM version 24.0 shall be used, which can be found in the `cfp-2026` branch.

Quantization settings to achieve target bit rates will be provided at a later
date (see JVET-AP2026).


### Random access 

`-c cfg/encoder_randomaccess_vtm.cfg -c cfg/per-sequence-Cfp2026/<seq>.cfg -q=<qp> --QPIncrementFrame=<qpif>`

where
- `<seq>` represents the sequence to encode
- `<qp>` and `<qpif>` represent the quantizer settings required to achieve a
given target bitrate. When `<qpif>` is equal to -1 for a sequence/rate
combination, the `--QPIncrementFrame` parameter shall be omitted.

### Low delay 

`-c cfg/encoder_lowdelay_vtm.cfg -c cfg/per-sequence-Cfp2026/<seq>.cfg -q=<qp> --QPIncrementFrame=<qpif>`

## Additional VTM references

### Random access, reduced runtime 1

`-c cfg/encoder_randomaccess_vtm.cfg -c cfg/alternative-addon/reduced_runtime1.cfg -c cfg/per-sequence-Cfp2026/<seq>.cfg -q=<qp> --QPIncrementFrame=<qpif>`

### Low delay, reduced runtime 1

`-c cfg/encoder_lowdelay_vtm.cfg -c cfg/alternative-addon/reduced_runtime1.cfg -c cfg/per-sequence-Cfp2026/<seq>.cfg -q=<qp> --QPIncrementFrame=<qpif>`

### Random access, reduced runtime 2

`-c cfg/encoder_randomaccess_vtm.cfg -c cfg/alternative-addon/reduced_runtime2.cfg -c cfg/per-sequence-Cfp2026/<seq>.cfg -q=<qp> --QPIncrementFrame=<qpif>`

### Low delay, reduced runtime 2

`-c cfg/encoder_lowdelay_vtm.cfg -c cfg/alternative-addon/reduced_runtime2.cfg -c cfg/per-sequence-Cfp2026/<seq>.cfg -q=<qp> --QPIncrementFrame=<qpif>`

### Random access, reduced runtime 3

`-c cfg/encoder_randomaccess_vtm.cfg -c cfg/alternative-addon/rt_0.2_ra.cfg -c cfg/per-sequence-Cfp2026/<seq>.cfg -q=<qp> --QPIncrementFrame=<qpif>`

### Low delay, reduced runtime 3

`-c cfg/encoder_lowdelay_vtm.cfg -c cfg/alternative-addon/rt_0.2_ld.cfg -c cfg/per-sequence-Cfp2026/<seq>.cfg -q=<qp> --QPIncrementFrame=<qpif>`

### Random access, high performance

`-c cfg/alternative_high_perf/encoder_randomaccess_vtm_perf.cfg -c cfg/per-sequence-Cfp2026/<seq>.cfg -q=<qp> --QPIncrementFrame=<qpif>`

### Low delay, high performance

`-c cfg/alternative_high_perf/encoder_lowdelay_vtm_perf.cfg -c cfg/per-sequence-Cfp2026/<seq>.cfg -q=<qp> --QPIncrementFrame=<qpif>`

### Random access with adaptive frame resampling

`-c cfg/encoder_randomaccess_vtm.cfg -c cfg/alternative-addon/rpr+bim2.cfg -c cfg/per-sequence-Cfp2026/<seq>.cfg -q=<qp> --QPIncrementFrame=<qpif>`

### Low delay with adaptive frame resampling

`-c cfg/encoder_lowdelay_vtm.cfg -c cfg/alternative-addon/rpr+bim2.cfg -c cfg/per-sequence-Cfp2026/<seq>.cfg -q=<qp> --QPIncrementFrame=<qpif>`


## VTM decoder usage

The `--UpscaledOutput=2` command-line parameter shall be used when invoking
the decoder, such as to output frames in their original resolution when
adaptive frame resampling is enabled in the encoder.


## PYUV conversion tool

A tool `PYUVConversionApp` for converting between YUV and PYUV (packed YUV)
formats is included in the VTM software package in the `cfp-2026` branch. A
YUV file can be converted to a PYUV file using the parameters `-i my_file.yuv
-o my_file.pyuv`.
