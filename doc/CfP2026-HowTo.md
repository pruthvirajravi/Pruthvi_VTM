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

Quantization settings are provided for each encoder configuration.



### Random access 

`-c cfg/encoder_randomaccess_vtm.cfg -c cfg/per-sequence-Cfp2026/<seq>.cfg -q=<qp> --QPIncrementFrame=<qpif>`

where
- `<seq>` represents the sequence to encode
- `<qp>` and `<qpif>` represent the quantizer settings required to achieve a
given target bitrate. When `<qpif>` is equal to -1 for a sequence/rate
combination, the `--QPIncrementFrame` parameter shall be omitted.

For each sequence (identified by its identifier SID) and each target rate, a
pair of values representing `<qp>` and `<qpif>` are given in the table below:

| SID  | Rate 1 | Rate 2 | Rate 3 | Rate 4 | Rate 5 |
|:----:|:------:|:------:|:------:|:------:|:------:|
| SRU1 | 52,200 | 48,400 | 43,350 | 37,100 | 32,100 |
| SRU2 | 51, 60 | 46,120 | 41,180 | 36,120 | 32,420 |
| SRU3 | 45,230 | 39,240 | 33,200 | 28,140 | 23,180 |
| SRU4 | 44, 75 | 40,175 | 34,125 | 29, -1 | 24, 50 |
| SRH1 | 50, 70 | 45,265 | 40,374 | 37,255 | 33,130 |
| SRH2 | 44, 10 | 40, 50 | 36,140 | 32,240 | 28,400 |
| SRH3 | 51,128 | 47, 96 | 43,128 | 39,320 | 34, 64 |
| SRH4 | 52,330 | 48, -1 | 42,340 | 35,108 | 30, 46 |
| HPQ1 | 55,176 | 50,208 | 44, 92 | 37,352 | 30,176 |
| HPQ2 | 47,256 | 38,129 | 30,239 | 22,140 | 18,349 |
| HPQ3 | 43,416 | 38,451 | 33,288 | 28, 32 | 24,195 |
| HPQ4 | 46,293 | 41,244 | 36,510 | 30,310 | 25,126 |
| HLG1 | 48, 20 | 44,250 | 39, -1 | 33,200 | 28,150 |
| HLG2 | 50, -1 | 44, 50 | 40,400 | 35,200 | 29, 50 |
| HLG3 | 46,448 | 42,232 | 37,256 | 33,390 | 29, 80 |
| HLG4 | 45,192 | 40, 32 | 36,352 | 30,  8 | 26, 20 |
| URH1 | 52,160 | 48,320 | 43, 60 | 38, 90 | 33,210 |
| URH2 | 48, 46 | 44,160 | 39, 64 | 35,184 | 30,216 |
| URH3 | 50, 90 | 46, -1 | 42,128 | 39,420 | 34,360 |
| URH4 | 50, 90 | 45, 32 | 41,256 | 37,160 | 32,160 |

### Low delay 

`-c cfg/encoder_lowdelay_vtm.cfg -c cfg/per-sequence-Cfp2026/<seq>.cfg -q=<qp> --QPIncrementFrame=<qpif>`

| SID  | Rate 1 | Rate 2 | Rate 3 | Rate 4 | Rate 5 |
|:----:|:------:|:------:|:------:|:------:|:------:|
| SLH1 | 44,260 | 38, 35 | 33, 80 | 29, 25 | 26,225 |
| SLH2 | 46, 40 | 41,150 | 36, 95 | 31, -1 | 27,205 |
| SLH3 | 49, 90 | 43,205 | 36, 85 | 30,150 | 26,165 |
| SLH4 | 45,  5 | 39,260 | 32,165 | 26,  5 | 23,225 |
| GLH1 | 44,200 | 41,285 | 37,147 | 34,532 | 30,440 |
| GLH2 | 45,408 | 40,200 | 36,450 | 32,116 | 29,294 |
| GLH3 | 47, 32 | 43,128 | 40,192 | 37,272 | 34,480 |
| GLH4 | 46,544 | 42,320 | 38,128 | 35,480 | 31,352 |
| GLU5 | 47,461 | 42,423 | 37,334 | 33,434 | 30,417 |
| GLU6 | 46,430 | 41,  2 | 38,575 | 34, -1 | 30,402 |

## Additional VTM references

### Random access, reduced runtime 1

`-c cfg/encoder_randomaccess_vtm.cfg -c cfg/alternative-addon/reduced_runtime1.cfg -c cfg/per-sequence-Cfp2026/<seq>.cfg -q=<qp> --QPIncrementFrame=<qpif>`

| SID  | Rate 1 | Rate 2 | Rate 3 | Rate 4 | Rate 5 |
|:----:|:------:|:------:|:------:|:------:|:------:|
| SRU1 | 52,200 | 48,400 | 43,300 | 37, 50 | 32, 50 |
| SRU2 | 51, 60 | 46, 60 | 41,120 | 36, 60 | 32,420 |
| SRU3 | 45,230 | 39,230 | 33,190 | 28,100 | 23,170 |
| SRU4 | 44, 75 | 40,175 | 34,125 | 29, -1 | 24, 50 |
| SRH1 | 50, 73 | 45,275 | 40,375 | 37,255 | 33,128 |
| SRH2 | 44, 10 | 40, 50 | 36,130 | 32,200 | 28,400 |
| SRH3 | 51,128 | 47, 83 | 43,114 | 39,309 | 34, 27 |
| SRH4 | 52,327 | 48,493 | 42,311 | 35, 74 | 30, 17 |
| HPQ1 | 55,176 | 50,208 | 44, 90 | 37,320 | 30,160 |
| HPQ2 | 47,266 | 38,122 | 30,232 | 22,128 | 18,337 |
| HPQ3 | 43,444 | 38,448 | 33,275 | 28, 20 | 24,188 |
| HPQ4 | 46,302 | 41,260 | 36,530 | 30,294 | 25,102 |
| HLG1 | 48, 20 | 44,250 | 39,480 | 33,150 | 28,100 |
| HLG2 | 50, -1 | 44, 50 | 40,400 | 35,200 | 29, 20 |
| HLG3 | 46,448 | 42,232 | 37,256 | 33,384 | 29, 64 |
| HLG4 | 45,192 | 40, 28 | 36,352 | 30,  4 | 26, 12 |
| URH1 | 52,129 | 48,320 | 43,  1 | 38, 33 | 33,129 |
| URH2 | 48, 44 | 44,160 | 39, 56 | 35,168 | 30,200 |
| URH3 | 50, 64 | 46,577 | 42, 97 | 39,385 | 34,320 |
| URH4 | 50, 96 | 45, 33 | 41,225 | 37,160 | 32,129 |

### Low delay, reduced runtime 1

`-c cfg/encoder_lowdelay_vtm.cfg -c cfg/alternative-addon/reduced_runtime1.cfg -c cfg/per-sequence-Cfp2026/<seq>.cfg -q=<qp> --QPIncrementFrame=<qpif>`

| SID  | Rate 1 | Rate 2 | Rate 3 | Rate 4 | Rate 5 |
|:----:|:------:|:------:|:------:|:------:|:------:|
| SLH1 | 44,255 | 38, 20 | 33, 60 | 29,  5 | 26,205 |
| SLH2 | 46, 40 | 41,140 | 36, 80 | 31,290 | 27,190 |
| SLH3 | 49, 85 | 43,205 | 36, 65 | 30,145 | 26,160 |
| SLH4 | 46, -1 | 39,245 | 32,150 | 27, -1 | 23,210 |
| GLH1 | 44,232 | 41,266 | 37, 99 | 34,499 | 30,415 |
| GLH2 | 45,408 | 40,196 | 36,408 | 32,100 | 29,264 |
| GLH3 | 47, 36 | 43,136 | 40,188 | 37,268 | 34,456 |
| GLH4 | 46,540 | 42,316 | 38,124 | 35,444 | 31,308 |
| GLU5 | 47,460 | 42,423 | 37,327 | 33,415 | 30,412 |
| GLU6 | 46,400 | 42,580 | 38,520 | 34,560 | 30,380 |

### Random access, reduced runtime 2

`-c cfg/encoder_randomaccess_vtm.cfg -c cfg/alternative-addon/reduced_runtime2.cfg -c cfg/per-sequence-Cfp2026/<seq>.cfg -q=<qp> --QPIncrementFrame=<qpif>`

| SID  | Rate 1 | Rate 2 | Rate 3 | Rate 4 | Rate 5 |
|:----:|:------:|:------:|:------:|:------:|:------:|
| SRU1 | 52,200 | 48,400 | 43,300 | 37, 50 | 32, 50 |
| SRU2 | 51,  0 | 46,  0 | 41,120 | 36,  0 | 32,360 |
| SRU3 | 45,220 | 39,230 | 33,180 | 28, 90 | 23,170 |
| SRU4 | 44, 50 | 40,150 | 34,100 | 29,200 | 24, 25 |
| SRH1 | 50, 82 | 45,272 | 40,365 | 37,244 | 33,117 |
| SRH2 | 44, 30 | 40, 50 | 36,120 | 32,200 | 28,350 |
| SRH3 | 51,136 | 47, 89 | 43, 99 | 39,297 | 34, 22 |
| SRH4 | 52,373 | 48, -1 | 42,320 | 35, 72 | 30, 13 |
| HPQ1 | 55,192 | 50,224 | 44, 88 | 37,296 | 30,142 |
| HPQ2 | 47,224 | 38,108 | 30,216 | 22,113 | 18,337 |
| HPQ3 | 43,378 | 38,390 | 33,245 | 29,576 | 24,124 |
| HPQ4 | 46,416 | 41,290 | 36,530 | 30,291 | 25, 98 |
| HLG1 | 48, 20 | 44,250 | 39,400 | 33,150 | 28,100 |
| HLG2 | 50, -1 | 44, 20 | 40,350 | 35,150 | 30,480 |
| HLG3 | 46,480 | 42,256 | 37,266 | 33,400 | 29, 78 |
| HLG4 | 45,201 | 40, 28 | 36,320 | 31, -1 | 26,  0 |
| URH1 | 52,192 | 48,321 | 43,  1 | 38,  1 | 33,128 |
| URH2 | 48, 64 | 44,160 | 39, 56 | 35,160 | 30,192 |
| URH3 | 50, 64 | 46,577 | 42, 97 | 39,384 | 34,288 |
| URH4 | 50, 81 | 45, 64 | 41,241 | 37,160 | 32,128 |

### Low delay, reduced runtime 2

`-c cfg/encoder_lowdelay_vtm.cfg -c cfg/alternative-addon/reduced_runtime2.cfg -c cfg/per-sequence-Cfp2026/<seq>.cfg -q=<qp> --QPIncrementFrame=<qpif>`

| SID  | Rate 1 | Rate 2 | Rate 3 | Rate 4 | Rate 5 |
|:----:|:------:|:------:|:------:|:------:|:------:|
| SLH1 | 44,240 | 39, -1 | 33, 30 | 30,480 | 26,205 |
| SLH2 | 46, 35 | 41,135 | 36, 80 | 31,285 | 27,185 |
| SLH3 | 49, 65 | 43,180 | 36, 50 | 30,145 | 26,160 |
| SLH4 | 46,295 | 39,225 | 32,125 | 27,285 | 23,205 |
| GLH1 | 44,198 | 41,250 | 37, 99 | 34,499 | 30,415 |
| GLH2 | 45,381 | 40,184 | 36,400 | 32, 88 | 29,264 |
| GLH3 | 47, 16 | 43,160 | 40,208 | 37,288 | 34,448 |
| GLH4 | 46,512 | 42,320 | 38,128 | 35,464 | 31,336 |
| GLU5 | 47,445 | 42,397 | 37,319 | 33,404 | 30,412 |
| GLU6 | 46,360 | 42,530 | 38,470 | 34,510 | 30,380 |

### Random access, reduced runtime 3

`-c cfg/encoder_randomaccess_vtm.cfg -c cfg/alternative-addon/rt_0.2_ra.cfg -c cfg/per-sequence-Cfp2026/<seq>.cfg -q=<qp> --QPIncrementFrame=<qpif>`

| SID  | Rate 1 | Rate 2 | Rate 3 | Rate 4 | Rate 5 |
|:----:|:------:|:------:|:------:|:------:|:------:|
| SRU1 | 52, -1 | 47,150 | 42, 50 | 37,300 | 32,250 |
| SRU2 | 51,180 | 46,120 | 41,180 | 36,120 | 32,420 |
| SRU3 | 44,150 | 38,150 | 33, -1 | 28,180 | 23,220 |
| SRU4 | 44, 75 | 40,150 | 34, 50 | 29, 75 | 25,200 |
| SRH1 | 50,420 | 44,150 | 39,248 | 36,152 | 33,463 |
| SRH2 | 44,290 | 40,350 | 35, 10 | 31, 50 | 27, 90 |
| SRH3 | 51,160 | 47,110 | 43,120 | 39,280 | 35,482 |
| SRH4 | 52,390 | 47, 20 | 42,297 | 35, 25 | 31,470 |
| HPQ1 | 55,232 | 50,268 | 44,104 | 37,276 | 30,116 |
| HPQ2 | 47,210 | 38, 98 | 30,193 | 22, 74 | 18,303 |
| HPQ3 | 43,325 | 38,320 | 33, 90 | 29,290 | 25,420 |
| HPQ4 | 46,456 | 41,390 | 36,554 | 30,280 | 25, 60 |
| HLG1 | 48, 50 | 44,300 | 39,480 | 33,150 | 28, 50 |
| HLG2 | 50, -1 | 44, 50 | 40,300 | 35,100 | 30,400 |
| HLG3 | 46,480 | 42,284 | 37,272 | 33,400 | 29, 62 |
| HLG4 | 45,216 | 40, 31 | 36,320 | 31,484 | 27,477 |
| URH1 | 52,510 | 47, 30 | 43,410 | 38,410 | 33,440 |
| URH2 | 48, 64 | 44,160 | 39, 52 | 35,150 | 30,168 |
| URH3 | 50,448 | 45,280 | 42,384 | 38, 20 | 34,530 |
| URH4 | 50,250 | 45,128 | 41,288 | 37,150 | 32, 40 |

### Low delay, reduced runtime 3

`-c cfg/encoder_lowdelay_vtm.cfg -c cfg/alternative-addon/rt_0.2_ld.cfg -c cfg/per-sequence-Cfp2026/<seq>.cfg -q=<qp> --QPIncrementFrame=<qpif>`

| SID  | Rate 1 | Rate 2 | Rate 3 | Rate 4 | Rate 5 |
|:----:|:------:|:------:|:------:|:------:|:------:|
| SLH1 | 44,260 | 39,480 | 34,495 | 30,380 | 26,100 |
| SLH2 | 46, 50 | 41,125 | 36, 45 | 31,225 | 27, 90 |
| SLH3 | 49, 65 | 43,165 | 36, 15 | 30, 95 | 26,115 |
| SLH4 | 46, -1 | 39,195 | 32, 40 | 27,160 | 23,130 |
| GLH1 | 44,200 | 41,250 | 37, 66 | 34,464 | 30,380 |
| GLH2 | 45,300 | 41,565 | 36,171 | 33,258 | 30,466 |
| GLH3 | 47, 48 | 43,176 | 40,224 | 37,304 | 34,496 |
| GLH4 | 45,192 | 42,544 | 38,256 | 34, 16 | 31,256 |
| GLU5 | 47,455 | 42,383 | 37,279 | 33,356 | 30,374 |
| GLU6 | 46,280 | 42,300 | 38,220 | 34,230 | 30,220 |

### Random access, high performance

`-c cfg/alternative_high_perf/encoder_randomaccess_vtm_perf.cfg -c cfg/per-sequence-Cfp2026/<seq>.cfg -q=<qp> --QPIncrementFrame=<qpif>`

| SID  | Rate 1 | Rate 2 | Rate 3 | Rate 4 | Rate 5 |
|:----:|:------:|:------:|:------:|:------:|:------:|
| SRU1 | 52,100 | 48,300 | 43,250 | 37, 50 | 32,100 |
| SRU2 | 52,410 | 47,420 | 42,540 | 37,540 | 32,360 |
| SRU3 | 45,220 | 39,230 | 33,180 | 28,100 | 23,160 |
| SRU4 | 44,  0 | 40, 75 | 34, 50 | 29,175 | 24,  0 |
| SRH1 | 51,459 | 45,198 | 40,328 | 37,234 | 33,121 |
| SRH2 | 45,200 | 41,400 | 36, 60 | 32,150 | 28,300 |
| SRH3 | 52,480 | 48,448 | 44,496 | 39,224 | 35,489 |
| SRH4 | 52,192 | 48,384 | 42,256 | 35, 64 | 30, 15 |
| HPQ1 | 55, 80 | 50,140 | 44, 60 | 37,312 | 30,162 |
| HPQ2 | 47,200 | 38,120 | 30,236 | 22,132 | 18,360 |
| HPQ3 | 43, 60 | 38,150 | 33, 60 | 29,370 | 24, 80 |
| HPQ4 | 46,200 | 41,190 | 36,470 | 30,290 | 25,100 |
| HLG1 | 49,300 | 44,150 | 39,300 | 33,150 | 28,100 |
| HLG2 | 50,300 | 45,450 | 40,250 | 35,100 | 30,480 |
| HLG3 | 46, 80 | 43,512 | 37,  0 | 33,288 | 30, -1 |
| HLG4 | 45, 31 | 41,384 | 36,192 | 31,400 | 27, -1 |
| URH1 | 53,512 | 48, 64 | 44,511 | 39,576 | 33, 65 |
| URH2 | 49,158 | 44, 64 | 40,192 | 35,113 | 30,160 |
| URH3 | 51,401 | 46,321 | 43,512 | 39,225 | 34,192 |
| URH4 | 51,128 | 46, 96 | 42, -1 | 38,256 | 33, -1 |

### Low delay, high performance

`-c cfg/alternative_high_perf/encoder_lowdelay_vtm_perf.cfg -c cfg/per-sequence-Cfp2026/<seq>.cfg -q=<qp> --QPIncrementFrame=<qpif>`

| SID  | Rate 1 | Rate 2 | Rate 3 | Rate 4 | Rate 5 |
|:----:|:------:|:------:|:------:|:------:|:------:|
| SLH1 | 44,250 | 38, 35 | 33, 65 | 29, 20 | 26,220 |
| SLH2 | 46, 35 | 41,145 | 36, 95 | 31,295 | 27,200 |
| SLH3 | 49, 80 | 43,200 | 36, 80 | 30,155 | 26,175 |
| SLH4 | 45, 30 | 39,280 | 32,200 | 26, 25 | 23,250 |
| GLH1 | 44,200 | 41,282 | 37,132 | 34,500 | 30,433 |
| GLH2 | 45,400 | 40,200 | 36,447 | 32,116 | 29,304 |
| GLH3 | 47, 16 | 43,128 | 40,192 | 37,272 | 34,480 |
| GLH4 | 46,544 | 42,304 | 38,128 | 35,464 | 31,336 |
| GLU5 | 47,458 | 42,408 | 37,325 | 33,430 | 30,420 |
| GLU6 | 46,400 | 41,  0 | 38,560 | 34, -1 | 30,400 |

### Random access with adaptive frame resampling

`-c cfg/encoder_randomaccess_vtm.cfg -c cfg/alternative-addon/rpr+bim2.cfg -c cfg/per-sequence-Cfp2026/<seq>.cfg -q=<qp> --QPIncrementFrame=<qpif>`

| SID  | Rate 1 | Rate 2 | Rate 3 | Rate 4 | Rate 5 |
|:----:|:------:|:------:|:------:|:------:|:------:|
| SRU1 | 52,350 | 48,450 | 43,400 | 37,200 | 32,150 |
| SRU2 | 51,240 | 46,120 | 41,  0 | 37,360 | 32, 60 |
| SRU3 | 44,220 | 38,200 | 32,150 | 28,200 | 23,240 |
| SRU4 | 43,125 | 39,175 | 33, 50 | 29, -1 | 24, 75 |
| SRH1 | 49, 91 | 44,211 | 39,228 | 36, 91 | 33,277 |
| SRH2 | 45,450 | 40, 20 | 36,100 | 32,200 | 28,300 |
| SRH3 | 51,314 | 47,241 | 43,256 | 39,422 | 34,161 |
| SRH4 | 51,125 | 47,249 | 41, 64 | 35,251 | 30,147 |
| HPQ1 | 55,128 | 50,192 | 44,100 | 37,320 | 30,102 |
| HPQ2 | 45, 10 | 37,104 | 30,256 | 22,167 | 17, 12 |
| HPQ3 | 43,448 | 38,384 | 33,217 | 29,200 | 25,452 |
| HPQ4 | 46,444 | 41,402 | 36, -1 | 30,356 | 25,174 |
| HLG1 | 48, 50 | 44, -1 | 38, 50 | 33,300 | 28,200 |
| HLG2 | 49,150 | 44,250 | 40, -1 | 35,300 | 29, 50 |
| HLG3 | 46,352 | 42, 96 | 37,128 | 33,320 | 30, -1 |
| HLG4 | 45,304 | 40,128 | 36,448 | 31,352 | 27,384 |
| URH1 | 52, 90 | 48,320 | 43, 60 | 38, 70 | 33,190 |
| URH2 | 49,224 | 44,120 | 39, 14 | 35,150 | 30,172 |
| URH3 | 51,256 | 46,350 | 43,360 | 39, 60 | 35, -1 |
| URH4 | 50, 64 | 46,256 | 41,128 | 37, 64 | 32, 90 |


### Low delay with adaptive frame resampling

`-c cfg/encoder_lowdelay_vtm.cfg -c cfg/alternative-addon/rpr+bim2.cfg -c cfg/per-sequence-Cfp2026/<seq>.cfg -q=<qp> --QPIncrementFrame=<qpif>`

| SID  | Rate 1 | Rate 2 | Rate 3 | Rate 4 | Rate 5 |
|:----:|:------:|:------:|:------:|:------:|:------:|
| SLH1 | 44,460 | 39,350 | 34,360 | 30,300 | 26, 65 |
| SLH2 | 45, 30 | 40, 85 | 36,290 | 30, 55 | 27,260 |
| SLH3 | 48, 45 | 42,140 | 36,250 | 30,220 | 26,225 |
| SLH4 | 44, 30 | 38,160 | 32,235 | 26, 35 | 23,255 |
| GLH1 | 44,332 | 41,353 | 37,233 | 34,532 | 30,424 |
| GLH2 | 44,296 | 39, 81 | 35,257 | 32,354 | 29, -1 |
| GLH3 | 46,112 | 42,240 | 39,256 | 36,224 | 33,304 |
| GLH4 | 45,464 | 41,192 | 38,544 | 34,256 | 30, 96 |
| GLU5 | 46,364 | 41,342 | 36,231 | 32,264 | 30,582 |
| GLU6 | 45,258 | 41,419 | 37,359 | 33,351 | 29,110 |

## VTM decoder usage

The `--UpscaledOutput=2` command-line parameter shall be used when invoking
the decoder, such as to output frames in their original resolution when
adaptive frame resampling is enabled in the encoder.


## PYUV conversion tool

A tool `PYUVConversionApp` for converting between YUV and PYUV (packed YUV)
formats is included in the VTM software package in the `cfp-2026` branch. A
YUV file can be converted to a PYUV file using the parameters `-i my_file.yuv
-o my_file.pyuv`.
