#!/bin/bash
# integration/ci/flash_image_gen.sh
# Simple helper to append header with CRC32 to a built binary
set -e
if [ "$#" -ne 2 ]; then
  echo "Usage: $0 <input_bin> <output_image.bin>"
  exit 1
fi
IN=$1
OUT=$2
LEN=$(wc -c < "$IN")
CRC=$(python3 - <<PY
import zlib,sys
b=open('$IN','rb').read()
print(zlib.crc32(b) & 0xFFFFFFFF)
PY
)
printf "WTFW" > hdr.bin
printf "\x01\x00\x00\x00" >> hdr.bin
python3 - <<PY
import struct
crc=$CRC
with open('hdr.bin','ab') as f:
    f.write(struct.pack('<I', $LEN))
    f.write(struct.pack('<I', crc))
PY
cat hdr.bin "$IN" > "$OUT"
rm -f hdr.bin

echo "Image with header written to $OUT"
