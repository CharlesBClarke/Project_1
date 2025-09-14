# debug.gdb  (replace myfile.c with your file)
set breakpoint pending on

break 62 if match_len > 0
commands
  break match_header if index > 0
  record
  continue
end

run 50
continue
