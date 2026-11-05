"""Outil serie cote PC pour la NeuroBoard-S3.

Commandes :
    python tools/serial_tool.py ports                       liste les ports serie
    python tools/serial_tool.py log  -p COM5 -t 20 -o m.txt enregistre 20 s de sortie texte
    python tools/serial_tool.py capture -p COM5 -n 10 -o img --format g
                                                            capture 10 images (sketch cam_capture)

Formats de capture : j = JPEG 320x240, g = niveaux de gris 96x96, r = RGB565 96x96.
Les images sont enregistrees en .jpg (JPEG) ou .png (gris, RGB565 converti en RGB888).
Dependances : pyserial, numpy, pillow.
"""

import argparse
import sys
import time
from pathlib import Path


def open_port(port: str, timeout: float = 2.0):
    import serial

    ser = serial.Serial(port, 115200, timeout=timeout)
    time.sleep(0.3)
    ser.reset_input_buffer()
    return ser


def cmd_ports(_args) -> None:
    from serial.tools import list_ports

    for p in list_ports.comports():
        print(f"{p.device:10s} {p.description}  (VID:PID {p.vid}:{p.pid})" if p.vid else f"{p.device:10s} {p.description}")


def cmd_log(args) -> None:
    ser = open_port(args.port, timeout=0.5)
    out = open(args.output, "w", encoding="utf-8") if args.output else None
    t_end = time.time() + args.time
    while time.time() < t_end:
        line = ser.readline().decode("utf-8", errors="replace").rstrip()
        if not line:
            continue
        print(line)
        if out:
            out.write(line + "\n")
    if out:
        out.close()


def _rgb565_to_rgb888(raw: bytes, w: int, h: int):
    import numpy as np

    # Octets envoyes par l'ESP32 : poids fort en premier
    v = np.frombuffer(raw, dtype=">u2").reshape(h, w).astype(np.uint32)
    r = ((v >> 11) & 0x1F) * 255 // 31
    g = ((v >> 5) & 0x3F) * 255 // 63
    b = (v & 0x1F) * 255 // 31
    return np.stack([r, g, b], axis=-1).astype(np.uint8)


def read_frame(ser):
    """Lit les lignes jusqu'a l'en-tete FRAME, puis les octets de l'image."""
    deadline = time.time() + 10
    while time.time() < deadline:
        line = ser.readline().decode("utf-8", errors="replace").strip()
        if line.startswith("FRAME"):
            _, w, h, fmt, n = line.split()
            data = ser.read(int(n))
            if len(data) != int(n):
                raise IOError(f"image incomplete : {len(data)} octets sur {n}")
            return int(w), int(h), fmt, data
        if line:
            print(line)
    raise TimeoutError("pas d'image recue (sketch cam_capture flashe ?)")


def cmd_capture(args) -> None:
    import numpy as np
    from PIL import Image

    out = Path(args.output)
    out.mkdir(parents=True, exist_ok=True)
    ser = open_port(args.port)
    ser.write(args.format.encode())
    time.sleep(1.0)                                   # reinitialisation de la camera
    for i in range(args.count):
        ser.write(b"c")
        w, h, fmt, data = read_frame(ser)
        stem = out / f"{args.prefix}{int(time.time() * 1000)}_{i:03d}"
        if fmt == "jpeg":
            path = stem.with_suffix(".jpg")
            path.write_bytes(data)
        elif fmt == "gray":
            path = stem.with_suffix(".png")
            Image.fromarray(np.frombuffer(data, np.uint8).reshape(h, w), "L").save(path)
        elif fmt == "rgb565":
            path = stem.with_suffix(".png")
            Image.fromarray(_rgb565_to_rgb888(data, w, h), "RGB").save(path)
        else:
            raise ValueError(fmt)
        print(f"{path}  ({w}x{h} {fmt}, {len(data)} octets)")
        time.sleep(args.delay)


def main(argv=None) -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = parser.add_subparsers(dest="cmd", required=True)
    sub.add_parser("ports")
    p = sub.add_parser("log")
    p.add_argument("-p", "--port", required=True)
    p.add_argument("-t", "--time", type=float, default=10.0)
    p.add_argument("-o", "--output")
    p = sub.add_parser("capture")
    p.add_argument("-p", "--port", required=True)
    p.add_argument("-n", "--count", type=int, default=1)
    p.add_argument("-o", "--output", default="captures")
    p.add_argument("--format", choices=["j", "g", "r"], default="j")
    p.add_argument("--prefix", default="")
    p.add_argument("--delay", type=float, default=0.5, help="pause entre deux captures (s)")
    args = parser.parse_args(argv)
    {"ports": cmd_ports, "log": cmd_log, "capture": cmd_capture}[args.cmd](args)


if __name__ == "__main__":
    sys.exit(main())
