#!/bin/bash
set -e
rm -f results/*
mkdir -p logs
mkdir -p tmp

echo "Test Embedded data...(expected: 1 warning, no errors)"
orca ./test_embedded_data.mra -O ./results
echo
echo "Test md5 mismatch...(expected: 1 warning, no errors)"
orca ./test_md5_mismatch.mra -O ./results
echo
echo "Test offset and length...(expected: 1 warning, no errors)"
orca ./test_offset_length.mra -O ./results
echo
echo "Test repeat...(expected: no warnings)"
orca ./test_repeat.mra -O ./results
echo
echo "Test interleaving...(expected: no warnings)"
orca ./test_interleaved_part.mra -O ./results
echo
echo "Test Mister interleaving...(expected: no warnings)"
orca ./test_mister_interleave.mra -O ./results
echo
echo "Test endianess...(expected: no warnings)"
orca ./test_endianess.mra -O ./results
echo
echo "Test groups...(expected: 1 error)"
orca ./test_groups.mra -O ./results
echo
echo "Test ARC file...(expected: 1 error, 1 warning)"
orca ./test_arc.mra -A -O ./results
echo
echo "Test ARC file pages...(expected: 1 warning)"
orca ./test_arc_page.mra -A -O ./results
echo
echo "Test ARC file standalone...(expected: no warnings)"
orca ./test_arc_no_rom.mra -s -A -O ./results
echo
echo "Test ARC file w/o default switches...(expected: no warnings)"
orca ./test_arc_no_defaults.mra -s -A -O ./results
echo
echo "Test Selection by CRC...(expected: no warnings)"
orca ./test_select_by_crc.mra -O ./results
echo
echo "Test Multi zips source...(expected: 1 warning)"
orca ./test_multi_zips.mra -O ./results
echo
echo "Test Patch...(expected: no warnings)"
orca ./test_patch.mra -O ./results
echo
echo "Test file names...(expected: no warnings)"
./orca_dir.sh ./samples/Robotron -AO ./tmp > ./logs/test_file_names.log
ls -1 ./tmp | grep -E '\.rom|\.arc' | LC_ALL=C sort > ./results/filenames_test
echo
echo "Test command line args...(expected: no warnings)"
orca ./test_arc.mra -Ava "custom name.mra" -o "custom name.rom" -O ./results > ./logs/test_command_line.log
echo
echo "Result files (visualize with hexdump -Cv)..."
ls -l ./results

if [[ `git status --porcelain ./results` ]]; then
  # Changes
  echo "Output files with errors:"
  git status --porcelain ./results
  exit 1
else
  rm -rf ./logs
  rm -rf ./tmp
  echo "All tests passed"
fi
