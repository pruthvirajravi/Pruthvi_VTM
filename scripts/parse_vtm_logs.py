#!/usr/bin/env python3
"""Parse VTM encoder logs into a tidy results.csv. Preserves all logs; never fails the build."""
import sys
import os
import re
import csv
import glob


def parse_log(path):
    base = os.path.basename(path).replace(".log", "")
    if "_qp" in base:
        seq, qp = base.rsplit("_qp", 1)
    else:
        seq, qp = base, ""
    text = open(path, "r", errors="ignore").read()
    row = {
        "sequence": seq,
        "qp": int(qp) if qp.isdigit() else qp,
        "frames": "",
        "bitrate_kbps": "",
        "y_psnr": "",
        "u_psnr": "",
        "v_psnr": "",
        "yuv_psnr": "",
    }
    lines = text.splitlines()
    for i, ln in enumerate(lines):
        if "Total Frames" in ln and "PSNR" in ln:
            for j in range(i + 1, min(i + 4, len(lines))):
                m = re.match(r"\s*(\d+)\s+[a-z]\s+([\d.]+)\s+([\d.]+)\s+([\d.]+)\s+([\d.]+)\s+([\d.]+)", lines[j])
                if m:
                    row["frames"] = m.group(1)
                    row["bitrate_kbps"] = m.group(2)
                    row["y_psnr"] = m.group(3)
                    row["u_psnr"] = m.group(4)
                    row["v_psnr"] = m.group(5)
                    row["yuv_psnr"] = m.group(6)
                    break
            break
    return row


def main():
    logdir = sys.argv[1]
    out = sys.argv[2]
    outdir = os.path.dirname(out)
    if outdir:
        os.makedirs(outdir, exist_ok=True)
    rows = [parse_log(p) for p in sorted(glob.glob(os.path.join(logdir, "*.log")))]
    fields = ["sequence", "qp", "frames", "bitrate_kbps", "y_psnr", "u_psnr", "v_psnr", "yuv_psnr"]
    with open(out, "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=fields)
        w.writeheader()
        w.writerows(rows)
    print("Wrote " + str(len(rows)) + " rows to " + out)


if __name__ == "__main__":
    main()
